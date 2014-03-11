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
	//FOSC Settings
	OSCCONbits.SCS = 0b00;			//Default primary oscillator
	OSCCONbits.IRCF = 0b111;		//16Mhz
	OSCTUNEbits.PLLEN = 1;			//PLL Enabled
	OSCTUNEbits.TUN = 0b000011;		//Tune the internal oscillator

	//I/O Settings
	ANCON0bits.ANSEL0 = 0;			//AN0 - Digital
	ANCON0bits.ANSEL1 = 0;			//AN1 - Digital
	ANCON0bits.ANSEL2 = 0;			//AN2 - Digital
	ANCON0bits.ANSEL3 = 0;			//AN3 - Digital
	ANCON0bits.ANSEL4 = 0;			//AN4 - Digital
	ANCON1bits.ANSEL8 = 0;			//AN8 - Digital
	ANCON1bits.ANSEL9 = 0;			//AN9 - Digital
	ANCON1bits.ANSEL10 = 0;			//AN10 - Digital

	TRISAbits.TRISA0 = 0;			//RA0 - Output
	TRISAbits.TRISA1 = 0;			//RA1 - Output
	TRISAbits.TRISA2 = 0;			//RA2 - Output
	TRISAbits.TRISA3 = 0;			//RA3 - Output
	TRISAbits.TRISA5 = 0;			//RA5 - Output
	TRISAbits.TRISA6 = 0;			//RA6 - Output
	TRISAbits.TRISA7 = 0;			//RA7 - Output

	TRISBbits.TRISB0 = 1;			//RB0 - Input
	TRISBbits.TRISB1 = 1;			//RB1 - Input
	TRISBbits.TRISB2 = 0;			//RB2 - Output
	TRISBbits.TRISB3 = 1;			//RB3 - Input
	TRISBbits.TRISB4 = 0;			//RB4 - Output
	TRISBbits.TRISB5 = 0;			//RB5 - Output

	TRISCbits.TRISC0 = 0;			//RC0 - Output
	TRISCbits.TRISC1 = 0;			//RC1 - Output
	TRISCbits.TRISC5 = 0;			//RC5 - Output
	TRISCbits.TRISC6 = 1;			//RC6 - Input
	TRISCbits.TRISC7 = 1;			//RC7 - Input

	PORTA = 0;						//Reset PORTs
	LATA = 0;
	PORTB = 0;						//Reset PORTs
	LATB = 0;
	PORTC = 0;						//Reset PORTs
	LATC = 0;

	//Interrupts Settings
	RCONbits.IPEN = 1;
	INTCONbits.GIEH = 1;
	INTCONbits.GIEL = 1;
	PIE1bits.TX1IE = 0;
	PIE1bits.RC1IE = 0;
	IPR1bits.TX1IP = 1;
	IPR1bits.RC1IP = 1;

	//EUART Settings
	BAUDCON1bits.BRG16 = 1;
	BAUDCON1bits.CKTXP = 0;
	
	SPBRG1 = ((_XTAL_FREQ/4)/AX_BAUD_RATE_KBPS)-1;		//Calculate BAUDRATE register value for AX Servo comms

	TXSTA1bits.BRGH = 1;
	TXSTA1bits.SYNC = 0;
	TXSTA1bits.TXEN = 0;
	TXSTA1bits.TX9 = 0;

	RCSTA1bits.CREN = 0;
	RCSTA1bits.RX9 = 0;
	RCSTA1bits.SPEN = 1;

	//ECAN Settings
	CANCONbits.REQOP = 0b100;		//Request configuration mode
	while(CANSTATbits.OPMODE != 4);	//Wait for configuration mode
	ECANCONbits.MDSEL = 0b00;		//Legacy mode (Mode 0, default)

	BRGCON1bits.BRP = 0b111111;		//TQ = (2 x 64)/FOSC
	BRGCON1bits.SJW = 0b11;			//Synchronization jump width time = 4 x TQ
	//Normally a large SJW is only necessary when the clock generation of the
	//different nodes is inaccurate or unstable, such as using ceramic resonators.
	BRGCON2bits.PRSEG = 0b001;		//Propagation time = 2 x TQ
	BRGCON2bits.SEG1PH = 0b110;		//Phase Segment 1 time = 7 x TQ
	BRGCON2bits.SAM = 0;			//Bus line is sampled once at the sample point
	BRGCON2bits.SEG2PHTS = 1;		//Phase Segment 2 - Freely programmable
	BRGCON3bits.SEG2PH = 0b101;		//Phase Segment 2 time = 6 x TQ
	BRGCON3bits.WAKFIL = 0;			//CAN bus line filter is not used for wake-up
	BRGCON3bits.WAKDIS = 0;			//Enable CAN bus activity wake-up feature

	RXB0CONbits.RXM0 = 1;			//Receive all messages (including those with errors); filter criteria is ignored
	RXB0CONbits.RXM1 = 1;
	RXB1CONbits.RXM0 = 1;			//Receive all messages (including those with errors); filter criteria is ignored
	RXB1CONbits.RXM1 = 1;

	CANCONbits.REQOP = 0b000;		//Request normal mode
	while(CANSTATbits.OPMODE != 0);	//Wait for normal mode
}

void Pb1Wait(void)
{
	while(!PB1){
		if(PB1){
			while(PB1);
			Delay(10);
			break;
		}
	}
}

void Pb2Wait(void)
{
	while(!PB2){
		if(PB2){
			while(PB2);
			Delay(10);
			break;
		}
	}
}

void LedsOn(void)
{
	LED1 = 1;
	LED2 = 1;
	LED3 = 1;
	LED4 = 1;
	LED5 = 1;
	LED6 = 1;
}

void LedsOff(void)
{
	LED1 = 0;
	LED2 = 0;
	LED3 = 0;
	LED4 = 0;
	LED5 = 0;
	LED6 = 0;
}

void LedsBlink(void)
{
	LED1 = 1;
	Delay(50);						//TODO Test - change LED timings
	LED1 = 0;
	LED2 = 1;
	Delay(50);
	LED2 = 0;
	LED3 = 1;
	Delay(50);
	LED3 = 0;
	LED4 = 1;
	Delay(50);
	LED4 = 0;
	LED5 = 1;
	Delay(50);
	LED5 = 0;
	LED6 = 1;
	Delay(50);
	LED6 = 0;
}

void Delay(unsigned short int t)
{
	unsigned short int n;
	for(n = 0; n <= t; n++){
		__delay_ms(1);
	}
}