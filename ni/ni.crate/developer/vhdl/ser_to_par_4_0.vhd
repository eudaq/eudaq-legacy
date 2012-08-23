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

entity ser_to_par_4_0 is
    Port ( 	clk_4_0 		: in  STD_LOGIC;
	reset_4_0 	: in std_logic;
           	SI_4_0 		: in  STD_LOGIC;
	zerro_4_0 	: in std_logic;
           	PO_4_0 		: out  STD_LOGIC_VECTOR (15 downto 0));
end ser_to_par_4_0;

architecture Behavioral of ser_to_par_4_0 is
	signal 	tmp_4_0	: std_logic_vector(15 downto 0); 
begin
    process (clk_4_0) 
      begin  
	if (clk_4_0'event and clk_4_0='1') then 
		if (zerro_4_0='1') then-- RST
			tmp_4_0 <= (others => '0');
			tmp_4_0(15) <=  SI_4_0;  
		else   
			tmp_4_0 <= SI_4_0 & tmp_4_0(15 downto 1); 
		end if;
	end if; 
    end process; 
    PO_4_0 <= tmp_4_0; 
end Behavioral;