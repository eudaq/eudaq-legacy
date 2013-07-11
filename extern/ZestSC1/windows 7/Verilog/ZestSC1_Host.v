/*
   ZestSC1 Host Interface Code
   File name: ZestSC1_Host.v
   Version: 1.10
   Date: 14/3/2006

   Copyright (C) 2005 Orange Tree Technologies Ltd. All rights reserved.
   Orange Tree Technologies grants the purchaser of a ZestSC1 the right to use and
   modify this logic core in any form including but not limited to Verilog source code or
   EDIF netlist in FPGA designs that target the ZestSC1.
   Orange Tree Technologies prohibits the use of this logic core or any modification of
   it in any form including but not limited to Verilog source code or EDIF netlist in
   FPGA or ASIC designs that target any other hardware unless the purchaser of the
   ZestSC1 has purchased the appropriate licence from Orange Tree Technologies.
   Contact Orange Tree Technologies if you want to purchase such a licence.

  *****************************************************************************************
  **
  **  Disclaimer: LIMITED WARRANTY AND DISCLAIMER. These designs are
  **              provided to you "as is". Orange Tree Technologies and its licensors 
  **              make and you receive no warranties or conditions, express, implied, 
  **              statutory or otherwise, and Orange Tree Technologies specifically 
  **              disclaims any implied warranties of merchantability, non-infringement,
  **              or fitness for a particular purpose. Orange Tree Technologies does not
  **              warrant that the functions contained in these designs will meet your 
  **              requirements, or that the operation of these designs will be 
  **              uninterrupted or error free, or that defects in the Designs will be 
  **              corrected. Furthermore, Orange Tree Technologies does not warrant or 
  **              make any representations regarding use or the results of the use of the 
  **              designs in terms of correctness, accuracy, reliability, or otherwise.                                               
  **
  **              LIMITATION OF LIABILITY. In no event will Orange Tree Technologies 
  **              or its licensors be liable for any loss of data, lost profits, cost or 
  **              procurement of substitute goods or services, or for any special, 
  **              incidental, consequential, or indirect damages arising from the use or 
  **              operation of the designs or accompanying documentation, however caused 
  **              and on any theory of liability. This limitation will apply even if 
  **              Orange Tree Technologies has been advised of the possibility of such 
  **              damage. This limitation shall apply notwithstanding the failure of the 
  **              essential purpose of any limited remedies herein.
  **
  *****************************************************************************************
*/

