//--------------------------------------------------------------
//	File:			USART.c
//	Author:			Vadim Melnicuk
//	Company:		Birmingham City University
//	Date:			11/11/2013
//	Description:
//--------------------------------------------------------------

//Header files
#include "Config.h"

//--------------------------------------------------------------

void USART1_Mode(unsigned char Mode)
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

unsigned short int TX1_TCTI(unsigned char c1, unsigned char c2)	//Two char (low and high bytes) to int
{
	unsigned short int i;
	i = (c2 << 8) | c1;

	return i;
}