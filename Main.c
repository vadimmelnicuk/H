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

	#ifdef MODE_CON
		Delay(100);					//Delay 100ms
		LegsReset();				//Reset legs
		Pb1Wait();					//Wait for PB1 to be pressed
		LegsPing();					//Ping legs

		LegStep(5, 1, 60);			//Send the step command to a leg
		
		while(1){					//Enter Infinite loop
			if(timer0_dt){
				timer0_dt = 0;		//Clear Timer0 timeout flag
				
			}
		}
	#endif
	#ifdef MODE_LEG
		AxInit();
		while(!EcanRxPing(LEG.ID));	//Wait for ping instruction
		while(1){					//Enter Infinite loop
			if(timer0_dt){
				timer0_dt = 0;		//Clear Timer0 timeout flag
				if(EcanRxI(LEG.ID)){
					LegProcessInstruction();
				}
			}
		}
	#endif
	#ifdef MODE_FLASH
		AX_Flash();					//Flash a servo with desired settings
		LedsOn();					//Let me know if everything is OK
		while(1);
	#endif
}