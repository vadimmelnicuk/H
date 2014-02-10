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
	double Coxa;
	double Femur;
	double Tibia;
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

struct AX_ERRORS {
	unsigned char RX;
	unsigned char CANNOT_REACH;
	unsigned char COXA_ANGLE_LIMITS;
	unsigned char FEMUR_ANGLE_LIMITS;
	unsigned char TIBIA_ANGLE_LIMITS;
};
typedef struct AX_ERRORS AX_ERRORS;

/*
The BANK SIZE is limited to hold a variable with the maximum size of 256 bytes
The global struct of size of 324 bytes could not be initialised, because of that
All 6 Legs will ne initialised seperatly without holding them in one global structure
representing whole Hexapod.
*/

//Function prototypes
void AX_Init(void);
void AX_Init_Leg(unsigned char);
void AX_Flash();
void AX_Go_To(unsigned char, unsigned short int, unsigned short int);
void AX_Test(void);
void AX_Move_Leg(unsigned char, unsigned char, unsigned short int, double, double, double);
unsigned char AX_Leg_Moving(unsigned char);
void AX_Leg_Angles(double, double, double);
unsigned char AX_Check_Angle_Limits();
void AX_Starting_Position(unsigned char);
void AX_Ping(unsigned char);
void AX_TX_I(unsigned char, const unsigned char, unsigned char *);
unsigned short int AX_TX_IS(unsigned char, const unsigned char, unsigned char *);
void AX_RX_Status(void);
struct AX_PARAMS AX_Read_Params(unsigned char);

//Defines
#ifdef MODE_DEV
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
#define AX_STATUS_RETURN_LEVEL 1			//
#define AX_LOCK 0							//
#define AX_COXA_HOME_POSITION 512			//150 Deg
#define AX_FEMUR_HOME_POSITION 470			//140 Deg
#define AX_TIBIA_HOME_POSITION 240			//90 Deg
#define COXA_LENGTH 43.0					//Coxa->Femur (mm)
#define FEMUR_LENGTH 120.0					//Femur->Tibia (mm)
#define TIBIA_LENGTH 166.0					//Tibia->End of foot (mm)
#define COXA_POLAR_ANGLE 150.0				//
#define FEMUR_POLAR_ANGLE 255.0				//Fix the Femur Polar Angle since the part is bended
#define TIBIA_POLAR_ANGLE 225.0				//Fix the Tibia Polar Angle

//Global variables
const unsigned char AX_PING = 1;
const unsigned char AX_READ = 2;
unsigned char AX_READ_BAUD_RATE[] = {2,4,1};
unsigned char AX_READ_DELAY_TIME[] = {2,5,1};
unsigned char AX_READ_CW_LIMIT[] = {2,6,2};
unsigned char AX_READ_CCW_LIMIT[] = {2,8,2};
unsigned char AX_READ_HIGHEST_LIMIT_TEMPERATURE[] = {2,11,1};
unsigned char AX_READ_LOWEST_LIMIT_VOLTAGE[] = {2,12,1};
unsigned char AX_READ_HIGHEST_LIMIT_VOLTAGE[] = {2,13,1};
unsigned char AX_READ_MAX_TORQUE[] = {2,14,2};
unsigned char AX_READ_STATUS_RETURN_LEVEL[] = {2,16,1};
unsigned char AX_READ_PRESENT_POSITION[] = {2,36,2};
unsigned char AX_READ_PRESENT_SPEED[] = {2,38,2};
unsigned char AX_READ_PRESENT_LOAD[] = {2,40,2};
unsigned char AX_READ_PRESENT_VOLTAGE[] = {2,42,1};
unsigned char AX_READ_PRESENT_TEMPERATURE[] = {2,43,1};
unsigned char AX_READ_MOVING[] = {2,46,1};
unsigned char AX_READ_LOCK[] = {2,47,1};

const unsigned char AX_WRITE = 3;
const unsigned char AX_SYNC_WRITE = 131;
unsigned char AX_WRITE_ID[] = {2,3,1};
unsigned char AX_WRITE_BAUD_RATE_1M[] = {2,4,1};
unsigned char AX_WRITE_BAUD_RATE_500K[] = {2,4,3};
unsigned char AX_WRITE_BAUD_RATE_250K[] = {2,4,7};
unsigned char AX_WRITE_DELAY_TIME[] = {2,5,250};
unsigned char AX_WRITE_CW_LIMIT[] = {3,6,0,0};
unsigned char AX_WRITE_CCW_LIMIT[] = {3,8,255,3};
unsigned char AX_WRITE_HIGHEST_LIMIT_TEMPERATURE[] = {2,11,70};
unsigned char AX_WRITE_LOWEST_LIMIT_VOLTAGE[] = {2,12,60};
unsigned char AX_WRITE_HIGHEST_LIMIT_VOLTAGE[] = {2,13,140};
unsigned char AX_WRITE_MAX_TORQUE[] = {3,14,255,3};
unsigned char AX_WRITE_STATUS_RETURN_LEVEL[] = {2,16,2};
unsigned char AX_WRITE_TORQUE_EN[] = {2,24,1};
unsigned char AX_WRITE_LOCK[] = {2,47,0};
unsigned char AX_WRITE_GOAL_POSITION[] = {5,30,0,2,0,2};
unsigned char AX_WRITE_GOAL_POSITION_HOME[] = {5,30,0,2,0,2};
unsigned char AX_WRITE_LEG_GOAL_POSITION[] = {17,30,4,0,0,2,0,2,0,0,2,0,2,0,0,2,0,2};

unsigned char AX_SERVO_ID[6][3] = {{1,2,3},{4,5,6},{7,8,9},{10,11,12},{13,14,15},{16,17,18}};

//Declare structs in .h in order to make them global
POINT_3D STARTING_POSITION = {0};
AX_LEG_ANGLES LEG_ANGLES = {0};
AX_PARAMS AX_F1R = {0};
AX_PARAMS AX_F2R = {0};
AX_PARAMS AX_F3R = {0};
AX_ERRORS ERRORS = {0};

/* XC8 does not support designated initializers like C99 does. Use comments instead. */
//Set it to "const" in order to store in program memory

#endif	//DYNAMIXEL_H