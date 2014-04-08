//--------------------------------------------------------------
//	File:			Dynamixel.h
//	Author:			Vadim Melnicuk
//	Company:		Birmingham City University
//	Date:			11/11/2013
//	Description:
//--------------------------------------------------------------

#ifndef DYNAMIXEL_H
#define	DYNAMIXEL_H

//Structures
struct POINT_2D {
	double X;
	double Y;
};
typedef struct POINT_2D POINT_2D;		//Declaration of type

struct POINT_3D {
	double X;
	double Y;
	double Z;
};
typedef struct POINT_3D POINT_3D;

struct AX_LEG_ANGLES {
	double COXA;
	double FEMUR;
	double TIBIA;
};
typedef struct AX_LEG_ANGLES AX_LEG_ANGLES;

struct AX_PARAMS {
	unsigned char ID;
	unsigned char BAUD_RATE;
	unsigned char DELAY_TIME;
	unsigned short int CW_LIMIT;
	unsigned short int CCW_LIMIT;
	unsigned char HIGHEST_LIMIT_TEMPERATURE;
	unsigned char LOWEST_LIMIT_VOLTAGE;
	unsigned char HIGHEST_LIMIT_VOLTAGE;
	unsigned short int MAX_TORQUE;
	unsigned char STATUS_RETURN_LEVEL;
	unsigned short int PRESENT_POSITION;
	unsigned short int PRESENT_SPEED;
	unsigned short int PRESENT_LOAD;
	unsigned char PRESENT_VOLTAGE;
	unsigned char PRESENT_TEMPERATURE;
	unsigned char MOVING;
	unsigned char LOCK;
};
typedef struct AX_PARAMS AX_PARAMS;

struct AX_LEG {
	unsigned char ID;
	unsigned short int SPEED;
	AX_PARAMS COXA;
	AX_PARAMS FEMUR;
	AX_PARAMS TIBIA;
	AX_LEG_ANGLES TARGET_ANGLES;
	POINT_3D TARGET_POSITION;
	POINT_3D STARTING_POSITION;
	POINT_3D HOME_POSITION;
};
typedef struct AX_LEG AX_LEG;

/*
The BANK SIZE is limited to hold a variable with the maximum size of 256 bytes
The global struct of size of 324 bytes could not be initialised, because of that
All 6 Legs will ne initialised seperatly without holding them in one global structure
representing whole Hexapod.
*/

//Function prototypes
void AxInitLeg(void);
void AxFlash(void);
unsigned char AxPing(unsigned char);
void AxGoTo(unsigned char, unsigned short int, unsigned short int);
void AxTest(void);
void AxLegStepTable(void);
void AxLegStep(unsigned char);
void AxLegStepTransit(unsigned char);
void AxLegMove(double, double, double);
unsigned char AxLegMoving(void);
void AxLegAngles(double, double, double);
unsigned char AxCheckAngleLimits(void);
void AxStartingPosition(void);
void AxTxI(unsigned char, unsigned char, unsigned char *);
unsigned short int AxTxIS(unsigned char, unsigned char, unsigned char *);
void AxRxS(void);
struct AX_PARAMS AxReadParams(unsigned char);
void AxFindLeg(void);

//Defines
//AX Default Settings
#define AX_FLASH_ID 1						//Set desired ID for flashed servo
#define AX_BAUD_RATE 3						//500 Kbps
#define AX_DELAY_TIME 250					//250 us
#define AX_COXA_CW_LIMIT 207				//60 Deg
#define AX_COXA_CCW_LIMIT 827				//240 Deg
#define AX_FEMUR_CW_LIMIT 470				//136 Deg
#define AX_FEMUR_CCW_LIMIT 1023				//300 Deg
#define AX_TIBIA_CW_LIMIT 240				//70 Deg
#define AX_TIBIA_CCW_LIMIT 1023				//300 Deg
#define AX_HIGHEST_LIMIT_TEMPERATURE 140	//70 Deg Celsius
#define AX_LOWEST_LIMIT_VOLTAGE 60			//6 V
#define AX_HIGHEST_LIMIT_VOLTAGE 140		//14 V
#define AX_MAX_TORQUE 1023					//100 %
#define AX_STATUS_RETURN_LEVEL 1			//Return only for the READ command
#define AX_LOCK 0							//EEPROM area can be modified

#define COXA_LENGTH 42.0					//Coxa->Femur mm
#define FEMUR_LENGTH 120.0					//Femur->Tibia mm
#define TIBIA_LENGTH 166.0					//Tibia->End of foot mm
#define COXA_POLAR_ANGLE 150.0				//Deg
#define FEMUR_POLAR_ANGLE 255.0				//Deg
#define TIBIA_POLAR_ANGLE 225.0				//Deg
#define DEFAULT_HOME_POSITION_X 80.0		//Default Home position
#define DEFAULT_HOME_POSITION_Y 0			//Default Home position
#define DEFAULT_HOME_POSITION_Z 0			//Default Home position
#define DEFAULT_SPEED 50					//Default Speed
#define STEP_RES 10
#define STEP_X 120.0
#define STEP_Y 30.0
#define STEP_Z 100.0

