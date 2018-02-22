/*
 * cyg_ant_mega.c
 *
 * Created: 2012/7/10 11:51:51
 * Copyright (C) 2012-2014 Jiaxing Lanwan Electronics co ., ltd .
 * Author: steven.zdwang
 */ 

#if (defined __AVR_ATmega88A__)

#include "cyg_config.h"

#if CYG_FEATURE_ANT

#include "cyg_ant.h"

/************************************************************************/
/* ANT File                                                             */
/************************************************************************/
static void cyg_ant_usart_init( unsigned int ubrr );
static int cyg_ant_usart_putc(char data, FILE * file);
static int cyg_ant_usart_getc(FILE * file);
FILE gfile_ant = FDEV_SETUP_STREAM(cyg_ant_usart_putc, cyg_ant_usart_getc, _FDEV_SETUP_RW);

/************************************************************************/
/* ANT USART in SPI mode                                                */
/************************************************************************/

#define ANT_BAUD            19200
#define ANT_UBBR            (F_CPU/16/ANT_BAUD) - 1
void cygOpenANT(void)
{	
	cyg_ant_usart_init(ANT_UBBR);

#if 1	
	// power on	
	DDRD |= _BV(PD4);		
    PORTD &= ~_BV(PD4);	
		
	// disable suspend & sleep
	DDRC |= _BV(PC2);
	PORTC |= _BV(PC2);  //suspend
	DDRC |= _BV(PC3);
	PORTC &= ~_BV(PC3);  //sleep
	
	// RTS enable, input pin
	DDRD &= ~_BV(PD2);	
	PORTD &= ~_BV(PD2);
	
	// reset 
	DDRD |= _BV(PD3);		
    PORTD &=~_BV(PD3);
	_delay_ms(10);
	PORTD |= _BV(PD3);	
#endif	
}

void cygCloseANT(void)
{
}

int cygRcvANTDataIsReady(void)
{
    return (UCSR0A & (1<<RXC0));
}

static void cyg_ant_usart_init( unsigned int ubrr )
{	
	/*Set baud rate */
	UBRR0H = ( unsigned char )(ubrr>>8);
	UBRR0L = ( unsigned char )ubrr;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/*  Set frame format: 8data, 1stop bit,   */
	UCSR0C = _BV(UCSZ00) | _BV(UCSZ01) | _BV(USBS0);	
}

static int cyg_ant_usart_putc(char data, FILE * file)
{						
    /* Wait for empty transmit buffer*/
    while( !( UCSR0A & (1<<UDRE0)) )
    ;
    /* Put data into buffer, sends the data*/
    UDR0 = data;
	
	return 0;
}

static int cyg_ant_usart_getc(FILE * file)
{			
	/* Wait for data to be received */
	//while ( !(UCSR0A & (1<<RXC0)) );			
	/* Get and return received data from buffer */
	return UDR0;	
}

#endif //CYG_FEATURE_ANT

#endif