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

entity ser_to_par_3_1 is
    Port ( 	clk_3_1 		: in  STD_LOGIC;
	reset_3_1 	: in std_logic;
           	SI_3_1 		: in  STD_LOGIC;
	zerro_3_1 	: in std_logic;
           	PO_3_1 		: out  STD_LOGIC_VECTOR (15 downto 0));
end ser_to_par_3_1;

architecture Behavioral of ser_to_par_3_1 is
	signal 	tmp_3_1	: std_logic_vector(15 downto 0); 
begin
    process (clk_3_1) 
      begin  
	if (clk_3_1'event and clk_3_1='1') then 
		if (zerro_3_1='1') then-- RST
			tmp_3_1 <= (others => '0');
			tmp_3_1(15) <=  SI_3_1;  
		else   
			tmp_3_1 <= SI_3_1 & tmp_3_1(15 downto 1); 
		end if;
	end if; 
    end process; 
    PO_3_1 <= tmp_3_1; 
end Behavioral;