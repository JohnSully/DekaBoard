----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    01:31:28 03/07/2013 
-- Design Name: 
-- Module Name:    ram_arbiter - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: 
--			This module implenets dual ports to the SDRAM.  Used to give priority access to display driver
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--		Not yet operating correctly.
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
use work.misc_funcs.all;
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity ram_arbiter is
	Port (
			  CLKI : in STD_LOGIC;
			  -- DRAM Interface
			  MEM_CLK : out STD_LOGIC;
			  MEM_CKE : out STD_LOGIC;
			  MEM_CS : out STD_LOGIC;
			  MEM_DQM : out STD_LOGIC;
			  MEM_BA : out STD_LOGIC_VECTOR(1 downto 0);
			  MEM_A : out STD_LOGIC_VECTOR(12 downto 0);
			  MEM_DQ : inout STD_LOGIC_VECTOR(7 downto 0);
			  MEM_WEn : out STD_LOGIC;
			  MEM_CASn : out STD_LOGIC;
			  MEM_RASn : out STD_LOGIC;
			  
			  -- Public Interface
			  EN : in STD_LOGIC;
			  RD : in STD_LOGIC;
			  WR : in STD_LOGIC;
			  ADDR : in STD_LOGIC_VECTOR(31 downto 0);
			  DIN : in STD_LOGIC_VECTOR(7 downto 0);
			  DOUT : out STD_LOGIC_VECTOR(7 downto 0);
			  RDY : out STD_LOGIC;
			  DATA_VALID : out STD_LOGIC;
			  OK_STROBE : out STD_LOGIC;
			  
			  VBLANK : in STD_LOGIC;
			  
			  -- HI-PRI sequential readonly interface
			  HIPRI_RD : in STD_LOGIC;
			  HIPRI_ADDR : in STD_LOGIC_VECTOR(31 downto 0);
			  HIPRI_DOUT : out STD_LOGIC_VECTOR(7 downto 0);
			  HIPRI_RDY  : out STD_LOGIC
		);
end ram_arbiter;

architecture Behavioral of ram_arbiter is
	COMPONENT memcnt
	Port (
			  CLKI : in STD_LOGIC;
			  MEM_CLK : out STD_LOGIC;
			  MEM_CKE : out STD_LOGIC;
			  MEM_CS : out STD_LOGIC;
			  MEM_DQM : out STD_LOGIC;
			  MEM_BA : out STD_LOGIC_VECTOR(1 downto 0);
			  MEM_A : out STD_LOGIC_VECTOR(12 downto 0);
			  MEM_DQ : inout STD_LOGIC_VECTOR(7 downto 0);
			  MEM_WEn : out STD_LOGIC;
			  MEM_CASn : out STD_LOGIC;
			  MEM_RASn : out STD_LOGIC;
			  
			  -- Public Interface
			  RD : in STD_LOGIC;
			  WR : in STD_LOGIC;
			  ADDR : in STD_LOGIC_VECTOR(31 downto 0);
			  DIN : in STD_LOGIC_VECTOR(7 downto 0);
			  DOUT : out STD_LOGIC_VECTOR(7 downto 0);
			  RDY : out STD_LOGIC;
			  OK_STROBE : out STD_LOGIC;
			  
			  VBLANK : in STD_LOGIC
			  );
	end component;
	
	signal MUX_RD  : STD_LOGIC;
	signal MUX_WR  : STD_LOGIC;
	signal MUX_RDY : STD_LOGIC;
	signal MUX_ADDR : STD_LOGIC_VECTOR(31 downto 0);
	signal MUX_DOUT : STD_LOGIC_VECTOR(7 downto 0);
	signal RDY_LAST : STD_LOGIC;
	
	signal EN_LATCH : STD_LOGIC;
begin

	-- INPUTS
	MUX_RD		<= (RD and EN) or HIPRI_RD;
	MUX_WR		<= (WR and EN) and not HIPRI_RD;
	MUX_ADDR 	<= HIPRI_ADDR when HIPRI_RD = '1' else ADDR;
	DOUT			<= MUX_DOUT;		-- Should be ignored when RDY = '0'
	HIPRI_DOUT 	<= MUX_DOUT;
	
	RDY       <= MUX_RDY and not HIPRI_RD;
	HIPRI_RDY <= MUX_RDY and HIPRI_RD;
	
	DATA_VALID <= MUX_RDY and not HIPRI_RD;
	
	U_MEMCNT : MEMCNT
	Port Map(
			  CLKI => CLKI,
			  MEM_CLK => MEM_CLK,
			  MEM_CKE => MEM_CKE,
			  MEM_CS => MEM_CS,
			  MEM_DQM => MEM_DQM,
			  MEM_BA => MEM_BA,
			  MEM_A => MEM_A,
			  MEM_DQ => MEM_DQ,
			  MEM_WEn => MEM_WEn,
			  MEM_CASn => MEM_CASn,
			  MEM_RASn => MEM_RASn,
			  
			  RD => MUX_RD,
			  WR => MUX_WR,
			  ADDR => MUX_ADDR,
			  DIN => DIN,
			  DOUT => MUX_DOUT,
			  RDY => MUX_RDY,
			  OK_STROBE => OK_STROBE,
			  
			  VBLANK => VBLANK
			  );
end Behavioral;

