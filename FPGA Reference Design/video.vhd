----------------------------------------------------------------------------------
-- Company: 
-- Engineer: John Sully
-- 
-- Create Date:    15:24:17 03/07/2013 
-- Design Name: 
-- Module Name:    video - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: 
--		Interconnects VGA signal timing, the frame buffer, and DVI driver components.
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

entity video is
	Port (clk_100 : in   STD_LOGIC;
			tmds    : out  STD_LOGIC_VECTOR(3 downto 0);
         tmdsb   : out  STD_LOGIC_VECTOR(3 downto 0);
			vSync_o  : out  STD_LOGIC;
			
			mem_rd	: out STD_LOGIC;
			mem_addr : out STD_LOGIC_VECTOR(31 downto 0);
			mem_din	: in  STD_LOGIC_VECTOR(7 downto 0);
			mem_rdy  : in  STD_LOGIC;
			
			--TEMP DEBUG
			Blue_i	: in STD_LOGIC_VECTOR(7 downto 0)
		);
end video;

architecture Behavioral of video is
	component clocking
   port (
      -- Clock in ports
      CLK_100           : in     std_logic;
      -- Clock out ports
      CLK_DVI          : out    std_logic;
      CLK_DVIn         : out    std_logic;
      CLK_VGA          : out    std_logic
   );
   end component;

   COMPONENT dvid
   PORT(
      clk      : IN std_logic;
      clk_n    : IN std_logic;
      clk_pixel: IN std_logic;
      red_p   : IN std_logic_vector(7 downto 0);
      green_p : IN std_logic_vector(7 downto 0);
      blue_p  : IN std_logic_vector(7 downto 0);
      blank   : IN std_logic;
      hsync   : IN std_logic;
      vsync   : IN std_logic;          
      red_s   : OUT std_logic;
      green_s : OUT std_logic;
      blue_s  : OUT std_logic;
      clock_s : OUT std_logic
      );
   END COMPONENT;
	
	component framebuffer
	 generic (
		hRez			: natural;
		vRez			: natural
	 );
    Port ( mem_clk 	: in STD_LOGIC;
			  pixel_clk : in  STD_LOGIC;
           Red : out  STD_LOGIC_VECTOR (7 downto 0);
           Green : out  STD_LOGIC_VECTOR (7 downto 0);
           Blue : out  STD_LOGIC_VECTOR (7 downto 0);
           pixel_x : in  STD_LOGIC_VECTOR (10 downto 0);
           pixel_y : in  STD_LOGIC_VECTOR (10 downto 0);
			  blanking : in STD_LOGIC;
			  hSync	  : in STD_LOGIC;
			  vSync    : in STD_LOGIC;
           mem_rd : out  STD_LOGIC;
           mem_addr : out  STD_LOGIC_VECTOR (31 downto 0);
           mem_din : in  STD_LOGIC_VECTOR (7 downto 0);
			  mem_rdy : in STD_LOGIC
			  );
	end component;

   COMPONENT VGA_TIMING
   generic (
      hRez        : natural;
      hStartSync  : natural;
      hEndSync    : natural;
      hMaxCount   : natural;
      hsyncActive : std_logic;

      vRez        : natural;
      vStartSync  : natural;
      vEndSync    : natural;
      vMaxCount   : natural;
      vsyncActive : std_logic
    );
   PORT(
      pixelClock : IN std_logic;          
      hSync : OUT std_logic;
      vSync : OUT std_logic;
      blank : OUT std_logic;
		
		pixel_x : OUT std_logic_vector(10 downto 0);
		pixel_y : OUT std_logic_Vector(10 downto 0)
      );
   END COMPONENT;
	
	
	   signal clk_dvi  : std_logic := '0';
		signal clk_dvin : std_logic := '0';
		signal clk_vga  : std_logic := '0';

		signal red     : std_logic_vector(7 downto 0) := (others => '0');
		signal green   : std_logic_vector(7 downto 0) := (others => '0');
		signal blue    : std_logic_vector(7 downto 0) := (others => '0');
		signal hsync   : std_logic := '0';
		signal vsync   : std_logic := '0';
		signal blank   : std_logic := '0';
		signal red_s   : std_logic;
		signal green_s : std_logic;
		signal blue_s  : std_logic;
		signal clock_s : std_logic;
		signal pixel_x : std_logic_vector(10 downto 0);
		signal pixel_y : std_logic_vector(10 downto 0);
begin
		vSync_o <= vSync;
		Blue <= Blue_i;
		
	   U_FRAMEBUFFER : framebuffer
			generic map (
			hRez			=> 640,
			vRez			=> 480
			)
			Port  map( 
			  mem_clk 	=> clk_100,
			  pixel_clk => clk_vga,
           Red => Red,
           Green => Green,
           --Blue => Blue,
           pixel_x => pixel_x,
           pixel_y => pixel_y,
			  blanking => blank,
			  hSync => hSync,
			  vSync => vSync,
           mem_rd => mem_rd,
           mem_addr => mem_addr,
           mem_din => mem_din,
			  mem_rdy => mem_rdy
			  );
			  

		clocking_inst : clocking port map (
				CLK_100   => CLK_100,
				-- Clock out ports
				CLK_DVI  => clk_dvi,
				CLK_DVIn => clk_dvin,
				CLK_VGA  => clk_vga
			 );

		Inst_dvid: dvid PORT MAP(
				clk       => clk_dvi,
				clk_n     => clk_dvin, 
				clk_pixel => clk_vga,
				red_p     => red,
				green_p   => green,
				blue_p    => blue,
				blank     => blank,
				hsync     => hsync,
				vsync     => vsync,
				-- outputs to TMDS drivers
				red_s     => red_s,
				green_s   => green_s,
				blue_s    => blue_s,
				clock_s   => clock_s
			);
   
		OBUFDS_blue  : OBUFDS port map ( O  => TMDS(0), OB => TMDSB(0), I  => blue_s  );
		OBUFDS_red   : OBUFDS port map ( O  => TMDS(1), OB => TMDSB(1), I  => green_s );
		OBUFDS_green : OBUFDS port map ( O  => TMDS(2), OB => TMDSB(2), I  => red_s   );
		OBUFDS_clock : OBUFDS port map ( O  => TMDS(3), OB => TMDSB(3), I  => clock_s );  
   
	Inst_vga: VGA_TIMING GENERIC MAP (
      hRez       => 640, hStartSync => 656, hEndSync   => 752, hMaxCount  => 800, hsyncActive => '0',
      vRez       => 480, vStartSync => 490, vEndSync   => 492, vMaxCount  => 525, vsyncActive => '1'
   ) PORT MAP(
      pixelClock => clk_vga,
      hSync      => hSync,
      vSync      => vSync,
      blank      => blank,
		
		pixel_x	  => pixel_x,
		pixel_y	  => pixel_y
   );
end Behavioral;

