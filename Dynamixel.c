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

void AxInitLeg(void)
{
	LEG.SPEED = DEFAULT_SPEED;							//Default speed
	LEG.HOME_POSITION.X = DEFAULT_HOME_POSITION_X;		//Default home position x
	LEG.HOME_POSITION.Y = DEFAULT_HOME_POSITION_Y;		//Default home position y
	LEG.HOME_POSITION.Z = DEFAULT_HOME_POSITION_Z;		//Default home position z
	LEG.COXA = AxReadParams(ax_servo_ids[LEG.ID][0]);	//Read default coxa servo settings
	LEG.FEMUR = AxReadParams(ax_servo_ids[LEG.ID][1]);	//Read default femur servo settings
	LEG.TIBIA = AxReadParams(ax_servo_ids[LEG.ID][2]);	//Read default tibia servo settings
}

void AxFlash(void)
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
		if(rx1_timeout){
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
	AxMoveLeg(60, 100, 0, -50);					//Speed, Coxa, Femur, Tibia
	AxLegMoving();
	AxMoveLeg(60, 100, 20, 0);
	AxLegMoving();
	AxMoveLeg(60, 100, 0, 50);
	AxLegMoving();
	AxMoveLeg(60, 100, 20, 0);
	AxLegMoving();
}

void AxMoveLeg(unsigned short int speed, double x, double y, double z){
	unsigned char speedl, speedh;
	y += LEG.HOME_POSITION.Y;
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

void AxLegMoving(void)
{
	unsigned char n;
	unsigned char c = 1;
	while(c){
		c = 0;
		for(n = 0; n <= 2; n++){
			if(AxTxIS(ax_servo_ids[LEG.ID][n], ax_read, ax_read_moving)){
				c = 1;
			}
		}
		Delay(10);
	}
}

void AxLegAngles(double x, double y, double z)
{
	double a, d, h;
	//Femur-Tibia Joint coordinates starting from Femur-Coxa Bracket point
	POINT_2D TF_JOINT;
	//Calculate the distance between the centres of the circles in Z plane
	d = sqrt(pow(x, 2) + pow(z, 2));
	//Check for solutions
	if(d != 0 && d < (FEMUR_LENGTH + TIBIA_LENGTH)){
		//Circles intersect at one or two points
		a = (pow(FEMUR_LENGTH, 2) - pow(TIBIA_LENGTH, 2) + pow(d, 2))/(2*d);
		h = sqrt(pow(FEMUR_LENGTH, 2) - pow(a, 2));
		TF_JOINT.X = (x*a - y*h)/d;
		TF_JOINT.Y = (y*a + x*h)/d;
		//Check Coxa Quarter and find Coxa Polar Angle in rad
		//The Coxa can only move in Quarter I and IV, therefore X is always positive
		h = atan2(z, x + COXA_LENGTH)*180/M_PI;
		//Check Femur Quarter and find Femur Polar Angle in rad
		a = atan2(TF_JOINT.Y, TF_JOINT.X)*180/M_PI;
		//Check Tibia Quarter and find Tibia Polar Angle in rad
		d = atan2(y - TF_JOINT.Y, x - TF_JOINT.X)*180/M_PI;
		//Convert angles into AX type for left side servos
		if(LEG.ID > 2){
			LEG.TARGET_ANGLES.COXA = (COXA_POLAR_ANGLE - h)/0.29;
		}else{
			LEG.TARGET_ANGLES.COXA = (COXA_POLAR_ANGLE + h)/0.29;
		}
		LEG.TARGET_ANGLES.FEMUR = (FEMUR_POLAR_ANGLE - a)/0.29;
		LEG.TARGET_ANGLES.TIBIA = (TIBIA_POLAR_ANGLE + d - a)/0.29;
	}else{
		//If unreachable set all angles to zero
		LEG.TARGET_ANGLES.COXA = 0;
		LEG.TARGET_ANGLES.FEMUR = 0;
		LEG.TARGET_ANGLES.TIBIA = 0;
	}
}

unsigned char AxCheckAngleLimits(void)
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

void AxStartingPosition(void)
{
	AX_LEG_ANGLES ANGLES;
	ANGLES.COXA = (AxTxIS(ax_servo_ids[LEG.ID][0], ax_read, ax_read_present_position)*0.29 - COXA_POLAR_ANGLE)*M_PI/180;
	ANGLES.FEMUR = (FEMUR_POLAR_ANGLE - AxTxIS(ax_servo_ids[LEG.ID][1], ax_read, ax_read_present_position)*0.29)*M_PI/180;
	ANGLES.TIBIA = (AxTxIS(ax_servo_ids[LEG.ID][2], ax_read, ax_read_present_position)*0.29 + ANGLES.FEMUR - TIBIA_POLAR_ANGLE)*M_PI/180;
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
		return tcti(rx1_buffer[5], rx1_buffer[6]);
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