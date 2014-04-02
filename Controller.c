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
	Delay(50);						//TODO Test - change LED timings
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
	Delay(250);
	if(EcanTxPing(3)){						//Ping Servo
		legs_status_buffer[3] = 1;
		LED4 = 1;
	}
	Delay(250);
	if(EcanTxPing(2)){						//Ping Servo
		legs_status_buffer[2] = 1;
		LED3 = 1;
	}
	Delay(250);
	if(EcanTxPing(1)){						//Ping Servo
		legs_status_buffer[1] = 1;
		LED2 = 1;
	}
	Delay(250);
	if(EcanTxPing(0)){						//Ping Servo
		legs_status_buffer[0] = 1;
		LED1 = 1;
	}
	Delay(250);
	LegsStatus();
}

void LegsStatus(void)
{
	unsigned char n;
	for(n = 0; n <= 5; n++){
		if(!legs_status_buffer[n]){
			LedsOff();
			while(1){
				for(n = 0; n <= 5; n++){
					if(!legs_status_buffer[n] && n == 0) LED1 = ~LED1;
					if(!legs_status_buffer[n] && n == 1) LED2 = ~LED2;
					if(!legs_status_buffer[n] && n == 2) LED3 = ~LED3;
					if(!legs_status_buffer[n] && n == 3) LED4 = ~LED4;
					if(!legs_status_buffer[n] && n == 4) LED5 = ~LED5;
					if(!legs_status_buffer[n] && n == 5) LED6 = ~LED6;
				}
				Delay(100);
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

void LegStep(unsigned char id, unsigned char dir, unsigned short int speed)
{
	ecan_tx_buffer[0] = 1;					//Step
	ecan_tx_buffer[1] = dir;				//Direction - Forward
	ecan_tx_buffer[2] = speed & 0xFF;		//SpeedL
	ecan_tx_buffer[3] = speed >> 8;			//SpeedH
	EcanTxI(id, ecan_write);
}

void LegProcessInstruction(void)
{
	switch(ecan_rx_buffer[0])
	{
		case 2:								//Read
			break;
		case 3:								//Write
			switch(ecan_rx_buffer[1]){
				case 1:						//Step
					AxMoveLeg(tcti(ecan_rx_buffer[3], ecan_rx_buffer[4]), 100, 0, -50);		//Speed, X, Y, Z
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