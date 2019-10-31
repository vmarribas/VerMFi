-- The Keccak sponge function, designed by Guido Bertoni, Joan Daemen,
-- Michal Peeters and Gilles Van Assche. For more information, feedback or
-- questions, please refer to our website: http://keccak.noekeon.org/

-- Implementation by the designers,
-- hereby denoted as "the implementer".

-- To the extent possible under law, the implementer has waived all copyright
-- and related or neighboring rights to the source code in this file.
-- http://creativecommons.org/publicdomain/zero/1.0/

library work;
use work.keccak_globals.all;
	
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity keccak_round_constants_gen is
port(

    round_number: in unsigned(4 downto 0);
    round_constant_signal_out    : out std_logic_vector(7 downto 0));

end keccak_round_constants_gen;

architecture rtl of keccak_round_constants_gen is


  ----------------------------------------------------------------------------
  -- Internal signal declarations
  ----------------------------------------------------------------------------

 
  signal round_constant_signal: std_logic_vector(7 downto 0);
 
  
begin  -- Rtl




round_constants : process (round_number)
begin
	case round_number is
       when "00000" => round_constant_signal <= X"01" ;
	    when "00001" => round_constant_signal <= X"82" ;
	    when "00010" => round_constant_signal <= X"8A" ;
	    when "00011" => round_constant_signal <= X"00" ;
	    when "00100" => round_constant_signal <= X"8B" ;
	    when "00101" => round_constant_signal <= X"01" ;
	    when "00110" => round_constant_signal <= X"81" ;
	    when "00111" => round_constant_signal <= X"09" ;
	    when "01000" => round_constant_signal <= X"8A" ;
	    when "01001" => round_constant_signal <= X"88" ;
	    when "01010" => round_constant_signal <= X"09" ;
	    when "01011" => round_constant_signal <= X"0A" ;
	    when "01100" => round_constant_signal <= X"8B" ;
	    when "01101" => round_constant_signal <= X"8B" ;
	    when "01110" => round_constant_signal <= X"89" ;
	    when "01111" => round_constant_signal <= X"03" ;
	    when "10000" => round_constant_signal <= X"02" ;
	    when "10001" => round_constant_signal <= X"80" ;
	    when "10010" => round_constant_signal <= X"0A" ;
	    when "10011" => round_constant_signal <= X"0A" ;
	    when "10100" => round_constant_signal <= X"81" ;
	    when "10101" => round_constant_signal <= X"80" ;
	    when "10110" => round_constant_signal <= X"01" ;
	    when "10111" => round_constant_signal <= X"08" ;	    	    
	    when others => round_constant_signal <=(others => '0');
        end case;
end process round_constants;

round_constant_signal_out<=round_constant_signal;
end rtl;
