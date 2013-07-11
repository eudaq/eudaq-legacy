/*
   ZestSC1 Example 4
   File name: Example4.v
   Version: 1.10
   Date: 6/2/2006

  
   SRAM test
   DMA controller between host and SRAM and back again
   Registers are:
        Offset      Register
            0       SRAM Address Low
            1       SRAM Address Middle
            2       SRAM Address High
            3       SRAM Write Counter Low
            4       SRAM Write Counter Middle
            5       SRAM Write Counter High
            6       SRAM Read Counter Low
            7       SRAM Read Counter Middle
            8       SRAM Read Counter High (Write to this starts read)
  

   Copyright (C) 2005-2006 Orange Tree Technologies Ltd. All rights reserved.
   Orange Tree Technologies grants the purchaser of a ZestSC1 the right to use and 
   modify this logic core in any form such as Verilog source code or EDIF netlist in 
   FPGA designs that target the ZestSC1.
   Orange Tree Technologies prohibits the use of this logic core in any form such 
   as Verilog source code or EDIF netlist in FPGA designs that target any other
   hardware unless the purchaser of the ZestSC1 has purchased the appropriate 
   licence from Orange Tree Technologies. Contact Orange Tree Technologies if you 
   want to purchase such a licence.

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

module Example4 (
        input USB_StreamCLK,
        output [1:0] USB_StreamFIFOADDR,
        output USB_StreamPKTEND_n,
        input [2:0] USB_StreamFlags_n,
        output USB_StreamSLOE_n,
        output USB_StreamSLRD_n,
        output USB_StreamSLWR_n,
        inout [15:0] USB_StreamData,
        input USB_StreamFX2Rdy,

        input USB_RegCLK,
        input [15:0] USB_RegAddr,
        inout [7:0] USB_RegData,
        input USB_RegOE_n,
        input USB_RegRD_n,
        input USB_RegWR_n,
        input USB_RegCS_n,

        output USB_Interrupt,

        inout [7:0] User_Signals,

        output S_CLK,
        output [22:0] S_A,
        inout [8:0] S_DA,
        inout [8:0] S_DB,
        output S_ADV_LD_N,
        output S_BWA_N,
        output S_BWB_N,
        output S_OE_N,
        output S_WE_N,

        inout IO_CLK_N,
        inout IO_CLK_P,
        inout [46:0] IO
    );

    // Declare signals
    wire CLK;
    wire RST;
    wire [7:0] LEDs;

    // Streaming interface
    wire [15:0] DataIn;
    wire DataInBusy;
    wire DataInWE;

    wire [15:0] DataOut;
    wire DataOutBusy;
    wire DataOutWE;

    // Register interface
    wire [15:0] RegAddr;
    wire [7:0] RegDataIn;
    wire [7:0] RegDataOut;
    wire RegWE;
    wire RegRE;

    // SRAM interface
    wire [22:0] SRAMAddr;
    wire [17:0] SRAMDataOut;
    wire [17:0] SRAMDataIn;
    wire SRAMWE;
    wire SRAMRE;
    wire SRAMValid;

    // Registers
    reg [23:0] Address;
    reg [23:0] WriteCount;
    reg WriteActive;
    reg [23:0] ReadCount;
    reg ReadActive;
    reg LastReadActive;
    reg ReadState;
    reg LastSRAMValid;
    reg [15:0] SRAMWriteReg;
    reg [1:0] SRAMReadReg;

    // Tie unused signals
    assign LEDs = 8'b11111111;
    assign User_Signals = 8'hZ;
    assign IO_CLK_N = 1'bZ;
    assign IO_CLK_P = 1'bZ;
    assign IO = {LEDs[7:2], 39'hZ, LEDs[1:0]};

    // Implement DMA controllers
    always @ (posedge RST or posedge CLK)
        if (RST==1) begin
            ReadCount <= 0;
            WriteCount <= 0;
            Address <= 0;
            ReadActive <= 0;
            WriteActive <= 0;
            LastSRAMValid <= 0;
            ReadState <= 0;

        end else begin
            if (RegWE==1) begin

                // Register access from the host
                case (RegAddr)
                    16'h2000: Address[7:0] <= RegDataIn;
                    16'h2001: Address[15:8] <= RegDataIn;
                    16'h2002: Address[23:16] <= RegDataIn;
                    16'h2003: WriteCount[7:0] <= RegDataIn;
                    16'h2004: WriteCount[15:8] <= RegDataIn;
                    16'h2005: begin WriteCount[23:16] <= RegDataIn; WriteActive<=1; end
                    16'h2006: ReadCount[7:0] <= RegDataIn; 
                    16'h2007: ReadCount[15:8] <= RegDataIn;
                    16'h2008: begin ReadCount[23:16] <= RegDataIn; ReadState <= 0; ReadActive<=1; end
                endcase

            end else begin

                if (WriteActive==1 && WriteCount!=0) begin

                    // DMA controller from Host to the SRAM
                    if (DataInWE==1) begin
                        if (Address[0]==0) 
                            SRAMWriteReg <= DataIn;
                        Address <= Address + 1;
                        WriteCount <= WriteCount - 1;
                        if (WriteCount==1)
                            WriteActive <= 0;
                    end

                end else if (ReadActive==1 && ReadCount!=0) begin

                    // DMA controller from SRAM to the Host
                    // Only issue one read at a time to ensure the FIFO to
                    // the host does not overflow.
                    if (ReadState==0) begin
                        if (DataOutBusy==0) begin
                            Address <= Address + 2;
                            ReadState <= 1;
                        end
                    end else begin
                        if (SRAMValid==1) begin
                            SRAMReadReg <= SRAMDataIn[17:16];
                            ReadCount <= ReadCount - 2;
                            if (ReadCount==2)
                                ReadActive <= 0;
                            ReadState <= 0;
                        end
                    end
                end
            end
            LastSRAMValid <= SRAMValid;
            LastReadActive <= ReadActive;
        end

    // Connect signals for Host to SRAM transfers
    assign DataInBusy = (~WriteActive) | RegWE;
    assign SRAMAddr = Address[23:1];
    assign SRAMWE = DataInWE & Address[0];
    assign SRAMDataOut = { DataIn[1:0], SRAMWriteReg };

    // Connect signals for SRAM to Host transfers
    assign DataOutWE = (ReadActive | LastReadActive) & (SRAMValid | LastSRAMValid);
    assign DataOut = (SRAMValid==1) ? SRAMDataIn[15:0] : {14'b0, SRAMReadReg};
    assign SRAMRE = (DataOutBusy==0 && ReadActive==1 && ReadState==0 && RegWE==0) ? 1 : 0;

    // Instantiate interfaces component
    ZestSC1_Interfaces Interfaces (
            .USB_StreamCLK (USB_StreamCLK),
            .USB_StreamFIFOADDR (USB_StreamFIFOADDR),
            .USB_StreamPKTEND_n (USB_StreamPKTEND_n),
            .USB_StreamFlags_n (USB_StreamFlags_n),
            .USB_StreamSLOE_n (USB_StreamSLOE_n),
            .USB_StreamSLRD_n (USB_StreamSLRD_n),
            .USB_StreamSLWR_n (USB_StreamSLWR_n),
            .USB_StreamData (USB_StreamData),
            .USB_StreamFX2Rdy (USB_StreamFX2Rdy),

            .USB_RegCLK (USB_RegCLK),
            .USB_RegAddr (USB_RegAddr),
            .USB_RegData (USB_RegData),
            .USB_RegOE_n (USB_RegOE_n),
            .USB_RegRD_n (USB_RegRD_n),
            .USB_RegWR_n (USB_RegWR_n),
            .USB_RegCS_n (USB_RegCS_n),

            .USB_Interrupt (USB_Interrupt),

            .S_CLK (S_CLK),
            .S_A (S_A),
            .S_ADV_LD_N (S_ADV_LD_N),
            .S_BWA_N (S_BWA_N),
            .S_BWB_N (S_BWB_N),
            .S_DA (S_DA),
            .S_DB (S_DB),
            .S_OE_N (S_OE_N),
            .S_WE_N (S_WE_N),

            // User connections
            // Streaming interface
            .User_CLK (CLK),
            .User_RST (RST),

            .User_StreamBusGrantLength (12'h100),

            .User_StreamDataIn (DataIn),
            .User_StreamDataInWE (DataInWE),
            .User_StreamDataInBusy (DataInBusy),

            .User_StreamDataOut (DataOut), 
            .User_StreamDataOutWE (DataOutWE),
            .User_StreamDataOutBusy (DataOutBusy),

            // Register interface
            .User_RegAddr (RegAddr),
            .User_RegDataIn (RegDataIn),
            .User_RegDataOut (RegDataOut),
            .User_RegWE (RegWE),
            .User_RegRE (RegRE),

            // Interrupts
            .User_Interrupt (1'b0),

            // SRAM interface
            .User_SRAM_A (SRAMAddr),
            .User_SRAM_W (SRAMWE),
            .User_SRAM_R (SRAMRE),
            .User_SRAM_DR_VALID (SRAMValid),
            .User_SRAM_DW (SRAMDataOut),
            .User_SRAM_DR (SRAMDataIn)
        );

endmodule
