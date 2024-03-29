/*
** Filename: DekaBoard_iomux.c
**
** Automatically created by Application Wizard 2.0.0
** 
** Part of solution DekaBoard in project DekaBoard
**
** Comments: 
**
** Important: Sections between markers "FTDI:S*" and "FTDI:E*" will be overwritten by
** the Application Wizard

Copyright (C) 2013 John Sully

Permission is hereby granted, free of charge, to any person obtaining a copy of 
this software and associated documentation files (the "Software"), to deal in 
the Software without restriction, including without limitation the rights to 
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "vos.h"

void iomux_setup(void)
{
	/* FTDI:SIO IOMux Functions */
	unsigned char packageType;
	
	packageType = vos_get_package_type();
	if (packageType == VINCULUM_II_48_PIN)
	{
		// Debugger to pin 11 as Bi-Directional.
		vos_iomux_define_bidi(199, IOMUX_IN_DEBUGGER, IOMUX_OUT_DEBUGGER);
		// PWM_1 to pin 12 as Output.
		vos_iomux_define_output(12, IOMUX_OUT_PWM_1);
		// PWM_2 to pin 13 as Output.
		vos_iomux_define_output(13, IOMUX_OUT_PWM_2);
		// PWM_3 to pin 14 as Output.
		vos_iomux_define_output(14, IOMUX_OUT_PWM_3);
		// SPI_Slave_0_CLK to pin 15 as Input.
		vos_iomux_define_input(15, IOMUX_IN_SPI_SLAVE_0_CLK);
		// SPI_Slave_0_MOSI to pin 16 as Input.
		vos_iomux_define_input(16, IOMUX_IN_SPI_SLAVE_0_MOSI);
		// SPI_Slave_0_MISO to pin 18 as Output.
		vos_iomux_define_output(18, IOMUX_OUT_SPI_SLAVE_0_MISO);
		// SPI_Slave_0_CS to pin 19 as Input.
		vos_iomux_define_input(19, IOMUX_IN_SPI_SLAVE_0_CS);
		// SPI_Master_CLK to pin 20 as Output.
		vos_iomux_define_output(20, IOMUX_OUT_SPI_MASTER_CLK);
		// GPIO_Port_B_5 to pin 21 as Input.
		vos_iomux_define_input(21, IOMUX_IN_GPIO_PORT_B_5);
		vos_iocell_set_config(21, 0, 0, 0, 2);
		// SPI_Master_MISO to pin 22 as Input.
		vos_iomux_define_input(22, IOMUX_IN_SPI_MASTER_MISO);
		// SPI_Master_CS_0 to pin 23 as Output.
		vos_iomux_define_output(23, IOMUX_OUT_SPI_MASTER_CS_0);
		// UART_TXD to pin 31 as Output.
		vos_iomux_define_output(31, IOMUX_OUT_UART_TXD);
		// UART_RXD to pin 32 as Input.
		vos_iomux_define_input(32, IOMUX_IN_UART_RXD);
		// UART_RTS_N to pin 33 as Output.
		vos_iomux_define_output(33, IOMUX_OUT_UART_RTS_N);
		// UART_CTS_N to pin 34 as Input.
		vos_iomux_define_input(34, IOMUX_IN_UART_CTS_N);
		// GPIO_Port_B_0 to pin 35 as Bi-Directional.
		vos_iomux_define_bidi(35, IOMUX_IN_GPIO_PORT_B_0, IOMUX_OUT_GPIO_PORT_B_0);
		// GPIO_Port_B_1 to pin 36 as Bi-Directional.
		vos_iomux_define_bidi(36, IOMUX_IN_GPIO_PORT_B_1, IOMUX_OUT_GPIO_PORT_B_1);
		// GPIO_Port_B_2 to pin 37 as Bi-Directional.
		vos_iomux_define_bidi(37, IOMUX_IN_GPIO_PORT_B_2, IOMUX_OUT_GPIO_PORT_B_2);
		// GPIO_Port_B_3 to pin 38 as Bi-Directional.
		vos_iomux_define_bidi(38, IOMUX_IN_GPIO_PORT_B_3, IOMUX_OUT_GPIO_PORT_B_3);
		// UART_TX_Active to pin 41 as Output.
		vos_iomux_define_output(41, IOMUX_OUT_UART_TX_ACTIVE);
		// GPIO_Port_A_5 to pin 42 as Output.
		vos_iomux_define_output(42, IOMUX_OUT_GPIO_PORT_A_5);
		// GPIO_Port_A_6 to pin 43 as Output.
		vos_iomux_define_output(43, IOMUX_OUT_GPIO_PORT_A_6);
		// GPIO_Port_A_7 to pin 44 as Output.
		vos_iomux_define_output(44, IOMUX_OUT_GPIO_PORT_A_7);
		// GPIO_Port_A_0 to pin 45 as Bi-Directional.
		vos_iomux_define_bidi(45, IOMUX_IN_GPIO_PORT_A_0, IOMUX_OUT_GPIO_PORT_A_0);
		// GPIO_Port_A_1 to pin 46 as Bi-Directional.
		vos_iomux_define_bidi(46, IOMUX_IN_GPIO_PORT_A_1, IOMUX_OUT_GPIO_PORT_A_1);
		// GPIO_Port_A_2 to pin 47 as Bi-Directional.
		vos_iomux_define_bidi(47, IOMUX_IN_GPIO_PORT_A_2, IOMUX_OUT_GPIO_PORT_A_2);
		// GPIO_Port_A_3 to pin 48 as Bi-Directional.
		vos_iomux_define_bidi(48, IOMUX_IN_GPIO_PORT_A_3, IOMUX_OUT_GPIO_PORT_A_3);
	
	}
	
	/* FTDI:EIO */
}
