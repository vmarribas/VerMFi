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

entity pi is
	port (
		 pi_in : in  k_state;
		 pi_out: out k_state);
end pi;

architecture Behavioral of pi is

begin
i3001: for y in 0 to 4 generate
	i3002: for x in 0 to 4 generate
		i3003: for i in 0 to 7 generate
			pi_out((2*x+3*y) mod 5)(0*x+1*y)(i)<=pi_in(y) (x)(i);
		end generate;	
	end generate;
end generate;
	
end Behavioral;