module ZestSC1_Host (
        // FPGA pin connections
        input USB_StreamCLK,
        output [1:0] USB_StreamFIFOADDR,
        output USB_StreamPKTEND_n,
        input [2:0] USB_StreamFlags_n,
        output USB_StreamSLOE_n,
        output USB_StreamSLRD_n,
        output USB_StreamSLWR_n,
        input USB_StreamFX2Rdy,
        inout [15:0] USB_StreamData,

        input USB_RegCLK,
        input [15:0] USB_RegAddr,
        inout [7:0] USB_RegData,
        input USB_RegOE_n,
        input USB_RegRD_n,
        input USB_RegWR_n,
        input USB_RegCS_n,

        output USB_Interrupt,

        // User connections
        // Streaming interface
        output User_CLK,
        output User_RST,
        output DCMLocked,

        input [11:0] User_StreamBusGrantLength,

        output [15:0] User_StreamDataIn,
        output User_StreamDataInWE,
        input User_StreamDataInBusy,

        input [15:0] User_StreamDataOut,
        input User_StreamDataOutWE,
        output User_StreamDataOutBusy,

        // Register interface
        output [15:0] User_RegAddr,
        output [7:0] User_RegDataIn,
        input [7:0] User_RegDataOut,
        output User_RegWE,
        output User_RegRE,

        // Interrupts
        input User_Interrupt
    );

    // General signals
    wire GSR;
    wire RST;
    wire StreamCLK;
    wire StreamCLKFB;
    wire RegCLK;
    wire RegCLKFB;

    // Streaming interface
    reg StreamRead;
    reg StreamWrite;
    reg [15:0] StreamDataIn;
    reg [15:0] StreamDataOut;
    reg DataOutRegFull;
    wire StreamBusy;
    wire StreamDataAvailable;
    wire StreamReadStrobe;
    wire WriteToFIFOIn;
    wire StreamWriteStrobe;
    wire ReadFromFIFOOut;
    wire ReadOK;
    wire WriteOK;
    reg ReadCycle;
    reg WriteCycle;
    wire User_DataInStrobe;
    reg [11:0] GrantPeriod;
    reg Granted;
    reg LastDir;

    reg FX2FIFOFull;
    reg FX2FIFOEmpty;

    reg [15:0] FIFOOut [15:0];
    reg [15:0] FIFOIn [15:0];
    wire FIFOInEmpty;

    reg [3:0] FIFOOutWriteCount;
    wire [3:0] FIFOOutWriteCountG;
    reg [3:0] RegFIFOOutWriteCountG;
    wire [3:0] FIFOOutWriteCountInUSB;

    reg [3:0] FIFOOutReadCount;
    wire [3:0] FIFOOutReadCountG;
    reg [3:0] RegFIFOOutReadCountG;
    wire [3:0] FIFOOutReadCountInUser;

    reg [3:0] FIFOInWriteCount;
    wire [3:0] FIFOInWriteCountG;
    reg [3:0] RegFIFOInWriteCountG;
    wire [3:0] FIFOInWriteCountInUser;

    reg [3:0] FIFOInReadCount;
    wire [3:0] FIFOInReadCountG;
    reg [3:0] RegFIFOInReadCountG;
    wire [3:0] FIFOInReadCountInUSB;

    wire [3:0] FIFOOutDataCount;
    wire [3:0] FIFOInDataCount;

    // Memory mapped interface
    wire RegCS;
    reg RegLastCS;
    wire RegOE;
    reg RegLastOE;
    wire RegWR;
    reg RegLastWR;
    wire RegRD;
    reg RegLastRD;
    reg [7:0] RegOutput;

    // Counter for interrupt generation
    reg [2:0] IntCounter;

    // Generate resets and clocks
    assign RST = GSR;
    assign User_RST = RST;
    assign User_CLK = RegCLK;

    /////////////////////////
    // Streaming interface //
    /////////////////////////

    // Control signals
    assign USB_StreamPKTEND_n = 1;
    assign USB_StreamSLRD_n = ~StreamReadStrobe;
    assign USB_StreamSLWR_n = ~StreamWriteStrobe;
    assign USB_StreamSLOE_n = (ReadCycle==1 || StreamRead==1) ? 0 : 1;
    assign USB_StreamData = (WriteCycle==1 || StreamWrite==1) ? StreamDataOut : 16'hZ;
    assign USB_StreamFIFOADDR = (ReadCycle==1 || StreamRead==1) ? 2'b00 : 2'b10;

    // Generate FIFO full and empty flags for the FX2
    // This must be done internally because the timing of the FX2 flags
    // and read/write strobes is such that we cannot respond to the flags
    // in one clock cycle.  The flags from the FX2 are therefore set to
    // one empty place (full flag) and one available word (empty flag).
    always @ (posedge RST or posedge StreamCLK)
        if (RST==1) begin
            FX2FIFOFull <= 1;
            FX2FIFOEmpty <= 1;
        end
        else begin
            if (USB_StreamFlags_n[0]==1 && USB_StreamFX2Rdy==1) begin
                FX2FIFOEmpty <= 0;
            end else if (StreamReadStrobe==1) begin
                FX2FIFOEmpty <= 1;
            end

            if (USB_StreamFlags_n[1]==1 && USB_StreamFX2Rdy==1) begin
                FX2FIFOFull <= 0;
            end else if (StreamWriteStrobe==1) begin
                FX2FIFOFull <= 1;
            end
        end

    // Read and write strobe generation and registering of input and output data
    assign StreamReadStrobe = (ReadCycle==0 && ReadOK==1) ? 1 : 0;
    assign WriteToFIFOIn = (ReadCycle==1) ? 1 : 0;
    assign ReadOK = (FX2FIFOEmpty==0 && StreamBusy==0 &&
                     USB_StreamFX2Rdy==1 && StreamRead==1) ? 1 : 0;
    always @ (posedge StreamCLK)
        if (StreamReadStrobe==1)
            StreamDataIn = USB_StreamData;

    assign StreamWriteStrobe = (WriteCycle==1) ? 1 : 0;
    assign ReadFromFIFOOut = (WriteCycle==0 && DataOutRegFull==0 &&
                              StreamDataAvailable==1) ? 1 : 0;
    assign WriteOK = (FX2FIFOFull==0 && (StreamDataAvailable==1 || DataOutRegFull==1) &&
                      USB_StreamFX2Rdy==1 && StreamWrite==1) ? 1 : 0;
    always @ (posedge RST or posedge StreamCLK)
        if (RST==1)
            DataOutRegFull <= 0;
        else begin
            if (ReadFromFIFOOut==1) begin
                StreamDataOut <= FIFOOut[FIFOOutReadCount];
                DataOutRegFull <= 1;
            end else if (StreamWriteStrobe==1) begin
                DataOutRegFull <= 0;
            end
        end

    // Control individual reads and writes from/to the FX2
    always @ (posedge RST or posedge StreamCLK)
        if (RST==1) begin
            ReadCycle <= 0;
            WriteCycle <= 0;
        end else begin

            if (ReadCycle==0)
                ReadCycle <= ReadOK;
            else
                ReadCycle <= 0;

            if (WriteCycle==0)
                WriteCycle <= WriteOK;
            else
                WriteCycle <= 0;

        end

    // Manage transfers
    // Checks whether a transfer is needed and possible
    // Use round robin to alternate between reads and writes
    //
    // GrantLength limits the length of time the streaming bus is granted to
    // reads or writes.  Set it low for rapid bus turn arounds at the expense
    // of overall bandwidth.  Set high for larger maximum bandwidth at the 
    // expense of slower bus turnarounds.
    always @ (posedge RST or posedge StreamCLK)
        if (RST==1) begin
            StreamRead <= 0;
            StreamWrite <= 0;
            GrantPeriod <= 0;
            Granted <= 0;
            FIFOInWriteCount <= 0;
            FIFOOutReadCount <= 0;
            LastDir <= 0;
        end else begin
            if (Granted==0) begin

                // Not currently in a transfer
                // Try to start a new one
                if (!(LastDir==0 && FX2FIFOFull==0 && (StreamDataAvailable==1 || DataOutRegFull==1)) &&
                    FX2FIFOEmpty==0 && StreamBusy==0) begin
                    // EP2 empty flag is not set and we can receive data
                    StreamRead <= 1;
                    StreamWrite <= 0;
                    GrantPeriod <= User_StreamBusGrantLength;
                    LastDir <= 0;
                    Granted <= 1;

                end else if (FX2FIFOFull==0 && (StreamDataAvailable==1 || DataOutRegFull==1)) begin
                    // EP6 full flag is clear and we have data
                    StreamRead <= 0;
                    StreamWrite <= 1;
                    GrantPeriod <= User_StreamBusGrantLength;
                    LastDir <= 1;
                    Granted <= 1;
                end

            end else begin

                GrantPeriod <= GrantPeriod - 1;
                if (GrantPeriod==4) begin
                    // Take grant away early as the accesses take more than 1 cycle
                    StreamRead <= 0;
                    StreamWrite <= 0;
                end
                if (GrantPeriod==1) begin
                    Granted <= 0;
                end

            end

            if (WriteToFIFOIn==1) 
                FIFOInWriteCount <= FIFOInWriteCount + 1;
            if (ReadFromFIFOOut==1)
                FIFOOutReadCount <= FIFOOutReadCount + 1;

        end

    // Short FIFOs for crossing clock domains
    // User domain -> USB
    always @ (posedge RST or posedge RegCLK)
        if (RST==1)
            FIFOOutWriteCount <= 0;
        else
            if (User_StreamDataOutWE==1)
                FIFOOutWriteCount <= FIFOOutWriteCount + 1;
    always @ (posedge RegCLK)
        if (User_StreamDataOutWE==1)
            FIFOOut[FIFOOutWriteCount] <= User_StreamDataOut;

    // USB -> User domain
    always @ (posedge RST or posedge RegCLK)
        if (RST==1)
            FIFOInReadCount <= 0;
        else
            if (User_DataInStrobe==1)
                FIFOInReadCount <= FIFOInReadCount + 1;
    always @ (posedge StreamCLK)
        if (WriteToFIFOIn==1)
            FIFOIn[FIFOInWriteCount] <= StreamDataIn;

    // Note fixup for behavioural simulation - holds the data during write strobe
    assign #1 User_StreamDataIn = FIFOIn[FIFOInReadCount];
    assign User_StreamDataInWE = User_DataInStrobe;
    assign User_DataInStrobe = ~FIFOInEmpty & ~User_StreamDataInBusy;

    // Generate busy flag to User
    // Grey code the DataOut read counter, cross clock domain and decode
    assign FIFOOutReadCountG = { FIFOOutReadCount[3],
                                 FIFOOutReadCount[3] ^ FIFOOutReadCount[2],
                                 FIFOOutReadCount[2] ^ FIFOOutReadCount[1],
                                 FIFOOutReadCount[1] ^ FIFOOutReadCount[0] };
    always @ (posedge RST or posedge RegCLK)
        if (RST==1)
            RegFIFOOutReadCountG <= 0;
        else 
            RegFIFOOutReadCountG <= FIFOOutReadCountG;
    assign FIFOOutReadCountInUser = { RegFIFOOutReadCountG[3],
                                      RegFIFOOutReadCountG[3] ^ RegFIFOOutReadCountG[2],
                                      RegFIFOOutReadCountG[3] ^ RegFIFOOutReadCountG[2] ^ RegFIFOOutReadCountG[1],
                                      RegFIFOOutReadCountG[3] ^ RegFIFOOutReadCountG[2] ^ RegFIFOOutReadCountG[1] ^ RegFIFOOutReadCountG[0] };
    assign FIFOOutDataCount = FIFOOutWriteCount-FIFOOutReadCountInUser;
    assign User_StreamDataOutBusy = (FIFOOutDataCount[3]==1) ? 1 : 0;

    // Generate write enable strobe to the User
    // Grey code the DataIn write counter, cross clock domain and decode
    assign FIFOInWriteCountG = { FIFOInWriteCount[3],
                                 FIFOInWriteCount[3] ^ FIFOInWriteCount[2],
                                 FIFOInWriteCount[2] ^ FIFOInWriteCount[1],
                                 FIFOInWriteCount[1] ^ FIFOInWriteCount[0] };
    always @ (posedge RST or posedge RegCLK)
        if (RST==1)
            RegFIFOInWriteCountG <= 0;
        else
            RegFIFOInWriteCountG <= FIFOInWriteCountG;
    assign FIFOInWriteCountInUser = { RegFIFOInWriteCountG[3], 
                                      RegFIFOInWriteCountG[3] ^ RegFIFOInWriteCountG[2],
                                      RegFIFOInWriteCountG[3] ^ RegFIFOInWriteCountG[2] ^ RegFIFOInWriteCountG[1],
                                      RegFIFOInWriteCountG[3] ^ RegFIFOInWriteCountG[2] ^ RegFIFOInWriteCountG[1] ^ RegFIFOInWriteCountG[0] };
    assign FIFOInEmpty = (FIFOInWriteCountInUser==FIFOInReadCount) ? 1 : 0;

    // Generate 'data available' flag to the USB chip
    assign FIFOOutWriteCountG = { FIFOOutWriteCount[3],
                                  FIFOOutWriteCount[3] ^ FIFOOutWriteCount[2],
                                  FIFOOutWriteCount[2] ^ FIFOOutWriteCount[1],
                                  FIFOOutWriteCount[1] ^ FIFOOutWriteCount[0] };
    always @ (posedge RST or posedge StreamCLK)
        if (RST==1)
            RegFIFOOutWriteCountG <= 0;
        else
            RegFIFOOutWriteCountG <= FIFOOutWriteCountG;
    assign FIFOOutWriteCountInUSB = { RegFIFOOutWriteCountG[3],
                                      RegFIFOOutWriteCountG[3] ^ RegFIFOOutWriteCountG[2],
                                      RegFIFOOutWriteCountG[3] ^ RegFIFOOutWriteCountG[2] ^ RegFIFOOutWriteCountG[1],
                                      RegFIFOOutWriteCountG[3] ^ RegFIFOOutWriteCountG[2] ^ RegFIFOOutWriteCountG[1] ^ RegFIFOOutWriteCountG[0] };
    assign StreamDataAvailable = (FIFOOutWriteCountInUSB!=FIFOOutReadCount) ? 1 : 0;

    // Generate 'space available' flag to the USB chip
    assign FIFOInReadCountG = { FIFOInReadCount[3],
                                FIFOInReadCount[3] ^ FIFOInReadCount[2],
                                FIFOInReadCount[2] ^ FIFOInReadCount[1],
                                FIFOInReadCount[1] ^ FIFOInReadCount[0] };
    always @ (posedge RST or posedge StreamCLK)
        if (RST==1)
            RegFIFOInReadCountG <= 0;
        else
            RegFIFOInReadCountG <= FIFOInReadCountG;
    assign FIFOInReadCountInUSB = { RegFIFOInReadCountG[3],
                                    RegFIFOInReadCountG[3] ^ RegFIFOInReadCountG[2],
                                    RegFIFOInReadCountG[3] ^ RegFIFOInReadCountG[2] ^ RegFIFOInReadCountG[1],
                                    RegFIFOInReadCountG[3] ^ RegFIFOInReadCountG[2] ^ RegFIFOInReadCountG[1] ^ RegFIFOInReadCountG[0] };
    assign FIFOInDataCount = FIFOInWriteCount-FIFOInReadCountInUSB;
    assign StreamBusy = (FIFOInDataCount[3]==1) ? 1 : 0;


    /////////////////////////////
    // Memory mapped interface //
    /////////////////////////////
    assign User_RegAddr = USB_RegAddr - 16'h2000; // 8051 external memory starts at 0x2000
    assign User_RegWE = RegCS & RegLastCS & RegWR & RegLastWR;
    assign User_RegRE = RegCS & ~RegLastCS & RegRD & ~RegLastRD;
    assign User_RegDataIn = USB_RegData;

    assign RegCS = ~USB_RegCS_n;
    assign RegOE = ~USB_RegOE_n;
    assign RegWR = ~USB_RegWR_n;
    assign RegRD = ~USB_RegRD_n;
    always @ (posedge RegCLK)
        begin
            RegLastCS <= RegCS;
            RegLastRD <= RegRD;
            RegLastWR <= RegWR;
            RegLastOE <= RegOE;
        end

    // Register read interface
    always @ (posedge RegCLK)
        RegOutput <= User_RegDataOut;

    assign USB_RegData = (RegRD==1 && RegCS==1 && RegOE==1) ? RegOutput : 16'hZ;


    ///////////////////////////
    // Generate an interrupt //
    ///////////////////////////
    // Interrupt is active low, edge triggered and must be held for
    // 4 cycles of the register interface clock
    always @ (posedge RST or posedge RegCLK)
        if (RST==1)
            IntCounter <= 0;
        else begin
            if (User_Interrupt==1)
                IntCounter <= 4;
            else if (IntCounter!=0)
                IntCounter <= IntCounter + 1;
        end
    assign USB_Interrupt = ~IntCounter[2];


    ///////////////////////////////
    // Instatiate DCMs on clocks //
    ///////////////////////////////
    DCM StreamDCM (
        .CLK0(StreamCLKFB),
        .CLKFB(StreamCLK),
        .CLKIN(USB_StreamCLK),
        .RST(RST),
        .PSCLK(1'b0),
        .PSEN(1'b0),
        .PSINCDEC(1'b0),
        .DSSEN(1'b0)
    );
// synthesis attribute CLK_FEEDBACK of StreamDCM is "1X"
// synthesis attribute CLKDV_DIVIDE of StreamDCM is 2
// synthesis attribute CLKFX_DIVIDE of StreamDCM is 1
// synthesis attribute CLKFX_MULTIPLY of StreamDCM is 4
// synthesis attribute CLKIN_DIVIDE_BY_2 of StreamDCM is "FALSE"
// synthesis attribute CLKIN_PERIOD of StreamDCM is 20.8
// synthesis attribute CLKOUT_PHASE_SHIFT of StreamDCM is "NONE"
// synthesis attribute DESKEW_ADJUST of StreamDCM is "SYSTEM_SYNCHRONOUS"
// synthesis attribute DFS_FREQUENCY_MODE of StreamDCM is "LOW"
// synthesis attribute DLL_FREQUENCY_MODE of StreamDCM is "LOW"
// synthesis attribute DUTY_CYCLE_CORRECTION of StreamDCM is "TRUE"
// synthesis attribute PHASE_SHIFT of StreamDCM is 0
// synthesis attribute STARTUP_WAIT of StreamDCM is "FALSE"
// synthesis translate_off
defparam StreamDCM.CLK_FEEDBACK="1X";
defparam StreamDCM.CLKDV_DIVIDE=2;
defparam StreamDCM.CLKFX_DIVIDE=1;
defparam StreamDCM.CLKFX_MULTIPLY=2;
defparam StreamDCM.CLKIN_DIVIDE_BY_2="FALSE";
defparam StreamDCM.CLKIN_PERIOD=20.8;
defparam StreamDCM.CLKOUT_PHASE_SHIFT="NONE";
defparam StreamDCM.DESKEW_ADJUST="SYSTEM_SYNCHRONOUS";
defparam StreamDCM.DFS_FREQUENCY_MODE="LOW";
defparam StreamDCM.DLL_FREQUENCY_MODE="LOW";
defparam StreamDCM.DUTY_CYCLE_CORRECTION="TRUE";
defparam StreamDCM.PHASE_SHIFT=0;
defparam StreamDCM.STARTUP_WAIT="FALSE";
// synthesis translate_on 

    BUFG StreamCLK_BUFG (
        .I (StreamCLKFB),
        .O (StreamCLK)
    );

    DCM RegDCM (
        .CLK0(RegCLKFB),
        .CLKFB(RegCLK),
        .CLKIN(USB_RegCLK),
        .RST(RST),
        .PSCLK(1'b0),
        .PSEN(1'b0),
        .PSINCDEC(1'b0),
        .DSSEN(1'b0),
        .LOCKED(DCMLocked)
    );
// synthesis attribute CLK_FEEDBACK of StreamDCM is "1X"
// synthesis attribute CLKDV_DIVIDE of StreamDCM is 2
// synthesis attribute CLKFX_DIVIDE of StreamDCM is 1
// synthesis attribute CLKFX_MULTIPLY of StreamDCM is 4
// synthesis attribute CLKIN_DIVIDE_BY_2 of StreamDCM is "FALSE"
// synthesis attribute CLKIN_PERIOD of StreamDCM is 20.8
// synthesis attribute CLKOUT_PHASE_SHIFT of StreamDCM is "NONE"
// synthesis attribute DESKEW_ADJUST of StreamDCM is "SYSTEM_SYNCHRONOUS"
// synthesis attribute DFS_FREQUENCY_MODE of StreamDCM is "LOW"
// synthesis attribute DLL_FREQUENCY_MODE of StreamDCM is "LOW"
// synthesis attribute DUTY_CYCLE_CORRECTION of StreamDCM is "TRUE"
// synthesis attribute PHASE_SHIFT of StreamDCM is 0
// synthesis attribute STARTUP_WAIT of StreamDCM is "FALSE"
// synthesis translate_off
defparam StreamDCM.CLK_FEEDBACK="1X";
defparam StreamDCM.CLKDV_DIVIDE=2;
defparam StreamDCM.CLKFX_DIVIDE=1;
defparam StreamDCM.CLKFX_MULTIPLY=2;
defparam StreamDCM.CLKIN_DIVIDE_BY_2="FALSE";
defparam StreamDCM.CLKIN_PERIOD=20.8;
defparam StreamDCM.CLKOUT_PHASE_SHIFT="NONE";
defparam StreamDCM.DESKEW_ADJUST="SYSTEM_SYNCHRONOUS";
defparam StreamDCM.DFS_FREQUENCY_MODE="LOW";
defparam StreamDCM.DLL_FREQUENCY_MODE="LOW";
defparam StreamDCM.DUTY_CYCLE_CORRECTION="TRUE";
defparam StreamDCM.PHASE_SHIFT=0;
defparam StreamDCM.STARTUP_WAIT="FALSE";
// synthesis translate_on 

    BUFG RegCLK_BUFG (
        .I (RegCLKFB),
        .O (RegCLK)
    );

endmodule
