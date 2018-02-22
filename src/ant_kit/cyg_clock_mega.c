/*
 * cyg_clock_mega.c
 *
 * Created: 2012/7/10 11:51:51
 * Copyright (C) 2012-2014 Jiaxing Lanwan Electronics co ., ltd .
 * Author: steven.zdwang
 */ 

#if (defined __AVR_ATmega88A__)

#include "cyg_config.h"

#include "cyg_clock.h"

static volatile clock_time_t current_clock = 0;


// fcpu = 1MHz, 1ms reload timer ISR
ISR( TIMER1_OVF_vect )
{
    TCNT1L = 0x18;
	TCNT1H = 0xFC;
	
	current_clock ++;													
}

void
clock_init()
{ 		
    TIMSK1 |= _BV(TOIE1);   
    // TIMER1, clkI/O/8
    TCCR1B |= _BV(CS11);
	
    TCNT1L = 0x18;
	TCNT1H = 0xFC;			
}

clock_time_t
cygClockTime(void)
{
    return current_clock;
}

void
cygOpenClock(void)
{ 	   
	clock_init();
}	

void 
cygCloseClock(void)
{
	TIMSK1 &= ~_BV(TOIE1); 
}


#endif