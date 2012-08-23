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

entity ser_to_par_1_1 is
    Port ( 	clk_1_1 		: in  STD_LOGIC;
	reset_1_1 	: in std_logic;
           	SI_1_1 		: in  STD_LOGIC;
	zerro_1_1 	: in std_logic;
           	PO_1_1 		: out  STD_LOGIC_VECTOR (15 downto 0));
end ser_to_par_1_1;

architecture Behavioral of ser_to_par_1_1 is
	signal 	tmp_1_1	: std_logic_vector(15 downto 0); 
begin
    process (clk_1_1) 
      begin  
	if (clk_1_1'event and clk_1_1='1') then 
		if (zerro_1_1='1') then-- RST
			tmp_1_1 <= (others => '0');
			tmp_1_1(15) <=  SI_1_1;  
		else   
			tmp_1_1 <= SI_1_1 & tmp_1_1(15 downto 1); 
		end if;
	end if; 
    end process; 
    PO_1_1 <= tmp_1_1; 
end Behavioral;