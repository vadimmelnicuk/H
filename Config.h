//--------------------------------------------------------------
//	File:			Config.h
//	Author:			Vadim Melnicuk
//	Company:		Birmingham City University
//	Date:			11/11/2013
//	Description:
//--------------------------------------------------------------

#ifndef CONFIG_H
#define	CONFIG_H

//Header files
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Eusart.h"
#include "Dynamixel.h"

//PIC settings
#pragma config XINST=OFF, PLLCFG=ON, FOSC=INTIO1, CANMX=PORTB

//Function prototypes
void Init(void);
void Wait_PB1(void);
void Wait_PB2(void);
void Delay(unsigned short int);

//Defines
#define _XTAL_FREQ 64000000

#define LED1 LATAbits.LATA0
#define LED2 LATAbits.LATA1
#define LED3 LATAbits.LATA2
#define LED4 LATAbits.LATA3
#define LED5 LATAbits.LATA7
#define LED6 LATAbits.LATA6

#define PB1 LATBbits.LATB0
#define PB2 LATBbits.LATB1

#define RESET_LEGS LATCbits.LATC5

//Global variables


#endif	//CONFIG_H