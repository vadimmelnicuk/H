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
	Init();
	AX_INIT();

	//F1R = AX_READ_PARAMS(F1R_ID);
	//F2R = AX_READ_PARAMS(F2R_ID);

	//AX_TX_Instruction(AX_SERVO_1.ID, AX_WRITE, AX_WRITE_GOAL_POSITION_HOME);
	//AX_RX_Status();

	//AX_TX_Instruction(AX_SERVO_2.ID, AX_WRITE, AX_WRITE_GOAL_POSITION_HOME);
	//AX_RX_Status();

	while(1){
		
	}
}