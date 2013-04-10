/*
** Filename: DekaBoard.c
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

#include "DekaBoard.h"
#include "ports.h"
#include "flash.h"
#include <stdio.h>
#include <string.h>

/* FTDI:STP Thread Prototypes */
vos_tcb_t *tcbFIRMWARE;

void firmware();
/* FTDI:ETP */

/* FTDI:SDH Driver Handles */
VOS_HANDLE hUSBHOST_1; // USB Host Port 1
VOS_HANDLE hUSBHOST_2; // USB Host Port 2
VOS_HANDLE hUART; // UART Interface Driver
VOS_HANDLE hSPI_MASTER; // SPIMaster Interface Driver
VOS_HANDLE hGPIO_PORT_A; // GPIO Port A Driver
VOS_HANDLE hGPIO_PORT_B; // GPIO Port B Driver
VOS_HANDLE hFAT_FILE_SYSTEM; // FAT File System for FAT32 and FAT16
VOS_HANDLE hBOMS; // Bulk Only Mass Storage for USB disks
VOS_HANDLE hUSBHOST_HID_1; // Connects to a HIDdevice on the USB Host Interface
VOS_HANDLE hUSBHOST_HID_2; // Connects to a HIDdevice on the USB Host Interface
/* FTDI:EDH */

/* Declaration for IOMUx setup function */
void iomux_setup(void);

/* Main code - entry point to firmware */
void main(void)
{
	/* FTDI:SDD Driver Declarations */
	// UART Driver configuration context
	uart_context_t uartContext;
	// SPI Master configuration context
	spimaster_context_t spimContext;
	// GPIO Port A configuration context
	gpio_context_t gpioContextA;
	// GPIO Port B configuration context
	gpio_context_t gpioContextB;
	// USB Host configuration context
	usbhost_context_t usbhostContext;
	/* FTDI:EDD */

	/* FTDI:SKI Kernel Initialisation */
	vos_init(50, VOS_TICK_INTERVAL, VOS_NUMBER_DEVICES);
	vos_set_clock_frequency(VOS_48MHZ_CLOCK_FREQUENCY);
	vos_set_idle_thread_tcb_size(512);
	/* FTDI:EKI */

	iomux_setup();
	
	/* FTDI:SDI Driver Initialisation */
	// Initialise UART
	uartContext.buffer_size = VOS_BUFFER_SIZE_128_BYTES;
	uart_init(VOS_DEV_UART,&uartContext);
	
	// Initialise SPI Master
	spimContext.buffer_size = VOS_BUFFER_SIZE_128_BYTES;
	spimaster_init(VOS_DEV_SPI_MASTER,&spimContext);
	
	// Initialise GPIO A
	gpioContextA.port_identifier = GPIO_PORT_A;
	gpio_init(VOS_DEV_GPIO_PORT_A,&gpioContextA);
	
	// Initialise GPIO B
	gpioContextB.port_identifier = GPIO_PORT_B;
	gpio_init(VOS_DEV_GPIO_PORT_B,&gpioContextB);
	
	// Initialise FAT File System Driver
	fatdrv_init(VOS_DEV_FAT_FILE_SYSTEM);
	
	// Initialise BOMS Device Driver
	boms_init(VOS_DEV_BOMS);
	
	// Initialise USB HID Device
	usbHostHID_init(VOS_DEV_USBHOST_HID_1);
	
	// Initialise USB HID Device
	usbHostHID_init(VOS_DEV_USBHOST_HID_2);
	
	
	
	
	
	// Initialise USB Host
	usbhostContext.if_count = 8;
	usbhostContext.ep_count = 16;
	usbhostContext.xfer_count = 2;
	usbhostContext.iso_xfer_count = 2;
	if(usbhost_init(VOS_DEV_USBHOST_1, VOS_DEV_USBHOST_2, &usbhostContext) != 0)
		while(1);	//FAILED
	/* FTDI:EDI */

	/* FTDI:SCT Thread Creation */
	tcbFIRMWARE = vos_create_thread_ex(31, 2048, firmware, "Application", 0);
	/* FTDI:ECT */

	vos_delay_msecs(100);
	vos_start_scheduler();

main_loop:
	vos_delay_msecs(100);
	goto main_loop;
}

