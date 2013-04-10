----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    21:32:50 02/27/2013 
-- Design Name: 
-- Module Name:    clocking - Behavioral 
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
-- Copyright (C) 2013 John Sully
--
-- Permission is hereby granted, free of charge, to any person obtaining a copy of 
-- this software and associated documentation files (the "Software"), to deal in 
-- the Software without restriction, including without limitation the rights to 
-- use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
-- the Software, and to permit persons to whom the Software is furnished to do so,
-- subject to the following conditions:
--
-- The above copyright notice and this permission notice shall be included in all
-- copies or substantial portions of the Software.
--
-- THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
-- IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
-- FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
-- COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
-- IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
-- CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
library UNISIM;
use UNISIM.VComponents.all;

entity clocking is
    Port ( CLK_100 : in  STD_LOGIC;
           CLK_DVI : out  STD_LOGIC;
           CLK_DVIn : out  STD_LOGIC;
           CLK_VGA : out  STD_LOGIC);
end clocking;

architecture Behavioral of clocking is
component clkm
port
 (-- Clock in ports
  CLK_IN1           : in     std_logic;
  -- Clock out ports
  CLK_OUT1          : out    std_logic;
  CLK_OUT2          : out    std_logic;
  CLK_OUT3          : out    std_logic

 );
end component;
	
	signal CLK0 : std_logic;
begin

	   DCM_SP_inst : DCM_SP
   generic map (
      CLKDV_DIVIDE => 4.0,                   -- CLKDV divide value
                                             -- (1.5,2,2.5,3,3.5,4,4.5,5,5.5,6,6.5,7,7.5,8,9,10,11,12,13,14,15,16).
      CLKFX_DIVIDE => 4,                     -- Divide value on CLKFX outputs - D - (1-32)
      CLKFX_MULTIPLY => 5,                   -- Multiply value on CLKFX outputs - M - (2-32)
      CLKIN_DIVIDE_BY_2 => FALSE,            -- CLKIN divide by two (TRUE/FALSE)
      CLKIN_PERIOD => 10.0,                  -- Input clock period specified in nS
      CLKOUT_PHASE_SHIFT => "NONE",          -- Output phase shift (NONE, FIXED, VARIABLE)
      CLK_FEEDBACK => "1X",                  -- Feedback source (NONE, 1X, 2X)
      DESKEW_ADJUST => "SYSTEM_SYNCHRONOUS", -- SYSTEM_SYNCHRNOUS or SOURCE_SYNCHRONOUS
      DFS_FREQUENCY_MODE => "LOW",           -- Unsupported - Do not change value
      DLL_FREQUENCY_MODE => "LOW",           -- Unsupported - Do not change value
      DSS_MODE => "NONE",                    -- Unsupported - Do not change value
      DUTY_CYCLE_CORRECTION => TRUE,         -- Unsupported - Do not change value
      FACTORY_JF => X"c080",                 -- Unsupported - Do not change value
      PHASE_SHIFT => 0,                      -- Amount of fixed phase shift (-255 to 255)
      STARTUP_WAIT => FALSE                  -- Delay config DONE until DCM_SP LOCKED (TRUE/FALSE)
   )
   port map (
      CLK0 => CLK0,         -- 1-bit output: 0 degree clock output
      --CLK180 => CLK180,     -- 1-bit output: 180 degree clock output
      --CLK270 => CLK270,     -- 1-bit output: 270 degree clock output
      --CLK2X => CLK2X,       -- 1-bit output: 2X clock frequency clock output
      --CLK2X180 => CLK2X180, -- 1-bit output: 2X clock frequency, 180 degree clock output
      --CLK90 => CLK90,       -- 1-bit output: 90 degree clock output
      CLKDV => CLK_VGA,       -- 1-bit output: Divided clock output
      CLKFX => CLK_DVI,       -- 1-bit output: Digital Frequency Synthesizer output (DFS)
      CLKFX180 => CLK_DVIn, -- 1-bit output: 180 degree CLKFX output
      --LOCKED => LOCKED,     -- 1-bit output: DCM_SP Lock Output
      --PSDONE => PSDONE,     -- 1-bit output: Phase shift done output
      --STATUS => STATUS,     -- 8-bit output: DCM_SP status output
      CLKFB => CLK0,       -- 1-bit input: Clock feedback input
      CLKIN => CLK_100,       -- 1-bit input: Clock input
      DSSEN => '0',       -- 1-bit input: Unsupported, specify to GND.
      PSCLK => '0',       -- 1-bit input: Phase shift clock input
      PSEN => '0',         -- 1-bit input: Phase shift enable
      PSINCDEC => '0', -- 1-bit input: Phase shift increment/decrement input
      RST => '0'            -- 1-bit input: Active high reset input
   );
end Behavioral;

