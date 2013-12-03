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
	F1R = AX_READ_PARAMS(F1R_D.ID);
	while(1){
		AX_Go_To(F1R_D.ID, F1R_D.CW_LIMIT, 200);
		AX_Go_To(F2R_D.ID, F2R_D.CCW_LIMIT, 200);
		
		while(AX_Is_Moving(F1R_D.ID));

		AX_Go_To(F1R_D.ID, F1R_D.CCW_LIMIT, 200);
		AX_Go_To(F2R_D.ID, F2R_D.CW_LIMIT, 200);
		
		while(AX_Is_Moving(F1R_D.ID));
	}
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

void AX_Go_To(unsigned char ID, unsigned short int Position, unsigned short int Speed)
{
	AX_WRITE_GOAL_POSITION[2] = Position & 0xFF;	//Position Low byte
	AX_WRITE_GOAL_POSITION[3] = Position >> 8;	//Position High Byte
	AX_WRITE_GOAL_POSITION[4] = Speed & 0xFF;	//Speed Low Byte
	AX_WRITE_GOAL_POSITION[5] = Speed >> 8;		//Speed High Byte
	AX_TX_Instruction(ID, AX_WRITE, AX_WRITE_GOAL_POSITION);
	AX_RX_Status();
}

unsigned char AX_Is_Moving(unsigned char ID)
{
	AX_TX_Instruction(ID, AX_READ, AX_READ_MOVING);
	AX_RX_Status();
	return RX1_Buffer[5];
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
	Servo.CW_LIMIT = TX1_TCTI(RX1_Buffer[5], RX1_Buffer[6]);
	AX_TX_Instruction(ID, AX_READ, AX_READ_CCW_LIMIT);
	AX_RX_Status();
	Servo.CCW_LIMIT = TX1_TCTI(RX1_Buffer[5], RX1_Buffer[6]);
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
	Servo.MAX_TORQUE = TX1_TCTI(RX1_Buffer[5], RX1_Buffer[6]);
	AX_TX_Instruction(ID, AX_READ, AX_READ_STATUS_RETURN_LEVEL);
	AX_RX_Status();
	Servo.STATUS_RETURN_LEVEL = RX1_Buffer[5];
	AX_TX_Instruction(ID, AX_READ, AX_READ_PRESENT_POSITION);
	AX_RX_Status();
	Servo.PRESENT_POSITION = TX1_TCTI(RX1_Buffer[5], RX1_Buffer[6]);
	AX_TX_Instruction(ID, AX_READ, AX_READ_PRESENT_SPEED);
	AX_RX_Status();
	Servo.PRESENT_SPEED = TX1_TCTI(RX1_Buffer[5], RX1_Buffer[6]);
	AX_TX_Instruction(ID, AX_READ, AX_READ_PRESENT_LOAD);
	AX_RX_Status();
	Servo.PRESENT_LOAD = TX1_TCTI(RX1_Buffer[5], RX1_Buffer[6]);
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