//--------------------------------------------------------------
//	File:			USART.h
//	Author:			Vadim Melnicuk
//	Company:		Birmingham City University
//	Date:			11/11/2013
//	Description:	
//--------------------------------------------------------------

#ifndef USART_H
#define	USART_H

//Function prototypes
void USART1_Mode(unsigned char);
void TX1_Byte(unsigned char);
unsigned char RX1_Byte(void);
unsigned short int TX1_TCTI(unsigned char, unsigned char);

//Defines
#define TX1_EN TXSTA1bits.TXEN
#define TX1_EN_PORT PORTAbits.RA0
#define TX1_FLAG PIR1bits.TX1IF
#define TX1_STATUS TXSTA1bits.TRMT
#define TX1_REG TXREG1
#define RX1_EN RCSTA1bits.CREN
#define RX1_EN_PORT PORTAbits.RA1
#define RX1_FLAG PIR1bits.RC1IF
#define RX1_REG RCREG1

//Global variables
unsigned char TX1_Buffer[50];
unsigned char RX1_Buffer[10];

#endif	//USART_H