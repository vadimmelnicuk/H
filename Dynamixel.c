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

void AxInit(void)
{
	AxFindLeg();
	AxInitLeg();
	AxMoveLeg(50, 100, 50, 50);
	while(1);
}

void AxInitLeg()
{
	LEG.COXA = AxReadParams(ax_servo_ids[LEG.ID][0]);
	LEG.FEMUR = AxReadParams(ax_servo_ids[LEG.ID][1]);
	LEG.TIBIA = AxReadParams(ax_servo_ids[LEG.ID][2]);

	if(LEG.ID > 2){
		//Polar Angles for left side legs
		LEG.POLAR_ANGLES.COXA = 150;
		LEG.POLAR_ANGLES.FEMUR = 45;
		LEG.POLAR_ANGLES.TIBIA = 75;
	}else{
		//Polar Angles for right side legs
		LEG.POLAR_ANGLES.COXA = 150;
		LEG.POLAR_ANGLES.FEMUR = 255;
		LEG.POLAR_ANGLES.TIBIA = 225;
	}

	AxGoTo(ax_servo_ids[LEG.ID][0], COXA_HOME_POSITION, 100);
	AxGoTo(ax_servo_ids[LEG.ID][1], FEMUR_HOME_POSITION, 100);
	AxGoTo(ax_servo_ids[LEG.ID][2], TIBIA_HOME_POSITION, 100);
}

void AxFlash()
{
	unsigned char ax_flash1[] = {2, 16, AX_STATUS_RETURN_LEVEL};
	unsigned char ax_flash2[] = {4, 3, AX_FLASH_ID, AX_BAUD_RATE, AX_DELAY_TIME};
	AxTxI(1, ax_write, ax_flash1);
	Delay(10);
	AxTxI(1, ax_write, ax_flash2);
	Delay(10);
}

unsigned char AxPing(unsigned char id)
{
	unsigned char n;
	unsigned char length = 3;
	AxTxI(id, ax_ping, ax_read_ping);
	Eusart1Mode(1);
	for(n = 0; n <= length; n++){
		rx1_buffer[n] = Rx1ByteTimeOut();
		if(rx1_error){
			return 0;
		}
		if(n == 3){
			length += rx1_buffer[3];
		}
	}
	return 1;
}

void AxGoTo(unsigned char id, unsigned short int position, unsigned short int speed)
{
	ax_write_goal_position[2] = position & 0xFF;	//Position Low byte
	ax_write_goal_position[3] = position >> 8;		//Position High Byte
	ax_write_goal_position[4] = speed & 0xFF;		//Speed Low Byte
	ax_write_goal_position[5] = speed >> 8;			//Speed High Byte
	AxTxI(id, ax_write, ax_write_goal_position);
}

void AxTest(void)
{
	AxMoveLeg(60, 150, -100, -100);					//Speed, Coxa, Femur, Tibia
	while(AxLegMoving()){__delay_ms(10);}
	AxMoveLeg(60, 150, 0, -20);
	while(AxLegMoving()){__delay_ms(10);}
	AxMoveLeg(60, 150, 100, -100);
	while(AxLegMoving()){__delay_ms(10);}
	AxMoveLeg(60, 150, 0, -20);
	while(AxLegMoving()){__delay_ms(10);}
}

void AxMoveLeg(unsigned short int speed, double x, double y, double z){
	unsigned char speedl, speedh;
	AxLegAngles(x, y, z);
	if(LEG.TARGET_ANGLES.COXA  == 0 && LEG.TARGET_ANGLES.FEMUR == 0 && LEG.TARGET_ANGLES.TIBIA == 0){
		//TODO - Error handler will be here
	}else{
		if(AxCheckAngleLimits()){
			speedl = speed & 0xFF;
			speedh = speed >> 8;
			ax_write_leg_goal_position[3] = ax_servo_ids[LEG.ID][0];					//Coxa ID
			ax_write_leg_goal_position[4] = (int)LEG.TARGET_ANGLES.COXA & 0xFF;		//Coxa Position Low byte
			ax_write_leg_goal_position[5] = (int)LEG.TARGET_ANGLES.COXA >> 8;		//Coxa Position High byte
			ax_write_leg_goal_position[6] = speedl;									//Coxa Speed Low Byte
			ax_write_leg_goal_position[7] = speedh;									//Coxa Speed High Byte
			ax_write_leg_goal_position[8] = ax_servo_ids[LEG.ID][1];					//Femur ID
			ax_write_leg_goal_position[9] = (int)LEG.TARGET_ANGLES.FEMUR & 0xFF;	//Femur Position Low byte
			ax_write_leg_goal_position[10] = (int)LEG.TARGET_ANGLES.FEMUR >> 8;		//Femur Position High byte
			ax_write_leg_goal_position[11] = speedl;								//Femur Speed Low Byte
			ax_write_leg_goal_position[12] = speedh;								//Femur Speed High Byte
			ax_write_leg_goal_position[13] = ax_servo_ids[LEG.ID][2];				//Tibia ID
			ax_write_leg_goal_position[14] = (int)LEG.TARGET_ANGLES.TIBIA & 0xFF;	//Tibia Position Low byte
			ax_write_leg_goal_position[15] = (int)LEG.TARGET_ANGLES.TIBIA >> 8;		//Tibia Position High byte
			ax_write_leg_goal_position[16] = speedl;								//Tibia Speed Low Byte
			ax_write_leg_goal_position[17] = speedh;								//Tibia Speed High Byte
			AxTxI(254, ax_sync_write, ax_write_leg_goal_position);
		}
	}
}

