//--------------------------------------------------------------
//	File:			Dynamixel.h
//	Author:			Vadim Melnicuk
//	Company:		Birmingham City University
//	Date:			11/11/2013
//	Description:	
//--------------------------------------------------------------

#ifndef DYNAMIXEL_H
#define	DYNAMIXEL_H

//Function prototypes
void AX_Init(void);
void AX_Init_Legs(void);
void AX_Ping(unsigned char);
void AX_TX_Instruction(unsigned char, const unsigned char, unsigned char *);
void AX_TX_Instruction_With_Status(unsigned char, const unsigned char, unsigned char *);
void AX_RX_Status(void);
void AX_Go_To(unsigned char, unsigned short int, unsigned short int);
unsigned char AX_Is_Moving(unsigned char);
unsigned short int AX_Read_Present_Position(unsigned char);
unsigned short int AX_Read_Goal_Position(unsigned char);
unsigned short int AX_Read_Present_Speed(unsigned char);
unsigned short int AX_Read_Present_Load(unsigned char);
unsigned char AX_Read_Present_Voltage(unsigned char);
unsigned char AX_Read_Present_Temperature(unsigned char);
unsigned char AX_Read_Moving(unsigned char);
unsigned char AX_Read_Lock(unsigned char);
struct AX_PARAMS AX_Read_Params(unsigned char);
void AX_Flash(void);
void AX_Test(void);
void Reach(double, double, double);

//Structures
struct POINT_2D {
	double X;
	double Y;
};
typedef struct POINT_2D POINT_2D;

struct POINT_3D {
	double X;
	double Y;
	double Z;
};
typedef struct POINT_3D POINT_3D;

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
	unsigned short int GOAL_POSITION;
	unsigned short int PRESENT_SPEED;
	unsigned short int PRESENT_LOAD;
	unsigned char PRESENT_VOLTAGE;
	unsigned char PRESENT_TEMPERATURE;
	unsigned char MOVING;
	unsigned char LOCK;
};
typedef struct AX_PARAMS AX_PARAMS;	//Declaration of type

struct AX_DEFAULT_PARAMS {
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
	unsigned char LOCK;
	unsigned short int HOME_POSITION;
};
typedef struct AX_DEFAULT_PARAMS AX_DEFAULT_PARAMS;

struct AX_FLASH_PARAMS {
	unsigned char ID;
	unsigned char BAUD_RATE;
	unsigned char DELAY_TIME;
	unsigned char STATUS_RETURN_LEVEL;
};
typedef struct AX_FLASH_PARAMS AX_FLASH_PARAMS;

/*
The BANK SIZE is limited to hold a variable with the maximum size of 256 bytes
The global struct of size of 324 bytes could not be initialised, because of that
All 6 Legs will ne initialised seperatly without holding them in one global structure
representing whole Hexapod.
*/

//Defines
#ifdef MODE_DEV
	#define AX_BAUD_RATE 500000
#endif
#ifdef MODE_FLASH
	#define AX_BAUD_RATE 1000000
#endif

#define COXA_LENGTH 37.0		//Coxa -> Femur (mm)
#define FEMUR_LENGTH 120.0		//Femur -> Tibia (mm)
#define TIBIA_LENGTH 166.0		//Tibia -> End of foot (mm)

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
unsigned char AX_READ_TORQUE_EN[] = {2,24,1};
unsigned char AX_READ_PRESENT_POSITION[] = {2,36,2};
unsigned char AX_READ_PRESENT_SPEED[] = {2,38,2};
unsigned char AX_READ_PRESENT_LOAD[] = {2,40,2};
unsigned char AX_READ_PRESENT_VOLTAGE[] = {2,42,1};
unsigned char AX_READ_PRESENT_TEMPERATURE[] = {2,43,1};
unsigned char AX_READ_MOVING[] = {2,46,1};
unsigned char AX_READ_LOCK[] = {2,47,1};

const unsigned char AX_WRITE = 3;
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

//Declare structs in .h in order to make them global
POINT_3D TARGET_POINT = {0};
AX_PARAMS AX_M1R = {0};
AX_PARAMS AX_M2R = {0};
AX_PARAMS AX_M3R = {0};

/* XC8 does not support designated initializers like C99 does. Use comments instead. */

const AX_DEFAULT_PARAMS AX_M1R_D = {	//Set it to "const" in order to store in program memory
	1,			//ID
	3,			//BAUD_RATE - 500Kbps
	250,		//DELAY_TYME - 250us
	207,		//CW_LIMIT - 60 Degrees
	827,		//CCW_LIMIT - 240 Degrees
	70,			//HIGHEST_LIMIT_TEMPERATURE
	60,			//LOWEST_LIMIT_VOLTAGE
	140,		//HIGHEST_LIMIT_VOLTAGE
	1023,		//MAX_TORQUE
	1,			//STATUS_RETURN_LEVEL
	0,			//LOCK
	512,		//HOME POSITION
};

const AX_DEFAULT_PARAMS AX_M2R_D = {
	2,			//ID
	3,			//BAUD_RATE - 500Kbps
	250,		//DELAY_TYME - 250us
	483,		//CW_LIMIT - 140 Degrees
	1023,		//CCW_LIMIT - 300 Degrees
	70,			//HIGHEST_LIMIT_TEMPERATURE
	60,			//LOWEST_LIMIT_VOLTAGE
	140,		//HIGHEST_LIMIT_VOLTAGE
	1023,		//MAX_TORQUE
	1,			//STATUS_RETURN_LEVEL
	0,			//LOCK
	483,		//HOME POSITION
};

const AX_DEFAULT_PARAMS AX_M3R_D = {
	3,			//ID
	3,			//BAUD_RATE - 500Kbps
	250,		//DELAY_TYME - 250us
	310,		//CW_LIMIT - 90 Degrees
	690,		//CCW_LIMIT - 200 Degrees
	70,			//HIGHEST_LIMIT_TEMPERATURE
	60,			//LOWEST_LIMIT_VOLTAGE
	140,		//HIGHEST_LIMIT_VOLTAGE
	1023,		//MAX_TORQUE
	1,			//STATUS_RETURN_LEVEL
	0,			//LOCK
	310,		//HOME POSITION
};

const AX_FLASH_PARAMS AX_FLASH_D = {	//Struct to hold default target flashing setting
	3,			//ID
	3,			//BAUD_RATE - 500Kbps
	250,		//DELAY_TYME - 250us
	1,			//STATUS_RETURN_LEVEL
};

#endif	//DYNAMIXEL_H