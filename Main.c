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
	Delay(100);						//Delay 100ms

	#ifdef MODE_CON
		ResetLegs();
		Pb1Wait();
		if(EcanTxPing(5)){			//Ping Servo
			LED6 = 1;
		}
		if(EcanTxPing(4)){
			LED5 = 1;
		}
		while(1);
	#endif
	#ifdef MODE_LEG
		AxInit();
		while(1){
			//EcanRx(LEG.ID);
			AxTest();
			Delay(10);
		}
	#endif
	#ifdef MODE_FLASH
		AX_Flash();					//Flash a servo with desired settings
		LedsOn();					//Let me know if everything is OK
		while(1);
	#endif
}