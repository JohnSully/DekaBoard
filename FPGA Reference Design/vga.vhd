----------------------------------------------------------------------------------
-- Engineer:       Mike Field <hamster@snap.net.nz>
-- Module Name:    ColourTest - Behavioral 
-- Description:    Generates an 640x480 VGA showing all colours
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use IEEE.NUMERIC_STD.ALL;
Library UNISIM;
use UNISIM.vcomponents.all;

entity vga is
   generic (
      hRez       : natural := 640;	
      hStartSync : natural := 656;
      hEndSync   : natural := 752;
      hMaxCount  : natural := 800;
      hsyncActive : std_logic := '0';
		
      vRez       : natural := 480;
      vStartSync : natural := 490;
      vEndSync   : natural := 492;
      vMaxCount  : natural := 525;
      vsyncActive : std_logic := '1'
   );

    Port ( pixelClock : in  STD_LOGIC;
           Red        : out STD_LOGIC_VECTOR (7 downto 0);
           Green      : out STD_LOGIC_VECTOR (7 downto 0);
           Blue       : out STD_LOGIC_VECTOR (7 downto 0);
           hSync      : out STD_LOGIC;
           vSync      : out STD_LOGIC;
           blank      : out STD_LOGIC
			 
			  );
end vga;

architecture Behavioral of vga is
   type reg is record
      hCounter : std_logic_vector(11 downto 0);
      vCounter : std_logic_vector(11 downto 0);

      red      : std_logic_vector(7 downto 0);
      green    : std_logic_vector(7 downto 0);
      blue     : std_logic_vector(7 downto 0);

      hSync    : std_logic;
      vSync    : std_logic;
      blank    : std_logic;		
   end record;
	
COMPONENT CHAR_ROM
  PORT (
    a : IN STD_LOGIC_VECTOR(11 DOWNTO 0);
    spo : OUT STD_LOGIC_VECTOR(7 DOWNTO 0)
  );
END COMPONENT;

   signal r : reg := ((others=>'0'), (others=>'0'),
                      (others=>'0'), (others=>'0'), (others=>'0'), 
                      '0', '0', '0');
   signal n : reg;   
	
	signal charrom_x : std_logic_vector(2 downto 0);
	signal charrom_y : std_logic_vector(3 downto 0);
	signal char_x		: std_logic_vector(6 downto 0);
	signal char_y		: std_logic_vector(5 downto 0);
	signal charrom_addr : std_logic_vector(11 downto 0);
	signal char_val   : std_logic_vector(7 downto 0);
	signal char_bit   : std_logic;
	
begin
   -- Assign the outputs
   hSync <= r.hSync;
   vSync <= r.vSync;
   Red   <= r.red;
   Green <= r.green;
   Blue  <= r.blue;
   blank <= r.blank;
	
   -- TEXT GENERATOR START
	charrom_x <= n.hCounter(2 downto 0);
	charrom_y <= n.vCounter(3 downto 0);
	char_x <= n.hCounter(9 downto 3);
	char_y <= n.vCounter(8 downto 3);
	charrom_addr <= x"52" & charrom_y;
	with charrom_x select
		char_bit <= char_val(7) when "000",
						char_val(6) when "001",
						char_val(5) when "010",
						char_val(4) when "011",
						char_val(3) when "100",
						char_val(2) when "101",
						char_val(1) when "110",
						char_val(0) when "111",
						'0' when others;
	CHAR_ROM : CHAR_ROM PORT MAP (
		 a => charrom_addr,
		 spo => char_val
	  );

   process(r,n)
   begin

		-- VGA SIGNALS START
		
      n <= r;
      n.hSync <= not hSyncActive;      
      n.vSync <= not vSyncActive;      

      -- Count the lines and rows      
      if r.hCounter = hMaxCount-1 then
         n.hCounter <= (others => '0');
         if r.vCounter = vMaxCount-1 then
            n.vCounter <= (others => '0');
         else
            n.vCounter <= r.vCounter+1;
         end if;
      else
         n.hCounter <= r.hCounter+1;
      end if;

      if r.hCounter  < hRez and r.vCounter  < vRez then
			n.green(7) <= char_bit;
         n.blank <= '0';
      else
         n.red   <= (others => '0');
         n.green <= (others => '0');
         n.blue  <= (others => '0');
         n.blank <= '1';
      end if;
      
      -- Are we in the hSync pulse?
      if r.hCounter >= hStartSync and r.hCounter < hEndSync then
         n.hSync <= hSyncActive;
      end if;

      -- Are we in the vSync pulse?
      if r.vCounter >= vStartSync and r.vCounter < vEndSync then
         n.vSync <= vSyncActive; 
      end if;
   end process;

   process(pixelClock,n)
   begin
      if rising_edge(pixelClock)
      then
         r <= n;
      end if;
   end process;
end Behavioral;
