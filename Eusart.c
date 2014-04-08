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

void Eusart1Mode(unsigned char mode)
{
	switch(mode){
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

void Eusart1Test(void)
{
	Eusart1Mode(0);
	Tx1Byte(rand()%255);					//Send random Byte
	while(!TX1_STATUS);						//Wait for transmit to finish
}

void Tx1Byte(unsigned char byte)
{
	while(!TX1_FLAG);
	TX1_REG = byte;
}

unsigned char Rx1Byte(void)
{
	while(!RX1_FLAG);
	return RX1_REG;
}

unsigned char Rx1ByteTimeOut(void)
{
	unsigned short int n = 0;
	while(!RX1_FLAG){
		__delay_us(1);						//Delay 1 us
		n++;
		if(n == 1000){
			rx1_timeout = 1;
			return 0;
		}
	}
	rx1_timeout = 0;
	return RX1_REG;
}

void TxTerminate(void)
{
	printf("\r\n");
}

void TxWelcomeScreen(void)
{
	printf("  _   _                                _ \r\n");
	printf(" | | | | _____  ____ _ _ __   ___   __| |\r\n");
	printf(" | |_| |/ _ \\ \\/ / _` | '_ \\ / _ \\ / _` |\r\n");
	printf(" |  _  |  __/>  < (_| | |_) | (_) | (_| |\r\n");
	printf(" |_|_|_|\\___/_/\\_\\__,_| .__/ \\___/ \\__,_|\r\n");
	printf(" |  _ \\ _ __ ___ (_) _|_| ___| |_        \r\n");
	printf(" | |_) | '__/ _ \\| |/ _ \\/ __| __|       \r\n");
	printf(" |  __/| | | (_) | |  __/ (__| |_        \r\n");
	printf(" |_|   |_|  \\___// |\\___|\\___|\\__|       \r\n");
	printf("               |__/                      \r\n");
	printf("*****************************************\r\n");
	printf(" Birmingham City University\r\n");
	printf(" Designed by Vadim Melnicuk\r\n");
	printf(" 2014\r\n");
	printf("*****************************************\r\n");
}

void TxClearScreen(void)
{
	printf("\033[2J\033[;H");				//Clear screen & move cursor to home position
}