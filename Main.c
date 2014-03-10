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
	Delay(10);						//Delay 10ms

	#ifdef MODE_CON
		RESET_LEGS = 1;				//Take Legs out of reset
		while(1){
			PB1_Wait();
			ECAN_TX(rand()%255);
			LED1 = 1;
			Delay(1000);
			LED1 = 0;
		}
	#endif
	#ifdef MODE_LEG
		AX_Init();
		//ECAN_RX();
	#endif
	#ifdef MODE_FLASH
		AX_Flash();					//Flash a servo with desired settings
		LEDS_On();					//Let me know if everything is OK
		while(1);
	#endif
}