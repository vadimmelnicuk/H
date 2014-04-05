//--------------------------------------------------------------
//	File:			Controller.h
//	Author:			Vadim Melnicuk
//	Company:		Birmingham City University
//	Date:			28/03/2014
//	Description:
//--------------------------------------------------------------

#ifndef CONTROLLER_H
#define	CONTROLLER_H

//Function prototypes
void Pb1Wait(void);
void Pb2Wait(void);
void LedsOn(void);
void LedsOff(void);
void LedsBlink(void);
void LegsPing(void);
void LegsStatus(void);
void LegsReset(void);
void LegStep(unsigned char, unsigned char);
void LegHome(unsigned char);
void LegYShift(unsigned char, signed short int);
void LegSpeed(unsigned char, unsigned short int);
void LegMove(unsigned char, signed short int, signed short int, signed short int);
void LegProcessInstruction(void);

//Defines

//Global variables
unsigned char legs_status_buffer[6] = {0};

#endif	//CONTROLLER_H