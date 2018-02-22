/*
 * ant_kit.c
 *
 * Created: 2012/7/10 11:51:51
 * Copyright (C) 2012-2014 Jiaxing Lanwan Electronics co ., ltd .
 * Author: steven.zdwang
 */ 

#include "cyg_config.h"

#if ANT_IMP_KIT

#include "./ant/ant_frame.h"
#include "./ant/ant_messages.h"

UCHAR gucSensorDataBuffer[8] = {1,2,3,4,5,6,7,8};
UCHAR gucChannelState;

extern CYG_ANT_CHANNEL_T cygANTChannels[MAX_CHANNEL_NUM + 1];
	
static void wdt_and_sleep_mode_set(void)
{
	cli();
	/* Start timed sequence */
    WDTCSR |= _BV(WDCE) | _BV(WDE);
	
    /* Set new prescaler(time-out) value = 64K cycles (~0.125 s) */
    WDTCSR = _BV(WDE) | _BV(WDP0) | _BV(WDP1);
	
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);	
	sei();
	
	sleep_mode();        
}	

static void enter_low_power_mode(void)
{
	/* 0. GPIO set output low  */
	DDRB = 0Xff;
	//DDRC = 0Xff;
	//DDRD = 0Xff;
    PORTB = 0x00;
	//PORTC = 0x00;
	//PORTD = 0x00;
			
	/* 1. Disable ADC */
	ADCSRA &= ~_BV(ADEN);
	
	/* 2. Disable Analog Comparator */
	ACSR |= _BV(ACD);
	ACSR &= ~_BV(ACBG);
	
	/* 3. Disable Brown-out Detector */
	MCUCR |= _BV(PUD);
	
	/* 4. Disable Internal Voltage Reference */
	/* 5. Disable Watchdog Timer */	
	/* 6. Disable Digital Input */
	DIDR1 = 0x03;   // AIN1/AIN0
    DIDR0 = 0x3f;   // ADC0~ADC5
	
    /* 7. Disable On-chip Debug System */   	          
	
	/* 8. Enable Power Reduction Register */
	PRR |= _BV(PRTWI) | (PRTIM0) | (PRTIM1) | (PRTIM2) |_BV(PRSPI) | _BV(PRADC);				
}

static PT_THREAD( PT_Setup(struct pt *pt) )
{	
	struct timer tm;
    PT_BEGIN(pt);
	
	gucChannelState = STATUS_UNASSIGNED_CHANNEL;	

    while(1)
    {       		
		PT_WAIT_UNTIL(pt, gucChannelState == STATUS_UNASSIGNED_CHANNEL );
		
		DbgPrint("cygANT_ResetSystem\r\n");
		timer_set(&tm, 10);
		cygANT_ResetSystem();		
		PT_WAIT_UNTIL(pt, timer_expired(&tm) );	
		  				
        DbgPrint("cygANT_UnAssignChannel\r\n");
        cygANT_UnAssignChannel(0);
        PT_WAIT_UNTIL(pt, (WAIT_RESPONSE_OK == cygANT_WaitForResponse( &cygANTChannels[0])) );	
				
		//DbgPrint("cygANT_SetNetworkKey\r\n");		
		//unsigned char szNetworkKey[8] = {0xB9, 0xA5, 0x21, 0xFB, 0xBD, 0x72, 0xC3, 0x45};
		//cygANT_SetNetworkKey(0, &szNetworkKey[0]);
		//PT_WAIT_UNTIL(pt, (WAIT_RESPONSE_OK == cygANT_WaitForResponse( &cygANTChannels[0])) );	
				  
		DbgPrint("cygANT_AssignChannel\r\n");
		cygANT_AssignChannel(0, 0x10, 0);
        PT_WAIT_UNTIL(pt, (WAIT_RESPONSE_OK == cygANT_WaitForResponse( &cygANTChannels[0])) );	
		
		DbgPrint("cygANT_SetChannelId\r\n");
        cygANT_SetChannelId(0, 33, 1, 1);		
		PT_WAIT_UNTIL(pt, (WAIT_RESPONSE_OK == cygANT_WaitForResponse( &cygANTChannels[0])) ) ;	
		
		DbgPrint("cygANT_SetChannelPeriod\r\n");
		cygANT_SetChannelPeriod(0, 8192);		
		PT_WAIT_UNTIL(pt, (WAIT_RESPONSE_OK == cygANT_WaitForResponse( &cygANTChannels[0])) );	
		
		DbgPrint("cygANT_ChannelRFFreq\r\n");
		cygANT_ChannelRFFreq(0, 66);		
		PT_WAIT_UNTIL(pt, (WAIT_RESPONSE_OK == cygANT_WaitForResponse( &cygANTChannels[0])) );			
		
		DbgPrint("cygANT_OpenChannel\r\n");
		cygANT_OpenChannel(0);		
		PT_WAIT_UNTIL(pt, (WAIT_RESPONSE_OK == cygANT_WaitForResponse( &cygANTChannels[0])) );	
		
		gucChannelState = STATUS_ASSIGNED_CHANNEL;	
		
		PT_EXIT(pt);		
    }
	
    PT_END(pt);	
}

