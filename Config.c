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
	OSCCONbits.SCS = 0b00;		//Default primary oscillator
	OSCCONbits.IRCF = 0b111;	//16Mhz
	OSCTUNEbits.PLLEN = 1;		//PLL Enabled
	OSCTUNEbits.TUN = 0b000011;	//Tune the oscillator

	//PORTs
	ANCON0bits.ANSEL0 = 0;		//AN0 - Digital
	ANCON0bits.ANSEL1 = 0;		//AN1 - Digital
	ANCON0bits.ANSEL2 = 0;		//AN2 - Digital
	ANCON0bits.ANSEL3 = 0;		//AN3 - Digital
	ANCON0bits.ANSEL4 = 0;		//AN4 - Digital
	ANCON1bits.ANSEL8 = 0;		//AN8 - Digital
	ANCON1bits.ANSEL9 = 0;		//AN9 - Digital
	ANCON1bits.ANSEL10 = 0;		//AN10 - Digital

	TRISAbits.TRISA0 = 0;		//RA0 - Output
	TRISAbits.TRISA1 = 0;		//RA1 - Output
	TRISAbits.TRISA2 = 0;		//RA2 - Output
	TRISAbits.TRISA3 = 0;		//RA3 - Output
	TRISAbits.TRISA5 = 0;		//RA5 - Output
	TRISAbits.TRISA6 = 0;		//RA6 - Output
	TRISAbits.TRISA7 = 0;		//RA7 - Output

	TRISBbits.TRISB0 = 1;		//RB0 - Input
	TRISBbits.TRISB1 = 1;		//RB1 - Input
	TRISBbits.TRISB4 = 0;		//RB4 - Output
	TRISBbits.TRISB5 = 0;		//RB5 - Output

	TRISCbits.TRISC0 = 0;		//RC0 - Output
	TRISCbits.TRISC1 = 0;		//RC1 - Output
	TRISCbits.TRISC5 = 0;		//RC5 - Output
	TRISCbits.TRISC6 = 1;		//RC6 - Input
	TRISCbits.TRISC7 = 1;		//RC7 - Input

	PORTA = 0;					//Reset PORTs
	LATA = 0;
	PORTB = 0;					//Reset PORTs
	LATB = 0;
	PORTC = 0;					//Reset PORTs
	LATC = 0;

	//Interrupts
	RCONbits.IPEN = 1;
	INTCONbits.GIEH = 1;
	INTCONbits.GIEL = 1;
	PIE1bits.TX1IE = 0;
	PIE1bits.RC1IE = 0;
	IPR1bits.TX1IP = 1;
	IPR1bits.RC1IP = 1;

	//EUART
	BAUDCON1bits.BRG16 = 1;
	BAUDCON1bits.CKTXP = 0;
	
	SPBRG1 = ((_XTAL_FREQ/4)/AX_BAUD_RATE_KBPS)-1;

	TXSTA1bits.BRGH = 1;
	TXSTA1bits.SYNC = 0;
	TXSTA1bits.TXEN = 0;
	TXSTA1bits.TX9 = 0;

	RCSTA1bits.CREN = 0;
	RCSTA1bits.RX9 = 0;
	RCSTA1bits.SPEN = 1;
}

void Wait_PB1(void)
{
	
}

void Wait_PB2(void)
{

}

void Delay(unsigned short int t)
{
	unsigned short int i;
	for(i = 0; i <= t; i++){
		__delay_ms(1);
	}
}