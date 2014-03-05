//--------------------------------------------------------------
//	File:			Dynamixel.c
//	Author:			Vadim Melnicuk
//	Company:		Birmingham City University
//	Date:			11/11/2013
//	Description:
//--------------------------------------------------------------

//Header files
#include "Config.h"

//--------------------------------------------------------------

void AX_Init(void)
{
	AX_F1R = AX_Read_Params(1);
	AX_F2R = AX_Read_Params(2);
	AX_F3R = AX_Read_Params(3);

	AX_Init_Leg(1);

	while(1){
		AX_Test();
	}
}

void AX_Init_Leg(unsigned char Leg)
{
	Leg -= 1;										//Convert to leg array number
	AX_Go_To(AX_SERVO_ID[Leg][0], AX_COXA_HOME_POSITION, 100);
	AX_Go_To(AX_SERVO_ID[Leg][1], AX_FEMUR_HOME_POSITION, 100);
	AX_Go_To(AX_SERVO_ID[Leg][2], AX_TIBIA_HOME_POSITION, 100);
}

void AX_Flash()
{
	unsigned char AX_FLASH_1[] = {2,16,AX_STATUS_RETURN_LEVEL};
	unsigned char AX_FLASH_2[] = {4,3,AX_FLASH_ID,AX_BAUD_RATE,AX_DELAY_TIME};
	AX_TX_I(1, AX_WRITE, AX_FLASH_1);
	AX_RX_S();
	AX_TX_I(1, AX_WRITE, AX_FLASH_2);
	AX_RX_S();
}

void AX_Go_To(unsigned char ID, unsigned short int Position, unsigned short int Speed)
{
	AX_WRITE_GOAL_POSITION[2] = Position & 0xFF;	//Position Low byte
	AX_WRITE_GOAL_POSITION[3] = Position >> 8;		//Position High Byte
	AX_WRITE_GOAL_POSITION[4] = Speed & 0xFF;		//Speed Low Byte
	AX_WRITE_GOAL_POSITION[5] = Speed >> 8;			//Speed High Byte
	AX_TX_I(ID, AX_WRITE, AX_WRITE_GOAL_POSITION);
}

void AX_Test(void)
{
	AX_Move_Leg(1, 60, 150, -100, -100);			//Leg, Steps, Speed, Coxa, Femur, Tibia
	while(AX_Leg_Moving(1)){__delay_ms(10);}
	AX_Move_Leg(1, 60, 150, 0, -20);
	while(AX_Leg_Moving(1)){__delay_ms(10);}
	AX_Move_Leg(1, 60, 150, 100, -100);
	while(AX_Leg_Moving(1)){__delay_ms(10);}
	AX_Move_Leg(1, 60, 150, 0, -20);
	while(AX_Leg_Moving(1)){__delay_ms(10);}
}

void AX_Move_Leg(unsigned char Leg, unsigned short int Speed, double X, double Y, double Z){
	unsigned char SpeedL, SpeedH;
	Leg -= 1;										//Convert to leg array number
	AX_Leg_Angles(X, Y, Z);
	if(LEG_ANGLES.Coxa == 0 && LEG_ANGLES.Femur == 0 && LEG_ANGLES.Tibia == 0){
		//Error handler will be here
	}else{
		if(AX_Check_Angle_Limits()){
			SpeedL = Speed & 0xFF;
			SpeedH = Speed >> 8;
			AX_WRITE_LEG_GOAL_POSITION[3] = AX_SERVO_ID[Leg][0];			//Coxa ID
			AX_WRITE_LEG_GOAL_POSITION[4] = (int)LEG_ANGLES.Coxa & 0xFF;	//Coxa Position Low byte
			AX_WRITE_LEG_GOAL_POSITION[5] = (int)LEG_ANGLES.Coxa >> 8;		//Coxa Position High byte
			AX_WRITE_LEG_GOAL_POSITION[6] = SpeedL;							//Coxa Speed Low Byte
			AX_WRITE_LEG_GOAL_POSITION[7] = SpeedH;							//Coxa Speed High Byte
			AX_WRITE_LEG_GOAL_POSITION[8] = AX_SERVO_ID[Leg][1];			//Femur ID
			AX_WRITE_LEG_GOAL_POSITION[9] = (int)LEG_ANGLES.Femur & 0xFF;	//Femur Position Low byte
			AX_WRITE_LEG_GOAL_POSITION[10] = (int)LEG_ANGLES.Femur >> 8;	//Femur Position High byte
			AX_WRITE_LEG_GOAL_POSITION[11] = SpeedL;						//Femur Speed Low Byte
			AX_WRITE_LEG_GOAL_POSITION[12] = SpeedH;						//Femur Speed High Byte
			AX_WRITE_LEG_GOAL_POSITION[13] = AX_SERVO_ID[Leg][2];			//Tibia ID
			AX_WRITE_LEG_GOAL_POSITION[14] = (int)LEG_ANGLES.Tibia & 0xFF;	//Tibia Position Low byte
			AX_WRITE_LEG_GOAL_POSITION[15] = (int)LEG_ANGLES.Tibia >> 8;	//Tibia Position High byte
			AX_WRITE_LEG_GOAL_POSITION[16] = SpeedL;						//Tibia Speed Low Byte
			AX_WRITE_LEG_GOAL_POSITION[17] = SpeedH;						//Tibia Speed High Byte
			AX_TX_I(254, AX_SYNC_WRITE, AX_WRITE_LEG_GOAL_POSITION);
		}
	}
}

