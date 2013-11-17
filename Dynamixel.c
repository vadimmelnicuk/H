//--------------------------------------------------------------
//	File:			Dynamixel.c
//	Author:		Vadim Melnicuk
//	Company:		Birmingham City University
//	Date:			11/11/2013
//	Description:
//--------------------------------------------------------------

//Header files
#include "Config.h"

//--------------------------------------------------------------

void AX_INIT(void)
{
	const struct AX_PARAMS F1R_D = {1,7,250,0,1023,70,60,140,1023,2,0};
	const struct AX_PARAMS F2R_D = {2,7,250,0,1023,70,60,140,1023,2,0};
}

void AX_PING(unsigned char ID)
{
	unsigned char Checksum;

	Checksum = (~(ID + 3)) & 0xFF; 
	USART1_Mode(0);

	TX1_Byte(255);
	TX1_Byte(255);
	TX1_Byte(ID);
	TX1_Byte(2);
	TX1_Byte(1);
	TX1_Byte(Checksum);

	RX1_Clear_Buffer();

	while(!TX1_STATUS);
}

void AX_TX_Instruction(unsigned char ID, const unsigned char Instruction, unsigned char *Parameters)
{
	unsigned char i, Length;
	unsigned int Checksum, Parameters_Checksum = 0;

	Length = Parameters[0] + 2;
	for(i=1; i<=Parameters[0]; i++){
		Parameters_Checksum += Parameters[i];
	}
	Checksum = (~(ID + Length + Instruction + Parameters_Checksum)) & 0xFF;
	USART1_Mode(0);

	TX1_Byte(255);
	TX1_Byte(255);
	TX1_Byte(ID);
	TX1_Byte(Length);
	TX1_Byte(Instruction);
	for(i=1;i<=Parameters[0];i++){
		TX1_Byte(Parameters[i]);
	}
	TX1_Byte(Checksum);

	RX1_Clear_Buffer();

	while(!TX1_STATUS);
}

void AX_RX_Status(void)
{
	unsigned char i, Length = 4;

	USART1_Mode(1);

	for(i=0; i<Length; i++){
		RX1_Buffer[i] = RX1_Byte();
		if(i == 3){
			Length += RX1_Buffer[3];
		}
	}
}

void AX_GOTO(unsigned char ID, unsigned short int Angle, unsigned short int Speed, unsigned short int Torque)
{
	AX_TX_Instruction(ID, AX_WRITE, AX_WRITE_GOAL_POSITION_HOME);
	AX_RX_Status();
}

struct AX_PARAMS AX_READ_PARAMS(unsigned char ID)
{
	struct AX_PARAMS Servo;

	Servo.ID = ID;
	AX_TX_Instruction(ID, AX_READ, AX_READ_BAUD_RATE);
	AX_RX_Status();
	Servo.BAUD_RATE = RX1_Buffer[5];
	AX_TX_Instruction(ID, AX_READ, AX_READ_DELAY_TIME);
	AX_RX_Status();
	Servo.DELAY_TIME = RX1_Buffer[5];
	AX_TX_Instruction(ID, AX_READ, AX_READ_CW_LIMIT);
	AX_RX_Status();
	Servo.CW_LIMIT = ((RX1_Buffer[6] << 8) | RX1_Buffer[5]);
	AX_TX_Instruction(ID, AX_READ, AX_READ_CCW_LIMIT);
	AX_RX_Status();
	Servo.CCW_LIMIT = ((RX1_Buffer[6] << 8) | RX1_Buffer[5]);
	AX_TX_Instruction(ID, AX_READ, AX_READ_HIGHEST_LIMIT_TEMPERATURE);
	AX_RX_Status();
	Servo.HIGHEST_LIMIT_TEMPERATURE = RX1_Buffer[5];
	AX_TX_Instruction(ID, AX_READ, AX_READ_LOWEST_LIMIT_VOLTAGE);
	AX_RX_Status();
	Servo.LOWEST_LIMIT_VOLTAGE = RX1_Buffer[5];
	AX_TX_Instruction(ID, AX_READ, AX_READ_HIGHEST_LIMIT_VOLTAGE);
	AX_RX_Status();
	Servo.HIGHEST_LIMIT_VOLTAGE = RX1_Buffer[5];
	AX_TX_Instruction(ID, AX_READ, AX_READ_MAX_TORQUE);
	AX_RX_Status();
	Servo.MAX_TORQUE = ((RX1_Buffer[6] << 8) | RX1_Buffer[5]);
	AX_TX_Instruction(ID, AX_READ, AX_READ_STATUS_RETURN_LEVEL);
	AX_RX_Status();
	Servo.STATUS_RETURN_LEVEL = RX1_Buffer[5];
	AX_TX_Instruction(ID, AX_READ, AX_READ_PRESENT_POSITION);
	AX_RX_Status();
	Servo.PRESENT_POSITION = ((RX1_Buffer[6] << 8) | RX1_Buffer[5]);
	AX_TX_Instruction(ID, AX_READ, AX_READ_PRESENT_SPEED);
	AX_RX_Status();
	Servo.PRESENT_SPEED = ((RX1_Buffer[6] << 8) | RX1_Buffer[5]);
	AX_TX_Instruction(ID, AX_READ, AX_READ_PRESENT_LOAD);
	AX_RX_Status();
	Servo.PRESENT_LOAD = ((RX1_Buffer[6] << 8) | RX1_Buffer[5]);
	AX_TX_Instruction(ID, AX_READ, AX_READ_PRESENT_VOLTAGE);
	AX_RX_Status();
	Servo.PRESENT_VOLTAGE = RX1_Buffer[5];
	AX_TX_Instruction(ID, AX_READ, AX_READ_PRESENT_TEMPERATURE);
	AX_RX_Status();
	Servo.PRESENT_TEMPERATURE = RX1_Buffer[5];
	AX_TX_Instruction(ID, AX_READ, AX_READ_MOVING);
	AX_RX_Status();
	Servo.MOVING = RX1_Buffer[5];
	AX_TX_Instruction(ID, AX_READ, AX_READ_LOCK);
	AX_RX_Status();
	Servo.LOCK = RX1_Buffer[5];

	return Servo;
}