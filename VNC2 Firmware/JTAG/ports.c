/*******************************************************/
/* file: ports.c                                       */
/* abstract:  This file contains the routines to       */
/*            output values on the JTAG ports, to read */
/*            the TDO bit, and to read a byte of data  */
/*            from the prom                            */
/* Revisions:                                          */
/* 12/01/2008:  Same code as before (original v5.01).  */
/*              Updated comments to clarify instructions.*/
/*              Add print in setPort for xapp058_example.exe.*/
/*******************************************************/
#include "ports.h"
/*#include "prgispx.h"*/

#include "stdio.h"
#include <string.h>
#include "lenval.h"
#include "flash.h"

port porta_out @ 0x186;

//extern FILE *in;
static int  g_iTCK = 0; /* For xapp058_example .exe */
static int  g_iTMS = 0; /* For xapp058_example .exe */
static int  g_iTDI = 0; /* For xapp058_example .exe */

extern FILE *fpga_conf_f;

unsigned char portA = 0;


#ifdef WIN95PP
#include "conio.h"

#define DATA_OFFSET    (unsigned short) 0
#define STATUS_OFFSET  (unsigned short) 1
#define CONTROL_OFFSET (unsigned short) 2

typedef union outPortUnion {
    unsigned char value;
    struct opBitsStr {
        unsigned char tdi:1;
        unsigned char tck:1;
        unsigned char tms:1;
        unsigned char zero:1;
        unsigned char one:1;
        unsigned char bit5:1;
        unsigned char bit6:1;
        unsigned char bit7:1;
    } bits;
} outPortType;

typedef union inPortUnion {
    unsigned char value;
    struct ipBitsStr {
        unsigned char bit0:1;
        unsigned char bit1:1;
        unsigned char bit2:1;
        unsigned char bit3:1;
        unsigned char tdo:1;
        unsigned char bit5:1;
        unsigned char bit6:1;
        unsigned char bit7:1;
    } bits;
} inPortType;

static inPortType in_word;
static outPortType out_word;
static unsigned short base_port = 0x378;
static int once = 0;
#endif


/*BYTE *xsvf_data=0;*/


/* setPort:  Implement to set the named JTAG signal (p) to the new value (v).*/
/* if in debugging mode, then just set the variables */

/* The VNC2 compiler is truly attrocious.  I have to use these if statements to 
	get it to work.  */
void setPort(unsigned short p,short val)
{
	if (val)
		portA |= p;
	else
		portA = portA & (0xff - p);
	
    if (p==TCK) {
		porta_out = portA;
    }
}

void toggleTCK()
{
	porta_out = portA & (0xff - TCK_MASK);
	porta_out = portA | TCK_MASK;
}

/* toggle tck LH.  No need to modify this code.  It is output via setPort. */
void pulseClock()
{
    //setPort(TCK,0);  /* set the TCK port to low  */
    //setPort(TCK,1);  /* set the TCK port to high */
	porta_out = portA & (0xff - TCK_MASK);
	porta_out = portA | TCK_MASK;
}

unsigned char *pbjtag;
volatile int cbjtag = 0;
int vfWriteFlash;
int vpage = 0;
void init_jtag_buf()
{
	cbjtag = 0;
	vpage = 0;
}
void buffered_read(unsigned char *pv, short cb)
{
	short cbcpy;
	short cbOffset = 0;
	while (cb > 0)
	{
		if (cbjtag == 0)
		{
			int cbread = 0;
			if (fpga_conf_f == stdin)
			{
				int cbBuffer = JTAG_BUFFER_SIZE;
				vos_dev_write(hUART, &cbBuffer, 4, NULL);
			}
			
			if (fpga_conf_f == FILE_FLASH)
			{
				// Load from flash
				flash_read(vpage, pbjtag);
			}
			else
			{
				do{
					cbread += fread(pbjtag, JTAG_BUFFER_SIZE, 1, fpga_conf_f);
				}while(cbread < JTAG_BUFFER_SIZE && fpga_conf_f == stdin);
				if (vfWriteFlash)
				{
					flash_write(vpage, pbjtag);
				}
			}
			vpage++;
		}
		cbcpy = JTAG_BUFFER_SIZE - cbjtag;
		if (cbcpy > cb)
			cbcpy = cb;

		memcpy(pv + cbOffset, pbjtag+cbjtag, cbcpy);
		cb -= cbcpy;
		cbOffset += cbcpy;
		cbjtag += cbcpy;
		if (cbjtag >= JTAG_BUFFER_SIZE)
			cbjtag = 0;
	}
}
		
