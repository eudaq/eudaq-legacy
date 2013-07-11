/*
   ZestSC1 Example 3
   File name: Example3.v
   Version: 1.10
   Date: 6/2/2006

  
   ZestSC1 Example 3
   Register implementation, loopback of signals and interrupt generation
  

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

module Example3 (
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

    // Register interface
    wire [15:0] Addr;
    wire [7:0] DataIn;
    reg [7:0] DataOut;
    wire WE;
    wire RE;

    // Interrupt signal
    reg Interrupt;

    // Registers
    reg [7:0] Value;

    // Signals
    reg [3:0] Sig;

    // Tie unused signals
    assign LEDs = 8'b11111111;
    assign IO_CLK_N = 1'bZ;
    assign IO_CLK_P = 1'bZ;
    assign IO = {LEDs[7:2], 39'hZ, LEDs[1:0]};

    // Implement register write
    // Note that for compatibility with the FX2LP device only registers
    // above 2000 Hex are used
    always @ (posedge RST or posedge CLK)
        if (RST==1)
            Value <= 0;
        else begin
            if (WE==1)
                case (Addr)
                    16'h2000: Interrupt <= 1;
                    16'h207B: begin Value <= DataIn + 1; Interrupt <= 0; end // 0x2000 + 123 decimal
                    default:  Interrupt <= 0;
                endcase
            else
                Interrupt <= 0;

        end

    // Implement register read
    always @ (Addr or Value)
        case (Addr)
            16'h207B: DataOut <= Value; // 0x2000 + 123 decimal
            default:  DataOut <= 0;
        endcase

    // Loop back signals
    assign User_Signals[7:4] = Sig;
    always @ (posedge CLK)
        Sig <= User_Signals[3:0];

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

            .User_StreamBusGrantLength (12'h002),

            //.User_StreamDataIn
            //.User_StreamDataInWE
            .User_StreamDataInBusy (1'b1),

            .User_StreamDataOut (16'b0),
            .User_StreamDataOutWE (1'b0),
            //.User_StreamDataOutBusy

            // Register interface
            .User_RegAddr (Addr),
            .User_RegDataIn (DataIn),
            .User_RegDataOut (DataOut),
            .User_RegWE (WE),
            .User_RegRE (RE),

            // Interrupts
            .User_Interrupt (Interrupt),

            // SRAM interface
            .User_SRAM_A (23'b0),
            .User_SRAM_W (1'b0),
            .User_SRAM_R (1'b0),
            //.User_SRAM_DR_VALID
            .User_SRAM_DW (18'b0)
            //.User_SRAM_DR
        );

endmodule
