--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   18:50:49 03/06/2013
-- Design Name:   
-- Module Name:   C:/Users/John/fpga_projects/DekaBoard/memcnt_test.vhd
-- Project Name:  DekaBoard
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: memcnt
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
 
ENTITY memcnt_test IS
END memcnt_test;
 
ARCHITECTURE behavior OF memcnt_test IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
    COMPONENT memcnt
    PORT(
         CLKI : IN  std_logic;
         MEM_CLK : OUT  std_logic;
         MEM_CKE : OUT  std_logic;
         MEM_CS : OUT  std_logic;
         MEM_DQM : OUT  std_logic;
         MEM_BA : OUT  std_logic_vector(1 downto 0);
         MEM_A : OUT  std_logic_vector(12 downto 0);
         MEM_DQ : INOUT  std_logic_vector(7 downto 0);
         MEM_WEn : OUT  std_logic;
         MEM_CASn : OUT  std_logic;
         MEM_RASn : OUT  std_logic;
			OK_STROBE : OUT std_logic;
			
			vblank : in std_logic;
			
			-- Public Interface
			  RD : in STD_LOGIC;
			  WR : in STD_LOGIC;
			  ADDR : in STD_LOGIC_VECTOR(31 downto 0);
			  DIN : in STD_LOGIC_VECTOR(7 downto 0);
			  DOUT : out STD_LOGIC_VECTOR(7 downto 0);
			  RDY : out STD_LOGIC
        );
    END COMPONENT;
    
	 COMPONENT mt48lc32m8a2 
	 PORT (
		Dq : inout std_logic_vector(7 downto 0); 
		Addr : in std_logic_vector(12 downto 0);
		Ba : in std_logic_vector(1 downto 0);
		Clk : in std_logic; 
		Cke : in std_logic; 
		Cs_n : in std_logic; 
		Ras_n : in std_logic; 
		Cas_n : in std_logic; 
		We_n : in std_logic;
		Dqm : in std_logic);
	 End Component;

   --Inputs
   signal CLKI : std_logic := '0';

	--BiDirs
   signal MEM_DQ : std_logic_vector(7 downto 0);

 	--Outputs
   signal MEM_CLK : std_logic;
   signal MEM_CKE : std_logic;
   signal MEM_CS : std_logic;
   signal MEM_DQM : std_logic;
   signal MEM_BA : std_logic_vector(1 downto 0);
   signal MEM_A : std_logic_vector(12 downto 0);
   signal MEM_WEn : std_logic;
   signal MEM_CASn : std_logic;
   signal MEM_RASn : std_logic;
   signal DOUT : std_logic_vector(7 downto 0);
   signal RDY : std_logic;
	signal OK_STROBE : std_logic;

   -- Clock period definitions
   constant CLKI_period : time := 10 ns;
   constant MEM_CLK_period : time := 10 ns;
 
	signal RD : std_logic;
	signal WR : std_logic;
	
	signal reading_data : std_logic;
	signal ADDR : std_logic_vector(31 downto 0);
BEGIN
 
	-- Instantiate the Unit Under Test (UUT)
   uut: memcnt PORT MAP (
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
          DOUT => DOUT,
          RDY => RDY,
			 
			 vblank =>'0',
			 RD => RD,
			 WR => WR,
			 ADDR => ADDR,
			 DIN => x"FE",
			 OK_STROBE => OK_STROBE
        );
		  
	u_mem : mt48lc32m8a2 PORT MAP
		(Dq => MEM_DQ, 
		Addr => MEM_A, 
		Ba => MEM_BA, 
		Clk => MEM_CLK, 
		Cke => MEM_CKE, 
		Cs_n => MEM_CS, 
		Ras_n => MEM_RASn, 
		Cas_n => MEM_CASn, 
		We_n => MEM_WEn, 
		Dqm => MEM_DQM
		);

   -- Clock process definitions
   CLKI_process :process
   begin
		CLKI <= '0';
		wait for CLKI_period/2;
		CLKI <= '1';
		wait for CLKI_period/2;
   end process;
 
 

   -- Stimulus process
   stim_proc: process
   begin		
      -- hold reset state for 100 ns.
      --wait for 100 ns;	
		ADDR <= x"0000FFFF";
		RD <= '0';
		WR <= '0';
      wait until RDY = '1'; -- Initialization
		while ADDR < x"00010001" loop
			RD <= '0';
			WR <= '1';
			wait until rising_edge(OK_STROBE);
			WR <= '0';
			RD <= '1';
			wait until rising_edge(OK_STROBE);
			ADDR <= std_logic_vector(unsigned(ADDR)+1);
		end loop;
		WR <= '0';
		RD <= '0';

      -- insert stimulus here 
      wait;
   end process;

END;