unsigned char AxLegMoving()
{
	unsigned char n;
	for(n = 0; n <= 2; n++){
		if(AxTxIS(ax_servo_ids[LEG.ID][n], ax_read, ax_read_moving)){
			return 1;
		}
	}
	return 0;
}

void AxLegAngles(double x, double y, double z)
{
	double a, d, h;
	//Femur-Tibia Joint coordinates starting from Femur-Coxa Bracket point
	POINT_2D JOINT;
	//Calculate the distance between the centres of the circles in Z plane
	d = sqrt(pow(x, 2) + pow(z, 2));
	//Check for solutions
	if(d != 0 && d < (FEMUR_LENGTH + TIBIA_LENGTH)){
		//Circles intersect at one or two points
		a = (pow(FEMUR_LENGTH, 2) - pow(TIBIA_LENGTH, 2) + pow(d, 2))/(2*d);
		h = sqrt(pow(FEMUR_LENGTH, 2) - pow(a, 2));
		JOINT.X = (x*a - z*h)/d;
		JOINT.Y = (z*a + x*h)/d;
		//Check Coxa Quarter and find Coxa Polar Angle in rad
		//The Coxa can only move in Quarter I and IV, therefore X is always positive
		//Also trasfer the angle in rad into AX type
		h = atan2(y, x + COXA_LENGTH)*180/M_PI;
		LEG.TARGET_ANGLES.COXA = (LEG.POLAR_ANGLES.COXA + h)/0.29;
		//Check Femur Quarter and find Femur Polar Angle in rad
		//Also trasfer the angle in rad into AX type
		a = atan2(JOINT.Y, JOINT.X)*180/M_PI;
		LEG.TARGET_ANGLES.FEMUR = (LEG.POLAR_ANGLES.FEMUR - a)/0.29;
		//Check Tibia Quarter and find Tibia Polar Angle in rad
		//Also trasfer the angle in rad into AX type
		d = atan2(z - JOINT.Y, x - JOINT.X)*180/M_PI;
		LEG.TARGET_ANGLES.TIBIA = (LEG.POLAR_ANGLES.TIBIA + d - a)/0.29;
	}else{
		//If unreachable set all angles to zero
		LEG.TARGET_ANGLES.COXA = 0;
		LEG.TARGET_ANGLES.FEMUR = 0;
		LEG.TARGET_ANGLES.TIBIA = 0;
	}
}

unsigned char AxCheckAngleLimits()
{
	if(LEG.TARGET_ANGLES.COXA < AX_COXA_CW_LIMIT || LEG.TARGET_ANGLES.COXA > AX_COXA_CCW_LIMIT){
		//TODO - Error handler will be here
		return 0;
	}else if(LEG.TARGET_ANGLES.FEMUR < AX_FEMUR_CW_LIMIT || LEG.TARGET_ANGLES.FEMUR > AX_FEMUR_CCW_LIMIT){
		//TODO - Error handler will be here
		return 0;
	}else if(LEG.TARGET_ANGLES.TIBIA < AX_TIBIA_CW_LIMIT || LEG.TARGET_ANGLES.TIBIA > AX_TIBIA_CCW_LIMIT){
		//TODO - Error handler will be here
		return 0;
	}else{
		return 1;
	}
}

