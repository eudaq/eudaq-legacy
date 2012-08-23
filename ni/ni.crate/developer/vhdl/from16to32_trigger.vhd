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

entity from16to32_trigger is
    Port ( 	clk_16to32_trigger 	: in  STD_LOGIC;
	reset_16to32_trigger 	: in std_logic;
   	in16L_16to32_trigger 	: in std_logic_vector (15 downto 0);
	in16H_16to32_trigger 	: in std_logic_vector (15 downto 0);
   	out32_16to32_trigger 	: out  STD_LOGIC_VECTOR (31 downto 0));
end from16to32_trigger;

architecture Behavioral of from16to32_trigger is
	signal tmp_16to32_trigger	: std_logic_vector(31 downto 0); 
begin
    process (clk_16to32_trigger) 
      begin  
	if (clk_16to32_trigger'event and clk_16to32_trigger='1') then 
		 -- out32 <= (others =>'0');
		tmp_16to32_trigger(15 downto 0)  <= in16L_16to32_trigger ;
		tmp_16to32_trigger(31 downto 16) <= in16H_16to32_trigger ;
	end if; 
    end process; 
	 out32_16to32_trigger <= tmp_16to32_trigger; 
end Behavioral;