//--------------------------------------------------------------
//	File:			Config.c
//	Author:			Vadim Melnicuk
//	Company:		Birmingham City University
//	Date:			11/11/2013
//	Description:
//--------------------------------------------------------------

//Header files
#include "Config.h"

//--------------------------------------------------------------

void Init(void)
{
	//FOSC
	OSCCONbits.OSTS = 1;
	OSCCONbits.IRCF = 0b111;

	//PORTs
	ANSELAbits.ANSA0 = 0;
	ANSELAbits.ANSA1 = 0;
	ANSELAbits.ANSA2 = 0;
	ANSELCbits.ANSC6 = 0;
	ANSELCbits.ANSC7 = 0;

	TRISAbits.RA0 = 0;
	TRISAbits.RA1 = 0;
	TRISAbits.RA2 = 0;
	TRISCbits.RC6 = 1;
	TRISCbits.RC7 = 1;

	PORTA = 0;

	//Interrupts
	RCONbits.IPEN = 1;
	INTCONbits.GIEH = 1;
	INTCONbits.GIEL = 1;
	PIE1bits.TX1IE = 0;
	PIE1bits.RC1IE = 0;
	IPR1bits.TX1IP = 1;
	IPR1bits.RC1IP = 1;

	//UART
	BAUDCON1bits.BRG16 = 0;
	BAUDCON1bits.CKTXP = 0;
	SPBRG1 = ((_XTAL_FREQ/16)/AX_BAUD_RATE)-1;

	TXSTA1bits.BRGH = 1;
	TXSTA1bits.SYNC = 0;
	TXSTA1bits.TXEN = 0;
	TXSTA1bits.TX9 = 0;

	RCSTA1bits.CREN = 0;
	RCSTA1bits.RX9 = 0;
	RCSTA1bits.SPEN = 1;
}

void interrupt High_interrupt(void)
{
	
}