-- ZestSC1 Example 4 Test Bench
-- File name: TestBench.vhd
-- Version: 1.00
-- Date: 25/4/2005

-- Copyright (C) 2005 Orange Tree Technologies Ltd. All rights reserved.
-- Orange Tree Technologies grants the purchaser of a ZestSC1 the right to use and 
-- modify this logic core in any form such as VHDL source code or EDIF netlist in 
-- FPGA designs that target the ZestSC1.
-- Orange Tree Technologies prohibits the use of this logic core in any form such 
-- as VHDL source code or EDIF netlist in FPGA designs that target any other
-- hardware unless the purchaser of the ZestSC1 has purchased the appropriate 
-- licence from Orange Tree Technologies. Contact Orange Tree Technologies if you 
-- want to purchase such a licence.

--*****************************************************************************************
--**
--**  Disclaimer: LIMITED WARRANTY AND DISCLAIMER. These designs are
--**              provided to you "as is". Orange Tree Technologies and its licensors 
--**              make and you receive no warranties or conditions, express, implied, 
--**              statutory or otherwise, and Orange Tree Technologies specifically 
--**              disclaims any implied warranties of merchantability, non-infringement,
--**              or fitness for a particular purpose. Orange Tree Technologies does not
--**              warrant that the functions contained in these designs will meet your 
--**              requirements, or that the operation of these designs will be 
--**              uninterrupted or error free, or that defects in the Designs will be 
--**              corrected. Furthermore, Orange Tree Technologies does not warrant or 
--**              make any representations regarding use or the results of the use of the 
--**              designs in terms of correctness, accuracy, reliability, or otherwise.                                               
--**
--**              LIMITATION OF LIABILITY. In no event will Orange Tree Technologies 
--**              or its licensors be liable for any loss of data, lost profits, cost or 
--**              procurement of substitute goods or services, or for any special, 
--**              incidental, consequential, or indirect damages arising from the use or 
--**              operation of the designs or accompanying documentation, however caused 
--**              and on any theory of liability. This limitation will apply even if 
--**              Orange Tree Technologies has been advised of the possibility of such 
--**              damage. This limitation shall apply notwithstanding the failure of the 
--**              essential purpose of any limited remedies herein.
--**
--*****************************************************************************************

library ieee;
use ieee.std_logic_arith.all;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;
use std.textio.all;

entity ZestSC1TestBench is
end ZestSC1TestBench;

architecture arch of ZestSC1TestBench is 

	-- Component Declaration for the Unit Under Test (UUT)
	component example4
	    port (
    		USB_StreamCLK : in std_logic;
    		USB_StreamFlags_n : in std_logic_vector(2 downto 0);
    		USB_StreamFX2Rdy : in std_logic;
    		USB_RegCLK : in std_logic;
    		USB_RegAddr : in std_logic_vector(15 downto 0);
    		USB_RegOE_n : in std_logic;
    		USB_RegRD_n : in std_logic;
    		USB_RegWR_n : in std_logic;
    		USB_RegCS_n : in std_logic;    
    		USB_StreamData : inout std_logic_vector(15 downto 0);
    		USB_RegData : inout std_logic_vector(7 downto 0);
    		User_Signals : inout std_logic_vector(7 downto 0);
    		S_DA : inout std_logic_vector(8 downto 0);
    		S_DB : inout std_logic_vector(8 downto 0);
    		IO_CLK_N : inout std_logic;
    		IO_CLK_P : inout std_logic;
    		IO : inout std_logic_vector(46 downto 0);      
    		USB_StreamFIFOADDR : out std_logic_vector(1 downto 0);
    		USB_StreamPKTEND_n : out std_logic;
    		USB_StreamSLOE_n : out std_logic;
    		USB_StreamSLRD_n : out std_logic;
    		USB_StreamSLWR_n : out std_logic;
    		USB_Interrupt : out std_logic;
    		S_CLK : out std_logic;
    		S_A : out std_logic_vector(22 downto 0);
    		S_ADV_LD_N : out std_logic;
    		S_BWA_N : out std_logic;
    		S_BWB_N : out std_logic;
    		S_OE_N : out std_logic;
    		S_WE_N : out std_logic
		);
	end component;

	-- Declare signals
	signal USB_StreamCLK :  std_logic := '0';
	signal USB_StreamFX2Rdy :  std_logic := '0';
	signal USB_StreamFlags_n :  std_logic_vector(2 downto 0) := (others=>'0');
	signal USB_StreamFIFOADDR :  std_logic_vector(1 downto 0);
	signal USB_StreamPKTEND_n :  std_logic;
	signal USB_StreamSLOE_n :  std_logic;
	signal USB_StreamSLRD_n :  std_logic;
	signal USB_StreamSLWR_n :  std_logic;
	signal USB_StreamData :  std_logic_vector(15 downto 0);
	signal StreamData :  std_logic_vector(15 downto 0);

	signal USB_RegCLK :  std_logic := '0';
	signal USB_RegOE_n :  std_logic := '0';
	signal USB_RegRD_n :  std_logic := '0';
	signal USB_RegWR_n :  std_logic := '0';
	signal USB_RegCS_n :  std_logic := '0';
	signal USB_RegAddr :  std_logic_vector(15 downto 0) := (others=>'0');
	signal USB_RegData :  std_logic_vector(7 downto 0);

	signal S_DA :  std_logic_vector(8 downto 0);
	signal S_DB :  std_logic_vector(8 downto 0);
	signal S_CLK :  std_logic;
	signal S_A :  std_logic_vector(22 downto 0);
	signal S_ADV_LD_N :  std_logic;
	signal S_BWA_N :  std_logic;
	signal S_BWB_N :  std_logic;
	signal S_OE_N :  std_logic;
	signal S_WE_N :  std_logic;

	signal User_Signals :  std_logic_vector(7 downto 0);
	signal USB_Interrupt :  std_logic;
	signal IO_CLK_N :  std_logic;
	signal IO_CLK_P :  std_logic;
	signal IO :  std_logic_vector(46 downto 0);

    -- SRAM simulation
    --type SRAM_TYPE is array(0 to (512*1024-1)) or std_logic_vector(17 downto 0); -- 1MB
    type SRAM_TYPE is array(0 to (4*1024*1024-1)) of std_logic_vector(17 downto 0); -- 8MB
    shared variable SRAM : SRAM_TYPE;
    signal SRAMData1 : std_logic_vector(17 downto 0);
    signal SRAMData2 : std_logic_vector(17 downto 0);
    signal SRAMAddr1 : std_logic_vector(22 downto 0);
    signal SRAMAddr2 : std_logic_vector(22 downto 0);
    signal SRAMWE1 : std_logic;
    signal SRAMWE2 : std_logic;
    signal DataA : std_logic_vector(8 downto 0);
    signal DataB : std_logic_vector(8 downto 0);

    -- Stream input and output files
    file InFile : TEXT is in "InFile.txt";
    file OutFile : TEXT is out "OutFile.txt";

