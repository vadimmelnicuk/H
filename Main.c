//--------------------------------------------------------------
//	File:			Main.c
//	Author:			Vadim Melnicuk
//	Company:		Birmingham City University
//	Date:			11/11/2013
//	Description:	
//--------------------------------------------------------------

//Header files
#include "Config.h"

//--------------------------------------------------------------

void main(void)
{
	Init();							//Initialise Microcontroller
	Delay(10);

	RESET_LEGS = 1;					//Take Legs out of reset

	#ifdef MODE_DEV
		//AX_Init();
	#endif
	#ifdef MODE_FLASH
		AX_Flash();
	#endif

	while(1){
		LED1 = 1;
		Delay(50);
		LED1 = 0;
		LED2 = 1;
		Delay(50);
		LED2 = 0;
		LED3 = 1;
		Delay(50);
		LED3 = 0;
		LED4 = 1;
		Delay(50);
		LED4 = 0;
		LED5 = 1;
		Delay(50);
		LED5 = 0;
		LED6 = 1;
		Delay(50);
		LED6 = 0;
	}
}