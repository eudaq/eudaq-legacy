----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    22:14:48 04/25/2011 
-- Design Name: 
-- Module Name:    tlu_with_trig_num - Behavioral 
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

use ieee.numeric_std.all;
library UNISIM;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity tlu_with_trig_num is
    Port ( clk 	: in  STD_LOGIC;
			  reset 	: in std_logic;
			  trigger : in  STD_LOGIC;
			  marker	 : in std_logic;
			  clk_in	 : in std_logic_vector (15 downto 0);
			  trigger_clk	 : out std_logic_vector (15 downto 0);
			  busy_out : out  STD_LOGIC;
           send_out : out  STD_LOGIC);
end tlu_with_trig_num;

architecture Behavioral of tlu_with_trig_num is
	signal trigger_yes 	: std_logic							  	:= '0';
	signal frame 			: std_logic_vector(1 downto 0)	:= B"00";	

begin
	process (clk) 
      begin 
		if (clk'event and clk='1') then
			if (reset='1') then             
            busy_out <= '1'; 
				send_out <= '0';
				frame <= B"00";
				trigger_yes <= '0';
			end if;
			if (marker='1') then
				if (frame = B"10") then
					frame <= B"00";
					send_out <= '0';
				end if;
				if (frame = B"01") then 
					busy_out <= '0';
					trigger_clk <= B"0010010100011100";
					frame <=B"10";
				end if;	
				if (trigger_yes = '1') then
					send_out <= '1';
					frame <= B"01";
					trigger_yes <= '0';
				end if; 
			end if;
			if (trigger ='1') then
				trigger_yes <= '1'; 
				busy_out <= '1';
				trigger_clk <= clk_in;
			end if;
		end if;
	end process; 

end behavioral;