//Global variables
unsigned char ax_ping = 1;
unsigned char ax_read_ping[] = {0};
unsigned char ax_read = 2;
unsigned char ax_read_baud_rate[] = {2,4,1};
unsigned char ax_read_delay_time[] = {2,5,1};
unsigned char ax_read_cw_limit[] = {2,6,2};
unsigned char ax_read_ccw_limit[] = {2,8,2};
unsigned char ax_read_highest_limit_temperature[] = {2,11,1};
unsigned char ax_read_lowest_limit_voltage[] = {2,12,1};
unsigned char ax_read_highest_limit_voltage[] = {2,13,1};
unsigned char ax_read_max_torque[] = {2,14,2};
unsigned char ax_read_status_return_level[] = {2,16,1};
unsigned char ax_read_present_position[] = {2,36,2};
unsigned char ax_read_present_speed[] = {2,38,2};
unsigned char ax_read_present_load[] = {2,40,2};
unsigned char ax_read_present_voltage[] = {2,42,1};
unsigned char ax_read_present_temperature[] = {2,43,1};
unsigned char ax_read_moving[] = {2,46,1};
unsigned char ax_read_lock[] = {2,47,1};

unsigned char ax_write = 3;
unsigned char ax_sync_write = 131;
unsigned char ax_write_id[] = {2,3,1};
unsigned char ax_write_baud_rate_1m[] = {2,4,1};
unsigned char ax_write_baud_rate_500k[] = {2,4,3};
unsigned char ax_write_baud_rate_250k[] = {2,4,7};
unsigned char ax_write_delay_time[] = {2,5,250};
unsigned char ax_write_cw_limit[] = {3,6,0,0};
unsigned char ax_write_ccw_limit[] = {3,8,255,3};
unsigned char ax_write_highest_limit_temperature[] = {2,11,70};
unsigned char ax_write_lowest_limit_voltage[] = {2,12,60};
unsigned char ax_write_highest_limit_voltage[] = {2,13,140};
unsigned char ax_write_max_torque[] = {3,14,255,3};
unsigned char ax_write_status_return_level[] = {2,16,2};
unsigned char ax_write_torque_en[] = {2,24,0};
unsigned char ax_write_lock[] = {2,47,0};
unsigned char ax_write_goal_position[] = {5,30,0,2,0,2};
unsigned char ax_write_goal_position_home[] = {5,30,0,2,0,2};
unsigned char ax_write_leg_goal_position[] = {17,30,4,0,0,2,0,2,0,0,2,0,2,0,0,2,0,2};

unsigned char ax_servo_ids[6][3] = {{1,2,3},{4,5,6},{7,8,9},{10,11,12},{13,14,15},{16,17,18}};

const double step_table[40][3] = {{517.234376,492.468752,244.125000},{522.187500,500.320312,244.289064},{527.109376,508.648432,244.773440},{532.031250,517.437504,245.636720},{536.937504,526.562496,246.781248},{541.812496,535.984368,248.261720},{546.656256,545.640624,250.039063},{551.468752,555.406248,252.085938},{556.234376,565.312504,254.429688},{560.968752,575.203124,257.000000},{633.421872,611.421872,292.445312},{624.015625,606.281248,285.679680},{614.046875,601.406248,279.429688},{603.500000,596.828128,273.750000},{592.421872,592.671872,268.679680},{580.796880,588.968752,264.304688},{568.734368,585.781248,260.625008},{556.234376,583.203124,257.703120},{543.421872,581.281248,255.574224},{530.390624,580.140624,254.308592},{517.234376,579.734368,253.878912},{504.093750,580.140624,254.308592},{491.054688,581.281248,255.574224},{478.242188,583.203124,257.703120},{465.757808,585.781248,260.625008},{453.679680,588.968752,264.304688},{442.062500,592.671872,268.679680},{430.968752,596.828128,273.750000},{420.437504,601.406248,279.429688},{410.468752,606.281248,285.679680},{401.070312,547.140624,285.304688},{401.070312,541.453128,285.398440},{401.070312,535.875008,285.601568},{401.070312,530.546880,285.976560},{401.070312,525.375000,286.476560},{401.070312,520.421872,287.132812},{401.070312,515.625008,287.914064},{401.070312,511.085938,288.859376},{401.070312,506.703120,289.914064},{401.070312,502.585936,291.125000}};
const double cos_table[10] = {0,0.1736,0.342,0.5,0.6428,0.766,0.866,0.9397,0.9848,1};

//Declare structs in .h in order to make them global
AX_LEG LEG = {0};

//XC8 does not support designated initializers like C99 does. Use comments instead.
//Set it to "const" in order to store in program memory

#endif	//DYNAMIXEL_H