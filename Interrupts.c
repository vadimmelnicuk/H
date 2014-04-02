//--------------------------------------------------------------
//	File:			Interrupts.c
//	Author:			Vadim Melnicuk
//	Company:		Birmingham City University
//	Date:			03/12/2013
//	Description:
//--------------------------------------------------------------

//Header files
#include "Config.h"

//--------------------------------------------------------------

void interrupt HighInterrupt(void)
{
	if(INTCONbits.TMR0IE && INTCONbits.TMR0IF){
		INTCONbits.TMR0IF = 0;					//Clear Timer0 register
		timer0_dt = 1;							//Set Timer0 timeout
		TMR0H = TIMER0_COUNTER >> 8;			//Set counter
		TMR0L = TIMER0_COUNTER & 0xFF;
	}
}
