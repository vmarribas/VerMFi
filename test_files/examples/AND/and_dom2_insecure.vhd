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


--Input_shares: x1, x2, x3; y1, y2, y3.
--Random_vars: r1, r2, r3.
--Regs_layer: z1_reg, z2_reg, z3_reg, z4_reg, z5_reg, z6_reg, z7_reg, z8_reg, z9_reg.


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;


library work;
use work.MPC_globals.all;


entity and_dom2_insecure is
    port (  clk : in std_logic;
            x1, x2, x3 : in std_logic;
            y1, y2, y3 : in std_logic;
            r1, r2, r3 : in std_logic;
            z1, z2, z3 : out std_logic
    ) ;
end entity ;

architecture Behavioral of and_dom2_insecure is

    signal z1_next, z1_reg : std_logic;
    signal z2_next, z2_reg : std_logic;
    signal z3_next, z3_reg : std_logic;
    signal z4_next, z4_reg : std_logic;
    signal z5_next, z5_reg : std_logic;
    signal z6_next, z6_reg : std_logic;
    signal z7_next, z7_reg : std_logic;
    signal z8_next, z8_reg : std_logic;
 	signal z9_next, z9_reg : std_logic;
 
    signal z2_int : std_logic;
 
begin
    z1_next <= x1 and y1;
    z2_next <= (x1 and y2) xor r1;
    z3_next <= (x1 and y3) xor r2;

    z4_next <= (x2 and y1) xor r1;
    z5_next <= x2 and y2;
    z6_next <= (x2 and y3) xor r3;

    z7_next <= (x3 and y1) xor r2;
    z8_next <= (x3 and y2) xor r3;
    z9_next <= x3 and y3;

    ----------

    z1 <= z1_reg xor z2_reg xor z3_reg;
    z2_int <= z4_reg xor z5_reg xor z6_reg;
    z2 <= z2_int;

    z3 <= z7_reg xor z8_reg xor z9_reg xor z2_int;

 	seq : process(clk)
	begin 
		if(rising_edge(clk)) then 
			z1_reg <= z1_next;
            z2_reg <= z2_next;
            z3_reg <= z3_next;
            z4_reg <= z4_next;
            z5_reg <= z5_next;
            z6_reg <= z6_next;
            z7_reg <= z7_next;
            z8_reg <= z8_next;
            z9_reg <= z9_next;
		end if;
	end process;
 
 
end Behavioral ;