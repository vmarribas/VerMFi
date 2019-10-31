----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    15:11:23 07/21/2016 
-- Design Name: 
-- Module Name:    chi_td_4shares - Behavioral 
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
library work;
	use work.keccak_globals.all;
	
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity iota is
	port (
		 iota_in : in  k_state;
		 round_constant_signal    : in std_logic_vector(7 downto 0);
		 iota_out: out k_state);
end iota;

architecture Behavioral of iota is

begin
--iota

i5001: for y in 1 to 4 generate
	i5002: for x in 0 to 4 generate
		i5003: for i in 0 to 7 generate
			iota_out(y)(x)(i)<=iota_in(y)(x)(i);
		end generate;	
	end generate;
end generate;


	i5012: for x in 1 to 4 generate
		i5013: for i in 0 to 7 generate
			iota_out(0)(x)(i)<=iota_in(0)(x)(i);
		end generate;	
	end generate;



		i5103: for i in 0 to 7 generate
			iota_out(0)(0)(i)<=iota_in(0)(0)(i) xor round_constant_signal(i);
		end generate;	

	
end Behavioral;
