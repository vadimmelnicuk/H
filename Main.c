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
		
//		LegsPing();					//Ping legs
//		LegYShift(5, -50);			//Set leg's Y shift
//		LegSpeed(5, 200);			//Set leg's speed
//		LegMove(5, 200, 0, -50);	//Move a leg
//		LegHome(5);					//Send home command to a leg
//		LegHome(4);					//Send home command to a leg

		Eusart1Mode(0);				//TX
//		printf("\033[2J\033[;H");	//Clear screen
		TxWelcomeScreen();			//Print welcome screen
		Eusart1Mode(1);				//RX
		
		while(1){					//Enter Infinite loop
			if(timer0_dt){
				timer0_dt = 0;		//Clear Timer0 timeout flag
				rx1_buffer[0] = Rx1Byte();
			}
		}
	#endif
	#ifdef MODE_LEG
		AxFindLeg();				//Find leg's ID
		AxInitLeg();				//Initialise default settings
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