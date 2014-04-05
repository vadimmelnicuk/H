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

void EcanTxI(unsigned char id, unsigned char instruction)
{
	TXB0CONbits.TXPRI = 0;					//Priority Level 0 (0-3)
	TXB0DLCbits.DLC = 8;					//Set number of bytes (0-8)
	TXB0DLCbits.TXRTR = 0;					//Transmitted message will not have the TXRTR bit set
 	TXB0SIDLbits.EXIDE = 0;					//Message will transmit standard ID
	TXB0SIDLbits.SID = id;					//Standard Identifier bits (0-7)
	TXB0SIDHbits.SID = 0;					//Standard Identifier bits
	TXB0D0 = instruction;					//Loads data byte into buffer
	TXB0D1 = ecan_tx_buffer[0];				//Load parameters
	TXB0D2 = ecan_tx_buffer[1];
	TXB0D3 = ecan_tx_buffer[2];
	TXB0D4 = ecan_tx_buffer[3];
	TXB0D5 = ecan_tx_buffer[4];
	TXB0D6 = ecan_tx_buffer[5];
	TXB0D7 = ecan_tx_buffer[6];
	TXB0CONbits.TXREQ = 1;					//Requests to send a message
 	while(TXB0CONbits.TXREQ);				//Wait for transmit to finish
}

unsigned char EcanTxPing(unsigned char id)
{
	unsigned short int n;
	EcanTxI(id, ecan_ping);
	for(n = 0; n < 100; n++){
		if(EcanRxI(ecan_con_id)){
			if(ecan_rx_buffer[0] == ecan_ping && ecan_rx_buffer[1] == id){
				return 1;
			}
		}
		Delay(1);							//Delay 1ms
	}
	return 0;
}

unsigned char EcanRxI(unsigned char id)
{
	if(RXB0CONbits.RXFUL){					//Does RXB0 contain a message?
		RXB0CONbits.RXFUL = 0;				//Receiving is completed
		if(RXB0SIDLbits.SID == id){			//Is the message for me?
			if(RXB0DLCbits.DLC > 0){		//Any bytes to read?
				ecan_rx_buffer[0] = RXB0D0;	//Read instruction
				ecan_rx_buffer[1] = RXB0D1;	//Read parameters
				ecan_rx_buffer[2] = RXB0D2;
				ecan_rx_buffer[3] = RXB0D3;
				ecan_rx_buffer[4] = RXB0D4;
				ecan_rx_buffer[5] = RXB0D5;
				ecan_rx_buffer[6] = RXB0D6;
				ecan_rx_buffer[7] = RXB0D7;
				return 1;
			}
		}
	}
	return 0;
}