/* FTDI:SSP Support Functions */

unsigned char usbhost_connect_state(VOS_HANDLE hUSB)
{
	unsigned char connectstate = PORT_STATE_DISCONNECTED;
	usbhost_ioctl_cb_t hc_iocb;

	if (hUSB)
	{
		hc_iocb.ioctl_code = VOS_IOCTL_USBHOST_GET_CONNECT_STATE;
		hc_iocb.get        = &connectstate;
		vos_dev_ioctl(hUSB, &hc_iocb);

    // repeat if connected to see if we move to enumerated
		if (connectstate == PORT_STATE_CONNECTED)
		{
			vos_dev_ioctl(hUSB, &hc_iocb);
		}
	}
	return connectstate;
}


VOS_HANDLE fat_attach(VOS_HANDLE hMSI, unsigned char devFAT)
{
	fat_ioctl_cb_t           fat_ioctl;
	fatdrv_ioctl_cb_attach_t fat_att;
	VOS_HANDLE hFAT;

	// currently the MSI (BOMS or other) must be open
        // open the FAT driver
	hFAT = vos_dev_open(devFAT);

        // attach the FAT driver to the MSI driver
	fat_ioctl.ioctl_code = FAT_IOCTL_FS_ATTACH;
	fat_ioctl.set = &fat_att;
	fat_att.msi_handle = hMSI;
	fat_att.partition = 0;

	if (vos_dev_ioctl(hFAT, &fat_ioctl) != FAT_OK)
	{
                // unable to open the FAT driver
		vos_dev_close(hFAT);
		hFAT = NULL;
	}

	return hFAT;
}

void fat_detach(VOS_HANDLE hFAT)
{
	fat_ioctl_cb_t           fat_ioctl;

	if (hFAT)
	{
		fat_ioctl.ioctl_code = FAT_IOCTL_FS_DETACH;
		fat_ioctl.set = NULL;
		fat_ioctl.get = NULL;

		vos_dev_ioctl(hFAT, &fat_ioctl);
		vos_dev_close(hFAT);
	}
}

unsigned char getBusState(VOS_HANDLE hUsbHost)
{
	usbhost_ioctl_cb_t usbhost_iocb;
	unsigned char state;
	usbhost_iocb.ioctl_code = VOS_IOCTL_USBHOST_GET_USB_STATE;
	usbhost_iocb.get = &state;
	vos_dev_ioctl(hUsbHost, &usbhost_iocb);
	return state;
}

VOS_HANDLE boms_attach(VOS_HANDLE hUSB, unsigned char devBOMS)
{
	usbhost_device_handle_ex ifDisk = 0;
	usbhost_ioctl_cb_t hc_iocb;
	usbhost_ioctl_cb_class_t hc_iocb_class;
	msi_ioctl_cb_t boms_iocb;
	boms_ioctl_cb_attach_t boms_att;
	int error;
	VOS_HANDLE hBOMS;
	
	

	// find BOMS class device
	hc_iocb_class.dev_class = USB_CLASS_MASS_STORAGE;
	hc_iocb_class.dev_subclass = USB_SUBCLASS_MASS_STORAGE_SCSI;
	hc_iocb_class.dev_protocol = USB_PROTOCOL_MASS_STORAGE_BOMS;

	// user ioctl to find first hub device
	hc_iocb.ioctl_code = VOS_IOCTL_USBHOST_DEVICE_FIND_HANDLE_BY_CLASS;
	hc_iocb.handle.dif = NULL;
	hc_iocb.set = &hc_iocb_class;
	hc_iocb.get = &ifDisk;

	error = vos_dev_ioctl(hUSB, &hc_iocb);
	if (error != USBHOST_OK)
	{
		printf("ERROR: %d\n", error);
		printf("USB Bus 1 State: %d\n", getBusState(hUSBHOST_1));
		printf("USB Bus 2 State: %d\n", getBusState(hUSBHOST_2));
		return NULL;
	}

	// now we have a device, intialise a BOMS driver with it
	hBOMS = vos_dev_open(devBOMS);

	// perform attach
	boms_att.hc_handle = hUSB;
	boms_att.ifDev = ifDisk;

	boms_iocb.ioctl_code = MSI_IOCTL_BOMS_ATTACH;
	boms_iocb.set = &boms_att;
	boms_iocb.get = NULL;

	if (vos_dev_ioctl(hBOMS, &boms_iocb) != MSI_OK)
	{
		vos_dev_close(hBOMS);
		hBOMS = NULL;
	}

	return hBOMS;
}

