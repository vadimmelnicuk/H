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
#include "Ecan.h"
#include "Dynamixel.h"

//PIC settings
#pragma config XINST=OFF, PLLCFG=ON, FOSC=INTIO2, CANMX=PORTB

//Function prototypes
void Init(void);
void PB1_Wait(void);
void PB2_Wait(void);
void LEDS_On(void);
void LEDS_Blink(void);
void Delay(unsigned short int);

//Defines
#define _XTAL_FREQ 64000000

#define LED1 LATAbits.LATA0
#define LED2 LATAbits.LATA1
#define LED3 LATAbits.LATA2
#define LED4 LATAbits.LATA3
#define LED5 LATAbits.LATA7
#define LED6 LATAbits.LATA6

#define PB1 PORTBbits.RB0
#define PB2 PORTBbits.RB1

#define RESET_LEGS LATCbits.LATC5

//Global variables


#endif	//CONFIG_H