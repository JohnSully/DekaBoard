----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    02:22:01 03/07/2013 
-- Design Name: 
-- Module Name:    framebuffer - Behavioral 
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

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity framebuffer is
	generic (
		hRez			: natural;	-- MAX: 1280
		vRez			: natural;  -- MAX: 768
		bytes_pix   : natural := 3	-- 3 bytes = true colour, 2 bytes = HI Color, 1 bye = 256 Colors (no palette)
	 );
    Port ( 
			  mem_clk	: in STD_LOGIC;
			  pixel_clk : in  STD_LOGIC;
           Red : out  STD_LOGIC_VECTOR (7 downto 0);
           Green : out  STD_LOGIC_VECTOR (7 downto 0);
           Blue : out  STD_LOGIC_VECTOR (7 downto 0);
           pixel_x : in  STD_LOGIC_VECTOR (10 downto 0);
           pixel_y : in  STD_LOGIC_VECTOR (10 downto 0);
			  blanking : in STD_LOGIC;
			  hSync	  : in STD_LOGIC;
			  vSync	  : in STD_LOGIC;
			  
           mem_rd : out  STD_LOGIC;
           mem_addr : out  STD_LOGIC_VECTOR (31 downto 0);
           mem_din : in  STD_LOGIC_VECTOR (7 downto 0);
			  mem_rdy : in STD_LOGIC
			  );
end framebuffer;

architecture Behavioral of framebuffer is
	COMPONENT linebuffer
	  PORT (
		 clka : IN STD_LOGIC;
		 ena : IN STD_LOGIC;
		 wea : IN STD_LOGIC_VECTOR(0 DOWNTO 0);
		 addra : IN STD_LOGIC_VECTOR(11 DOWNTO 0);
		 dina : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
		 clkb : IN STD_LOGIC;
		 addrb : IN STD_LOGIC_VECTOR(11 DOWNTO 0);
		 doutb : OUT STD_LOGIC_VECTOR(7 DOWNTO 0)
	  );
	END COMPONENT;

	signal buffer_r  : std_logic_vector(7 downto 0);
	signal buffer_g  : std_logic_vector(7 downto 0);
	signal buffer_b  : std_logic_vector(7 downto 0);
	signal pixel_clk_last : std_logic;
	signal delay     : std_logic_vector(3 downto 0);
	
	signal counter_line  : std_logic_vector(9 downto 0);
	signal counter_pixel : std_logic_vector(11 downto 0);
	signal vSync_last   : std_logic;
	signal hSync_last   : std_logic;
	signal pixelclk_last : std_logic;
	signal rdy_last : std_logic;
	signal reg_memrd : std_logic;
	
	signal en_linebufa   : std_logic;
	signal addr_linebufa : std_logic_vector(11 downto 0);
	signal d_linebufa		: std_logic_vector(7 downto 0);
begin
	

	U_LINEBUF : linebuffer
	  PORT MAP (
		 clka => mem_clk,
		 ena => en_linebufa,
		 wea => "1",
		 addra => addr_linebufa,
		 dina => d_linebufa,
		 
		 clkb => pixel_clk,
		 addrb => "0" & pixel_x
		 --doutb => Red
	  );
	
	Red <= x"FF";
	
	mem_addr <= "00" & x"00" & "0000" & pixel_y(8 downto 0) & pixel_x(8 downto 0);

	process (mem_clk) begin
		if rising_edge(mem_clk) then
			pixel_clk_last <= pixel_clk;
			
			if pixel_clk = '0' and pixel_clk_last /= pixel_clk then  -- Pixel transition
				mem_rd <= not blanking;
				delay <= x"1";
			elsif mem_rdy = '1' and unsigned(delay) > 0 then
				delay <= std_logic_vector(unsigned(delay)-1);
			elsif mem_rdy = '1' and delay = x"0" then
				Green <= mem_din;
				mem_rd <= '0';
			end if;
		end if;
	end process;
--	mem_addr <= x"0000" & "00000" & pixel_x;
--	mem_rd <= reg_memrd;
--	process (mem_clk) begin
--		if rising_edge(mem_clk) then
--			pixelclk_last <= pixel_clk;
--			rdy_last <= mem_rdy;
--			if rdy_last = '0' and mem_rdy = '1' then
--				Red <= mem_din;
--				reg_memrd <= '0';
--			end if;
--			if pixel_clk = '1' and pixelclk_last = '0' and blanking = '0' then
--				reg_memrd <= '0';
--			end if;
--		end if;
--	end process;
end Behavioral;

