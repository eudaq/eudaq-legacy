
#include <usb.h>
#include <errno.h>
#include <memory.h>
#include <linux/compiler.h>
#include <linux/usbdevice_fs.h> // interface to kernel portion of user mode usb driver
#include <sys/ioctl.h>
#include "ZestSC1.h"
#include "Local.h"



/******************************************************************************
* Write to a memory-mapped register on the FPGA                               *
******************************************************************************/
ZESTSC1_STATUS ZestSC1WriteRegister(ZESTSC1_HANDLE Handle,
                                    unsigned long Offset,
                                    unsigned char Value)
{
    char Buffer[1];
    ZESTSC1_CHECK_HANDLE("ZestSC1WriteRegister", Handle);

    if (usb_control_msg(Struct->DeviceHandle, EP_CTRL_READ, VR_SET_REG, 
                        Offset, Value, Buffer, 1, Struct->TimeOut)<=0)
    {
        ZESTSC1_ERROR("ZestSC1WriteRegister", ZESTSC1_INTERNAL_ERROR);
    }

    return ZESTSC1_SUCCESS;
}

/******************************************************************************
* Read from a memory-mapped register on the FPGA                              *
******************************************************************************/
ZESTSC1_STATUS ZestSC1ReadRegister(ZESTSC1_HANDLE Handle,
                                   unsigned long Offset,
                                   unsigned char *Value)
{
    char Buffer[2];
    ZESTSC1_CHECK_HANDLE("ZestSC1ReadRegister", Handle);

    if (usb_control_msg(Struct->DeviceHandle, EP_CTRL_READ, VR_GET_REG, 
                        Offset, 0, Buffer, 2, Struct->TimeOut)<=0)
    {
        ZESTSC1_ERROR("ZestSC1ReadRegister", ZESTSC1_INTERNAL_ERROR);
    }

    *Value = Buffer[1];

    return ZESTSC1_SUCCESS;
}


/******************************************************************************
* Single function to read data from the card                                  *
******************************************************************************/
ZESTSC1_STATUS ZestSC1ReadData(ZESTSC1_HANDLE Handle,
                               void *Buffer,
                               unsigned long Length)
{
    ZESTSC1_STATUS Status;

    ZESTSC1_CHECK_HANDLE("ZestSC1ReadData", Handle);

    Status = ZestSC1_Transfer(Handle, EP_DATA_READ, Buffer, Length);
    if (Status!=ZESTSC1_SUCCESS)
    {
        ZESTSC1_ERROR("ZestSC1ReadData", Status);
    }

    return ZESTSC1_SUCCESS;
}

/******************************************************************************
* Single function to write data to the card                                   *
******************************************************************************/
ZESTSC1_STATUS ZestSC1WriteData(ZESTSC1_HANDLE Handle,
                                void *Buffer,
                                unsigned long Length)
{
    ZESTSC1_STATUS Status;
    
    ZESTSC1_CHECK_HANDLE("ZestSC1WriteData", Handle);

    Status = ZestSC1_Transfer(Handle, EP_DATA_WRITE, Buffer, Length);
    if (Status!=ZESTSC1_SUCCESS)
    {
        ZESTSC1_ERROR("ZestSC1WriteData", Status);
    }

    return ZESTSC1_SUCCESS;
}


/******************************************************************************
* Perform bulk transfer                                                       *
* Maintain 2 active asynchronous URBs to maximise data transfer rate.         *
******************************************************************************/
ZESTSC1_STATUS ZestSC1_Transfer(ZESTSC1_HANDLE Handle, int EP, void *Buffer, int Length)
{
    ZESTSC1_HANDLE_STRUCT *Struct = (ZESTSC1_HANDLE_STRUCT *)Handle;
    unsigned long Count;
    int fd = *((int *)(Struct->DeviceHandle)); // FIXME: Watch out here!
    struct usbdevfs_urb *urb[2];
    int Queued[2] = {0,0};
    ZESTSC1_STATUS Status = ZESTSC1_SUCCESS;
    struct usbdevfs_urb *urbreap;
    int i;
    struct timeval TimeEnd;
    struct timeval TimeOut;
    int Bytes;
    int LastTransfer;

    for (i=0; i<2; i++)
    {
        urb[i] = malloc(sizeof(struct usbdevfs_urb));
        if (urb[i]==NULL)
        {
            for (i--; i>=0; i--)
                free(urb[i]);
            return ZESTSC1_OUT_OF_MEMORY;
        }
    }

    gettimeofday(&TimeEnd, NULL);
    TimeEnd.tv_sec += Struct->TimeOut/1000;
    TimeEnd.tv_usec += (Struct->TimeOut%1000)*1000;
    TimeOut.tv_sec = 0;
    TimeOut.tv_usec = 1000; // 1msec

    i = 0;
    LastTransfer = 1;
    for (Count=0; Count<Length || LastTransfer==1; Count+=Bytes)
    {
        int RetVal;

        if ((Length-Count)<ZESTSC1_MAX_TRANSFER_LENGTH)
            Bytes = Length-Count;
        else
            Bytes = ZESTSC1_MAX_TRANSFER_LENGTH;
        if (Bytes==0)
            LastTransfer = 0;

        if (Bytes!=0)
        {
            /*
             * Submit the next URB
             */
            memset(urb[i], 0, sizeof(struct usbdevfs_urb));

            urb[i]->buffer_length = Bytes;
            urb[i]->actual_length = Bytes;
            urb[i]->buffer = ((char *)Buffer)+Count;
            urb[i]->type = ((EP==EP_INT_READ) ? USBDEVFS_URB_TYPE_INTERRUPT : USBDEVFS_URB_TYPE_BULK);
            urb[i]->endpoint = EP;

            RetVal = ioctl(fd, USBDEVFS_SUBMITURB, urb[i]);
            if (RetVal<0)
            {
                Status = ZESTSC1_INTERNAL_ERROR;
                goto Error;
            }
            Queued[i] = 1;
        }

        i=1-i;

        if (Count!=0)
        {
            /*
             * Reap the previous URB
             */
            struct timeval TimeNow;
            fd_set fset;

            FD_ZERO(&fset);
            FD_SET(fd, &fset);
            gettimeofday(&TimeNow);

            while ((RetVal=ioctl(fd, USBDEVFS_REAPURBNDELAY, &urbreap))==-1 &&
                   ((TimeNow.tv_sec<TimeEnd.tv_sec) ||
                    (TimeNow.tv_sec==TimeEnd.tv_sec && TimeNow.tv_usec<TimeEnd.tv_usec)))
            {
                if (errno!=EAGAIN)
                {
                    Status = ZESTSC1_INTERNAL_ERROR;
                    goto Error;
                }
                select(fd+1, NULL, &fset, NULL, &TimeOut);
                gettimeofday(&TimeNow);
            }
            if (RetVal==-1)
            {
                Status = ZESTSC1_TIMEOUT;
                goto Error;
            }
            if (urbreap->status!=0)
            {
                Status = ZESTSC1_INTERNAL_ERROR;
                goto Error;
            }
            Queued[i] = 0;
        }
    }

    for (i=0; i<2; i++)
        free(urb[i]);

    return ZESTSC1_SUCCESS;

Error:
    for (i=0; i<2; i++)
    {
        if (Queued[i])
        {
            // Cancel URB
            ioctl(fd, USBDEVFS_DISCARDURB, &urb[i]);
        }
        free(urb[i]);
    }

    return Status;
}

