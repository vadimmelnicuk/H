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
	if(EcanTxPing(5)){						//Ping Servo
		legs_status_buffer[5] = 1;
		LED6 = 1;
	}
	Delay(250);								//Delay 250ms
	if(EcanTxPing(4)){						//Ping Servo
		legs_status_buffer[4] = 1;
		LED5 = 1;
	}
	Delay(250);								//Delay 250ms
	if(EcanTxPing(3)){						//Ping Servo
		legs_status_buffer[3] = 1;
		LED4 = 1;
	}
	Delay(250);								//Delay 250ms
	if(EcanTxPing(2)){						//Ping Servo
		legs_status_buffer[2] = 1;
		LED3 = 1;
	}
	Delay(250);								//Delay 250ms
	if(EcanTxPing(1)){						//Ping Servo
		legs_status_buffer[1] = 1;
		LED2 = 1;
	}
	Delay(250);								//Delay 250ms
	if(EcanTxPing(0)){						//Ping Servo
		legs_status_buffer[0] = 1;
		LED1 = 1;
	}
	Delay(250);								//Delay 250ms
	LegsStatus();
}

void LegsStatus(void)
{
	unsigned char n;
	for(n = 0; n <= 5; n++){
		if(!legs_status_buffer[n]){
			while(1){
				LedsOff();
				Delay(100);					//Delay 100ms
				LED1 = !legs_status_buffer[0];
				LED2 = !legs_status_buffer[1];
				LED3 = !legs_status_buffer[2];
				LED4 = !legs_status_buffer[3];
				LED5 = !legs_status_buffer[4];
				LED6 = !legs_status_buffer[5];
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

void LegStep(unsigned char id, unsigned char dir)
{
	ecan_tx_buffer[0] = 1;					//Step command
	ecan_tx_buffer[1] = dir;				//Direction - Forward
	EcanTxI(id, ecan_write);
}

void LegHome(unsigned char id)
{
	ecan_tx_buffer[0] = 2;					//Home command
	EcanTxI(id, ecan_write);
}

void LegYShift(unsigned char id, signed short int y)
{
	ecan_tx_buffer[0] = 3;					//Y shift command
	ecan_tx_buffer[1] = y & 0xFF;			//Y low byte
	ecan_tx_buffer[2] = y >> 8;				//Y high byte
	EcanTxI(id, ecan_write);
}

void LegSpeed(unsigned char id, unsigned short int speed)
{
	ecan_tx_buffer[0] = 4;					//Speed command
	ecan_tx_buffer[1] = speed & 0xFF;		//Speed low byte
	ecan_tx_buffer[2] = speed >> 8;			//Speed high byte
	EcanTxI(id, ecan_write);
}

void LegMove(unsigned char id, signed short int x, signed short int y, signed short int z)
{
	ecan_tx_buffer[0] = 5;					//Move command
	ecan_tx_buffer[1] = x & 0xFF;			//X low byte
	ecan_tx_buffer[2] = x >> 8;				//X high byte
	ecan_tx_buffer[3] = y & 0xFF;			//Y low byte
	ecan_tx_buffer[4] = y >> 8;				//Y high byte
	ecan_tx_buffer[5] = z & 0xFF;			//Z low byte
	ecan_tx_buffer[6] = z >> 8;				//Z high byte
	EcanTxI(id, ecan_write);
}

void LegProcessInstruction(void)
{
	switch(ecan_rx_buffer[0]){
		case 1:								//Ping instruction
			ecan_tx_buffer[0] = LEG.ID;		//Sent ping acknowledgement
			EcanTxI(ecan_con_id, ecan_ping);
			break;
		case 2:								//Ping instruction
			break;
		case 3:								//Ping instruction
			switch(ecan_rx_buffer[1]){
				case 1:						//Step command TODO - implement steps
					AxMoveLeg(LEG.SPEED, 200, 0, 0);		//Speed, X, Y, Z
					AxLegMoving();
					break;
				case 2:						//Home command
					AxMoveLeg(LEG.SPEED, LEG.HOME_POSITION.X, LEG.HOME_POSITION.Y, LEG.HOME_POSITION.Z);
					AxLegMoving();
					break;
				case 3:						//Y shift command
					LEG.HOME_POSITION.Y = uitsi(tcti(ecan_rx_buffer[2], ecan_rx_buffer[3]));
					break;
				case 4:						//Speed command
					LEG.SPEED = tcti(ecan_rx_buffer[2], ecan_rx_buffer[3]);
					break;
				case 5:
					LEG.TARGET_POSITION.X = uitsi(tcti(ecan_rx_buffer[2], ecan_rx_buffer[3]));
					LEG.TARGET_POSITION.Y = uitsi(tcti(ecan_rx_buffer[4], ecan_rx_buffer[5]));
					LEG.TARGET_POSITION.Z = uitsi(tcti(ecan_rx_buffer[6], ecan_rx_buffer[7]));
					AxMoveLeg(LEG.SPEED, LEG.TARGET_POSITION.X, LEG.TARGET_POSITION.Y, LEG.TARGET_POSITION.Z);
					AxLegMoving();
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
}