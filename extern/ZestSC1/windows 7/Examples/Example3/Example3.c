//////////////////////////////////////////////////////////////////////
//
// File:      Example3.c
//
// Purpose:
//    ZestSC1 Example Programs
//    Low speed data transfer example
//  
// Copyright (c) 2004-2011 Orange Tree Technologies.
// May not be reproduced without permission.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include "ZestSC1.h"

//
// Error handler function
//
void ErrorHandler(const char *Function, 
                  ZESTSC1_HANDLE Handle,
                  ZESTSC1_STATUS Status,
                  const char *Msg)
{
    printf("**** Example3 - Function %s returned an error\n        \"%s\"\n\n", Function, Msg);
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
    unsigned char Result;

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
    // Then set 4 signals as outputs and 4 signals as inputs
    //
    ZestSC1OpenCard(CardIDs[0], &Handle);

    //
    // Configure the FPGA
    //
    if (FPGATypes[0]==ZESTSC1_XC3S400)
        ZestSC1ConfigureFromFile(Handle, "Example3_400.bit");
    else
        ZestSC1ConfigureFromFile(Handle, "Example3_1000.bit");
    ZestSC1SetSignalDirection(Handle, 0xf);

    //
    // Write and read a register in the FPGA
    // Note that registers from 0 to 0x2000 are not available
    // on boards fitted with the Cypress FX2LP device.  To allow
    // this example to work on all boards only use registers above
    // address 0x2000.
    //
    for (Count=0; Count<256; Count++)
    {
        ZestSC1WriteRegister(Handle, 0x2000+123, (unsigned char)Count);
        ZestSC1ReadRegister(Handle, 0x2000+123, &Result);
        if (Result!=((Count+1)&0xff))
        {
            printf("Register interface failed.  Wrote %d, Expected %d, Read %d\n",
                Count, ((Count+1)&0xff), Result);
            exit(1);
        }
    }
    printf("Register tests passed\n");

    //
    // Set and read signals in the FPGA
    //
    for (Count=0; Count<16; Count++)
    {
        ZestSC1SetSignals(Handle, (unsigned char)Count);
        ZestSC1ReadSignals(Handle, &Result);
        if (((Result>>4)&0xF)!=(Count&0xF))
        {
            printf("Signal interface failed.  Wrote %d, Expected %d, Read %d\n",
                Count, Count<<4, Result);
            exit(1);
        }
    }
    printf("Signal tests passed\n");

    //
    // Prompt an interrupt and wait for it
    //
    ZestSC1WriteRegister(Handle, 0x2000, 1);
    ZestSC1WaitForInterrupt(Handle);
    printf("Interrupt tests passed\n");

    //
    // Close the card
    //
    ZestSC1CloseCard(Handle);

    return 0;
}
