//--------------------------------------------------------------
//	File:			Main.c
//	Author:			Vadim Melnicuk
//	Company:		Birmingham City University
//	Date:			11/11/2013
//	Description:	
//--------------------------------------------------------------

//Header files
#include "Config.h"

//--------------------------------------------------------------

void main(void)
{
	Init();							//Initialise Microcontroller
	__delay_ms(5);

	#ifdef MODE_DEV
		AX_Init();
	#endif
	#ifdef MODE_FLASH
		AX_Flash();
	#endif
	
	while(1){
		PORTAbits.RA0 = ~PORTAbits.RA0;
	}
}