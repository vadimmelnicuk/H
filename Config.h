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
#include <string.h>
#include <math.h>

#include "Eusart.h"
#include "Dynamixel.h"

//PIC settings
#pragma config XINST=OFF, PLLCFG=ON, FOSC=INTIO2, CANMX=PORTB

//Function prototypes
void Init(void);

//Defines
#define _XTAL_FREQ 64000000

#define PB1 PORTBbits.RB0
#define PB2 PORTBbits.RB1

//Global variables


#endif	//CONFIG_H