//--------------------------------------------------------------
//	File:			Eusart.c
//	Author:			Vadim Melnicuk
//	Company:		Birmingham City University
//	Date:			11/11/2013
//	Description:
//--------------------------------------------------------------

//Header files
#include "Config.h"

//--------------------------------------------------------------

void EUSART1_Mode(unsigned char Mode)
{
	switch(Mode){
		case 0:
			RX1_EN_PORT = 0;
			RX1_EN = 0;
			TX1_EN_PORT = 1;
			TX1_EN = 1;
			break;
		case 1:
			TX1_EN_PORT = 0;
			TX1_EN = 0;
			RX1_EN_PORT = 1;
			RX1_EN = 1;
			break;
		default:
			TX1_EN_PORT = 0;
			TX1_EN = 0;
			RX1_EN_PORT = 0;
			RX1_EN = 0;
			break;
	}
}

void EUSART1_Test(void)
{
	while(1){
		EUSART1_Mode(0);
		TX1_Byte(rand()%255);				//Send random Byte
		while(!TX1_STATUS);					//Wait for transmit to finish
	}
}

void TX1_Byte(unsigned char Byte)
{
	while(!TX1_FLAG);
	TX1_REG = Byte;
}

unsigned char RX1_Byte(void)
{
	while(!RX1_FLAG);
	return RX1_REG;
}

unsigned char Rx1ByteTimeOut(void)
{
	unsigned short int i = 0;
	while(!RX1_FLAG){
		__delay_us(1);
		i++;
		if(i == 1000){
			RX1_Error = 1;
			return 0;
		}
	}
	RX1_Error = 0;
	return RX1_REG;
}

unsigned short int TX1_TCTI(unsigned char c1, unsigned char c2)	//Two char (low and high bytes) to int
{
	unsigned short int i;
	i = (c2 << 8) | c1;
	return i;
}