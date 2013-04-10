#include <vos.h>
#include <gpio.h>

#define FLASH_CS 1		// IOBUS16
#define FLASH_MISO 2	// IOBUS17
#define FLASH_MOSI 4	// IOBUS18
#define FLASH_SCK 8		// IOBUS19

#define PAGE_SIZE	528

extern VOS_HANDLE hGPIO_PORT_B;
port portb_out @ 391;
port portb_in  @ 396;

void spi_write(char *pb, int cb, int fAssertCS)
{
	int cbit;
	int ib;
	char b;
	for(ib = 0; ib < cb; ib++)
	{
		b = pb[ib];
		for(cbit = 0; cbit < 8; cbit++)
		{
			portb_out = (b & 0x80) ? FLASH_MOSI : 0;
			portb_out |= FLASH_SCK;
			b = b << 1;
		}
	}
	if (fAssertCS)
	{
		portb_out = FLASH_CS;
	}
}

void spi_read(char *pb, int cb, int fAssertCS)
{
	int ibit;
	int ibyte;
	int out = 0;
	for(ibyte = 0; ibyte < cb; ibyte++)
	{
		for(ibit = 0; ibit < 8; ibit++)
		{
			portb_out = 0;
			portb_out = FLASH_SCK;

			out = out << 1;
			
			if (portb_in & FLASH_MISO)
				out |= 1;
		}
		pb[ibyte] = (char)out;
	}
	if (fAssertCS)
	{
		portb_out = FLASH_CS;
	}
}

int FReady()
{
	char status;
	char cmd = 0xD7;
	spi_write(&cmd, 1, 0);
	spi_read(&status, 1, 1);
	
	return !!(status & 0x80);
}

void flash_wait()
{
	while(!FReady());	//wait
		vos_delay_msecs(1);
}

void flash_init()
{
	char b = FLASH_CS;
	
	gpio_ioctl_cb_t gpctl;
	gpctl.ioctl_code = VOS_IOCTL_GPIO_SET_MASK;
	gpctl.value = ~FLASH_MISO;	// ALL OUTPUT except MISO
	vos_dev_ioctl(hGPIO_PORT_B, &gpctl);
	
	vos_dev_write(hGPIO_PORT_B, (void*)&b, 1, NULL);
}
	
void flash_write(int page, char *pbuf)	// expects a buffer of 528 bytes
{
	char cmd = 0x87;
	char rgzero[] = {0,0,0};
	char bpage;
	
	flash_wait();
	
	// load page to buffer
	spi_write(&cmd, 1, 0);
	spi_write(rgzero,3,0);
	spi_write(pbuf, PAGE_SIZE, 1);
	
	// write buffer & erase page
	page = page << 10;
	cmd = 0x86;
	spi_write(&cmd, 1, 0);
	bpage = (char)(page >> 16);
	spi_write(&bpage, 1, 0);
	bpage = (char)(page >> 8);
	spi_write(&bpage, 1, 0);
	bpage = (char)page;
	spi_write(&bpage, 1, 1);
}
	
void flash_read(int page, char *pbuf)	// expects a buffer of 528 bytes
{
	char cmd = 0xD2;
	char bpage;
	int ib;
	
	//send cmd
	spi_write(&cmd, 1, 0);
	
	// send addr
	page = page << 10;
	bpage = (char)(page >> 16);
	spi_write(&bpage, 1, 0);
	bpage = (char)(page >> 8);
	spi_write(&bpage, 1, 0);
	bpage = (char)page;
	spi_write(&bpage, 1, 0);
	
	// send 4 dummies
	spi_write(&bpage, 1, 0);
	spi_write(&bpage, 1, 0);
	spi_write(&bpage, 1, 0);
	spi_write(&bpage, 1, 0);
	
	spi_read(pbuf, PAGE_SIZE, 1);
}
	
