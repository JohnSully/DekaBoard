----------------------------------------------------------------------------------
-- Engineer: Mike Field <hamster@snap.net.nz>
-- 
-- Description: dvid_test 
--  Top level design for testing my DVI-D interface
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_MISC.ALL;
use IEEE.NUMERIC_STD.ALL;
Library UNISIM;
use UNISIM.vcomponents.all;
use work.misc_funcs.all;

entity DekaTop is
    Port ( CLK_100  : in  STD_LOGIC;
           tmds    : out  STD_LOGIC_VECTOR(3 downto 0);
           tmdsb   : out  STD_LOGIC_VECTOR(3 downto 0);
			  
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
			  
			  AUDIO_L : out STD_LOGIC;
			  AUDIO_R : out STD_LOGIC;
			  
			  GPIO1   : out STD_LOGIC := '0'
			  );
end DekaTop;

architecture Behavioral of DekaTop is
	COMPONENT ram_arbiter
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
	end component;
	
	component video is
	Port (clk_100 : in   STD_LOGIC;
			tmds    : out  STD_LOGIC_VECTOR(3 downto 0);
         tmdsb   : out  STD_LOGIC_VECTOR(3 downto 0);
			vSync_o : out  STD_LOGIC;
			
			mem_rd	: out STD_LOGIC;
			mem_addr : out STD_LOGIC_VECTOR(31 downto 0);
			mem_din	: in  STD_LOGIC_VECTOR(7 downto 0);
			mem_rdy  : in  STD_LOGIC;
			
			Blue_i   : in STD_LOGIC_VECTOR(7 downto 0)
		);
	end component;
	
	COMPONENT mcu
	  PORT (
		 Clk : IN STD_LOGIC;
		 Reset : IN STD_LOGIC;
		 IO_Addr_Strobe : OUT STD_LOGIC;
		 IO_Read_Strobe : OUT STD_LOGIC;
		 IO_Write_Strobe : OUT STD_LOGIC;
		 IO_Address : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
		 IO_Byte_Enable : OUT STD_LOGIC_VECTOR(3 DOWNTO 0);
		 IO_Write_Data : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
		 IO_Read_Data : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
		 IO_Ready : IN STD_LOGIC;
		 GPO1 : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
		 GPI1 : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
		 GPI1_Interrupt : OUT STD_LOGIC;
		 UART_Rx : IN STD_LOGIC;
		 UART_Tx : OUT STD_LOGIC
	  );
	END COMPONENT;

	
	signal vSync   : std_logic;
	
	signal MEM_RDY     : std_logic;

	signal FB_DIN		: std_logic_vector(7 downto 0);
	signal FB_ADDR		: std_logic_vector(31 downto 0);
	signal FB_RD		: std_logic;
	signal FB_RDY		: std_logic;
	
	signal MCU_RD		: std_logic;
	signal MCU_WR		: std_logic;
	signal MCU_ADDR	: std_logic_vector(31 downto 0);
	signal MCU_DIN		: std_logic_vector(31 downto 0);
	signal MCU_DOUT	: std_logic_vector(31 downto 0);
	signal MCU_MRDY	: std_logic;
	signal MCU_MEN		: std_logic;
	
	signal GPO_0		: std_logic_vector(31 downto 0);
	signal Blue       : std_logic_vector(7 downto 0);
	
	signal IO_Addr_Strobe : std_logic;
	signal IO_Read_Strobe : std_logic;
	signal IO_Write_Strobe :std_logic;
	signal IO_Address : std_logic_vector(31 downto 0);
	signal IO_Write_data : std_logic_vector(31 downto 0);
	signal IO_Ready : std_logic;
	signal IO_Addr_Strobe_r : std_logic;
	signal IO_Read_Strobe_r : std_logic;
	signal IO_Write_Strobe_r :std_logic;
	signal IO_Address_r : std_logic_vector(31 downto 0);
	signal IO_Write_data_r : std_logic_vector(31 downto 0);
	signal IO_EN_r : std_logic;
	
	signal t : std_logic_vector(7 downto 0);
	
	signal CLK_167 : std_logic;
	signal CLK0 : std_logic;
