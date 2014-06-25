// Uncommented Version of RP6Base_Move_02.c
// written by Dominik S. Herwald
// ------------------------------------------------------------------------------------------

#include "RP6RobotBaseLib.h" 	

int main(void)
{
	initRobotBase(); 
	setLEDs(0b111111);
	mSleep(2500);
	
	powerON();

	while(true) 
	{		
		setLEDs(0b100100); 
		move(60, FWD, DIST_MM(300), true);  
		setLEDs(0b100000); 
		rotate(50, LEFT, 180, true); 
		setLEDs(0b100100); 
		move(60, FWD, DIST_MM(300), true);
		setLEDs(0b000100); 
		rotate(50, RIGHT, 180, true); 
	}
	return 0;
}
