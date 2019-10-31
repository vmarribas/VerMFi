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
use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity chi is
	port (
		 chi_in : in  k_state;
		 chi_out: out k_state);
end chi;

architecture Behavioral of chi is

begin
--chi
i0000: for y in 0 to 4 generate
	i0001: for x in 0 to 2 generate
		i0002: for i in 0 to 7 generate
			chi_out(y)(x)(i)<=chi_in(y)(x)(i) xor  ( not(chi_in (y)(x+1)(i))and chi_in (y)(x+2)(i));
		
		end generate;	
	end generate;
end generate;

	i0011: for y in 0 to 4 generate
		i0021: for i in 0 to 7 generate
			chi_out(y)(3)(i)<=chi_in(y)(3)(i) xor  ( not(chi_in (y)(4)(i))and chi_in (y)(0)(i));
		
		end generate;	
	end generate;
	
	i0012: for y in 0 to 4 generate
		i0022: for i in 0 to 7 generate
			chi_out(y)(4)(i)<=chi_in(y)(4)(i) xor  ( not(chi_in (y)(0)(i))and chi_in (y)(1)(i));
		
		end generate;	
	end generate;
	
end Behavioral;
