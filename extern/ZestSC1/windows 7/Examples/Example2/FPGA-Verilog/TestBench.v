/*
   ZestSC1 Example 2 Test Bench
   File name: TestBench.v
   Version: 1.00
   Date: 25/4/2005

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

`timescale 1 ns / 1ps

module ZestSC1TestBench ();

	// Declare signals
	reg USB_StreamCLK;
	wire USB_StreamFX2Rdy;
	reg [2:0] USB_StreamFlags_n;
	wire [1:0] USB_StreamFIFOADDR;
	wire USB_StreamPKTEND_n;
	wire USB_StreamSLOE_n;
	wire USB_StreamSLRD_n;
	wire USB_StreamSLWR_n;
	wire [15:0] USB_StreamData;
	reg [15:0] StreamData;

	reg USB_RegCLK;
	wire USB_RegOE_n;
	wire USB_RegRD_n;
	wire USB_RegWR_n;
	wire USB_RegCS_n;
	wire [15:0] USB_RegAddr;
	wire [7:0] USB_RegData;

	wire [8:0] S_DA;
	wire [8:0] S_DB;
	wire S_CLK;
	wire [22:0] S_A;
	wire S_ADV_LD_N;
	wire S_BWA_N;
	wire S_BWB_N;
	wire S_OE_N;
	wire S_WE_N;

	wire [7:0] User_Signals;
	wire USB_Interrupt;
	wire IO_CLK_N;
	wire IO_CLK_P;
	wire [46:0] IO;

    // SRAM simulation
    //reg [17:0] SRAM [0:(512*1024-1)];   // 1MB
    reg [17:0] SRAM [0:(4*1024*1024-1)];  // 8MB
    reg [17:0] SRAMData1;
    reg [17:0] SRAMData2;
    reg [22:0] SRAMAddr1;
    reg [22:0] SRAMAddr2;
    reg SRAMWE1;
    reg SRAMWE2;
    wire [8:0] DataA;
    wire [8:0] DataB;

    // File descriptors
    integer OutFile;
    integer InFile;
    integer endfile = 1;

    // Tied signals
    assign USB_StreamFX2Rdy = 1;
    assign uut.Interfaces.INST_ZestSC1_Host.GSR = glbl.GSR;

    // 48 MHz clocks
    // In simulation, these can be in sync but in general they are not
	always begin
        USB_StreamCLK <= 1;
        USB_RegCLK <= 1;
        # 10.416667
        USB_StreamCLK <= 0;
        USB_RegCLK <= 0;
        # 10.416667;
	end

    //////////////////////////////////////////////////////////////////
    // Simulate the streaming interface
    //////////////////////////////////////////////////////////////////

    // Simulation implements an infinite source and sink
    // NB: This does not simulate the complex timing of the full
    // USB interface, host driver and application.  It is merely
    // provided to check the algorithm under test is correct.
    initial begin
        // Wait for the completion of reset
	    USB_StreamFlags_n[0] <= 0; // USB device FIFO empty signal
	    USB_StreamFlags_n[1] <= 0; // USB device FIFO full signal
        USB_StreamFlags_n[2] <= 0; // Not used
        # 1000
	    USB_StreamFlags_n[0] <= 1; // USB device FIFO empty signal
	    USB_StreamFlags_n[1] <= 1; // USB device FIFO full signal
    end
    assign USB_StreamData = (USB_StreamSLOE_n==0) ? StreamData : 16'hZ;

    // Write data sent to the host to a file
    initial begin
        OutFile = $fopen("OutFile.txt", "w");

        while (1) begin
            # 10.416667;
            if (USB_StreamSLWR_n==0)
                $fdisplay(OutFile, "%d", USB_StreamData);
            $fflush(OutFile);
            # 10.416667;
        end
    end
    
    // Read simulation data from the host from a file
    initial begin
        InFile = $fopen("InFile.txt", "r");

        StreamData <= 0;
        # 10.416667;
        while (endfile>0) begin
            if (USB_StreamSLRD_n==0) begin
                endfile = $fscanf(InFile, "%d", StreamData);
            end
            # 20.833334;
        end
    end 

    //////////////////////////////////////////////////////////////////
    // Simulate register reads and writes
    //////////////////////////////////////////////////////////////////
    
    // Registers not used in this example
    assign USB_RegOE_n = 1;
    assign USB_RegRD_n = 1;
    assign USB_RegWR_n = 1;
    assign USB_RegCS_n = 1;
    assign USB_RegAddr = 16'h0;
    assign USB_RegData = 8'hZ;

    //////////////////////////////////////////////////////////////////
    // Simulate the SRAM
    //////////////////////////////////////////////////////////////////

    // Note: Not used in this application

    // Simulates a simple memory device
    assign DataA = (SRAMWE2==1) ? SRAMData2[8:0] : 9'bZZZZZZZZZ;
    assign DataB = (SRAMWE2==1) ? SRAMData2[17:9] : 9'bZZZZZZZZZ;
    assign # 4 S_DA = DataA;
    assign # 4 S_DB = DataB;
    always @ (posedge S_CLK) begin
        // Data phase of SRAM is delayed by 2 cycles on a write
        SRAMAddr1 <= S_A;
        SRAMAddr2 <= SRAMAddr1;
        SRAMWE1 <= S_BWA_N | S_WE_N;
        SRAMWE2 <= SRAMWE1;
        if (SRAMWE2==0) begin
            SRAM[SRAMAddr2][8:0] <= S_DA;
            SRAM[SRAMAddr2][17:9] <= S_DB;
        end

        // Data read pipeline cycles
        SRAMData1 <= SRAM[S_A];
        SRAMData2 <= SRAMData1;
    end

    //////////////////////////////////////////////////////////////////
	// Instantiate the Unit Under Test (UUT)
    //////////////////////////////////////////////////////////////////
	Example2  uut (
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
    		.User_Signals (User_Signals),
    		.S_CLK (S_CLK),
    		.S_A (S_A),
    		.S_DA (S_DA),
    		.S_DB (S_DB),
    		.S_ADV_LD_N (S_ADV_LD_N),
    		.S_BWA_N (S_BWA_N),
    		.S_BWB_N (S_BWB_N),
    		.S_OE_N (S_OE_N),
    		.S_WE_N (S_WE_N),
    		.IO_CLK_N (IO_CLK_N),
    		.IO_CLK_P (IO_CLK_P),
    		.IO (IO)
    	);

endmodule
