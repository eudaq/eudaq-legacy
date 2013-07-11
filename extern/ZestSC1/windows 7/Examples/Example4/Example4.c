//////////////////////////////////////////////////////////////////////
//
// File:      Example4.c
//
// Purpose:
//    ZestSC1 Example Programs
//    SRAM transfer example
//  
// Copyright (c) 2004-2011 Orange Tree Technologies.
// May not be reproduced without permission.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "ZestSC1.h"

#define TRANSFER_SIZE (2*1024*1024)

//
// Error handler function
//
void ErrorHandler(const char *Function, 
                  ZESTSC1_HANDLE Handle,
                  ZESTSC1_STATUS Status,
                  const char *Msg)
{
    printf("**** Example4 - Function %s returned an error\n        \"%s\"\n\n", Function, Msg);
    exit(1);
}

//
// Main program
//
int main(int argc, char **argv)
{
    unsigned long Count;
    unsigned long NumCards;
    unsigned long CardIDs[256];
    unsigned long SerialNumbers[256];
    ZESTSC1_FPGA_TYPE FPGATypes[256];
    ZESTSC1_HANDLE Handle;
    unsigned long *Buffer;

    //
    // Install an error handler
    //
    ZestSC1RegisterErrorHandler(ErrorHandler);

    //
    // Request information about the system
    //
    ZestSC1CountCards(&NumCards, CardIDs, SerialNumbers, FPGATypes);
    printf("%d available cards in the system\n\n\n", NumCards);
    if (NumCards==0)
    {
        printf("No cards in the system\n");
        exit(1);
    }

    for (Count=0; Count<NumCards; Count++)
    {
        printf("%d : CardID = 0x%08lx, SerialNum = 0x%08lx, FPGAType = %d\n",
            Count, CardIDs[Count], SerialNumbers[Count], FPGATypes[Count]);
    }

    //
    // Open the first card
    //
    ZestSC1OpenCard(CardIDs[0], &Handle);

    //
    // Configure the FPGA
    //
    if (FPGATypes[0]==ZESTSC1_XC3S400)
        ZestSC1ConfigureFromFile(Handle, "Example4_400.bit");
    else
        ZestSC1ConfigureFromFile(Handle, "Example4_1000.bit");

    //
    // Write block of memory and read it back
    //
    Buffer = malloc(TRANSFER_SIZE);
    if (Buffer==NULL)
    {
        printf("Not enough memory!\n");
        exit(1);
    }
    for (Count=0; Count<TRANSFER_SIZE/sizeof(unsigned long); Count++)
    {
        Buffer[Count] = Count;
    }

    // Set SRAM Address
    // Note that registers from 0 to 0x2000 are not available
    // on boards fitted with the Cypress FX2LP device.  To allow
    // this example to work on all boards only use registers above
    // address 0x2000.
    ZestSC1WriteRegister(Handle, 0x2000+0, 0);
    ZestSC1WriteRegister(Handle, 0x2000+1, 0);
    ZestSC1WriteRegister(Handle, 0x2000+2, 0);

    // Set Write Count
    Count = TRANSFER_SIZE/sizeof(unsigned long);
    Count *= 2;
    ZestSC1WriteRegister(Handle, 0x2000+3, (unsigned char)(Count&0xff));
    ZestSC1WriteRegister(Handle, 0x2000+4, (unsigned char)((Count>>8)&0xff));
    ZestSC1WriteRegister(Handle, 0x2000+5, (unsigned char)((Count>>16)&0xff));

    // Send data and clear buffer
    ZestSC1WriteData(Handle, Buffer, TRANSFER_SIZE);
    memset(Buffer, 0, TRANSFER_SIZE);

    // Set SRAM Address
    ZestSC1WriteRegister(Handle, 0x2000+0, 0);
    ZestSC1WriteRegister(Handle, 0x2000+1, 0);
    ZestSC1WriteRegister(Handle, 0x2000+2, 0);

    // Set Read Count
    ZestSC1WriteRegister(Handle, 0x2000+6, (unsigned char)(Count&0xff));
    ZestSC1WriteRegister(Handle, 0x2000+7, (unsigned char)((Count>>8)&0xff));
    ZestSC1WriteRegister(Handle, 0x2000+8, (unsigned char)((Count>>16)&0xff));

    // Read data
    ZestSC1ReadData(Handle, Buffer, TRANSFER_SIZE);

    // Check data
    for (Count=0; Count<TRANSFER_SIZE/sizeof(unsigned long); Count++)
    {
        if (Buffer[Count] != (Count&0x3FFFF))
        {
            printf("Error in data : Expected 0x%08lx, got 0x%08lx\n", Count&0x3ffff, Buffer[Count]);
            exit(1);
        }
    }
    printf("SRAM test passed\n");
    free(Buffer);

    //
    // Close the card
    //
    ZestSC1CloseCard(Handle);

    return 0;
}
