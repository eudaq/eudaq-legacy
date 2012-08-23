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

entity ser_to_par_Trig is
    Port ( 	clk_Trig 		: in  STD_LOGIC;
	reset_Trig 	: in std_logic;
           	SI_Trig 		: in  STD_LOGIC;
	zerro_Trig 	: in std_logic;
           	PO_Trig 		: out  STD_LOGIC_VECTOR (15 downto 0));
end ser_to_par_Trig;

architecture Behavioral of ser_to_par_Trig is
	signal 	tmp_Trig	: std_logic_vector(15 downto 0); 
begin
    process (clk_Trig) 
      begin  
	if (clk_Trig'event and clk_Trig='1') then 
		if (zerro_Trig='1') then-- RST
			tmp_Trig <= (others => '0');
			tmp_Trig(15) <=  SI_Trig;  
		else   
			tmp_Trig <= SI_Trig & tmp_Trig(15 downto 1); 
		end if;
	end if; 
    end process; 
    PO_Trig <= tmp_Trig; 
end Behavioral;