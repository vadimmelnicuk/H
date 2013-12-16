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

void AX_Init(void)
{
	AX_F1R = AX_Read_Params(AX_F1R_D.ID);
	AX_F2R = AX_Read_Params(AX_F2R_D.ID);
	AX_F3R = AX_Read_Params(AX_F3R_D.ID);

	//AX_TX_Instruction(F1R_D.ID, AX_WRITE, AX_WRITE_BAUD_RATE_500K);
	//AX_TX_Instruction(F2R_D.ID, AX_WRITE, AX_WRITE_BAUD_RATE_500K);

	//AX_WRITE_STATUS_RETURN_LEVEL[2] = 1;
	//AX_TX_Instruction(F1R_D.ID, AX_WRITE, AX_WRITE_STATUS_RETURN_LEVEL);
	//AX_TX_Instruction(F2R_D.ID, AX_WRITE, AX_WRITE_STATUS_RETURN_LEVEL);

	while(1){
		AX_Test();

	}
}

void AX_Ping(unsigned char ID)
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

void AX_TX_Instruction_With_Status(unsigned char ID, const unsigned char Instruction, unsigned char *Parameters)
{
	AX_TX_Instruction(ID, Instruction, Parameters);
	AX_RX_Status();

	while(RX1_Error){	//Error check, if true send instruction again.
		AX_TX_Instruction(ID, Instruction, Parameters);
		AX_RX_Status();
	}
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
		if(i == 4 && (RX1_Buffer[4] > 0 || RX1_Buffer[3] > 5)){
			RX1_Error = 1;
			break;
		}else{
			RX1_Error = 0;
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
}

unsigned char AX_Is_Moving(unsigned char ID)
{
	AX_TX_Instruction_With_Status(ID, AX_READ, AX_READ_MOVING);
	return RX1_Buffer[5];
}

unsigned short int AX_Read_Present_Position(unsigned char ID)
{
	AX_TX_Instruction(ID, AX_READ, AX_READ_PRESENT_POSITION);
	AX_RX_Status();
	return TX1_TCTI(RX1_Buffer[5], RX1_Buffer[6]);
}

unsigned short int AX_Read_Present_Speed(unsigned char ID)
{
	AX_TX_Instruction(ID, AX_READ, AX_READ_PRESENT_SPEED);
	AX_RX_Status();
	return TX1_TCTI(RX1_Buffer[5], RX1_Buffer[6]);
}

unsigned short int AX_Read_Present_Load(unsigned char ID)
{
	AX_TX_Instruction(ID, AX_READ, AX_READ_PRESENT_LOAD);
	AX_RX_Status();
	return TX1_TCTI(RX1_Buffer[5], RX1_Buffer[6]);
}

unsigned char AX_Read_Present_Voltage(unsigned char ID)
{
	AX_TX_Instruction(ID, AX_READ, AX_READ_PRESENT_VOLTAGE);
	AX_RX_Status();
	return RX1_Buffer[5];
}

unsigned char AX_Read_Present_Temperature(unsigned char ID)
{
	AX_TX_Instruction(ID, AX_READ, AX_READ_PRESENT_TEMPERATURE);
	AX_RX_Status();
	return RX1_Buffer[5];
}

unsigned char AX_Read_Moving(unsigned char ID)
{
	AX_TX_Instruction(ID, AX_READ, AX_READ_MOVING);
	AX_RX_Status();
	return RX1_Buffer[5];
}

unsigned char AX_Read_Lock(unsigned char ID)
{
	AX_TX_Instruction(ID, AX_READ, AX_READ_LOCK);
	AX_RX_Status();
	return RX1_Buffer[5];
}

struct AX_PARAMS AX_Read_Params(unsigned char ID)
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
	Servo.PRESENT_POSITION = AX_Read_Present_Position(ID);
	Servo.PRESENT_SPEED = AX_Read_Present_Speed(ID);
	Servo.PRESENT_LOAD = AX_Read_Present_Load(ID);
	Servo.PRESENT_VOLTAGE = AX_Read_Present_Voltage(ID);
	Servo.PRESENT_TEMPERATURE = AX_Read_Present_Temperature(ID);
	Servo.MOVING = AX_Read_Moving(ID);
	Servo.LOCK = AX_Read_Lock(ID);
	return Servo;
}

void AX_Flash(void)
{
	unsigned char AX_WRITE_FLASH_1[3];
	unsigned char AX_WRITE_FLASH_2[5];
	
	AX_WRITE_FLASH_1[0] = 2;
	AX_WRITE_FLASH_1[1] = 16;
	AX_WRITE_FLASH_1[2] = AX_FLASH_D.STATUS_RETURN_LEVEL;

	AX_WRITE_FLASH_2[0] = 4;
	AX_WRITE_FLASH_2[1] = 3;
	AX_WRITE_FLASH_2[2] = AX_FLASH_D.ID;
	AX_WRITE_FLASH_2[3] = AX_FLASH_D.BAUD_RATE;
	AX_WRITE_FLASH_2[4] = AX_FLASH_D.DELAY_TIME;

	AX_TX_Instruction(1, AX_WRITE, AX_WRITE_FLASH_1);
	AX_RX_Status();
	AX_TX_Instruction(1, AX_WRITE, AX_WRITE_FLASH_2);
	AX_RX_Status();
}

void AX_Test(void)
{
		unsigned short int Speed = 200;

		AX_Go_To(AX_F1R_D.ID, AX_F1R_D.HOME_POSITION, Speed);
		AX_Go_To(AX_F2R_D.ID, AX_F2R_D.HOME_POSITION, Speed);
		AX_Go_To(AX_F3R_D.ID, AX_F3R_D.HOME_POSITION, Speed);

		while(AX_Is_Moving(AX_F1R_D.ID)){
			__delay_ms(1);
		};

		AX_Go_To(AX_F3R_D.ID, AX_F3R_D.CCW_LIMIT, Speed);
		while(AX_Is_Moving(AX_F3R_D.ID)){
			__delay_ms(1);
		};

		AX_Go_To(AX_F3R_D.ID, AX_F3R_D.CW_LIMIT, Speed);
		while(AX_Is_Moving(AX_F3R_D.ID)){
			__delay_ms(1);
		};

		AX_Go_To(AX_F2R_D.ID, AX_F2R_D.CCW_LIMIT, Speed);
		while(AX_Is_Moving(AX_F2R_D.ID)){
			__delay_ms(1);
		};

		AX_Go_To(AX_F2R_D.ID, AX_F2R_D.CW_LIMIT, Speed);
		while(AX_Is_Moving(AX_F2R_D.ID)){
			__delay_ms(1);
		};

		AX_Go_To(AX_F1R_D.ID, AX_F1R_D.CW_LIMIT, Speed);
		while(AX_Is_Moving(AX_F1R_D.ID)){
			__delay_ms(1);
		};

		AX_Go_To(AX_F1R_D.ID, AX_F1R_D.CCW_LIMIT, Speed);
		while(AX_Is_Moving(AX_F1R_D.ID)){
			__delay_ms(1);
		};
}