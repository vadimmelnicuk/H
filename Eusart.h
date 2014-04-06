//--------------------------------------------------------------
//	File:			Eusart.h
//	Author:			Vadim Melnicuk
//	Company:		Birmingham City University
//	Date:			11/11/2013
//	Description:	
//--------------------------------------------------------------

#ifndef EUSART_H
#define	EUSART_H

//Function prototypes
void Eusart1Mode(unsigned char);
void Eusart1Test(void);
void Tx1Byte(unsigned char);
unsigned char Rx1Byte(void);
unsigned char Rx1ByteTimeOut(void);
void TxTerminate(void);
void TxWelcomeScreen(void);

//Defines
#define TX1_EN TXSTA1bits.TXEN
#define TX1_EN_PORT LATBbits.LATB5
#define TX1_FLAG PIR1bits.TX1IF
#define TX1_STATUS TXSTA1bits.TRMT
#define TX1_REG TXREG1
#define RX1_EN RCSTA1bits.CREN
#define RX1_EN_PORT LATBbits.LATB4
#define RX1_FLAG PIR1bits.RC1IF
#define RX1_REG RCREG1

//Global variables
unsigned char rx1_buffer[10] = {0};
unsigned char rx1_timeout = 0;

#endif	//EUSART_H