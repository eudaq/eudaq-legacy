-- ZestSC1 Example 4
-- File name: Example4.vhd
-- Version: 1.10
-- Date: 6/2/2006

--
-- SRAM test
-- DMA controller between host and SRAM and back again
-- Registers are:
--      Offset      Register
--          0       SRAM Address Low
--          1       SRAM Address Middle
--          2       SRAM Address High
--          3       SRAM Write Counter Low
--          4       SRAM Write Counter Middle
--          5       SRAM Write Counter High
--          6       SRAM Read Counter Low
--          7       SRAM Read Counter Middle
--          8       SRAM Read Counter High (Write to this starts read)
--

-- Copyright (C) 2005-2006 Orange Tree Technologies Ltd. All rights reserved.
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

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
--library UNISIM;
--use UNISIM.VComponents.all;

entity Example4 is
    port (
        USB_StreamCLK : in std_logic;
        USB_StreamFIFOADDR : out std_logic_vector(1 downto 0);
        USB_StreamPKTEND_n : out std_logic;
        USB_StreamFlags_n : in std_logic_vector(2 downto 0);
        USB_StreamSLOE_n : out std_logic;
        USB_StreamSLRD_n : out std_logic;
        USB_StreamSLWR_n : out std_logic;
        USB_StreamData : inout std_logic_vector(15 downto 0);
        USB_StreamFX2Rdy : in std_logic;

        USB_RegCLK : in std_logic;
        USB_RegAddr : in std_logic_vector(15 downto 0);
        USB_RegData : inout std_logic_vector(7 downto 0);
        USB_RegOE_n : in std_logic;
        USB_RegRD_n : in std_logic;
        USB_RegWR_n : in std_logic;
        USB_RegCS_n : in std_logic;

        USB_Interrupt : out std_logic;

        User_Signals : inout std_logic_vector(7 downto 0);

        S_CLK : out std_logic;
        S_A : out std_logic_vector(22 downto 0);
        S_DA : inout std_logic_vector(8 downto 0);
        S_DB : inout std_logic_vector(8 downto 0);
        S_ADV_LD_N : out std_logic;
        S_BWA_N : out std_logic;
        S_BWB_N : out std_logic;
        S_OE_N : out std_logic;
        S_WE_N : out std_logic;

        IO_CLK_N : inout std_logic;
        IO_CLK_P : inout std_logic;
        IO : inout std_logic_vector(46 downto 0)
    );
end Example4;

