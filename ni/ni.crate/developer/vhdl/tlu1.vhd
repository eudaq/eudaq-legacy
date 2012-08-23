----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    16:50:46 06/01/2011 
-- Design Name: 
-- Module Name:    tlu - Behavioral 
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

entity tlu is
 Port ( clk 	: in  STD_LOGIC;
			  reset 	: in std_logic;
			  trigger : in  STD_LOGIC;
			  marker	 : in std_logic;
			  clk_in	 : in std_logic_vector (15 downto 0);
			  trigger_clk	 : out std_logic_vector (15 downto 0);
			  busy_out : out  STD_LOGIC;
           send_out : out  STD_LOGIC;
			  
			  -------------------------------------
			  trigger_clk_out	: out	STD_LOGIC ; -- output for trigger clock 
			  trig_num_out		: out std_logic_vector (15 downto 0) ); -- output for number of trigger after reading trigger
			  
end tlu;

architecture Behavioral of tlu is
	signal busy 			: std_logic							  	:= '0';
	signal trigger_yes 	: std_logic							  	:= '0';
	signal frame 			: std_logic_vector(1 downto 0)	:= B"00";	
	
	--------------------------------------
	signal wait_start_clk	: std_logic						:= '0';
	signal start_clk			: std_logic								:= '0';
	signal tmp_trig_num		: std_logic_vector(15 downto 0)	:= B"0000000000000000";	
	
	shared variable wait_clk	: integer;  -- wait before start to generate trigger clock
	shared variable clk_trig	: integer;
	shared variable clk_trig_num	: integer; -- number of clock for trigger clock
	

begin
	process (clk) 
      begin 
		if (clk'event and clk='1') then
			if (reset='1') then  
            busy_out <= '0';
				send_out <= '0';
				frame <= B"00";
				trigger_yes <= '0';
				busy <= '0';
				---------------
				tmp_trig_num <= (others => '0');
				trig_num_out <= (others => '0');
				trigger_clk_out <= '0';
			end if;
			if (marker='1') then
				if (frame = B"10") then
					frame <= B"00";
					send_out <= '0';
				end if;
				if (frame = B"01") then 
					busy_out <= '0';
					busy <= '0';
					trigger_clk <= B"0010010100011100";
					frame <=B"10";
				end if;	
				if (trigger_yes = '1') then
					send_out <= '1';
					frame <= B"01";
					trigger_yes <= '0';
				end if; 
			end if;
			if (trigger ='1' and busy = '0') then
				trigger_yes <= '1'; 
				busy_out <= '1';
				busy <= '1';
				trigger_clk <= clk_in;
				------------------------------
				wait_start_clk <='1';
				wait_clk := 0;  -- wait before start to generate trigger clock
				tmp_trig_num <= (others => '0');
				trig_num_out <= (others => '0');
			end if;
			
			if (busy = '1') then
				if (wait_start_clk = '1') then
					wait_clk := wait_clk + 1;
				end if;

				if (wait_clk = 29 and wait_start_clk = '1' ) then
					start_clk <='1';
					clk_trig := 0;
					clk_trig_num := 0; 
					wait_start_clk <= '0';
				end if; 			
			
				if (start_clk = '1') then
					clk_trig := clk_trig + 1;
					
					if (clk_trig <= 6) then
						trigger_clk_out <= '1';
					end if;
					if (clk_trig >= 7) then
						trigger_clk_out <= '0';
					end if;
					
					if (clk_trig = 6) then
						if (clk_trig_num = 0) then
							tmp_trig_num (15) <= trigger; 
						end if;
						if (clk_trig_num > 0) then
							tmp_trig_num <= trigger & tmp_trig_num (15 downto 1);
						end if;
					end if;
					
					if (clk_trig = 12) then
						clk_trig_num := clk_trig_num + 1;
						clk_trig := 0;
					end if;
					
					if (clk_trig_num = 17) then 
						trig_num_out <= tmp_trig_num(15 downto 0);
						start_clk <= '0';
						trigger_clk_out <= '0';
						clk_trig_num := 0;
					end if;
					
				end if;
			end if; 
			
			if (busy = '0') then
				trigger_clk_out <= '0';
			end if;

		end if;
	end process; 

end Behavioral;

