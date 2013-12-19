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
	OSCCONbits.OSTS = 1;		//External Oscillator
	//OSCCONbits.IRCF = 0b111;

	//PORTs
	ANCON0bits.ANSEL0 = 0;		//RA0 - Digital
	ANCON0bits.ANSEL1 = 0;		//RA1 - Digital

	TRISAbits.TRISA0 = 0;		//RA0 - Output
	TRISAbits.TRISA1 = 0;		//RA1 - Output
	TRISCbits.TRISC6 = 1;		//RC6 - Input
	TRISCbits.TRISC7 = 1;		//RC7 - Input

	PORTA = 0;
	PORTC = 0;

	//Interrupts
	RCONbits.IPEN = 1;
	INTCONbits.GIEH = 1;
	INTCONbits.GIEL = 1;
	PIE1bits.TX1IE = 0;
	PIE1bits.RC1IE = 0;
	IPR1bits.TX1IP = 1;
	IPR1bits.RC1IP = 1;

	//UART
	BAUDCON1bits.BRG16 = 1;
	BAUDCON1bits.CKTXP = 0;

	SPBRG1 = ((_XTAL_FREQ/4)/AX_BAUD_RATE)-1;

	TXSTA1bits.BRGH = 1;
	TXSTA1bits.SYNC = 0;
	TXSTA1bits.TXEN = 0;
	TXSTA1bits.TX9 = 0;

	RCSTA1bits.CREN = 0;
	RCSTA1bits.RX9 = 0;
	RCSTA1bits.SPEN = 1;
}