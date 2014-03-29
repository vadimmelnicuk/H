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
void EcanTx(unsigned char, unsigned char);
unsigned char EcanTxPing(unsigned char);
void EcanRx(unsigned char);

//Defines
#define CANTX PORTBbits.RB2
#define CANRX PORTBbits.RB3

//Global variables


#endif	//ECAN_H