----------------------------------------------------------------------------------
-- Engineer:       Mike Field <hamster@snap.net.nz>
-- Module Name:    ColourTest - Behavioral 
-- Description:    Generates VGA signal timings
--
--	Code sourced from: http://hamsterworks.co.nz/mediawiki/index.php/Dvid_test
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use IEEE.NUMERIC_STD.ALL;
use IEEE.math_real.all;
Library UNISIM;
use UNISIM.vcomponents.all;

entity VGA_TIMING is
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
           hSync      : out STD_LOGIC;
           vSync      : out STD_LOGIC;
           blank      : out STD_LOGIC;
			  
			  pixel_x	 : out STD_LOGIC_VECTOR(10 downto 0);
			  pixel_y	 : out STD_LOGIC_VECTOR(10 downto 0)
			 
			  );
end VGA_TIMING;

architecture Behavioral of VGA_TIMING is
   type reg is record
      hCounter : std_logic_vector(11 downto 0);
      vCounter : std_logic_vector(11 downto 0);

      hSync    : std_logic;
      vSync    : std_logic;
      blank    : std_logic;		
   end record;

   signal r : reg := ((others=>'0'), (others=>'0'),
                      '0', '0', '0');
   signal n : reg;   
	signal delay : reg;
begin
   -- Assign the outputs
   hSync <= delay.hSync;
   vSync <= delay.vSync;
   blank <= delay.blank;

   process(r,n)
   begin		
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
         n.blank <= '0';
      else
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

delay <= r;
   process(pixelClock,n)
   begin
      if rising_edge(pixelClock)
      then
			pixel_x <= n.hCounter(10 downto 0);
			pixel_y <= n.vCounter(10 downto 0);
			r <= n;
         
      end if;
   end process;
end Behavioral;
