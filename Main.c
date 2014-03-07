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
		while(1){
			ECAN_TX(rand()%255);
			//ECAN_RX();
		}
	#endif
	#ifdef MODE_FLASH
		AX_Flash();
	#endif

	while(1){
		Blink_LEDS();
	}
}