//////////////////////////////////////////////////////////////////////
//
// File:      Example1.c
//
// Purpose:
//    ZestSC1 Example Programs
//    Simple card open/configure/close example
//  
// Copyright (c) 2004-2011 Orange Tree Technologies.
// May not be reproduced without permission.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include "ZestSC1.h"

//
// Declare configuration images
//
extern unsigned long Image400Length;
extern unsigned long Image400Image[];
extern unsigned long Image1000Length;
extern unsigned long Image1000Image[];

//
// Error handler function
//
void ErrorHandler(const char *Function, 
                  ZESTSC1_HANDLE Handle,
                  ZESTSC1_STATUS Status,
                  const char *Msg)
{
    printf("**** Example1 - Function %s returned an error\n        \"%s\"\n\n", Function, Msg);
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
    ZESTSC1_IMAGE Image;

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
    // Method 1: Configure the FPGA directly from a file
    //
    if (FPGATypes[0]==ZESTSC1_XC3S400)
        ZestSC1ConfigureFromFile(Handle, "Example1_400.bit");
    else
        ZestSC1ConfigureFromFile(Handle, "Example1_1000.bit");
    printf("Press Enter to continue\n");
    while(getchar()!='\n');

    //
    // Method 2: Pre-load image and configure later
    //
    if (FPGATypes[0]==ZESTSC1_XC3S400)
        ZestSC1LoadFile("Example1_400.bit", &Image);
    else
        ZestSC1LoadFile("Example1_1000.bit", &Image);

    ZestSC1Configure(Handle, Image);
    ZestSC1FreeImage(Image);
    printf("Press Enter to continue\n");
    while(getchar()!='\n');

    //
    // Method 3: configure from a linked image
    // The linked images are created by the GenCFG utility
    //
    if (FPGATypes[0]==ZESTSC1_XC3S400)
        ZestSC1RegisterImage(Image400Image, Image400Length, &Image);
    else
        ZestSC1RegisterImage(Image1000Image, Image1000Length, &Image);

    ZestSC1Configure(Handle, Image);
    ZestSC1FreeImage(Image);
    printf("Press Enter to continue\n");
    while(getchar()!='\n');

    //
    // Close the card
    //
    ZestSC1CloseCard(Handle);

    return 0;
}