unsigned char AX_Leg_Moving(unsigned char Leg)
{
	unsigned char i;
	Leg -= 1;										//Convert to leg array number
	for(i=0;i<=2;i++){
		if(AX_TX_IS(AX_SERVO_ID[Leg][i], AX_READ, AX_READ_MOVING)){
			return 1;
		}
	}
	return 0;
}

void AX_Leg_Angles(double X, double Y, double Z)
{
	double a, d, h;
	//Femur-Tibia Joint coordinates starting from Femur-Coxa Bracket point
	POINT_2D JOINT;
	//Calculate the distance between the centres of the circles in Z plane
	d = sqrt(pow(X, 2)+pow(Z, 2));
	//Check for solutions
	if(d != 0 && d < (FEMUR_LENGTH+TIBIA_LENGTH)){
		//Circles intersect at one or two points
		a = (pow(FEMUR_LENGTH, 2)-pow(TIBIA_LENGTH,2)+pow(d, 2))/(2*d);
		h = sqrt(pow(FEMUR_LENGTH, 2)-pow(a, 2));
		JOINT.X = (X*a-Z*h)/d;
		JOINT.Y = (Z*a+X*h)/d;
		//Check Coxa Quarter and find Coxa Polar Angle in rad
		//The Coxa can only move in Quarter I and IV, therefore X is always positive
		//Also trasfer the angle in rad into AX type
		h = atan2(Y,X+COXA_LENGTH)*180/M_PI;
		LEG_ANGLES.Coxa = (COXA_POLAR_ANGLE+h)/0.29;
		//Check Femur Quarter and find Femur Polar Angle in rad
		//Also trasfer the angle in rad into AX type
		a = atan2(JOINT.Y, JOINT.X)*180/M_PI;
		LEG_ANGLES.Femur = (FEMUR_POLAR_ANGLE-a)/0.29;
		//Check Tibia Quarter and find Tibia Polar Angle in rad
		//Also trasfer the angle in rad into AX type
		d = atan2(Z-JOINT.Y, X-JOINT.X)*180/M_PI;
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
		//TODO - Error handler will be here
		return 0;
	}else if(LEG_ANGLES.Femur < AX_FEMUR_CW_LIMIT || LEG_ANGLES.Femur > AX_FEMUR_CCW_LIMIT){
		//TODO - Error handler will be here
		return 0;
	}else if(LEG_ANGLES.Tibia < AX_TIBIA_CW_LIMIT || LEG_ANGLES.Tibia > AX_TIBIA_CCW_LIMIT){
		//TODO - Error handler will be here
		return 0;
	}else{
		return 1;
	}
}

void AX_Starting_Position(unsigned char Leg)
{
	AX_LEG_ANGLES ANGLES;
	Leg -= 1;										//Convert to leg array number
	ANGLES.Coxa = (AX_TX_IS(AX_SERVO_ID[Leg][0], AX_READ, AX_READ_PRESENT_POSITION)*0.29-COXA_POLAR_ANGLE)*M_PI/180;
	ANGLES.Femur = (FEMUR_POLAR_ANGLE-AX_TX_IS(AX_SERVO_ID[Leg][1], AX_READ, AX_READ_PRESENT_POSITION)*0.29)*M_PI/180;
	ANGLES.Tibia = (AX_TX_IS(AX_SERVO_ID[Leg][2], AX_READ, AX_READ_PRESENT_POSITION)*0.29+ANGLES.Femur-TIBIA_POLAR_ANGLE)*M_PI/180;
	STARTING_POSITION.X = FEMUR_LENGTH*cos(ANGLES.Femur)+TIBIA_LENGTH*cos(ANGLES.Femur+ANGLES.Tibia);
	STARTING_POSITION.Z = FEMUR_LENGTH*sin(ANGLES.Femur)+TIBIA_LENGTH*sin(ANGLES.Femur+ANGLES.Tibia);
	STARTING_POSITION.Y = (STARTING_POSITION.X+COXA_LENGTH)*tan(ANGLES.Coxa);
}

