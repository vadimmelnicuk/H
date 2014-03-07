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
	TXB0CONbits.TXPRI = 3;					//Priority Level 3 (highest priority) (0-3)
	TXB0DLCbits.DLC = 4;					//Set number of bytes (0-8)
	TXB0D0 = 0b01111110;					//Loads data byte into buffer
	TXB0D1 = 0b01111110;
	TXB0D3 = 0b01111110;
	TXB0D4 = 0b01111110;
 	TXB0SIDLbits.EXIDE = 0;					//Message will transmit standard ID
	TXB0SIDLbits.SID = 1;					//Standard Identifier bits (0-7)
	TXB0SIDHbits.SID = 0;					//Standard Identifier bits
	TXB0CONbits.TXREQ = 1;					//Requests to send a message
 	while(TXB0CONbits.TXREQ);				//Wait for transmit to finish
}

void ECAN_RX(void)
{
	unsigned char i, x, y, l;
	if(RXB0CONbits.RXFUL){					//Does RXB0 contain a message?
		if(RXB0SIDLbits.EXID){				//Is this Extended Identifier?
			
		}else{
			i = RXB0SIDLbits.SID;			//Read standart identifier
			y = RXB0SIDHbits.SID;			//Read standart identifier
			l = RXB0DLCbits.DLC;		//Read the number of bytes available to read
			if(RXB0DLCbits.DLC > 0){		//Any bytes to read?
				x = RXB0D1;					//Read a byte
			}
		}
		RXB0CONbits.RXFUL = 0;				//Receiving is completed
	}
}