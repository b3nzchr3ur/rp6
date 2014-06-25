// Uncommented Version of RP6Base_Move_01.c
// written by Dominik S. Herwald
// ------------------------------------------------------------------------------------------

#include "RP6RobotBaseLib.h" 

void bumpersStateChanged(void)
{
	if(bumper_left || bumper_right) 
	{
		moveAtSpeed(0,0);  
		setLEDs(0b010000);
		startStopwatch1();
	}
}

void blink(void)
{
	if(getStopwatch1() > 500)
	{
		statusLEDs.LED2 = !statusLEDs.LED2;
		statusLEDs.LED5 = !statusLEDs.LED5;
		updateStatusLEDs();
		setStopwatch1(0);
	}
}

int main(void)
{
	initRobotBase();
	setLEDs(0b111111);
	mSleep(1500);
	setLEDs(0b100001);

	BUMPERS_setStateChangedHandler(bumpersStateChanged);
	
	powerON(); 

 	changeDirection(FWD);
	moveAtSpeed(80,30);   

	while(true) 
	{
		blink();
		task_RP6System();
	}
	return 0;
}
