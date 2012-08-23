----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    02:06:36 02/20/2011 
-- Design Name: 
-- Module Name:    ser_in_par_out - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: 
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity trigger_collect is
    Port ( 	clk 	: in  STD_LOGIC;
	reset 	: in std_logic;
   	trigger_num 	: in std_logic_vector (15 downto 0);
	trigger_clk 	: in std_logic_vector (15 downto 0);
   	trigger_info_out 	: out  STD_LOGIC_VECTOR (31 downto 0));
end trigger_collect;

architecture Behavioral of trigger_collect is
	signal tmp	: std_logic_vector(31 downto 0); 
begin
    process (clk) 
      begin  
	if (clk'event and clk='1') then 
		 -- out32 <= (others =>'0');
		tmp(15 downto 0)  <= trigger_clk ;
		tmp(30 downto 16) <= trigger_num(15 downto 1) ;
	end if; 
    end process; 
	 trigger_info_out <= tmp; 
end Behavioral;