
#include <usb.h>
#include <errno.h>
#include <memory.h>
#include <sys/ioctl.h>
#include "ZestSC1.h"
#include "Local.h"
#include <stdio.h>


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

    if (usb_bulk_read(Struct->DeviceHandle, EP_DATA_READ, Buffer, Length, Struct->TimeOut)<=0)
    {
        ZESTSC1_ERROR("ZestSC1ReadData", ZESTSC1_INTERNAL_ERROR);
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

    if (usb_bulk_write(Struct->DeviceHandle, EP_DATA_WRITE, Buffer, Length, Struct->TimeOut)<=0)
    {
        ZESTSC1_ERROR("ZestSC1WriteData", ZESTSC1_INTERNAL_ERROR);
    }

    return ZESTSC1_SUCCESS;
}

