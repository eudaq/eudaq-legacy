
#include <usb.h>
#include <errno.h>
#include <memory.h>
#include <sys/ioctl.h>
#include "ZestSC1.h"
#include "Local.h"

/******************************************************************************
* Perform bulk transfer                                                       *
* Maintain 2 active asynchronous URBs to maximise data transfer rate.         *
******************************************************************************/
ZESTSC1_STATUS ZestSC1_Transfer(ZESTSC1_HANDLE Handle, int EP, void *Buffer, int Length)
{
    ZESTSC1_CHECK_HANDLE("ZestSC1ReadData", Handle);

    if (usb_bulk_read(Struct->DeviceHandle, EP, Buffer, Length, Struct->TimeOut)<=0)
    {
        ZESTSC1_ERROR("ZestSC1_Transfer", ZESTSC1_INTERNAL_ERROR);
    }

    return ZESTSC1_SUCCESS;
}