/*****************************************************************************
* Function:     readVal
* Description:  read from XSVF numBytes bytes of data into x.
* Parameters:   plv         - ptr to lenval in which to put the bytes read.
*               sNumBytes   - the number of bytes to read.
* Returns:      void.
*****************************************************************************/
void readVal( lenVal*   plv,
              short     sNumBytes )
{
#if 0
    unsigned char*  pucVal;
	
    plv->len    = sNumBytes;        /* set the length of the lenVal        */
    for (pucVal = plv->val ; sNumBytes; --sNumBytes, ++pucVal )
    {
        /* read a byte of data into the lenVal */
		readByte( pucVal );
    }
#else
	buffered_read(plv->val, sNumBytes);
	plv->len = sNumBytes;
#endif
}

/* readTDOBit:  Implement to return the current value of the JTAG TDO signal.*/
/* read the TDO bit from port */
unsigned char readTDOBit()
{
#ifdef WIN95PP
    /* Old Win95 example that is similar to a GPIO register implementation.
       The old Win95 reads the hardware input register and extracts the TDO
       value from the bit within the register that is assigned to the
       physical JTAG TDO signal. 
       */
    in_word.value = (unsigned char) _inp( (unsigned short) (base_port + STATUS_OFFSET) );
    if (in_word.bits.tdo == 0x1) {
        return( (unsigned char) 1 );
    }
#endif
    /* You must return the current value of the JTAG TDO signal. */
	char a_stat;
	vos_dev_read(hGPIO_PORT_A, &a_stat, 1, NULL);
    return( !!(a_stat & TDO_MASK));
}

/* waitTime:  Implement as follows: */
/* REQUIRED:  This function must consume/wait at least the specified number  */
/*            of microsec, interpreting microsec as a number of microseconds.*/
/* REQUIRED FOR SPARTAN/VIRTEX FPGAs and indirect flash programming:         */
/*            This function must pulse TCK for at least microsec times,      */
/*            interpreting microsec as an integer value.                     */
/* RECOMMENDED IMPLEMENTATION:  Pulse TCK at least microsec times AND        */
/*                              continue pulsing TCK until the microsec wait */
/*                              requirement is also satisfied.               */

void waitTime(long microsec)
{
	long tckCyclesPerMicrosec    = 1; /* must be at least 1 */
    long        tckCycles   = microsec * tckCyclesPerMicrosec;
    long        i;

    /* This implementation is highly recommended!!! */
    /* This implementation requires you to tune the tckCyclesPerMicrosec 
       variable (above) to match the performance of your embedded system
       in order to satisfy the microsec wait time requirement. */
    for ( i = 0; i < tckCycles; ++i )
    {
        pulseClock();
    }

#if 0
    /* Alternate implementation */
    /* For systems with TCK rates << 1 MHz;  Consider this implementation. */
    /* This implementation does not work with Spartan-3AN or indirect flash
       programming. */
    if ( microsec >= 50L )
    {
        /* Make sure TCK is low during wait for XC18V00/XCFxxS */
        /* Or, a running TCK implementation as shown above is an OK alternate */
        setPort( TCK, 0 );

        /* Use Windows Sleep().  Round up to the nearest millisec */
        _sleep( ( microsec + 999L ) / 1000L );
    }
    else    /* Satisfy FPGA JTAG configuration, startup TCK cycles */
    {
        for ( i = 0; i < microsec;  ++i )
        {
            pulseClock();
        }
    }
#endif

#if 0
    /* Alternate implementation */
    /* This implementation is valid for only XC9500/XL/XV, CoolRunner/II CPLDs, 
       XC18V00 PROMs, or Platform Flash XCFxxS/XCFxxP PROMs.  
       This implementation does not work with FPGAs JTAG configuration. */
    /* Make sure TCK is low during wait for XC18V00/XCFxxS PROMs */
    /* Or, a running TCK implementation as shown above is an OK alternate */
    setPort( TCK, 0 );
    /* Use Windows Sleep().  Round up to the nearest millisec */
    _sleep( ( microsec + 999L ) / 1000L );
#endif
}