architecture arch of Example4 is

    -- Declare interfaces component
    component ZestSC1_Interfaces
        port (
            -- FPGA pin connections
            USB_StreamCLK : in std_logic;
            USB_StreamFIFOADDR : out std_logic_vector(1 downto 0);
            USB_StreamPKTEND_n : out std_logic;
            USB_StreamFlags_n : in std_logic_vector(2 downto 0);
            USB_StreamSLOE_n : out std_logic;
            USB_StreamSLRD_n : out std_logic;
            USB_StreamSLWR_n : out std_logic;
            USB_StreamData : inout std_logic_vector(15 downto 0);
            USB_StreamFX2Rdy : in std_logic;

            USB_RegCLK : in std_logic;
            USB_RegAddr : in std_logic_vector(15 downto 0);
            USB_RegData : inout std_logic_vector(7 downto 0);
            USB_RegOE_n : in std_logic;
            USB_RegRD_n : in std_logic;
            USB_RegWR_n : in std_logic;
            USB_RegCS_n : in std_logic;

            USB_Interrupt : out std_logic;

            S_CLK: out std_logic;
            S_A: out std_logic_vector(22 downto 0);
            S_ADV_LD_N: out std_logic;
            S_BWA_N: out std_logic;
            S_BWB_N: out std_logic;
            S_DA: inout std_logic_vector(8 downto 0);
            S_DB: inout std_logic_vector(8 downto 0);
            S_OE_N: out std_logic;
            S_WE_N: out std_logic;

            -- User connections
            -- Streaming interface
            User_CLK : out std_logic;
            User_RST : out std_logic;

            User_StreamBusGrantLength : in std_logic_vector(11 downto 0);

            User_StreamDataIn : out std_logic_vector(15 downto 0);
            User_StreamDataInWE : out std_logic;
            User_StreamDataInBusy : in std_logic;

            User_StreamDataOut : in std_logic_vector(15 downto 0);
            User_StreamDataOutWE : in std_logic;
            User_StreamDataOutBusy : out std_logic;

            -- Register interface
            User_RegAddr : out std_logic_vector(15 downto 0);
            User_RegDataIn : out std_logic_vector(7 downto 0);
            User_RegDataOut : in std_logic_vector(7 downto 0);
            User_RegWE : out std_logic;
            User_RegRE : out std_logic;

            -- Signals and interrupts
            User_Interrupt : in std_logic;

            -- SRAM interface
            User_SRAM_A: in std_logic_vector(22 downto 0);
            User_SRAM_W: in std_logic;
            User_SRAM_R: in std_logic;
            User_SRAM_DR_VALID: out std_logic;
            User_SRAM_DW: in std_logic_vector(17 downto 0);
            User_SRAM_DR: out std_logic_vector(17 downto 0)
        );
    end component;

    -- Declare signals
    signal CLK : std_logic;
    signal RST : std_logic;
    signal LEDs : std_logic_vector(7 downto 0);

    -- Streaming interface
    signal DataIn : std_logic_vector(15 downto 0);
    signal DataInBusy : std_logic;
    signal DataInWE : std_logic;

    signal DataOut : std_logic_vector(15 downto 0);
    signal DataOutBusy : std_logic;
    signal DataOutWE : std_logic;

    -- Register interface
    signal RegAddr : std_logic_vector(15 downto 0);
    signal RegDataIn : std_logic_vector(7 downto 0);
    signal RegDataOut : std_logic_vector(7 downto 0);
    signal RegWE : std_logic;
    signal RegRE : std_logic;

    -- SRAM interface
    signal SRAMAddr : std_logic_vector(22 downto 0);
    signal SRAMDataOut : std_logic_vector(17 downto 0);
    signal SRAMDataIn : std_logic_vector(17 downto 0);
    signal SRAMWE : std_logic;
    signal SRAMRE : std_logic;
    signal SRAMValid : std_logic;

    -- Registers
    signal Address : std_logic_vector(23 downto 0);
    signal WriteCount : std_logic_vector(23 downto 0);
    signal WriteActive : std_logic;
    signal ReadCount : std_logic_vector(23 downto 0);
    signal ReadActive : std_logic;
    signal LastReadActive : std_logic;
    signal ReadState : std_logic;
    signal LastSRAMValid : std_logic;
    signal SRAMWriteReg : std_logic_vector(15 downto 0);
    signal SRAMReadReg : std_logic_vector(1 downto 0);

