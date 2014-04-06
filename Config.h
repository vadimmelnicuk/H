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
#include "Controller.h"
#include "Dynamixel.h"

//PIC settings
#pragma config XINST=OFF, PLLCFG=ON, FOSC=INTIO2, CANMX=PORTB

//Function prototypes
void Init(void);
unsigned short int tcti(unsigned char, unsigned char);
signed short int uitsi(unsigned short int);
void Delay(unsigned short int);
void putch(char);

//Defines
#define _XTAL_FREQ 64000000

#define TIMER0_COUNTER -1000					//1ms

#ifdef MODE_CON
	#define EUART_BAUD_RATE_KBPS 115200			//115200 bps
#elif MODE_LEG
	#define EUART_BAUD_RATE_KBPS 500000			//500 Kbps
#elif MODE_FLASH
	#define EUART_BAUD_RATE_KBPS 1000000		//1 Mbps
#endif

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
unsigned char timer0_dt = 0;

#endif	//CONFIG_H