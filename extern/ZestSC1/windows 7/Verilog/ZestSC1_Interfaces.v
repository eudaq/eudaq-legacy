/* ZestSC1 Top Level Code
   File name: ZestSC1_Interfaces.v
   Version: 1.00
   Date: 26/4/2005

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

module ZestSC1_Interfaces (
        // User connections
        // General connections
        output User_CLK,				                   // User logic clock
        output User_RST,                                   // User logic reset

        // USB Streaming interface
        input [11:0] User_StreamBusGrantLength,			   // Round robin grant length
                                                           // Controls read and write grant times
                                                           // on the streaming bus

        output [15:0] User_StreamDataIn,                   // Stream data from host
        output User_StreamDataInWE,                        // Stream write strobe from host
        input User_StreamDataInBusy,                       // Busy for stream from host

        input [15:0] User_StreamDataOut,                   // Stream data to host
        input User_StreamDataOutWE,                        // Stream write strobe to host
        output User_StreamDataOutBusy,                     // Busy for stream to host

        // USB Register interface
        output [15:0] User_RegAddr,                        // Register interface address
        output [7:0] User_RegDataIn,                       // Register write data
        input [7:0] User_RegDataOut,                       // Register read data
        output User_RegWE,                                 // Write strobe for register
        output User_RegRE,                                 // Read strobe for register

        // USB Interrupts
        input User_Interrupt,                              // Interrupt to host PC

        // SRAM interface
        input [22:0] User_SRAM_A,                          // 23-bit address
        input User_SRAM_W,                                 // write strobe active high
        input User_SRAM_R,                                 // read strobe active high
        output User_SRAM_DR_VALID,                         // read data valid strobe active high
        input [17:0] User_SRAM_DW,                         // 18-bit data bus for writing to SRAM
        output [17:0] User_SRAM_DR,                        // 18-bit data bus for reading from SRAM

        // FPGA pin connections
        // External USB Controller interface
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

        // External SRAM interface
        output S_CLK,
        output [22:0] S_A,
        output S_ADV_LD_N,
        output S_BWA_N,
        output S_BWB_N,
        inout [8:0] S_DA,
        inout [8:0] S_DB,
        output S_OE_N,
        output S_WE_N
	);

	// Declare signals
	wire Clk;
	wire Reset;
	wire SRAMReset;
	wire DCMLocked;

	// Continuous assignments
	assign User_CLK = Clk;
	assign User_RST = Reset;
	assign SRAMReset = Reset | ~DCMLocked;

	// Instantiate Host Interface component
	ZestSC1_Host INST_ZestSC1_Host (
        // FPGA pin connections
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

        // User connections
        // General connections
        .User_CLK (Clk),
        .User_RST (Reset),
        .DCMLocked (DCMLocked),
		
        // Streaming interface
        .User_StreamBusGrantLength (User_StreamBusGrantLength),

        .User_StreamDataIn (User_StreamDataIn),
        .User_StreamDataInWE (User_StreamDataInWE),
        .User_StreamDataInBusy (User_StreamDataInBusy),

        .User_StreamDataOut (User_StreamDataOut),
        .User_StreamDataOutWE (User_StreamDataOutWE),
        .User_StreamDataOutBusy (User_StreamDataOutBusy),

        // Register interface
        .User_RegAddr (User_RegAddr),
        .User_RegDataIn (User_RegDataIn),
        .User_RegDataOut (User_RegDataOut),
        .User_RegWE (User_RegWE),
        .User_RegRE (User_RegRE),

        // Interrupts
        .User_Interrupt (User_Interrupt)
    );

	ZestSC1_SRAM INST_SRAM (
        // User interface
        .USER_CLK (Clk),
        .USER_RESET (SRAMReset),
        .USER_A (User_SRAM_A),
        .USER_W (User_SRAM_W),
        .USER_R (User_SRAM_R),
        .USER_DR_VALID (User_SRAM_DR_VALID),
        .USER_DW (User_SRAM_DW),
        .USER_DR (User_SRAM_DR),

        // ZBT SRAM interface
        .CLK_SRAM (S_CLK),
        .S_A (S_A),    
        .S_ADV_LD_N (S_ADV_LD_N),
        .S_BWA_N (S_BWA_N),
        .S_BWB_N (S_BWB_N),
        .S_DA (S_DA),
        .S_DB (S_DB),
        .S_OE_N (S_OE_N),
        .S_WE_N (S_WE_N)
    );


endmodule
