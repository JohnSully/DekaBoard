/*******************************************************/
/* file: ports.h                                       */
/* abstract:  This file contains extern declarations   */
/*            for providing stimulus to the JTAG ports.*/
/*******************************************************/

#ifndef ports_dot_h
#define ports_dot_h

/* these constants are used to send the appropriate ports to setPort */
/* they should be enumerated types, but some of the microcontroller  */
/* compilers don't like enumerated types */
#define TCK_MASK 	1	// A0
#define TDI_MASK	2	// A1
#define TDO_MASK	4	// A2
#define TMS_MASK	8	// A3

#define FILE_FLASH 0xfff

#define TCK (short) TCK_MASK
#define TMS (short) TMS_MASK
#define TDI (short) TDI_MASK

#include <vos.h>
extern VOS_HANDLE hGPIO_PORT_A;
extern VOS_HANDLE hUART;

#define JTAG_NO_VERIFY

extern unsigned char *pbjtag;
extern int cbjtag;
extern int vfWriteFlash;
// This should be at 528 bytes otherwise you will have to rework the FLASH code
//	to handle writes not the same size as a page
#define JTAG_BUFFER_SIZE 528

/* set the port "p" (TCK, TMS, or TDI) to val (0 or 1) */
extern void setPort(short p, short val);
extern void toggleTCK();

/* read the TDO bit and store it in val */
extern unsigned char readTDOBit();

/* make clock go down->up->down*/
extern void pulseClock();

extern void buffered_read(unsigned char *data, short cb);

/* read the next byte of data from the xsvf file */
#define readByte(data) buffered_read(data, 1)

extern void waitTime(long microsec);

void init_jtag_buf();

#endif
