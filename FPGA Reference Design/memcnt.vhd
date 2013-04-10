----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 	John Sully
-- 
-- Create Date:    02:41:50 02/28/2013 
-- Design Name: 
-- Module Name:    memcnt - Behavioral 
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
--		Burst and pipelining not yet supported.
--		Data is valid on OK_STROBE pulse.  RDY signal is buggy, do not use.
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
use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
library UNISIM;
use UNISIM.VComponents.all;

use work.misc_funcs.all;

entity memcnt is
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
			  VBLANK : in STD_LOGIC;	-- Vertical blanking @ 60hz (synchronizes refresh)
			  RD : in STD_LOGIC;
			  WR : in STD_LOGIC;
			  ADDR : in STD_LOGIC_VECTOR(31 downto 0);
			  DIN : in STD_LOGIC_VECTOR(7 downto 0);
			  DOUT : out STD_LOGIC_VECTOR(7 downto 0);
			  RDY : out STD_LOGIC;
			  OK_STROBE : out STD_LOGIC
			  );
end memcnt;

architecture Behavioral of memcnt is
	signal refresh_timer : std_logic_vector (23 downto 0) := x"000000";
	signal wait_timer    : std_logic_vector (3 downto 0) := x"0";				   -- Delay for command synch
	signal is_initialized : std_logic := '0';
	signal refresh_pending : std_logic := '0';
	signal all_precharged : std_logic;
	signal not_ready  : std_logic;
	
	
	signal row_activated : std_logic;
	signal activated_bank : std_logic_vector(1 downto 0);
	signal activated_row  : std_logic_vector(12 downto 0);
	
	signal dout_en    : std_logic;
	signal mem_dout		: std_logic_vector(7 downto 0);
	
	signal bank_in 	: std_logic_vector(1 downto 0);
	signal row_in		: std_logic_vector(12 downto 0);
	signal col_in		: std_logic_vector(9 downto 0);
	
	signal MEM_DQin :  STD_LOGIC_VECTOR(7 downto 0);
	signal MEM_DQout :  STD_LOGIC_VECTOR(7 downto 0);
   signal MEM_DQouten :  STD_LOGIC;
	
	signal vblank_cnt : STD_LOGIC_VECTOR(1 downto 0);
	signal vblank_last : STD_LOGIC;
	
	signal op_needs_strobe : STD_LOGIC;
	
	signal refresh_count : std_logic_vector(23 downto 0);