void boms_detach(VOS_HANDLE hBOMS)
{
	msi_ioctl_cb_t boms_iocb;

	if (hBOMS)
	{
		boms_iocb.ioctl_code = MSI_IOCTL_BOMS_DETACH;
		boms_iocb.set = NULL;
		boms_iocb.get = NULL;

		vos_dev_ioctl(hBOMS, &boms_iocb);
		vos_dev_close(hBOMS);
	}
}


VOS_HANDLE hid_attach(VOS_HANDLE hUSB, unsigned char devHID)
{
	usbhost_device_handle_ex ifHID = 0;
	usbhost_ioctl_cb_t hc_iocb;
	usbhost_ioctl_cb_class_t hc_iocb_class;
	usbHostHID_ioctl_t hid_iocb;
	usbHostHID_ioctl_cb_attach_t hid_att;
	VOS_HANDLE hHID;

	// find HID class device
	hc_iocb_class.dev_class = USB_CLASS_HID;
	hc_iocb_class.dev_subclass = USB_SUBCLASS_ANY;
	hc_iocb_class.dev_protocol = USB_PROTOCOL_ANY;

	// user ioctl to find first hub device
	hc_iocb.ioctl_code = VOS_IOCTL_USBHOST_DEVICE_FIND_HANDLE_BY_CLASS;
	hc_iocb.handle.dif = NULL;
	hc_iocb.set = &hc_iocb_class;
	hc_iocb.get = &ifHID;

	if (vos_dev_ioctl(hUSB, &hc_iocb) != USBHOST_OK)
	{
		return NULL;
	}

	// now we have a device, intialise a HID driver with it
	hHID = vos_dev_open(devHID);

	// perform attach
	hid_att.hc_handle = hUSB;
	hid_att.ifDev = ifHID;

	hid_iocb.ioctl_code = VOS_IOCTL_USBHOSTHID_ATTACH;
	hid_iocb.set = &hid_att;
	hid_iocb.get = NULL;

	if (vos_dev_ioctl(hHID, &hid_iocb) != USBHOSTHID_OK)
	{
		vos_dev_close(hHID);
		hHID = NULL;
	}

	return hHID;
}

void HID_detach(VOS_HANDLE hHID)
{
	usbHostHID_ioctl_t hid_iocb;

	if (hHID)
	{
		hid_iocb.ioctl_code = VOS_IOCTL_USBHOSTHID_DETACH;

		vos_dev_ioctl(hHID, &hid_iocb);
		vos_dev_close(hHID);
	}
}

/* FTDI:ESP */

void open_drivers(void)
{
        /* Code for opening and closing drivers - move to required places in Application Threads */
        /* FTDI:SDA Driver Open */
        hUSBHOST_1 = vos_dev_open(VOS_DEV_USBHOST_1);
        hUSBHOST_2 = vos_dev_open(VOS_DEV_USBHOST_2);
        hUART = vos_dev_open(VOS_DEV_UART);
        hSPI_MASTER = vos_dev_open(VOS_DEV_SPI_MASTER);
        hGPIO_PORT_A = vos_dev_open(VOS_DEV_GPIO_PORT_A);
        hGPIO_PORT_B = vos_dev_open(VOS_DEV_GPIO_PORT_B);
        /* FTDI:EDA */
}

