//--------------------------------------------------------------
//	File:			Controller.c
//	Author:			Vadim Melnicuk
//	Company:		Birmingham City University
//	Date:			28/03/2014
//	Description:
//--------------------------------------------------------------

//Header files
#include "Config.h"

//--------------------------------------------------------------

void Pb1Wait(void)
{
	while(!PB1){
		if(PB1){
			while(PB1);
			Delay(10);
			break;
		}
	}
}

void Pb2Wait(void)
{
	while(!PB2){
		if(PB2){
			while(PB2);
			Delay(10);
			break;
		}
	}
}

void LedsOn(void)
{
	LED1 = 1;
	LED2 = 1;
	LED3 = 1;
	LED4 = 1;
	LED5 = 1;
	LED6 = 1;
}

void LedsOff(void)
{
	LED1 = 0;
	LED2 = 0;
	LED3 = 0;
	LED4 = 0;
	LED5 = 0;
	LED6 = 0;
}

void LedsBlink(void)
{
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

void LegsPing(void)
{
	if(LegReadPing(0)){						//Ping Servo
		legs_status[0] = 1;
		LED1 = 1;
	}
	if(LegReadPing(1)){						//Ping Servo
		legs_status[1] = 1;
		LED2 = 1;
	}
	if(LegReadPing(2)){						//Ping Servo
		legs_status[2] = 1;
		LED3 = 1;
	}
	if(LegReadPing(3)){						//Ping Servo
		legs_status[3] = 1;
		LED4 = 1;
	}
	if(LegReadPing(4)){						//Ping Servo
		legs_status[4] = 1;
		LED5 = 1;
	}
	if(LegReadPing(5)){						//Ping Servo
		legs_status[5] = 1;
		LED6 = 1;
	}
//	LegsStatus();							//Flash leds for inactive legs
}

void LegsStatus(void)
{
	unsigned char n;
	for(n = 0; n <= 5; n++){
		if(!legs_status[n]){
			while(1){
				LedsOff();
				Delay(100);					//Delay 100ms
				LED1 = !legs_status[0];
				LED2 = !legs_status[1];
				LED3 = !legs_status[2];
				LED4 = !legs_status[3];
				LED5 = !legs_status[4];
				LED6 = !legs_status[5];
				Delay(100);					//Delay 100ms
			}
		}
	}
}

void LegsReset(void)
{
	RESET_LEGS = 0;
	Delay(10);								//Delay 10ms
	RESET_LEGS = 1;
}

void LegWriteHome(unsigned char id)
{
	ecan_tx_buffer[0] = 1;					//Home command
	EcanTxI(id, ecan_write);
}

void LegWriteYShift(unsigned char id, signed short int y)
{
	ecan_tx_buffer[0] = 2;					//Y shift command
	ecan_tx_buffer[1] = y & 0xFF;			//Y low byte
	ecan_tx_buffer[2] = y >> 8;				//Y high byte
	EcanTxI(id, ecan_write);
}

void LegWriteSpeed(unsigned char id, unsigned short int speed)
{
	ecan_tx_buffer[0] = 3;					//Speed command
	ecan_tx_buffer[1] = speed & 0xFF;		//Speed low byte
	ecan_tx_buffer[2] = speed >> 8;			//Speed high byte
	EcanTxI(id, ecan_write);
}

void LegWriteMove(unsigned char id, signed short int x, signed short int y, signed short int z)
{
	ecan_tx_buffer[0] = 4;					//Move command
	ecan_tx_buffer[1] = x & 0xFF;			//X low byte
	ecan_tx_buffer[2] = x >> 8;				//X high byte
	ecan_tx_buffer[3] = y & 0xFF;			//Y low byte
	ecan_tx_buffer[4] = y >> 8;				//Y high byte
	ecan_tx_buffer[5] = z & 0xFF;			//Z low byte
	ecan_tx_buffer[6] = z >> 8;				//Z high byte
	EcanTxI(id, ecan_write);
}

void LegWriteTorqueOff(unsigned char id)
{
	ecan_tx_buffer[0] = 5;					//Torque off command
	EcanTxI(id, ecan_write);
}

void LegWriteStep(unsigned char id, unsigned char dir)
{
	ecan_tx_buffer[0] = 6;					//Step command
	ecan_tx_buffer[1] = dir;				//Direction - Forward
	EcanTxI(id, ecan_write);
//	while(!legs_step[id]){
//		 if(ConProcessStatus(ecan_write, id)){
//			 legs_step[id] = 1;
//		 }
//	}
//	legs_step[id] = 0;
}

void LegWriteStepTransit(unsigned char id, unsigned char dir)
{
	ecan_tx_buffer[0] = 7;					//Step command
	ecan_tx_buffer[1] = dir;				//Direction - Forward
	EcanTxI(id, ecan_write);
}

unsigned char LegReadPing(unsigned char id)
{
	EcanTxI(id, ecan_ping);
	return ConProcessStatus(ecan_ping, id);
}

unsigned char LegReadMoving(unsigned char id)
{
	ecan_tx_buffer[0] = 1;					//Check if a leg is in move command
	EcanTxI(id, ecan_read);
	return ConProcessStatus(ecan_read, 1);
}

void LegProcessInstruction(void)
{
	switch(ecan_rx_buffer[0]){
	case 1:								//Ping instruction
		ecan_tx_buffer[0] = LEG.ID;		//Sent ping acknowledgement
		EcanTxI(ecan_con_id, ecan_ping);
		break;
	case 2:								//Read instruction
		switch(ecan_rx_buffer[1]){
		case 1:						//Check if a leg is in move command
			ecan_tx_buffer[0] = AxLegMoving();	//Moving?
			EcanTxI(ecan_con_id, ecan_read);	//Send instruction to the main controller
			break;
		default:
			break;
		}
		break;
	case 3:								//Write instruction
		switch(ecan_rx_buffer[1]){
		case 1:						//Home command
			AxLegMove(LEG.HOME_POSITION.X, LEG.HOME_POSITION.Y, LEG.HOME_POSITION.Z);
			break;
		case 2:						//Y shift command
			LEG.HOME_POSITION.Y = uitsi(tcti(ecan_rx_buffer[2], ecan_rx_buffer[3]));
			break;
		case 3:						//Speed command
			LEG.SPEED = tcti(ecan_rx_buffer[2], ecan_rx_buffer[3]);
			break;
		case 4:						//Move command
			LEG.TARGET_POSITION.X = uitsi(tcti(ecan_rx_buffer[2], ecan_rx_buffer[3]));
			LEG.TARGET_POSITION.Y = uitsi(tcti(ecan_rx_buffer[4], ecan_rx_buffer[5]));
			LEG.TARGET_POSITION.Z = uitsi(tcti(ecan_rx_buffer[6], ecan_rx_buffer[7]));
			AxLegMove(LEG.TARGET_POSITION.X, LEG.TARGET_POSITION.Y, LEG.TARGET_POSITION.Z);
			break;
		case 5:						//Torque off command
			ax_write_torque_en[2] = 0;
			AxTxI(ax_servo_ids[LEG.ID][0], ax_write, ax_write_torque_en);
			AxTxI(ax_servo_ids[LEG.ID][1], ax_write, ax_write_torque_en);
			AxTxI(ax_servo_ids[LEG.ID][2], ax_write, ax_write_torque_en);
			break;
		case 6:						//Step command TODO - implement steps
			AxLegStep(ecan_rx_buffer[2]);
			ecan_tx_buffer[0] = LEG.ID;			//Finished
			EcanTxI(ecan_con_id, ecan_write);	//Send instruction to the main controller
			break;
		case 7:						//Step transit command TODO - implement steps
			AxLegStepTransit(ecan_rx_buffer[2]);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

unsigned char ConProcessStatus(unsigned char instruction, unsigned char status)
{
	unsigned char n;
	for(n = 0; n < 250; n++){
		if(EcanRxI(ecan_con_id)){
			if(ecan_rx_buffer[0] == instruction && ecan_rx_buffer[1] == status){
				return 1;
			}
		}
		Delay(1);							//Delay 1ms
	}
	return 0;
}