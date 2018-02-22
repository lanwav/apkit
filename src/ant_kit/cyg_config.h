#ifndef _CYG_CONFIG_H_
#define _CYG_CONFIG_H_


/************************************************************************/
/* MCU F_CPU                                                            */
/************************************************************************/
#ifndef F_CPU
#define F_CPU 3686400UL                 
#else
#error error! F_CPU has been defined.
#endif


/************************************************************************/
/* System Head Files                                                    */
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include "cyg_avr.h"

/************************************************************************/
/* PT-THREAD                                                           */
/************************************************************************/

#include "pt.h"
#include "pt-timer.h"

/************************************************************************/
/* Cygnus Platform Configure Settings                                   */
/************************************************************************/

#define CYG_DEBUG	                        0
#define CYG_FEATURE_SHELL_FILE              0
#define CYG_FEATURE_SHELL_COMMAND           0
#define CYG_FEATURE_ANT                     1     

/************************************************************************/
/* BSP files                                                            */
/************************************************************************/
#include "cyg_debug.h"
#include "cyg_ant.h"
#include "cyg_clock.h"
//#include "cyg_shell.h"
//#include "cyg_cmd.h"


#endif // _CYG_CONFIG_H_