void AxStartingPosition()
{
	AX_LEG_ANGLES ANGLES;
	ANGLES.COXA = (AxTxIS(ax_servo_ids[LEG.ID][0], ax_read, ax_read_present_position)*0.29 - LEG.POLAR_ANGLES.COXA)*M_PI/180;
	ANGLES.FEMUR = (LEG.POLAR_ANGLES.FEMUR-AxTxIS(ax_servo_ids[LEG.ID][1], ax_read, ax_read_present_position)*0.29)*M_PI/180;
	ANGLES.TIBIA = (AxTxIS(ax_servo_ids[LEG.ID][2], ax_read, ax_read_present_position)*0.29 + ANGLES.FEMUR - LEG.POLAR_ANGLES.TIBIA)*M_PI/180;
	LEG.STARTING_POSITION.X = FEMUR_LENGTH*cos(ANGLES.FEMUR) + TIBIA_LENGTH*cos(ANGLES.FEMUR + ANGLES.TIBIA);
	LEG.STARTING_POSITION.Z = FEMUR_LENGTH*sin(ANGLES.FEMUR) + TIBIA_LENGTH*sin(ANGLES.FEMUR + ANGLES.TIBIA);
	LEG.STARTING_POSITION.Y = (LEG.STARTING_POSITION.X + COXA_LENGTH)*tan(ANGLES.COXA);
}

void AxTxI(unsigned char id, unsigned char instruction, unsigned char *params)
{
	unsigned char n, length;
	unsigned int checksum, params_checksum = 0;
	if(params[0] > 0){
		//Number of passed parameters + 2
		length = params[0] + 2;
		//Do not count Params[0] as it is number of parameters passed through
		for(n = 1; n <= params[0]; n++){	//Calculate paramenters checksum
			params_checksum += params[n];
		}
	}else{
		length = 2;
		params_checksum = 0;
	}
	checksum = (~(id + length + instruction + params_checksum)) & 0xFF;
	Eusart1Mode(0);					//Change Mode to Transmit
	Tx1Byte(255);						//Acnowledge byte
	Tx1Byte(255);						//Acnowledge byte
	Tx1Byte(id);						//Servo ID
	Tx1Byte(length);					//Length of Instruction
	Tx1Byte(instruction);				//Type of Instruction
	//Do not pass Params[0] as it is number of parameters passed through
	for(n = 1; n <= params[0]; n++){
		Tx1Byte(params[n]);				//Send paramenters
	}
	Tx1Byte(checksum);					//Send Checksum
	while(!TX1_STATUS);					//Wait for transmit to finish
}

unsigned short int AxTxIS(unsigned char id, unsigned char instruction, unsigned char *params)
{
	AxTxI(id, instruction, params);
	AxRxS();
	if(params[2] > 1){
		return Tx1Tcti(rx1_buffer[5], rx1_buffer[6]);
	}else{
		return rx1_buffer[5];
	}
}

void AxRxS(void)
{
	unsigned char n, Length = 3;
	Eusart1Mode(1);
	for(n = 0; n <= Length; n++){
		rx1_buffer[n] = Rx1Byte();
		if(n == 3){
			Length += rx1_buffer[3];
		}
	}
}

struct AX_PARAMS AxReadParams(unsigned char id)
{
	struct AX_PARAMS SERVO;
	SERVO.ID = id;
	SERVO.BAUD_RATE = AxTxIS(id, ax_read, ax_read_baud_rate);
	SERVO.DELAY_TIME = AxTxIS(id, ax_read, ax_read_delay_time);
	SERVO.CW_LIMIT = AxTxIS(id, ax_read, ax_read_cw_limit);
	SERVO.CCW_LIMIT = AxTxIS(id, ax_read, ax_read_ccw_limit);
	SERVO.HIGHEST_LIMIT_TEMPERATURE = AxTxIS(id, ax_read, ax_read_highest_limit_temperature);
	SERVO.LOWEST_LIMIT_VOLTAGE = AxTxIS(id, ax_read, ax_read_lowest_limit_voltage);
	SERVO.HIGHEST_LIMIT_VOLTAGE = AxTxIS(id, ax_read, ax_read_highest_limit_voltage);
	SERVO.MAX_TORQUE = AxTxIS(id, ax_read, ax_read_max_torque);
	SERVO.STATUS_RETURN_LEVEL = AxTxIS(id, ax_read, ax_read_status_return_level);
	SERVO.PRESENT_POSITION = AxTxIS(id, ax_read, ax_read_present_position);
	SERVO.PRESENT_SPEED = AxTxIS(id, ax_read, ax_read_present_speed);
	SERVO.PRESENT_LOAD = AxTxIS(id, ax_read, ax_read_present_load);
	SERVO.PRESENT_VOLTAGE = AxTxIS(id, ax_read, ax_read_present_voltage);
	SERVO.PRESENT_TEMPERATURE = AxTxIS(id, ax_read, ax_read_present_temperature);
	SERVO.MOVING = AxTxIS(id, ax_read, ax_read_moving);
	SERVO.LOCK = AxTxIS(id, ax_read, ax_read_lock);
	return SERVO;
}

void AxFindLeg(void)
{
	unsigned char n;
	for(n = 0; n <= 5; n++){
		if(AxPing(ax_servo_ids[n][2])){
			LEG.ID = n;
			break;
		}
	}
}