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

#include "USART.h"
#include "Dynamixel.h"

//PIC settings
#pragma config FOSC=HSHP, PLLCFG=OFF, LVP=OFF

//Function prototypes
void Init(void);

//Defines
#define _XTAL_FREQ 20000000

//Global variables


#endif	//CONFIG_HP