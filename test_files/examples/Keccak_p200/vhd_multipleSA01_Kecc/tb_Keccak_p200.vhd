-----------------------------------------------------------------------------------
-- COMPANY:     COSIC, KU Leuven 
-- AUTHOR:      Victor Arribas
-----------------------------------------------------------------------------------
-- BSD 3-Clause License

-- Copyright (c) 2019, Victor Arribas
-- All rights reserved.

-- Redistribution and use in source and binary forms, with or without
-- modification, are permitted provided that the following conditions are met:

-- 1. Redistributions of source code must retain the above copyright notice, this
--    list of conditions and the following disclaimer.

-- 2. Redistributions in binary form must reproduce the above copyright notice,
--    this list of conditions and the following disclaimer in the documentation
--    and/or other materials provided with the distribution.

-- 3. Neither the name of the copyright holder nor the names of its
--    contributors may be used to endorse or promote products derived from
--    this software without specific prior written permission.

-- THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
-- AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
-- IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
-- DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
-- FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
-- DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
-- SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
-- CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
-- OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
-- OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
-----------------------------------------------------------------------------------

LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
 
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
USE ieee.numeric_std.ALL;
 
ENTITY tb_Keccak_p200 IS
END tb_Keccak_p200;
 
ARCHITECTURE behavior OF tb_Keccak_p200 IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
    COMPONENT Keccak_p200
    PORT(
         start : IN  std_logic;
         load : IN  std_logic;
         clk : IN  std_logic;
         rst : IN  std_logic;
         fault : IN  std_logic;
         fault2 : IN  std_logic;
         s_in : IN  std_logic_vector(199 downto 0);
         s_out_r : OUT  std_logic_vector(199 downto 0);
         ready : OUT  std_logic
        );
    END COMPONENT;
    

   --Inputs
   signal start : std_logic := '0';
   signal clk : std_logic := '0';
   signal rst : std_logic := '0';
   signal load : std_logic := '0';
   signal fault : std_logic := '0';
   signal fault2 : std_logic := '1';
   signal s_in : std_logic_vector(199 downto 0) := (others => '1');

 	--Outputs
   signal s_out_r : std_logic_vector(199 downto 0);
   signal ready : std_logic;

   -- Clock period definitions
   constant clk_period : time := 10 ns;
 
BEGIN
 
	-- Instantiate the Unit Under Test (UUT)
   uut: Keccak_p200 
			PORT MAP (
          start => start,
          load => load,
          clk => clk, 
          rst => rst,
          fault => fault,
          fault2 => fault2,
          s_in => s_in,
          s_out_r => s_out_r,
          ready => ready
        );

   -- Clock process definitions
   clk_process :process
   begin
		clk <= '0';
		wait for clk_period/2;
		clk <= '1';
		wait for clk_period/2;
   end process;
 

   -- Stimulus process
   stim_proc: process
   begin		
      -- hold reset state for 100 ns.
		rst <= '1';
      wait for 100 ns;	
		rst <= '0';
		wait for clk_period*10;
		load <= '1';
		s_in <= x"00000000000000000000000000000000000000000000000000";
--		s_in <= x"12345678901234567890123456789012345678901234567890";
      wait for clk_period*10;
		load<='0';
		start <= '1';
--		wait for clk_period*15;
--		fault <= '1'; --SA1 activate
--		fault2 <= '0'; --SA0
--		wait for clk_period;
--		fault <= '0'; --deactivate
--		fault2 <= '1';
		wait until ready = '1';
		s_in <= s_out_r;
		start <= '0';
		
		wait for 2*clk_period;
		load <= '1';
		wait for clk_period;
		load <= '0';
		start <= '1';
		wait for clk_period*15;
		fault <= '1'; --SA1 activate
		fault2 <= '0'; --SA0
		wait for clk_period;
		fault <= '0'; --deactivate
		fault2 <= '1';
		
--		s_in <= x"80d51675bfb5d80640a21ab48b235f74832a204dacdd749ff6";
--		s_in <= x"19fa0d1f31435567798b9dafc1d3e5f70a1c2e405264760880";

      -- insert stimulus here 

      wait;
   end process;

END;
