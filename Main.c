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

	#ifdef MODE_CON					//Main controller mode
		Delay(100);					//Delay 100ms
		LegsReset();				//Reset legs
		Pb1Wait();					//Wait for PB1 to be pressed
		
		LegsPing();					//Ping legs
//		LegWriteYShift(5, -30);		//Set leg's Y shift
//		LegWriteYShift(4, -20);		//Set leg's Y shift
//		LegWriteSpeed(5, 120);		//Set leg's speed
//		LegWriteSpeed(4, 120);		//Set leg's speed
		LegWriteStep(4, 1);			//Make a leg to step
//		while(LegReadMoving(4))Delay(10);	//Leg is moving?
//		LegWriteStep(5, 1);			//Make a leg to step
//		while(LegReadMoving(5))Delay(10);	//Leg is moving?
		
		LegWriteTorqueOff(4);		//Turn off a leg's torque
		LegWriteTorqueOff(5);		//Turn off a leg's torque
//		LegWriteHome(5);			//Send home command to a leg
//		LegWriteHome(4);			//Send home command to a leg

//		Eusart1Mode(0);				//TX mode
//		TxClearScreen();			//Clear terminal screen
//		TxWelcomeScreen();			//Print welcome screen in terminal
//		Eusart1Mode(1);				//RX mode
		
		while(1){					//Enter Infinite loop
			if(timer0_dt){
				timer0_dt = 0;		//Clear Timer0 timeout flag
//				rx1_buffer[0] = Rx1Byte();
			}
		}
	#endif
	#ifdef MODE_LEG					//Leg controller mode
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
#ifdef MODE_FLASH					//Main controller in flash mode
		AX_Flash();					//Flash a servo with desired settings
		LedsOn();					//Let me know if everything is OK
		while(1);
	#endif
}