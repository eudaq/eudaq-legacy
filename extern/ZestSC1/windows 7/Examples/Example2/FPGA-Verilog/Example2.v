/*
   ZestSC1 Example 2
   File name: Example2.v
   Version: 1.00
   Date: 26/4/2005

  
   Infinite sink and source for the streaming interface
   Measures bandwidth to and from the host
  

   Copyright (C) 2005 Orange Tree Technologies Ltd. All rights reserved.
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

module Example2 (
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

    wire [15:0] DataIn;
    wire DataInBusy;
    wire DataInWE;
    reg [15:0] Ramp;
    reg RampError;

    reg [15:0] DataOut;
    wire DataOutBusy;
    wire DataOutWE;
    reg SendData;

    // Tie unused signals
    assign User_Signals = 8'hZ;
    assign LEDs[7:1] = 7'b1111111;
    assign IO_CLK_N = 1'bZ;
    assign IO_CLK_P = 1'bZ;
    assign IO = {LEDs[7:2], 39'hZ, LEDs[1:0]};

    // Always receive data from the host
    assign DataInBusy = 0;
    always @ (posedge RST or posedge CLK)
        if (RST==1) begin
            RampError <= 0;
            Ramp <= 16'b0;
        end else begin
            if (DataInWE==1) begin
                if (DataIn!=Ramp)
                    RampError <= 1;
                Ramp <= Ramp + 1;
            end
        end
    assign LEDs[0] = ~RampError;

    // Generate ramp to the host
    // Wait until data arrives from the host to avoid sending too early
    assign DataOutWE = (DataOutBusy==0 && SendData==1) ? 1 : 0;
    always @ (posedge RST or posedge CLK)
        if (RST==1) begin
            DataOut <= 16'b0;
            SendData <= 0;
        end else begin
            if (DataInWE==1)
                SendData <= 1;
            if (DataOutBusy==0 && SendData==1)
                DataOut <= DataOut + 1;
        end

    // Instantiate interfaces component
    ZestSC1_Interfaces Interfaces (
            .USB_StreamCLK (USB_StreamCLK),
            .USB_StreamFIFOADDR (USB_StreamFIFOADDR),
            .USB_StreamPKTEND_n (USB_StreamPKTEND_n),
            .USB_StreamFlags_n (USB_StreamFlags_n),
            .USB_StreamSLOE_n (USB_StreamSLOE_n),
            .USB_StreamSLRD_n (USB_StreamSLRD_n),
            .USB_StreamSLWR_n (USB_StreamSLWR_n),
            .USB_StreamFX2Rdy (USB_StreamFX2Rdy),
            .USB_StreamData (USB_StreamData),

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

            .User_StreamBusGrantLength (12'h100), // 256 cycles = 512 bytes

            .User_StreamDataIn (DataIn),
            .User_StreamDataInWE (DataInWE),
            .User_StreamDataInBusy (DataInBusy),

            .User_StreamDataOut (DataOut), 
            .User_StreamDataOutWE (DataOutWE),
            .User_StreamDataOutBusy (DataOutBusy),

            // Register interface
            //.User_RegAddr
            //.User_RegDataIn
            .User_RegDataOut (8'b0),
            //.User_RegWE
            //.User_RegRE

            // Signals and interrupts
            .User_Interrupt (1'b0),

            // SRAM interface
            .User_SRAM_A (23'b0),
            .User_SRAM_W (1'b0),
            .User_SRAM_R (1'b0),
            //.User_SRAM_DR_VALID
            .User_SRAM_DW (18'b0)
            //User_SRAM_DR
        );

endmodule
