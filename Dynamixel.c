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
	AX_Move_Leg(1, 330, 0, 0, 100);		//

	AX_F1R = AX_Read_Params(1);
	AX_F2R = AX_Read_Params(2);
	AX_F3R = AX_Read_Params(3);

	//AX_Init_Legs();
}

void AX_Init_Legs(void)
{
	if(AX_F1R.PRESENT_POSITION != AX_COXA_HOME_POSITION){
		AX_Go_To(AX_F1R.ID, AX_COXA_HOME_POSITION, 300);
	}
	if(AX_F2R.PRESENT_POSITION != AX_FEMUR_HOME_POSITION){
		AX_Go_To(AX_F2R.ID, AX_FEMUR_HOME_POSITION, 300);
	}
	if(AX_F3R.PRESENT_POSITION != AX_TIBIA_HOME_POSITION){
		AX_Go_To(AX_F3R.ID, AX_TIBIA_HOME_POSITION, 300);
	}
}

void AX_Flash()
{
	unsigned char AX_FLASH_1[] = {2,16,AX_STATUS_RETURN_LEVEL};
	unsigned char AX_FLASH_2[] = {4,3,AX_FLASH_ID,AX_BAUD_RATE,AX_DELAY_TIME};
	AX_TX_Instruction(1, AX_WRITE, AX_FLASH_1);
	AX_RX_Status();
	AX_TX_Instruction(1, AX_WRITE, AX_FLASH_2);
	AX_RX_Status();
}

void AX_Go_To(unsigned char ID, unsigned short int Position, unsigned short int Speed)
{
	AX_WRITE_GOAL_POSITION[2] = Position & 0xFF;	//Position Low byte
	AX_WRITE_GOAL_POSITION[3] = Position >> 8;		//Position High Byte
	AX_WRITE_GOAL_POSITION[4] = Speed & 0xFF;		//Speed Low Byte
	AX_WRITE_GOAL_POSITION[5] = Speed >> 8;			//Speed High Byte
	AX_TX_Instruction(ID, AX_WRITE, AX_WRITE_GOAL_POSITION);
}

void AX_Test(void)
{
	
}

unsigned char AX_Move_Leg(unsigned char Leg, double C, double F, double T, unsigned short int Speed){
	AX_Calculate_Leg_Angles(C, F, T);
	if(LEG_ANGLES.Coxa == 0 && LEG_ANGLES.Femur == 0 && LEG_ANGLES.Tibia == 0){
		ERRORS.CANNOT_REACH = 1;
		return 0;
	}else{
		if(AX_Check_Angle_Limits()){
			AX_WRITE_LEG_GOAL_POSITION[3] = AX_SERVO_ID[Leg-1][0];			//Coxa ID
			AX_WRITE_LEG_GOAL_POSITION[4] = (int)LEG_ANGLES.Coxa & 0xFF;	//Coxa Position Low byte
			AX_WRITE_LEG_GOAL_POSITION[5] = (int)LEG_ANGLES.Coxa >> 8;		//Coxa Position High byte
			AX_WRITE_LEG_GOAL_POSITION[6] = Speed & 0xFF;					//Coxa Speed Low Byte
			AX_WRITE_LEG_GOAL_POSITION[7] = Speed >> 8;						//Coxa Speed High Byte
			AX_WRITE_LEG_GOAL_POSITION[8] = AX_SERVO_ID[Leg-1][1];			//Femur ID
			AX_WRITE_LEG_GOAL_POSITION[9] = (int)LEG_ANGLES.Femur & 0xFF;	//Femur Position Low byte
			AX_WRITE_LEG_GOAL_POSITION[10] = (int)LEG_ANGLES.Femur >> 8;	//Femur Position High byte
			AX_WRITE_LEG_GOAL_POSITION[11] = Speed & 0xFF;					//Femur Speed Low Byte
			AX_WRITE_LEG_GOAL_POSITION[12] = Speed >> 8;					//Femur Speed High Byte
			AX_WRITE_LEG_GOAL_POSITION[13] = AX_SERVO_ID[Leg-1][2];			//Tibia ID
			AX_WRITE_LEG_GOAL_POSITION[14] = (int)LEG_ANGLES.Tibia & 0xFF;	//Tibia Position Low byte
			AX_WRITE_LEG_GOAL_POSITION[15] = (int)LEG_ANGLES.Tibia >> 8;	//Tibia Position High byte
			AX_WRITE_LEG_GOAL_POSITION[16] = Speed & 0xFF;					//Tibia Speed Low Byte
			AX_WRITE_LEG_GOAL_POSITION[17] = Speed >> 8;					//Tibia Speed High Byte
			AX_TX_Instruction(254, AX_SYNC_WRITE, AX_WRITE_LEG_GOAL_POSITION);
			ERRORS.CANNOT_REACH = 0;
			return 1;
		}else{
			return 0;
		}
	}

}

