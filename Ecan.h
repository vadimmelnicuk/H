//--------------------------------------------------------------
//	File:			Ecan.h
//	Author:			Vadim Melnicuk
//	Company:		Birmingham City University
//	Date:			04/03/2014
//	Description:	
//--------------------------------------------------------------

#ifndef ECAN_H
#define	ECAN_H

//Function prototypes
void EcanTxI(unsigned char, unsigned char);
unsigned char EcanRxI(unsigned char);

//Defines
#define CANTX PORTBbits.RB2
#define CANRX PORTBbits.RB3

//Global variables
unsigned char ecan_ping = 1;
unsigned char ecan_read = 2;
unsigned char ecan_write = 3;

unsigned char ecan_con_id = 6;

unsigned char ecan_tx_buffer[7] = {0};
unsigned char ecan_rx_buffer[8] = {0};

#endif	//ECAN_H