void attach_drivers(void)
{
        /* FTDI:SUA Layered Driver Attach Function Calls */
        hBOMS = boms_attach(hUSBHOST_1, VOS_DEV_BOMS);
        hFAT_FILE_SYSTEM = fat_attach(hBOMS, VOS_DEV_FAT_FILE_SYSTEM);
        hUSBHOST_HID_1 = hid_attach(hUSBHOST_2, VOS_DEV_USBHOST_HID_1);
        hUSBHOST_HID_2 = hid_attach(hUSBHOST_1, VOS_DEV_USBHOST_HID_2);
        // TODO attach stdio to file system and stdio interface
        //fsAttach(hFAT_FILE_SYSTEM); // VOS_HANDLE for file system (typically FAT)
        //stdioAttach(hUART); // VOS_HANDLE for stdio interface (typically UART)
        /* FTDI:EUA */
		
}
		
int FOpenUSBKey(void)
{
	if (hBOMS == NULL)
		hBOMS = boms_attach(hUSBHOST_2, VOS_DEV_BOMS);
	
	if (hBOMS != NULL)
		hFAT_FILE_SYSTEM = fat_attach(hBOMS, VOS_DEV_FAT_FILE_SYSTEM);
	if (hBOMS != NULL && hFAT_FILE_SYSTEM != NULL)
	{
		fsAttach(hFAT_FILE_SYSTEM); // VOS_HANDLE for file system (typically FAT)
		return 1;
	}
	return 0;
}
	
void CloseUSBKey(void)
{
	if (hFAT_FILE_SYSTEM != NULL)
	{
		fat_detach(hFAT_FILE_SYSTEM);
		hFAT_FILE_SYSTEM = NULL;
	}
	if (hBOMS != NULL)
	{
		boms_detach(hBOMS);
		hBOMS = NULL;
	}
}

void close_drivers(void)
{
        /* FTDI:SDB Driver Close */
        vos_dev_close(hUSBHOST_1);
        vos_dev_close(hUSBHOST_2);
        vos_dev_close(hUART);
        vos_dev_close(hSPI_MASTER);
        vos_dev_close(hGPIO_PORT_A);
        vos_dev_close(hGPIO_PORT_B);
        /* FTDI:EDB */
}

void setup_jtag_gpio()
{
	char tmp = LED_GREEN_OFF;
	gpio_ioctl_cb_t gpctl;
	gpctl.ioctl_code = VOS_IOCTL_GPIO_SET_MASK;
	gpctl.value = ~4;	// ALL OUTPUT except TDO
	vos_dev_ioctl(hGPIO_PORT_A, &gpctl);
	
	vos_dev_write(hGPIO_PORT_A, &tmp, 1, NULL);
}

void close_jtag_gpio()
{
	char tmp = 0;
	gpio_ioctl_cb_t gpctl;
	gpctl.ioctl_code = VOS_IOCTL_GPIO_SET_MASK;
	gpctl.value = LED_RED_OFF | LED_GREEN_OFF;
	vos_dev_ioctl(hGPIO_PORT_A, &gpctl);	// set all to inputs (hi-z)
	tmp = LED_RED_OFF;
	vos_dev_write(hGPIO_PORT_A, &tmp, 1, NULL);
}

int FUartData()
{
	int cb;
	common_ioctl_cb_t ioctl = {0};
	
	ioctl.ioctl_code = VOS_IOCTL_COMMON_GET_RX_QUEUE_STATUS;
	vos_dev_ioctl(hUART, &ioctl);
	cb = ioctl.get.queue_stat;
	return (cb);
}
	
