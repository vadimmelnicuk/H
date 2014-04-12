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
		Pb1Wait();					//Wait for PB1 to be pressed
/*
<<<<<<< HEAD
		LegsPing();					//Ping legs

=======
>>>>>>> a1a1bf0522b8eb757d922e55a75c46eb3ed3d795
		LegWriteSpeed(5, 120);		//Set leg's speed
		LegWriteSpeed(4, 120);		//Set leg's speed
		LegWriteSpeed(3, 120);		//Set leg's speed
		LegWriteSpeed(2, 120);		//Set leg's speed
		LegWriteSpeed(1, 120);		//Set leg's speed
		LegWriteSpeed(0, 120);		//Set leg's speed

		LegWriteHome(5);			//Send home command to a leg
		LegWriteHome(4);			//Send home command to a leg
		LegWriteHome(3);			//Send home command to a leg
		LegWriteHome(2);			//Send home command to a leg
		LegWriteHome(1);			//Send home command to a leg
		LegWriteHome(0);			//Send home command to a leg

		LegWriteYShift(5, -60);		//Send Y shift command to a leg
		LegWriteYShift(4, -60);		//Send Y shift command to a leg
		LegWriteYShift(3, -60);		//Send Y shift command to a leg
		LegWriteYShift(2, -60);		//Send Y shift command to a leg
		LegWriteYShift(1, -60);		//Send Y shift command to a leg
		LegWriteYShift(0, -60);		//Send Y shift command to a leg

		Delay(3000);

		LegWriteStepBegin(5, 1);	//Send step begin command to a leg
		LegWriteStepBegin(4, 0);	//Send step begin command to a leg
		LegWriteStepBegin(3, 1);	//Send step begin command to a leg
		LegWriteStepBegin(2, 0);	//Send step begin command to a leg
		LegWriteStepBegin(1, 1);	//Send step begin command to a leg
		LegWriteStepBegin(0, 0);	//Send step begin command to a leg

		Delay(3000);

		Pb1Wait();					//Wait for PB1 to be pressed

		LegWriteStep(5, 1);			//Make a leg to step
//		LegWriteStep(4, 1);			//Make a leg to step
		LegWriteStep(3, 1);			//Make a leg to step
//		LegWriteStep(2, 1);			//Make a leg to step
		LegWriteStep(1, 1);			//Make a leg to step
//		LegWriteStep(0, 1);			//Make a leg to step

//		Delay(3000);				//TODO - wait for all legs to finish the step
		
//		LegWriteStepTransit(5, 1);	//Make a leg to step
		LegWriteStepTransit(4, 1);	//Make a leg to step
//		LegWriteStepTransit(3, 1);	//Make a leg to step
		LegWriteStepTransit(2, 1);	//Make a leg to step
//		LegWriteStepTransit(1, 1);	//Make a leg to step
		LegWriteStepTransit(0, 1);	//Make a leg to step

//		LegWriteTorqueOff(5);		//Turn off a leg's torque
//		while(LegReadMoving(5))Delay(10);	//Leg is moving?
*/
		Eusart1Mode(0);				//TX mode
		TxClearScreen();			//Clear terminal screen
		TxWelcomeScreen();			//Print welcome screen in terminal
//		AxLegStepTable();
		Eusart1Mode(1);				//RX mode
		
		while(1){					//Enter Infinite loop
			if(timer0_dt){
				timer0_dt = 0;		//Clear Timer0 timeout flag
				rx1_buffer[0] = Rx1Byte();
				Eusart1Mode(0);		//TX mode
				ConProcessCommand();
				Eusart1Mode(1);		//RX mode
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