void AX_Ping(unsigned char ID)
{
	AX_TX_IS(ID, AX_PING, 0);
}

void AX_TX_I(unsigned char ID, unsigned char Instruction, unsigned char *Params)
{
	unsigned char i, Length;
	unsigned int Checksum, Parameters_Checksum = 0;
	//Number of passed parameters + 2
	Length = Params[0] + 2;
	//Do not count Params[0] as it is number of parameters passed through
	for(i = 1; i <= Params[0]; i++){	//Calculate paramenters checksum
		Parameters_Checksum += Params[i];
	}
	Checksum = (~(ID + Length + Instruction + Parameters_Checksum)) & 0xFF;
	EUSART1_Mode(0);					//Change Mode to Transmit
	TX1_Byte(255);						//Acnowledge byte
	TX1_Byte(255);						//Acnowledge byte
	TX1_Byte(ID);						//Servo ID
	TX1_Byte(Length);					//Length of Instruction
	TX1_Byte(Instruction);				//Type of Instruction
	//Do not pass Params[0] as it is number of parameters passed through
	for(i = 1; i <= Params[0]; i++){
		TX1_Byte(Params[i]);			//Send paramenters
	}
	TX1_Byte(Checksum);					//Send Checksum
	while(!TX1_STATUS);					//Wait for transmit to finish
}

unsigned short int AX_TX_IS(unsigned char ID, unsigned char Instruction, unsigned char *Params)
{
	AX_TX_I(ID, Instruction, Params);
	AX_RX_S();
	if(Params[2] > 1){
		return TX1_TCTI(RX1_Buffer[5], RX1_Buffer[6]);
	}else{
		return RX1_Buffer[5];
	}
}

void AX_RX_S(void)
{
	unsigned char i, Length = 3;
	EUSART1_Mode(1);
	for(i = 0; i <= Length; i++){
		RX1_Buffer[i] = RX1_Byte();
		if(i == 3){
			Length += RX1_Buffer[3];
		}
	}
}

struct AX_PARAMS AX_Read_Params(unsigned char ID)
{
	struct AX_PARAMS Servo;
	Servo.ID = ID;
	Servo.BAUD_RATE = AX_TX_IS(ID, AX_READ, AX_READ_BAUD_RATE);
	Servo.DELAY_TIME = AX_TX_IS(ID, AX_READ, AX_READ_DELAY_TIME);
	Servo.CW_LIMIT = AX_TX_IS(ID, AX_READ, AX_READ_CW_LIMIT);
	Servo.CCW_LIMIT = AX_TX_IS(ID, AX_READ, AX_READ_CCW_LIMIT);
	Servo.HIGHEST_LIMIT_TEMPERATURE = AX_TX_IS(ID, AX_READ, AX_READ_HIGHEST_LIMIT_TEMPERATURE);
	Servo.LOWEST_LIMIT_VOLTAGE = AX_TX_IS(ID, AX_READ, AX_READ_LOWEST_LIMIT_VOLTAGE);
	Servo.HIGHEST_LIMIT_VOLTAGE = AX_TX_IS(ID, AX_READ, AX_READ_HIGHEST_LIMIT_VOLTAGE);
	Servo.MAX_TORQUE = AX_TX_IS(ID, AX_READ, AX_READ_MAX_TORQUE);
	Servo.STATUS_RETURN_LEVEL = AX_TX_IS(ID, AX_READ, AX_READ_STATUS_RETURN_LEVEL);
	Servo.PRESENT_POSITION = AX_TX_IS(ID, AX_READ, AX_READ_PRESENT_POSITION);
	Servo.PRESENT_SPEED = AX_TX_IS(ID, AX_READ, AX_READ_PRESENT_SPEED);
	Servo.PRESENT_LOAD = AX_TX_IS(ID, AX_READ, AX_READ_PRESENT_LOAD);
	Servo.PRESENT_VOLTAGE = AX_TX_IS(ID, AX_READ, AX_READ_PRESENT_VOLTAGE);
	Servo.PRESENT_TEMPERATURE = AX_TX_IS(ID, AX_READ, AX_READ_PRESENT_TEMPERATURE);
	Servo.MOVING = AX_TX_IS(ID, AX_READ, AX_READ_MOVING);
	Servo.LOCK = AX_TX_IS(ID, AX_READ, AX_READ_LOCK);
	return Servo;
}