begin

    -- Tie unused signals
    LEDs <= "11111111";
    User_Signals <= "ZZZZZZZZ";
    IO_CLK_N <= 'Z';
    IO_CLK_P <= 'Z';
    IO <= (0=>LEDs(0), 1=>LEDs(1), 41=>LEDs(2), 42=>LEDs(3), 43=>LEDs(4),
           44=>LEDs(5), 45=>LEDs(6), 46=>LEDs(7), others => 'Z');

    -- Implement DMA controllers
    -- Note that for compatibility with the FX2LP part only addresses
    -- above 2000 Hex are used
    process (RST, CLK)
    begin
        if (RST='1') then
            ReadCount <= (others => '0');
            WriteCount <= (others => '0');
            Address <= (others => '0');
            ReadActive <= '0';            
            WriteActive <= '0';            
            LastSRAMValid <= '0';
            ReadState <= '0';            

        elsif (CLK'event and CLK='1') then
            if (RegWE='1') then

                -- Register access from the host
                case RegAddr is
                    when X"2000" => Address(7 downto 0) <= RegDataIn;
                    when X"2001" => Address(15 downto 8) <= RegDataIn;
                    when X"2002" => Address(23 downto 16) <= RegDataIn;
                    when X"2003" => WriteCount(7 downto 0) <= RegDataIn;
                    when X"2004" => WriteCount(15 downto 8) <= RegDataIn;
                    when X"2005" => WriteCount(23 downto 16) <= RegDataIn; WriteActive<='1';
                    when X"2006" => ReadCount(7 downto 0) <= RegDataIn; 
                    when X"2007" => ReadCount(15 downto 8) <= RegDataIn;
                    when X"2008" => ReadCount(23 downto 16) <= RegDataIn; ReadState <= '0'; ReadActive<='1';
                    when others => null;
                end case;

            else

                if (WriteActive='1' and WriteCount/=X"000000") then

                    -- DMA controller from Host to the SRAM
                    if (DataInWE='1') then
                        if (Address(0)='0') then
                            SRAMWriteReg <= DataIn;
                        end if;
                        Address <= Address + 1;
                        WriteCount <= WriteCount - 1;
                        if (WriteCount=X"000001") then
                            WriteActive <= '0';
                        end if;
                    end if;

                elsif (ReadActive='1' and ReadCount/=X"000000") then

                    -- DMA controller from SRAM to the Host
                    -- Only issue one read at a time to ensure the FIFO to
                    -- the host does not overflow.
                    if (ReadState='0') then
                        if (DataOutBusy='0') then
                            Address <= Address + 2;
                            ReadState <= '1';
                        end if;
                    else
                        if (SRAMValid='1') then
                            SRAMReadReg <= SRAMDataIn(17 downto 16);
                            ReadCount <= ReadCount - 2;
                            if (ReadCount=X"000002") then
                                ReadActive <= '0';
                            end if;
                            ReadState <= '0';
                        end if;
                    end if;
                        
                end if;

            end if;
            LastSRAMValid <= SRAMValid;
            LastReadActive <= ReadActive;
        end if;
    end process;

    -- Connect signals for Host to SRAM transfers
    DataInBusy <= (not WriteActive) or RegWE;
    SRAMAddr <= Address(23 downto 1);
    SRAMWE <= DataInWE and Address(0);
    SRAMDataOut <= DataIn(1 downto 0) & SRAMWriteReg;

    -- Connect signals for SRAM to Host transfers
    DataOutWE <= (ReadActive or LastReadActive) and (SRAMValid or LastSRAMValid);
    DataOut <= SRAMDataIn(15 downto 0) when SRAMValid='1' else ("00000000000000" & SRAMReadReg);
    SRAMRE <= '1' when (DataOutBusy='0' and ReadActive='1' and ReadState='0' and RegWE='0') else '0';

    -- Instantiate interfaces component
    Interfaces : ZestSC1_Interfaces
        port map (
            USB_StreamCLK => USB_StreamCLK,
            USB_StreamFIFOADDR => USB_StreamFIFOADDR,
            USB_StreamPKTEND_n => USB_StreamPKTEND_n,
            USB_StreamFlags_n => USB_StreamFlags_n,
            USB_StreamSLOE_n => USB_StreamSLOE_n,
            USB_StreamSLRD_n => USB_StreamSLRD_n,
            USB_StreamSLWR_n => USB_StreamSLWR_n,
            USB_StreamData => USB_StreamData,
            USB_StreamFX2Rdy => USB_StreamFX2Rdy,

            USB_RegCLK => USB_RegCLK,
            USB_RegAddr => USB_RegAddr,
            USB_RegData => USB_RegData,
            USB_RegOE_n => USB_RegOE_n,
            USB_RegRD_n => USB_RegRD_n,
            USB_RegWR_n => USB_RegWR_n,
            USB_RegCS_n => USB_RegCS_n,

            USB_Interrupt => USB_Interrupt,

            S_CLK => S_CLK,
            S_A => S_A,
            S_ADV_LD_N => S_ADV_LD_N,
            S_BWA_N => S_BWA_N,
            S_BWB_N => S_BWB_N,
            S_DA => S_DA,
            S_DB => S_DB,
            S_OE_N => S_OE_N,
            S_WE_N => S_WE_N,

            -- User connections
            -- Streaming interface
            User_CLK => CLK,
            User_RST => RST,

            User_StreamBusGrantLength => X"100",

            User_StreamDataIn => DataIn,
            User_StreamDataInWE => DataInWE,
            User_StreamDataInBusy => DataInBusy,

            User_StreamDataOut => DataOut, 
            User_StreamDataOutWE => DataOutWE,
            User_StreamDataOutBusy => DataOutBusy,

            -- Register interface
            User_RegAddr => RegAddr,
            User_RegDataIn => RegDataIn,
            User_RegDataOut => RegDataOut,
            User_RegWE => RegWE,
            User_RegRE => RegRE,

            -- Interrupts
            User_Interrupt => '0',

            -- SRAM interface
            User_SRAM_A => SRAMAddr,
            User_SRAM_W => SRAMWE,
            User_SRAM_R => SRAMRE,
            User_SRAM_DR_VALID => SRAMValid,
            User_SRAM_DW => SRAMDataOut,
            User_SRAM_DR => SRAMDataIn
        );

end arch;
