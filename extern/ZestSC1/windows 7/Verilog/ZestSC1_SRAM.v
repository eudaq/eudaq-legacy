/*
   ZestSC1 ZBT SRAM Logic Core
   File name: ZestSC1_SRAM.vhd
   Version: 1.00
   Date: 6/12/2004

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

   The Pipelined ZBT SRAM device takes 2 clock cycles for a write and 2 clock
   cycles for a read. This logic core has one extra pipeline stage in the write
   path (giving 3 clock cycles) and two extra pipeline stages in the read path
   (giving 4 clock cycles).
  
   The user logic drives the write strobe high and the write address and
   data in the same clock cycle. The logic core registers all command and data
   signals and delays the data by 2 further clocks as required by the ZBT SRAM.
   Byte write strobes are not implemented.
  
   Read cycles are User Read command followed 4 clocks later by read data.
   The user logic drives the read strobe high and the read address in the
   same clock cycle, and the logic core registers all these signals. Valid data is
   returned to the user code 4 clocks after the user code drives the read strobe
   high. The valid data is accompanied by the active high data valid strobe. When
   the data valid strobe is high then the read data should be registered by the
   user code.
  
   All accesses are single accesses, burst accesses are not implemented.
   
   The SRAM clock is driven from the FPGA to the SRAM. This must arrive
   at the SRAM before the command and write data by the SRAM hold time of at
   least 0.5nS. This is achieved by using a DCM to generate the SRAM clock
   about 2nS in advance of the User clock, and clocking all signals in IOB's.
   The SRAM clock is used only as an output to the SRAM (via FDDR in IOB)
   and does not clock any signals in the FPGA.
*/

module ZestSC1_SRAM (
        // User interface
        input USER_CLK,                       // clock from user logic
        input USER_RESET,                     // reset
        input [22:0] USER_A,                  // 23-bit address
        input USER_W,                         // write strobe active high
        input USER_R,                         // read strobe active high
        output reg USER_DR_VALID,             // read data valid strobe active high
        input [17:0] USER_DW,                 // 18-bit data bus for writing to SRAM
        output reg [17:0] USER_DR,            // 18-bit data bus for reading from SRAM
        // ZBT SRAM interface
        output CLK_SRAM,
        output reg [22:0] S_A,
        output S_ADV_LD_N,
        output S_BWA_N,
        output S_BWB_N,
        inout [8:0] S_DA,
        inout [8:0] S_DB,
        output S_OE_N,
        output reg S_WE_N
    );

    // Declare signals
    wire Zero;
    wire SRAMClk;
    wire SRAMClkInv;
    wire SRAMClkOut;
    wire dcm_locked;
    wire Reset;
    reg OutputEnable0_n;
    reg OutputEnable1_n;
    reg S_OutputEnable_n;
    reg [17:0] Data0;
    reg [17:0] Data1;
    reg [17:0] S_DataOut;
    reg ReadDataValid0;
    reg ReadDataValid1;
    reg ReadDataValid2;

    assign Zero = 0;
    assign SRAMClkInv = ~SRAMClk;
    assign Reset = USER_RESET;

    // Advance SRAM clock relative to internal clock to ensure positive hold
    // time for signals at SRAM pins
    DCM DCM_INST (
        .CLKIN(USER_CLK),
        .CLKFB(SRAMClk),
        .RST(Reset),
        .PSCLK(Zero),
        .PSEN(Zero),
        .PSINCDEC(Zero),
        .DSSEN(Zero),

        .CLK0(SRAMClkOut),
        .LOCKED(dcm_locked)
    );
