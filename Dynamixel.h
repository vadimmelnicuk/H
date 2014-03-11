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
	AX_PARAMS COXA;
	AX_PARAMS FEMUR;
	AX_PARAMS TIBIA;
	AX_LEG_ANGLES TARGET_ANGLES;
	AX_LEG_ANGLES POLAR_ANGLES;
	POINT_3D STARTING_POSITION;
};
typedef struct AX_LEG AX_LEG;

/*
The BANK SIZE is limited to hold a variable with the maximum size of 256 bytes
The global struct of size of 324 bytes could not be initialised, because of that
All 6 Legs will ne initialised seperatly without holding them in one global structure
representing whole Hexapod.
*/

//Function prototypes
void AxInit(void);
void AxInitLeg();
void AxFlash();
unsigned char AxPing(unsigned char);
void AxGoTo(unsigned char, unsigned short int, unsigned short int);
void AxTest(void);
void AxMoveLeg(unsigned short int, double, double, double);
unsigned char AxLegMoving();
void AxLegAngles(double, double, double);
unsigned char AxCheckAngleLimits();
void AxStartingPosition();
void AxTxI(unsigned char, unsigned char, unsigned char *);
unsigned short int AxTxIS(unsigned char, unsigned char, unsigned char *);
void AxRxS(void);
struct AX_PARAMS AxReadParams(unsigned char);
void AxFindLeg(void);

//Defines
#if defined(MODE_LEG) | defined(MODE_CON)
	#define AX_BAUD_RATE_KBPS 500000		//500 Kbps
#endif
#ifdef MODE_FLASH
	#define AX_BAUD_RATE_KBPS 1000000		//1 Mbps
#endif

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

#define COXA_LENGTH 43.0					//Coxa->Femur mm
#define FEMUR_LENGTH 120.0					//Femur->Tibia mm
#define TIBIA_LENGTH 166.0					//Tibia->End of foot mm

#define COXA_HOME_POSITION 512				//150 Deg
#define FEMUR_HOME_POSITION 470				//140 Deg
#define TIBIA_HOME_POSITION 240				//90 Deg

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
unsigned char ax_write_torque_en[] = {2,24,1};
unsigned char ax_write_lock[] = {2,47,0};
unsigned char ax_write_goal_position[] = {5,30,0,2,0,2};
unsigned char ax_write_goal_position_home[] = {5,30,0,2,0,2};
unsigned char ax_write_leg_goal_position[] = {17,30,4,0,0,2,0,2,0,0,2,0,2,0,0,2,0,2};

unsigned char ax_servo_ids[6][3] = {{1,2,3},{4,5,6},{7,8,9},{10,11,12},{13,14,15},{16,17,18}};

//Declare structs in .h in order to make them global
AX_LEG LEG = {0};

//XC8 does not support designated initializers like C99 does. Use comments instead.
//Set it to "const" in order to store in program memory

#endif	//DYNAMIXEL_H