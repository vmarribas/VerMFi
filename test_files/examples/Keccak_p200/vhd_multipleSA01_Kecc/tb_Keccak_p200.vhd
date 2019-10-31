--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   16:36:42 05/24/2016
-- Design Name:   
-- Module Name:   /users/cosic/varribas/Documents/KETJE/tb_Keccak_p200.vhd
-- Project Name:  KETJE
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: Keccak_p200
-- 
-- Dependencies:
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
--
-- Notes: 
-- This testbench has been automatically generated using types std_logic and
-- std_logic_vector for the ports of the unit under test.  Xilinx recommends
-- that these types always be used for the top-level I/O of a design in order
-- to guarantee that the testbench will bind correctly to the post-implementation 
-- simulation model.
--------------------------------------------------------------------------------
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
