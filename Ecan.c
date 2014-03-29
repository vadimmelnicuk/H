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

void EcanTx(unsigned char id, unsigned char byte)
{
	TXB0CONbits.TXPRI = 3;					//Priority Level 3 (highest priority) (0-3)
	TXB0DLCbits.DLC = 8;					//Set number of bytes (0-8)
	TXB0DLCbits.TXRTR = 0;					//Transmitted message will have the TXRTR bit set
	TXB0D0 = byte;							//Loads data byte into buffer
	TXB0D1 = 1;
	TXB0D2 = 2;
	TXB0D3 = 3;
	TXB0D4 = 4;
	TXB0D5 = 5;
	TXB0D6 = 6;
	TXB0D7 = 7;
 	TXB0SIDLbits.EXIDE = 0;					//Message will transmit standard ID
	TXB0SIDLbits.SID = id;					//Standard Identifier bits (0-7)
	TXB0SIDHbits.SID = 0;					//Standard Identifier bits
	TXB0CONbits.TXREQ = 1;					//Requests to send a message
 	while(TXB0CONbits.TXREQ);				//Wait for transmit to finish
}

unsigned char EcanTxPing(unsigned char id)
{
	unsigned short int n;
	TXB0CONbits.TXPRI = 3;					//Priority Level 3 (highest priority) (0-3)
	TXB0DLCbits.DLC = 0;					//Set number of bytes (0-8)
	TXB0DLCbits.TXRTR = 1;					//Transmitted message will have the TXRTR bit set
	TXB0SIDLbits.EXIDE = 0;					//Message will transmit standard ID
	TXB0SIDLbits.SID = id;					//Standard Identifier bits (0-7)
	TXB0SIDHbits.SID = 0;					//Standard Identifier bits
	TXB0CONbits.TXREQ = 1;					//Requests to send a message
 	while(TXB0CONbits.TXREQ);				//Wait for transmit to finish
	for(n = 0; n < 1000; n++){				//
		if(RXB0CONbits.RXFUL){				//Does RXB0 contain a message?
			if(RXB0SIDLbits.SID == id){
				RXB0CONbits.RXFUL = 0;		//Receiving is completed
				return 1;
			}
			RXB0CONbits.RXFUL = 0;			//Receiving is completed
		}
		__delay_us(1);
	}
	return 0;
}

void EcanRx(unsigned char id)
{
	if(RXB0CONbits.RXFUL){					//Does RXB0 contain a message?
		if(RXB0SIDLbits.EXID){				//Is this Extended Identifier?
			
		}else{
			if(RXB0SIDLbits.SID == id){
				if(RXB0DLCbits.DLC > 0){	//Any bytes to read?
					
				}
			}
		}
		RXB0CONbits.RXFUL = 0;			//Receiving is completed
	}
}