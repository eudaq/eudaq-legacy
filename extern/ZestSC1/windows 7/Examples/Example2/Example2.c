//////////////////////////////////////////////////////////////////////
//
// File:      Example2.c
//
// Purpose:
//    ZestSC1 Example Programs
//    Data transfer rate measurement
//  
// Copyright (c) 2004-2011 Orange Tree Technologies.
// May not be reproduced without permission.
//
//////////////////////////////////////////////////////////////////////

#define TRANSFER_LENGTH (1024*1024)
#define NUM_TRANSFERS 50

#include <windows.h>
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
    printf("**** Example2 - Function %s returned an error\n        \"%s\"\n\n", Function, Msg);
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
    unsigned __int64 Freq;
    unsigned __int64 Start;
    unsigned __int64 End;
    unsigned char *Buffer;

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
        ZestSC1ConfigureFromFile(Handle, "Example2_400.bit");
    else
        ZestSC1ConfigureFromFile(Handle, "Example2_1000.bit");

    //
    // Send data to the card
    //
    Buffer = malloc(TRANSFER_LENGTH);
    for (Count=0; Count<TRANSFER_LENGTH/2; Count++)
        ((unsigned short *)Buffer)[Count] = (unsigned short)(Count&0xffff);

    QueryPerformanceFrequency((LARGE_INTEGER *)&Freq);
    QueryPerformanceCounter((LARGE_INTEGER *)&Start);
    for (Count=0; Count<NUM_TRANSFERS; Count++)
    {
        ZestSC1WriteData(Handle, Buffer, TRANSFER_LENGTH);
    }
    QueryPerformanceCounter((LARGE_INTEGER *)&End);
    printf("Transfer rate to card = %.2fMBytes/sec\n", 
        ((double)NUM_TRANSFERS*(double)TRANSFER_LENGTH*(double)Freq)/(1024.*1024.*(double)(End-Start)));

    //
    // Read data from the card
    //
    QueryPerformanceCounter((LARGE_INTEGER *)&Start);
    for (Count=0; Count<NUM_TRANSFERS; Count++)
    {
        ZestSC1ReadData(Handle, Buffer, TRANSFER_LENGTH);
    }
    QueryPerformanceCounter((LARGE_INTEGER *)&End);
    printf("\n");
    printf("Transfer rate from card = %.2fMBytes/sec\n", 
        ((double)NUM_TRANSFERS*(double)TRANSFER_LENGTH*(double)Freq)/(1024.*1024.*(double)(End-Start)));

    //
    // Close the card
    //
    free(Buffer);
    ZestSC1CloseCard(Handle);

    return 0;
}