void AX_Calculate_Leg_Angles(double X, double Y, double Z)
{
	double a, d, h;
	//Femur-Tibia Joint coordinates starting from Femur-Coxa Bracket point
	POINT_2D JOINT;
	//Calculate the distance between the centres of the circles in Z plane
	d = sqrt(pow(X-COXA_LENGTH, 2)+pow(Z, 2));
	//Check for solutions
	if(d != 0 && d < (FEMUR_LENGTH+TIBIA_LENGTH)){
		//Circles intersect at one or two points
		a = (pow(FEMUR_LENGTH, 2)-pow(TIBIA_LENGTH,2)+pow(d, 2))/(2*d);
		h = sqrt(pow(FEMUR_LENGTH, 2)-pow(a, 2));
		JOINT.X = ((X-COXA_LENGTH)*a-Z*h)/d;
		JOINT.Y = (Z*a+(X-COXA_LENGTH)*h)/d;
		//Check Coxa Quarter and find Coxa Polar Angle in rad
		//The Coxa can only move in Quarter I and IV, therefore X is always positive
		//Also trasfer the angle in rad into AX type
		LEG_ANGLES.Coxa = (COXA_POLAR_ANGLE+(atan2(Y,X)*180/M_PI))/0.29;
		//Check Femur Quarter and find Femur Polar Angle in rad
		//Also trasfer the angle in rad into AX type
		a = atan2(JOINT.Y, JOINT.X)*180/M_PI;
		LEG_ANGLES.Femur = (FEMUR_POLAR_ANGLE-a)/0.29;
		//Check Tibia Quarter and find Tibia Polar Angle in rad
		//Also trasfer the angle in rad into AX type
		d = atan2(Z-JOINT.Y, X-COXA_LENGTH-JOINT.X)*180/M_PI;
		LEG_ANGLES.Tibia = (TIBIA_POLAR_ANGLE+d-a)/0.29;
	}else{
		//If unreachable set all angles to zero
		LEG_ANGLES.Coxa = 0;
		LEG_ANGLES.Femur = 0;
		LEG_ANGLES.Tibia = 0;
	}
}

unsigned char AX_Check_Angle_Limits()
{
	if(LEG_ANGLES.Coxa < AX_COXA_CW_LIMIT || LEG_ANGLES.Coxa > AX_COXA_CCW_LIMIT){
		ERRORS.COXA_ANGLE_LIMITS = 1;
		return 0;
	}else if(LEG_ANGLES.Femur < AX_FEMUR_CW_LIMIT || LEG_ANGLES.Femur > AX_FEMUR_CCW_LIMIT){
		ERRORS.FEMUR_ANGLE_LIMITS = 1;
		return 0;
	}else if(LEG_ANGLES.Tibia < AX_TIBIA_CW_LIMIT || LEG_ANGLES.Tibia > AX_TIBIA_CCW_LIMIT){
		ERRORS.TIBIA_ANGLE_LIMITS = 1;
		return 0;
	}else{
		ERRORS.COXA_ANGLE_LIMITS = 0;
		ERRORS.FEMUR_ANGLE_LIMITS = 0;
		ERRORS.TIBIA_ANGLE_LIMITS = 0;
		return 1;
	}
}

void AX_Ping(unsigned char ID)
{
	AX_TX_Instruction_With_Status(ID, AX_PING, 0);
}

void AX_TX_Instruction(unsigned char ID, const unsigned char Instruction, unsigned char *Parameters)
{
	unsigned char i, Length;
	unsigned int Checksum, Parameters_Checksum = 0;
	Length = Parameters[0] + 2;
	if(Parameters[0]){
		for(i=1; i<=Parameters[0]; i++){
			Parameters_Checksum += Parameters[i];
		}
	}else{
		Parameters_Checksum = 0;
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
	while(ERRORS.RX){	//Error check, if true send instruction again.
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
			ERRORS.RX = 1;
			break;
		}else{
			ERRORS.RX = 0;
		}
	}
}

unsigned short int AX_Read_Present_Position(unsigned char ID)
{
	unsigned char AX_READ_PRESENT_POSITION[] = {2,36,2};
	AX_TX_Instruction(ID, AX_READ, AX_READ_PRESENT_POSITION);
	AX_RX_Status();
	return TX1_TCTI(RX1_Buffer[5], RX1_Buffer[6]);
}

unsigned short int AX_Read_Present_Speed(unsigned char ID)
{
	unsigned char AX_READ_PRESENT_SPEED[] = {2,38,2};
	AX_TX_Instruction(ID, AX_READ, AX_READ_PRESENT_SPEED);
	AX_RX_Status();
	return TX1_TCTI(RX1_Buffer[5], RX1_Buffer[6]);
}

unsigned short int AX_Read_Present_Load(unsigned char ID)
{
	unsigned char AX_READ_PRESENT_LOAD[] = {2,40,2};
	AX_TX_Instruction(ID, AX_READ, AX_READ_PRESENT_LOAD);
	AX_RX_Status();
	return TX1_TCTI(RX1_Buffer[5], RX1_Buffer[6]);
}

unsigned char AX_Read_Present_Voltage(unsigned char ID)
{
	unsigned char AX_READ_PRESENT_VOLTAGE[] = {2,42,1};
	AX_TX_Instruction(ID, AX_READ, AX_READ_PRESENT_VOLTAGE);
	AX_RX_Status();
	return RX1_Buffer[5];
}

unsigned char AX_Read_Present_Temperature(unsigned char ID)
{
	unsigned char AX_READ_PRESENT_TEMPERATURE[] = {2,43,1};
	AX_TX_Instruction(ID, AX_READ, AX_READ_PRESENT_TEMPERATURE);
	AX_RX_Status();
	return RX1_Buffer[5];
}

unsigned char AX_Read_Moving(unsigned char ID)
{
	unsigned char AX_READ_MOVING[] = {2,46,1};
	AX_TX_Instruction(ID, AX_READ, AX_READ_MOVING);
	AX_RX_Status();
	return RX1_Buffer[5];
}

unsigned char AX_Read_Lock(unsigned char ID)
{
	unsigned char AX_READ_LOCK[] = {2,47,1};
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