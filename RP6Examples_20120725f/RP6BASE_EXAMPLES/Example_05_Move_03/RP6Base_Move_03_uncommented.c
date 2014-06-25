// Uncommented Version of RP6Base_Move_03.c
// written by Dominik S. Herwald
// ------------------------------------------------------------------------------------------

#include "RP6RobotBaseLib.h" 	

#define MOVE_SPEED 50
#define TURN_SPEED 40

int main(void)
{
	initRobotBase(); 
	setLEDs(0b111111);
	mSleep(2500);
	
	powerON();

	uint8_t move_count = 0;
	uint8_t turn_direction = LEFT; 
	
	while(true) 
	{		
		setLEDs(0b100100); 
		
		move(MOVE_SPEED, FWD, DIST_MM(300), BLOCKING);
		
		statusLEDs.LED6 = (turn_direction==LEFT);
		statusLEDs.LED3 = (turn_direction==RIGHT);
		updateStatusLEDs();
		
		rotate(TURN_SPEED, turn_direction, 90, BLOCKING);

		if(move_count >= 3)
		{
			setLEDs(0b101101); 
			mSleep(1000);
		
			statusLEDs.LED6 = (turn_direction==LEFT);
			statusLEDs.LED3 = (turn_direction==RIGHT);
			updateStatusLEDs();
			
			rotate(TURN_SPEED, turn_direction, 90, BLOCKING);
			
			if(turn_direction == LEFT)
				turn_direction = RIGHT;
			else
				turn_direction = LEFT;
				
			move_count = 0;
		}
		else
			move_count++; 
	}
	return 0;
}
