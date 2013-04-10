/*
** Filename: DekaBoard.h
**
** Automatically created by Application Wizard 2.0.0
** 
** Part of solution DekaBoard in project DekaBoard
**
** Comments: 
**
** Important: Sections between markers "FTDI:S*" and "FTDI:E*" will be overwritten by
** the Application Wizard
*/

#ifndef _DekaBoard_H_
#define _DekaBoard_H_

#include "vos.h"

/* FTDI:SHF Header Files */
#include "USB.h"
#include "USBHost.h"
#include "ioctl.h"
#include "UART.h"
#include "SPIMaster.h"
#include "GPIO.h"
#include "FAT.h"
#include "msi.h"
#include "BOMS.h"
#include "USBHID.h"
#include "USBHostHID.h"
#include "FirmwareUpdate.h"
#include "stdio.h"
#include "errno.h"
#include "stdlib.h"
#include "string.h"
/* FTDI:EHF */

/* FTDI:SDC Driver Constants */
#define VOS_DEV_USBHOST_1 0
#define VOS_DEV_USBHOST_2 1
#define VOS_DEV_UART 2
#define VOS_DEV_SPI_MASTER 3
#define VOS_DEV_GPIO_PORT_A 4
#define VOS_DEV_GPIO_PORT_B 5
#define VOS_DEV_FAT_FILE_SYSTEM 6
#define VOS_DEV_BOMS 7
#define VOS_DEV_USBHOST_HID_1 8
#define VOS_DEV_USBHOST_HID_2 9

#define VOS_NUMBER_DEVICES 10
/* FTDI:EDC */

/* FTDI:SXH Externs */
/* FTDI:EXH */

#include  "micro.h"
#define LED_GREEN_OFF 0x40
#define LED_RED_OFF   0x80

#endif /* _DekaBoard_H_ */