void InitializeUART()
{
	common_ioctl_cb_t ioctl = {0};
	ioctl.ioctl_code = VOS_IOCTL_COMMON_ENABLE_DMA;
	vos_dev_ioctl(hUART, &ioctl);
	
	memset(&ioctl, 0, sizeof(common_ioctl_cb_t));
	ioctl.ioctl_code = VOS_IOCTL_UART_SET_BAUD_RATE;
	ioctl.set.uart_baud_rate = 921600;//UART_BAUD_57600;
	vos_dev_ioctl(hUART, &ioctl);
	
	memset(&ioctl, 0, sizeof(common_ioctl_cb_t));
	ioctl.ioctl_code = VOS_IOCTL_UART_SET_FLOW_CONTROL;
	ioctl.set.param = UART_FLOW_NONE;
	vos_dev_ioctl(hUART, &ioctl);
	
	memset(&ioctl, 0, sizeof(common_ioctl_cb_t));
	ioctl.ioctl_code = VOS_IOCTL_UART_SET_DATA_BITS;
	ioctl.set.param = 8;
	vos_dev_ioctl(hUART, &ioctl);
	
	memset(&ioctl, 0, sizeof(common_ioctl_cb_t));
	ioctl.ioctl_code = VOS_IOCTL_UART_SET_STOP_BITS;
	ioctl.set.param = 2;
	vos_dev_ioctl(hUART, &ioctl);
	
	stdioAttach(hUART); // VOS_HANDLE for stdio interface (typically UART)
}
	
void InitializeHID()
{
	//hUSBHOST_HID_1 = hid_attach(hUSBHOST_1, VOS_DEV_USBHOST_HID_1);
	//hUSBHOST_HID_2 = hid_attach(hUSBHOST_2, VOS_DEV_USBHOST_HID_2);
}
	
int memcmp(const char *p1, const char *p2, int cb)
{
	int ib;
	for (ib = 0; ib < cb; ib++)
	{
		if (*p1 - *p2)
			return (*p1 - *p2);
		p1++; p2++;
	}
	return 0;
}
	
char *fgets(char *str, int num, FILE *stream)
{
	int cb;
	for (cb = 0; cb < num; cb++)
	{
		fread(str+cb, 1, 1, stdin);
		if (str[cb] == '\n')
		{
			str[cb] = '\0';
			return;
		}
	}
	return str;
}

FILE *fpga_conf_f;
void ProgramFPGA(FILE *pfSrc)
{
	setup_jtag_gpio();
	init_jtag_buf();
	fpga_conf_f = pfSrc;
	xsvfExecute();
	close_jtag_gpio();
}
	
char *UART_ACK = "OK\n";
char *UART_NACK = "ERROR\n";
rom const char STR_MAGIC[] = "M8f9rHknjyOJ44P92YDZ";
	
/* Application Threads */
extern	void setPort(unsigned short p,short val);
void firmware()
{
	int i;
	char bufmgk[20];
	int idxMgk = 0;
	FILE *pf;
	
	/* Thread code to be added here */
	vos_delay_msecs(4000);	// 4 second delay for USB key to boot
	open_drivers();
	InitializeUART();
	flash_init();
	InitializeHID();
	printf("DekaBoard Rev: 1B\n");
	vfWriteFlash = 0;
	
PROGRAM:
	

	if (FOpenUSBKey())
	{
		pf = fopen("deka.svf", "r");
		if (pf != NULL)
		{
			ProgramFPGA(pf);
			fclose(pf);
		}
		CloseUSBKey();
	}
	else
	{
		ProgramFPGA(FILE_FLASH);
	}
	
	while(1)
	{
		char tmp;
		
		// First check if PROG button pressed
		vos_dev_read(hGPIO_PORT_B, &tmp, 1, NULL);
		if (!(tmp & 0x20))
			goto PROGRAM;
			
		// Else handle input
		if (FUartData())
		{
			fread(bufmgk, 1, 1, stdin);
			if (bufmgk[0] == STR_MAGIC[idxMgk])
			{
				idxMgk++;
			}
			else
			{
				idxMgk = 0;
			}
			if(idxMgk == 20)
			{
				fwrite(UART_ACK, 1, 3, stdout);
				fgets(bufmgk, 20, stdin);
				if (strcmp(bufmgk, "PROG XSVF") == 0)
				{
					fgets(bufmgk, 20, stdin);
					vfWriteFlash = (strcmp(bufmgk, "FLASH") == 0);
					fwrite(UART_ACK, 1, 3, stdout);
					ProgramFPGA(stdin);
					vfWriteFlash = 0;
					printf("DONE\n");
				}
				else
				{
					printf(UART_NACK);
				}
				idxMgk = 0;
			}
		}
	}
}

