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
	Init();
	
	#ifdef MODE_DEV
		AX_Init();
	#endif
	#ifdef MODE_FLASH
		AX_Flash();
	#endif
	
	while(1){
		
	}
}