// Uncommented Version of RP6Base_Move_04_FSM2.c!
// written by Dominik S. Herwald
// ------------------------------------------------------------------------------------------

#include "RP6RobotBaseLib.h" 	

#define STATE_START						0
#define STATE_WAIT_FOR_BUMPER_HIT_LEFT	1
#define STATE_MOVE_BACKWARDS			2
#define STATE_WAIT_FOR_BUMPER_HIT_RIGHT	3
#define STATE_MOVE_FORWARDS				4

uint8_t move_state = STATE_START;

void move_stateMachine(void)
{
	switch(move_state)
	{
		case STATE_START:
			setLEDs(0b010000); 
			move_state = STATE_WAIT_FOR_BUMPER_HIT_LEFT;
		break;
		
		case STATE_WAIT_FOR_BUMPER_HIT_LEFT:
			if(getStopwatch1() > 500)
			{
				statusLEDs.LED5 = !statusLEDs.LED5;
				updateStatusLEDs();
				setStopwatch1(0);
			}
			if(bumper_left)
			{
				setLEDs(0b011001); 
				move(50, BWD, DIST_MM(150), NON_BLOCKING);
				move_state = STATE_MOVE_BACKWARDS;
			}
		break;
		case STATE_MOVE_BACKWARDS:
			if(isMovementComplete())
			{
				setLEDs(0b000010); 
				move_state = STATE_WAIT_FOR_BUMPER_HIT_RIGHT;
			}
		break;
			
		case STATE_WAIT_FOR_BUMPER_HIT_RIGHT:
			if(getStopwatch1() > 500)
			{
				statusLEDs.LED2 = !statusLEDs.LED2;
				updateStatusLEDs();
				setStopwatch1(0);
			}
			if(bumper_right)
			{
				setLEDs(0b100110); 
				move(50, FWD, DIST_MM(150), NON_BLOCKING);
				move_state = STATE_MOVE_FORWARDS;
			}
		break;
		case STATE_MOVE_FORWARDS:
			if(isMovementComplete())
				move_state = STATE_START;
		break;
	}
}

int main(void)
{
	initRobotBase(); 
	setLEDs(0b111111);
	mSleep(1000);
	
	powerON();
	
	startStopwatch1();
	
	while(true) 
	{		
		move_stateMachine();
		task_RP6System(); 
	}
	return 0;
}