// synthesis attribute CLK_FEEDBACK of DCM_INST is "1X"
// synthesis attribute CLKDV_DIVIDE of DCM_INST is 2
// synthesis attribute CLKFX_DIVIDE of DCM_INST is 1
// synthesis attribute CLKFX_MULTIPLY of DCM_INST is 4
// synthesis attribute CLKIN_DIVIDE_BY_2 of DCM_INST is "FALSE"
// synthesis attribute CLKIN_PERIOD of DCM_INST is 20
// synthesis attribute CLKOUT_PHASE_SHIFT of DCM_INST is "FIXED"
// synthesis attribute DESKEW_ADJUST of DCM_INST is "SYSTEM_SYNCHRONOUS"
// synthesis attribute DFS_FREQUENCY_MODE of DCM_INST is "LOW"
// synthesis attribute DLL_FREQUENCY_MODE of DCM_INST is "LOW"
// synthesis attribute DUTY_CYCLE_CORRECTION of DCM_INST is "TRUE"
// synthesis attribute PHASE_SHIFT of DCM_INST is -25
// synthesis attribute STARTUP_WAIT of DCM_INST is "FALSE"
// synthesis translate_off
defparam DCM_INST.CLK_FEEDBACK="1X";
defparam DCM_INST.CLKDV_DIVIDE=2;
defparam DCM_INST.CLKFX_DIVIDE=1;
defparam DCM_INST.CLKFX_MULTIPLY=4;
defparam DCM_INST.CLKIN_DIVIDE_BY_2="FALSE";
defparam DCM_INST.CLKIN_PERIOD=20;
defparam DCM_INST.CLKOUT_PHASE_SHIFT="FIXED";
defparam DCM_INST.DESKEW_ADJUST="SYSTEM_SYNCHRONOUS";
defparam DCM_INST.DFS_FREQUENCY_MODE="LOW";
defparam DCM_INST.DLL_FREQUENCY_MODE="LOW";
defparam DCM_INST.DUTY_CYCLE_CORRECTION="TRUE";
defparam DCM_INST.PHASE_SHIFT=-25;
defparam DCM_INST.STARTUP_WAIT="FALSE";
// synthesis translate_on 

    BUFG CLK0_BUFG_INST (
        .I (SRAMClkOut),
        .O (SRAMClk)
    );

    // Use double data rate flip-flop for SRAM clock output to ensure
    // minimum delay on clock (using just an OBUF would use slower local routing
    // from SramClk to the OBUF)
    OFDDRCPE OFDDRCPE_CLK_SRAM (
        .D0 (1'b1),
        .D1 (1'b0),
        .C0 (SRAMClk),
        .C1 (SRAMClkInv),
        .Q (CLK_SRAM),
        .PRE (1'b0),
        .CLR (1'b0),
        .CE (1'b1)
    );

    // Byte writes are not implemented so tie byte write strobes low
    assign S_BWA_N = 0;
    assign S_BWB_N = 0;
    // Burst accesses are not implemented so tie Advance/Load_n strobe low
    assign S_ADV_LD_N = 0;
    // Outputs are tri-stated by SRAM itself during writes so tie OE_n low
    assign S_OE_N = 0;

    // Register user command signals
    always @ (posedge USER_CLK)
    begin
        S_WE_N <= ~USER_W;
        S_A <= USER_A;
    end

    // Write Data
    // 3-stage pipeline
    always @ (posedge USER_CLK)
    begin
        Data0 <= USER_DW;
        Data1 <= Data0;
        S_DataOut <= Data1;
    end

    // Tri-state control
    // Output enable strobe is user write enable delayed 3 clocks as write data
    // is valid 3 clocks after user address and write enable
    always @ (posedge USER_CLK)
    begin
        OutputEnable0_n <= ~USER_W;
        OutputEnable1_n <= OutputEnable0_n;
        S_OutputEnable_n <= OutputEnable1_n;
    end
    assign S_DA = S_OutputEnable_n==0 ? S_DataOut[8:0] : 9'hZ;
    assign S_DB = S_OutputEnable_n==0 ? S_DataOut[17:9] : 9'hZ;

    // Read Data
    always @ (posedge USER_CLK)
        USER_DR <= {S_DB, S_DA};

    // Read Data Valid Strobe
    // Read data is valid 4 clocks after read strobe
    always @ (posedge USER_CLK)
    begin
        ReadDataValid0 <= USER_R;
        ReadDataValid1 <= ReadDataValid0;
        ReadDataValid2 <= ReadDataValid1;
        USER_DR_VALID <= ReadDataValid2;
    end

endmodule
