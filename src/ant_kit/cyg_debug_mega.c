/*
 * cyg_debug_mega.c
 *
 * Created: 2012/7/10 11:51:51
 * Copyright (C) 2012-2014 Jiaxing Lanwan Electronics co ., ltd .
 * Author: steven.zdwang
 */ 
#if (defined __AVR_ATmega88A__)

#include "cyg_config.h"

#if CYG_DEBUG

#include "cyg_debug.h"

static int
cyg_debug_usart_putc(char data, FILE * file);
static int
cyg_debug_usart_getc(FILE * file);

/*
 * Debug file stream
 */
FILE gfile_dbg =
FDEV_SETUP_STREAM(cyg_debug_usart_putc, cyg_debug_usart_getc, _FDEV_SETUP_RW);

/**
* Setup C language stdio and redirect to UART port
*/
static int cyg_debug_usart_getc(FILE * file)
{
	return 0;
}

static int cyg_debug_usart_putc(char data, FILE * file)
{	
	return 0;
}

/*
 * Open Debug USART device
 */
void cygOpenDebugUART(uint32_t u32Baud)
{    
}

/*
 * Close Debug USART
 */
void cygCloseDebugUART(void)
{
}


#endif // CYG_DEBUG

#endif //__AVR_ATmega88A__