begin
   -- MEMORY
		   DCM_MEMCNT : DCM_SP
   generic map (
      CLKDV_DIVIDE => 4.0,                   -- CLKDV divide value
                                             -- (1.5,2,2.5,3,3.5,4,4.5,5,5.5,6,6.5,7,7.5,8,9,10,11,12,13,14,15,16).
      CLKFX_DIVIDE => 3,                     -- Divide value on CLKFX outputs - D - (1-32)
      CLKFX_MULTIPLY => 4,                   -- Multiply value on CLKFX outputs - M - (2-32)
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
      --CLKDV => CLK_VGA,       -- 1-bit output: Divided clock output
      CLKFX => CLK_167,       -- 1-bit output: Digital Frequency Synthesizer output (DFS)
      --CLKFX180 => CLK_DVIn, -- 1-bit output: 180 degree CLKFX output
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
	
	ram_arbit : ram_arbiter
	Port Map(
			  CLKI => CLK_167,
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
			  
			  EN => MCU_MEN,
			  RD => MCU_RD,
			  WR => MCU_WR,
			  ADDR => MCU_ADDR,
			  DIN => MCU_DIN(7 downto 0),
			  DOUT => MCU_DOUT(7 downto 0),
			  --RDY => MCU_MRDY,
			  OK_STROBE => MCU_MRDY,
			  
			  VBLANK => not vSync,
			  
			  --HIPRI_RD => FB_RD,
			  HIPRI_RD   => '0',
			  HIPRI_ADDR => FB_ADDR,
			  HIPRI_DOUT => FB_DIN,
			  HIPRI_RDY  => FB_RDY
			  );
		
		U_VIDEO :  video 
		Port Map 
		  (clk_100 => clk_100,
			tmds    => tmds,
         tmdsb   => tmdsb,
			vSync_o   => vSync,
			
			mem_rd	=> FB_RD,
			mem_addr => FB_ADDR,
			mem_din	=> FB_DIN,
			mem_rdy  => FB_RDY,
			
			Blue_i   => Blue
		);

		MCU_DIN(31 downto 8) <= x"000000";
				
		mcu0 : mcu
		PORT MAP (
		 Clk => clk_167,
		 Reset => '0',
		 IO_Addr_Strobe => IO_Addr_Strobe,
		 IO_Read_Strobe => IO_Read_Strobe,
		 IO_Write_Strobe => IO_Write_strobe,
		 IO_Address => IO_Address,
		 --IO_Byte_Enable => IO_Byte_Enable,
		 IO_Write_Data => IO_Write_Data,
		 IO_Read_Data => MCU_DOUT,
		 IO_Ready => IO_Ready,
		 GPO1 => GPO_0,
		 GPI1 => x"00000000",
		 Uart_RX => '0'
		 --GPI1_Interrupt => 
		);
	process(clk_100) begin
		if rising_edge(clk_100) then
			if IO_Addr_Strobe = '1' and IO_Address(30) = '1' then
				GPIO1 <= '1';
			end if;
		end if;
	end process;
	
	Blue <= GPO_0(7 downto 0);
	
	MCU_RD <= IO_Read_Strobe when IO_Addr_Strobe = '1' else IO_Read_Strobe_r;
	MCU_WR <= IO_Write_Strobe when IO_Addr_Strobe = '1' else IO_Write_Strobe_r;
	MCU_ADDR <= IO_Address when IO_Addr_Strobe = '1' else IO_Address_r;
	MCU_DIN <= IO_Write_Data when IO_Addr_Strobe = '1' else IO_Write_Data_r;
	MCU_MEN <= IO_Addr_Strobe or IO_EN_r;
	
	-- Nasty nasty latches but we need these because the mcu won't keep
	--		the signals asserted while it waits for the memory bus, and we don't
	--		want to add a cycle of latency for a FF
	process (clk_100) begin
		if rising_edge(clk_100) then
			if IO_Addr_Strobe = '1' then
				IO_Read_Strobe_r <= IO_Read_Strobe;
				IO_Write_Strobe_r <= IO_Write_Strobe;
				IO_Address_r <= IO_Address;
				IO_Write_Data_r <= IO_Write_Data;
				IO_EN_r <= '1';
				IO_Ready <= '0';
				t <= x"00";
			elsif MCU_MRDY = '1' then
				IO_EN_r <= '0';
				IO_Read_Strobe_r <= '0';
				IO_Write_Strobe_r <= '0';
				IO_Ready <= '1';
			elsif MCU_MRDY = '1' then
				t <= std_logic_vector(unsigned(t) + 1);
			else
				IO_Ready <= '0';
			end if;
		end if;
	end process;
	
	AUDIO_L <= '0';
	AUDIO_R <= '0';
	
end Behavioral;