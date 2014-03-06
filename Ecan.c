//--------------------------------------------------------------
//	File:			Ecan.c
//	Author:			Vadim Melnicuk
//	Company:		Birmingham City University
//	Date:			04/03/2014
//	Description:
//--------------------------------------------------------------

//Header files
#include "Config.h"

//--------------------------------------------------------------

void ECAN_TX(unsigned char Byte)
{
	TXB0D0 = Byte;							//Loads data byte into buffer
	TXB0SIDLbits.EXIDE = 0;					//Message will transmit standard ID
	TXB0SIDLbits.SID = 0b110;				//Standard Identifier bits
	TXB0SIDHbits.SID = 0b100100;			//Standard Identifier bits

	TXB0CONbits.TXREQ = 1;					//Requests sending a message
	while(TXB0CONbits.TXREQ);				//Wait for transmit to finish
}

void ECAN_RX(void)
{

}