begin

    -- Tied signals
    USB_StreamFX2Rdy <= '1';

    -- 48 MHz clocks
    -- In simulation, these can be in sync but in general they are not
	process
	begin
        USB_StreamCLK <= '1';
        USB_RegCLK <= '1';
        wait for 10.416667 ns;
        USB_StreamCLK <= '0';
        USB_RegCLK <= '0';
        wait for 10.416667 ns;
	end process;

    ------------------------------------------------------------------
    -- Simulate the streaming interface
    ------------------------------------------------------------------

    -- Simulation implements an infinite source and sink
    -- NB: This does not simulate the complex timing of the full
    -- USB interface, host driver and application.  It is merely
    -- provided to check the algorithm under test is correct.
    process
    begin
        -- Wait for the completion of reset
	    USB_StreamFlags_n(0) <= '0'; -- USB device FIFO empty signal
	    USB_StreamFlags_n(1) <= '0'; -- USB device FIFO full signal
        wait for 1000 ns;
	    USB_StreamFlags_n(0) <= '1'; -- USB device FIFO empty signal
	    USB_StreamFlags_n(1) <= '1'; -- USB device FIFO full signal
        wait;
    end process;
	USB_StreamFlags_n(2) <= '0'; -- Not used
    USB_StreamData <= StreamData when USB_StreamSLOE_n='0' else (others=>'Z');

    -- Write data sent to the host to a file
    process
        variable l : line;
    begin
        wait for 10.416667 ns;
        if (USB_StreamSLWR_n='0') then
            write(l, conv_integer(USB_StreamData));
            writeline(OutFile, l);
        end if;
        wait for 10.416667 ns;
    end process;
    
    -- Read simulation data from the host from a file
    process
        variable l : line;
        variable v : integer;
    begin
        StreamData <= (others=>'0');
        wait for 10.416667 ns;
        while not endfile(InFile) loop
            if (USB_StreamSLRD_n='0') then
                readline(InFile, l);
                read(l, v);
                StreamData <= conv_std_logic_vector(v, 16);
            end if;

            wait for 20.833334 ns;
        end loop;
        wait;
    end process;

    ------------------------------------------------------------------
    -- Simulate register reads and writes
    ------------------------------------------------------------------
    process
    begin
        USB_RegOE_n <= '1';
        USB_RegRD_n <= '1';
        USB_RegWR_n <= '1';
        USB_RegCS_n <= '1';
        USB_RegAddr <= (others => '0');
        USB_RegData <= (others => 'Z');

        -- Wait for reset to complete
        wait for 1041.6667 ns;
        wait for 10.416667 ns;

        -- Write to registers to set up transfer to the SRAM
        for i in 0 to 5 loop
            USB_RegOE_n <= '1';
            USB_RegRD_n <= '1';
            USB_RegWR_n <= '1';
            USB_RegCS_n <= '0';
            USB_RegAddr <= X"2000" + conv_std_logic_vector(i, 16); -- Fill in address here
            case i is
                when 0 => USB_RegData <= X"00"; -- Write to SRAM address 0
                when 1 => USB_RegData <= X"00";
                when 2 => USB_RegData <= X"00";
                when 3 => USB_RegData <= X"10"; -- Request a write of 16 words
                when 4 => USB_RegData <= X"00";
                when 5 => USB_RegData <= X"00";
                when others => null;
            end case;
            wait for 20.833334 ns;
            USB_RegWR_n <= '0';
            wait for 20.833334 ns;
            wait for 20.833334 ns;
            USB_RegWR_n <= '1';
            USB_RegCS_n <= '1';
            USB_RegData <= (others => 'Z');
            wait for 20.833334 ns;
        end loop;

        -- Wait for stream data to get to SRAM
        wait for 2083.3334 ns;

        -- Write to registers to set up transfer from the SRAM
        for i in 0 to 5 loop
            USB_RegOE_n <= '1';
            USB_RegRD_n <= '1';
            USB_RegWR_n <= '1';
            USB_RegCS_n <= '0';
            if (i<3) then
                USB_RegAddr <= X"2000" + conv_std_logic_vector(i, 16);
            else
                USB_RegAddr <= X"2000" + conv_std_logic_vector(i+3, 16);
            end if;
            case i is
                when 0 => USB_RegData <= X"00"; -- Read from SRAM address 0
                when 1 => USB_RegData <= X"00";
                when 2 => USB_RegData <= X"00";
                when 3 => USB_RegData <= X"10";
                when 4 => USB_RegData <= X"00";
                when 5 => USB_RegData <= X"00";
                when others => null;
            end case;
            wait for 20.833334 ns;
            USB_RegWR_n <= '0';
            wait for 20.833334 ns;
            wait for 20.833334 ns;
            USB_RegWR_n <= '1';
            USB_RegCS_n <= '1';
            USB_RegData <= (others => 'Z');
            wait for 20.833334 ns;
        end loop;

        wait;
    end process;

    ------------------------------------------------------------------
    -- Simulate the SRAM
    ------------------------------------------------------------------

    -- Simulates a simple memory device
    DataA <= SRAMData2(8 downto 0) when SRAMWE2='1' else "ZZZZZZZZZ";
    DataB <= SRAMData2(17 downto 9) when SRAMWE2='1' else "ZZZZZZZZZ";
    S_DA <= DataA after 4 ns;
    S_DB <= DataB after 4 ns;
    process (S_CLK)
    begin
        if (S_CLK'event and S_CLK='1') then
            -- Data phase of SRAM is delayed by 2 cycles on a write
            SRAMAddr1 <= S_A;
            SRAMAddr2 <= SRAMAddr1;
            SRAMWE1 <= S_BWA_N or S_WE_N;
            SRAMWE2 <= SRAMWE1;
            if (SRAMWE2='0') then
                SRAM(conv_integer(SRAMAddr2))(8 downto 0) := S_DA;
                SRAM(conv_integer(SRAMAddr2))(17 downto 9) := S_DB;
            end if;

            -- Data read pipeline cycles
            SRAMData1 <= SRAM(conv_integer(S_A));
            SRAMData2 <= SRAMData1;
        end if;
    end process;

    ------------------------------------------------------------------
	-- Instantiate the Unit Under Test (UUT)
    ------------------------------------------------------------------
	uut: example4
        port map (
    		USB_StreamCLK => USB_StreamCLK,
    		USB_StreamFIFOADDR => USB_StreamFIFOADDR,
    		USB_StreamPKTEND_n => USB_StreamPKTEND_n,
    		USB_StreamFlags_n => USB_StreamFlags_n,
    		USB_StreamSLOE_n => USB_StreamSLOE_n,
    		USB_StreamSLRD_n => USB_StreamSLRD_n,
    		USB_StreamSLWR_n => USB_StreamSLWR_n,
    		USB_StreamFX2Rdy => USB_StreamFX2Rdy,
    		USB_StreamData => USB_StreamData,
    		USB_RegCLK => USB_RegCLK,
    		USB_RegAddr => USB_RegAddr,
    		USB_RegData => USB_RegData,
    		USB_RegOE_n => USB_RegOE_n,
    		USB_RegRD_n => USB_RegRD_n,
    		USB_RegWR_n => USB_RegWR_n,
    		USB_RegCS_n => USB_RegCS_n,
    		USB_Interrupt => USB_Interrupt,
    		User_Signals => User_Signals,
    		S_CLK => S_CLK,
    		S_A => S_A,
    		S_DA => S_DA,
    		S_DB => S_DB,
    		S_ADV_LD_N => S_ADV_LD_N,
    		S_BWA_N => S_BWA_N,
    		S_BWB_N => S_BWB_N,
    		S_OE_N => S_OE_N,
    		S_WE_N => S_WE_N,
    		IO_CLK_N => IO_CLK_N,
    		IO_CLK_P => IO_CLK_P,
    		IO => IO
    	);

end;