static PT_THREAD( PT_BroadcastData(struct pt *pt) )
{
	PT_BEGIN(pt);
		
	while(1)
	{
		PT_WAIT_UNTIL(pt, gucChannelState == STATUS_ASSIGNED_CHANNEL );
        										
		cygANT_SendBroadcastData(0, gucSensorDataBuffer);							
	}
	
	PT_END(pt);
}

static PT_THREAD( PT_RecieveData(struct pt *pt) )
{
	static int nData;	
	PT_BEGIN(pt);
	
	while(1)
	{
		PT_WAIT_UNTIL(pt, cygRcvANTDataIsReady() );
        PT_WAIT_UNTIL(pt, ((nData=fgetc(&gfile_ant)) != EOF) );		
		
	    DbgPrint("[%02X]", nData);			
		cygANT_ProcessByte( (UCHAR)nData );							
	}
	
	PT_END(pt);
}

static PT_THREAD( PT_Idle(struct pt *pt) )
{
	PT_BEGIN(pt);
	
	while(1)
	{	
		PT_WAIT_UNTIL(pt, gucChannelState == STATUS_TRACKING_CHANNEL );
			
		//UART0 has sent data finished				
		//PT_WAIT_UNTIL(pt, !( UCSR0A & _BV(TXC0) ));		
		
		cli();
		
	    /* Start timed sequence */
        WDTCSR |= _BV(WDCE) | _BV(WDE);
	
        /* Set new prescaler(time-out) value = 64K cycles (~0.125 s) */
        WDTCSR = _BV(WDE) | _BV(WDP0) | _BV(WDP1);
	
	    set_sleep_mode(SLEEP_MODE_PWR_DOWN);	
	    sei();
	
	    sleep_mode();        

		PT_EXIT(pt);															
	}
	
	PT_END(pt);
}

void cygInitPlatform(void)
{

#if (defined __AVR_ATmega88A__)
    
	//enter_low_power_mode();		
	
    // enable power on
    DDRD |= _BV(PD4);
	PORTD |= _BV(PD4);		    
#endif
}

extern FILE gfile_dbg;
extern FILE gfile_ant;

static struct pt pt_setup, pt_send, pt_rcv, pt_idle;

int main(void)
{						
    cli();
			
	cygInitPlatform();
	
	cygOpenANT();		
	cygOpenClock();		    
		
	DbgPrint("ANT DBG Ready!\r\n");
	
	PT_INIT(&pt_setup);
	PT_INIT(&pt_rcv);
	PT_INIT(&pt_send);
	PT_INIT(&pt_idle);

	gucChannelState = STATUS_UNASSIGNED_CHANNEL;			
		
	sei();													
	
	while(1)
	{	
        PT_Setup(&pt_setup);		
        PT_BroadcastData(&pt_send);							
		PT_RecieveData(&pt_rcv);	        
	}				
	
	return 0;
}

ISR(WDT_vect)
{
	/* Turn off WDT */   
	wdt_disable();		
}


#endif 