begin
	--refresh_count <= x"81E200"; -- 64ms @ 133Mhz
	--refresh_count <= x"61A800";   -- 64 ms @ 100Mhz
	refresh_count <=   x"00030D";

	MEM_CS <= '0';
	
	MEM_DQ <= MEM_DQout when MEM_DQouten = '1' else "ZZZZZZZZ";
	MEM_DQout <= mem_dout;
	MEM_DQouten <= dout_en;
	MEM_DQin <= MEM_DQ;
	
	col_in <= ADDR(9 downto 0);
	row_in <= ADDR(22 downto 10);
	bank_in <= ADDR(24 downto 23);
	DOUT <= MEM_DQin;
	
	-- Can't directly route an internal clock to a pin (src'd from a BUFG)
	-- ODDR2 workaround as suggested by xilinx
	   ODDR2_inst : ODDR2
   generic map(
      DDR_ALIGNMENT => "NONE",
      INIT => '0',
      SRTYPE => "SYNC")
		port map (
			Q => MEM_CLK,    -- 1-bit output data
			C0 => CLKI,       -- 1-bit clock input
			C1 => (NOT CLKI), -- 1-bit clock input
			CE => '1',              -- 1-bit clock enable input
			D0 => '1',
			D1 => '0',
			R => '0',    -- 1-bit reset input
			S => '0'     -- 1-bit set input
		);
	
	process (CLKI) begin
		if rising_edge(CLKI) then
			vblank_last <= vblank;
			if is_initialized = '0' then
				-- Refresh timer is initially used to synch the boot sequence state machine, then it is used for its named purpose
				refresh_timer <= std_logic_vector(unsigned(refresh_timer)+1);
				MEM_DQM <= '1';
				RDY <= '0';
				MEM_BA <= "00";
				
				if unsigned(refresh_timer) <= 13300 then
					-- Initialization wait
					-- NOP
					MEM_WEn <= '1';
					MEM_CASn <= '1';
					MEM_RASn <= '1';
					MEM_A <= '0' & x"000";
					dout_en <= '0';
					if unsigned(refresh_timer) < 10000 then
						MEM_CKE <= '0';
					else
						MEM_CKE <= '1';  -- Starting at some point during this 100µs period, bring CKE HIGH. Continuing at least through the end of this period
					end if;
				-- Boot sequence
				elsif unsigned(refresh_timer) = 13301 then	-- Initialization wait complete
					-- First Precharge All
					MEM_A(10) <= '1';
					MEM_RASn <= '0';
					MEM_CASn <= '1';
					MEM_WEn <= '0';
				elsif unsigned(refresh_timer) <= 13303 then
					-- NOP for tRP
					MEM_A(10) <= '0';
					MEM_RASn <= '1';
					MEM_CASn <= '1';
					MEM_WEn  <= '1';
				elsif unsigned(refresh_timer) = 13304	then
					-- Auto Refresh
					MEM_RASn <= '0';
					MEM_CASn <= '0';
					MEM_WEn  <= '1';
				elsif unsigned(refresh_timer) <= 13311 then
					-- NOP for tRFC
					MEM_RASn <= '1';
					MEM_CASn <= '1';
					MEM_WEn  <= '1';
				elsif unsigned(refresh_timer) = 13312 then
					-- Auto Refresh
					MEM_RASn <= '0';
					MEM_CASn <= '0';
					MEM_WEn  <= '1';
				elsif unsigned(refresh_timer) <= 13319 then
					-- NOP for tRFC
					MEM_RASn <= '1';
					MEM_CASn <= '1';
					MEM_WEn  <= '1';
				elsif unsigned(refresh_timer) = 13320 then
					-- Load Mode Register
					MEM_A(9) <= '1';	-- NO BURST
					MEM_A(8 downto 7) <= "00"; -- Reserved
					MEM_A(6 downto 4) <= "010"; -- CAS LATENCY 2
					MEM_A(3) <= '0';  -- SEQUENTIAL BURST
					MEM_A(2 downto 0) <= "000"; -- 1 unit burst
					MEM_RASn <= '0';
					MEM_CASn <= '0';
					MEM_WEn  <= '0';
					
				elsif unsigned(refresh_timer) <= 13321 then
					-- NOP
					MEM_RASn <= '1';
					MEM_CASn <= '1';
					MEM_WEn  <= '1';
				else
					MEM_RASn <= '1';
					MEM_CASn <= '1';
					MEM_WEn  <= '1';
					all_precharged <= '0';
					
					is_initialized <= '1';
					refresh_timer <= refresh_count;
					RDY <= '1';
				end if;
			else -- Done Initialization
				dout_en <= '0';
				
				--Timeout Refresh Detection
				if refresh_timer = x"000000" then
					refresh_pending <= '1';
				else
					refresh_timer <= std_logic_vector(unsigned(refresh_timer)-1);
				end if;
				--VBLANK based Refresh detection
				if vblank = '1' and vblank /= vblank_last then
					if unsigned(vblank_cnt) = 2 then
						vblank_cnt <= "00";
						refresh_pending <= '1';
						refresh_timer <= refresh_count;
					else
						vblank_cnt <= std_logic_vector(unsigned(vblank_cnt)+1);
					end if;
				end if;
				
				if unsigned(wait_timer) > 0 then
					-- NOP for wait period
					MEM_RASn <= '1';
					MEM_CASn <= '1';
					MEM_WEn  <= '1';
					wait_timer <= std_logic_vector(unsigned(wait_timer)-1);
					MEM_DQM <= '1';
					if unsigned(wait_timer) = 1 then
						RDY <= '1';
						OK_STROBE <= op_needs_strobe;
						op_needs_strobe <= '0';
					else
						RDY <= '0';
						OK_STROBE <= '0';
					end if;
				else
					-- Can process command
					OK_STROBE <= '0';
					op_needs_strobe <= '0';
					
					if refresh_pending = '1' then
						-- Do refresh
						dout_en <= '0';
						RDY <= '0';
						if all_precharged = '0' then
							--Must precharge all
							MEM_RASn <= '0';
							MEM_CASn <= '1';
							MEM_WEn <= '0';
							MEM_A(10) <= '1';
							wait_timer <= x"5";
							all_precharged <= '1';
							row_activated <= '0';
						else
							-- OK to refresh
							MEM_RASn <= '0';
							MEM_CASn <= '0';
							MEM_WEn <= '1';
							MEM_A(10) <= '1';
							MEM_DQM <= '1';
							wait_timer <= x"8";
							refresh_timer <= refresh_count;
							refresh_pending <= '0';
						end if;
					elsif RD = '1' or WR = '1' then
						if row_activated = '0' or (bank_in /= activated_bank) or (row_in /= activated_row) then
							if all_precharged = '0' then
								-- Must precharge before new activation
								MEM_RASn <= '0';
								MEM_CASn <= '1';
								MEM_WEn <= '0';
								MEM_A(10) <= '1';
								wait_timer <= x"5";
								all_precharged <= '1';
								row_activated <= '0';
							else
								-- ACTIVATE
								MEM_RASn <= '0';
								MEM_CASn <= '1';
								MEM_WEn <= '1';
								all_precharged <= '0';
								row_activated <= '1';
								activated_bank <= bank_in;
								activated_row  <= row_in;
								MEM_A <= row_in;
								MEM_BA <= bank_in;
								wait_timer <= x"3";
								dout_en <= '0';
							end if;
						-- EVERYTHING IS ACTIVATED SO PROCESS THE COMMAND
						else
							-- PROCESS READ AND WRITE COMMANDS
							MEM_RASn <= '1';
							MEM_CASn <= '0';
							MEM_WEn <= not WR;
							MEM_DQM <= '0';
							MEM_A(10) <= '0';		-- NO AUTO precharge
							MEM_A(9 downto 0) <= col_in;
							all_precharged <= '0';
							MEM_BA <= bank_in;
							dout_en <= WR;
							RDY <= '0';
							wait_timer <= x"2";
							op_needs_strobe <= '1';
							mem_dout <= DIN;
						end if;
					else
						-- NOP
						RDY <= '1';
						MEM_DQM <= '1';
						MEM_RASn <= '1';
						MEM_CASn <= '1';
						MEM_WEn  <= '1';
						wait_timer <= x"0";
						dout_en <= '0';
						op_needs_strobe <= '0';
					end if;
				end if;
			end if;
		end if;
	end process;
end Behavioral;

