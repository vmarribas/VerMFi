----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    14:33:40 05/18/2016 
-- Design Name: 
-- Module Name:    Keccak_p200 - Behavioral 
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
---------------------------------------------------------------------------
--Input_shares: s_in.
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.numeric_std.all;

library work;
 use work.keccak_globals.all;
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity Keccak_p200 is
    Port ( start, load, clk, rst, fault : in std_logic;
			  s_in : in  STD_LOGIC_VECTOR (199 downto 0);
           s_out_r : out  STD_LOGIC_VECTOR (199 downto 0);--bitrate
			  ready : out std_logic -- High for one clock cycle when output is ready
			);
end Keccak_p200;

architecture Behavioral of Keccak_p200 is
	
	component keccak_round_constants_gen
		 port( round_number: in unsigned(4 downto 0);
					round_constant_signal_out    : out std_logic_vector(7 downto 0));
	end component;
	
	component keccak_round_blocks 
		 port ( round_in     : in  k_state;
				  round_constant_signal    : in std_logic_vector(7 downto 0);
				  round_out    : out k_state);
	end component;

	signal round_next, round_out, round_reg : k_state;
	signal ready_int, ready_reg : std_logic;--Handle value for ready
	signal round: unsigned(4 downto 0);
	signal n_round : unsigned (4 downto 0);
	signal round_cntnt : STD_LOGIC_VECTOR (7 downto 0);
	signal round_cntnt_f : STD_LOGIC_VECTOR (7 downto 0);
	
begin
	
	--Load state or assign the previous step
	i1: for y in 0 to 4 generate
		i2: for x in 0 to 4 generate
			i3: for i in 0 to 7 generate
			
					round_next(y)(x)(i) <= s_in((8*5*(4-y))+(8*(4-x))+i) when load = '1' else 
														round_out(y)(x)(i) when start='1' else 
														round_reg(y)(x)(i);
			end generate;	
		end generate;
	end generate;
	
	ready <= ready_reg;
	--Count rounds
	n_round <= (others=>'0') when (load = '1') else 
				round+1 when (start = '1' and (round <= 18)) else 
				round;
	
	--Finished? 
	ready_int <= '1' when (round = 18-1) else '0';
		
	--Compute Keccak
	--Fault round constant
	round_cntnt_f <= round_cntnt or (7 downto 1 => '0', 0 => fault);
	Kecc_cntnt:	component keccak_round_constants_gen
						port map( round_number => round, round_constant_signal_out => round_cntnt);
	Kecc_round:	component keccak_round_blocks 
						port map( round_in => round_reg, round_constant_signal => round_cntnt_f, round_out => round_out);



	F_proc : process (clk, rst)
	begin
		
		if (rst = '1') then
			round <=  (others => '0');
			round_reg <= (OTHERS => (OTHERS => (OTHERS => '0')));
			ready_reg <= '0';
		elsif rising_edge(clk) then
			round <= n_round;
			round_reg <= round_next;
			ready_reg <= ready_int;
		end if;
	end process;	
	
	--Output
	o002: for x in 0 to 4 generate
		o002: for i in 0 to 7 generate
				s_out_r(8*(4-x)+i+160)<= round_reg(0)(x)(i) when (ready_reg = '1') else '0';

				s_out_r(8*(4-x)+i+120)<= round_reg(1)(x)(i) when (ready_reg = '1') else '0';

				s_out_r(8*(4-x)+i+80)<= round_reg(2)(x)(i)  when (ready_reg = '1') else '0';

				s_out_r(8*(4-x)+i+40)<= round_reg(3)(x)(i)  when (ready_reg = '1') else '0';

				s_out_r(8*(4-x)+i)<= round_reg(4)(x)(i)     when (ready_reg = '1') else '0';
		end generate;
	end generate;
end Behavioral;