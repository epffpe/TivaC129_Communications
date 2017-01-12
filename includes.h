/*
 * includes.h
 *
 *  Created on: Oct 18, 2016
 *      Author: epenate
 */

#ifndef INCLUDES_H_
#define INCLUDES_H_

#include <string.h>

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

/* NDK BSD support */
#include <sys/socket.h>


/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>
// #include <ti/drivers/I2C.h>
// #include <ti/drivers/SDSPI.h>
// #include <ti/drivers/SPI.h>
#include <ti/drivers/UART.h>
// #include <ti/drivers/Watchdog.h>
// #include <ti/drivers/WiFi.h>

#include <stdint.h>
#include <ctype.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "inc/hw_types.h"
#include "utils/ustdlib.h"
#include "utils/uartstdio.h"
#include "driverlib/sysctl.h"
#include "driverlib/eeprom.h"
#include "driverlib/interrupt.h"
#include "cmdline.h"
#include "tty.h"
#include "commands.h"

/* Board Header file */
#include "Board.h"

#include "tty.h"
#include "ttybin.h"
#include "dio.h"
#include "Controller.h"


/*
 *
 * http://www.ti.com/lit/ug/spnu151n/spnu151n.pdf
 * PAG 155
 *
 */
#define OS_ENTER_CRITICAL() _disable_IRQ()
#define OS_EXIT_CRITICAL()  _enable_IRQ()
//#define OS_ENTER_CRITICAL() IntMasterDisable()
//#define OS_EXIT_CRITICAL()  IntMasterEnable()

//#define os_enter_critical() __disable_irq() // PRIMARIMASK -> CPSID I
//                                                                                        // __disable_fiq(); // FAULTMASK -> CPSID F
//                                                                                        //          __set_BASEPRI(30);
//#define os_exit_critical()  __enable_irq()  // PRIMARIMASK -> CPSIE I
//                                                                                        // __enable_fiq();  // FAULTMASK -> CPSID F
//                                                                                        // __set_BASEPRI(0);

#ifdef __cplusplus
extern "C"  {
#endif

#ifdef	__INCLUDES_GLOBAL
	#define	__INCLUDES_EXT
#else
	#define __INCLUDES_EXT	extern
#endif


#ifdef __cplusplus
}
#endif





#ifdef __cplusplus


#endif



#endif /* INCLUDES_H_ */
