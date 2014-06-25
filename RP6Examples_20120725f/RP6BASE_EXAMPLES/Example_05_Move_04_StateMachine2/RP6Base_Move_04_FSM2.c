/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - ROBOT BASE EXAMPLES
 * ****************************************************************************
 * Example: Movement 4 - Excursion: Finite State Machines Part 2
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 * 
 * Now we use a FSM together with some movement! 
 * In this example the Robot first waits for the Left Bumper switch to be hit.
 * It shows this with a blinking Status LED5.
 * When you hit the bumper, the Robot moves 15cm backwards. 
 * Then it waits for the right Bumper to be hit, which it shows with blinking
 * Status LED2. When you hit it, then it moves forwards 15cm and all this
 * is repeated.
 *
 * You will see in this example that you can also perform simple timing 
 * things within FSMs using the stopwatches. With this we implement the blinking
 * LEDs that show which Bumper has to be hit. 
 * Additionally, you can see how to use the "isMovementComplete" function to 
 * check if a movement has completed and a new one can be started.
 * 
 * ############################################################################
 * #+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+
 * 
 * ATTENTION: THE ROBOT MOVES AROUND IN THIS EXAMPLE! PLEASE PROVIDE ABOUT
 * 1m x 1m OR MORE FREE SPACE FOR THE ROBOT! 
 *
 * >>> DO NOT FORGET TO REMOVE THE FLAT CABLE CONNECTION TO THE USB INTERFACE
 * BEFORE YOU START THIS PROGRAM BY PRESSING THE START BUTTON ON THE ROBOT!
 *
 * #+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+
 * ############################################################################
 * ****************************************************************************
 */

/*****************************************************************************/
// Includes:

#include "RP6RobotBaseLib.h" 	

/*****************************************************************************/
// States:

#define STATE_START						0
#define STATE_WAIT_FOR_BUMPER_HIT_LEFT	1
#define STATE_MOVE_BACKWARDS			2
#define STATE_WAIT_FOR_BUMPER_HIT_RIGHT	3
#define STATE_MOVE_FORWARDS				4

// This is our state variable which stores the current state:
uint8_t move_state = STATE_START;

/*****************************************************************************/
// Main:

/**
 * The FSM task.
 */
void move_stateMachine(void)
{
	switch(move_state)
	{
// Start - set LEDs and directly move on to STATE_WAIT_FOR_BUMPER_HIT_LEFT!
		case STATE_START:
			setLEDs(0b010000); 
			move_state = STATE_WAIT_FOR_BUMPER_HIT_LEFT;
		break;
		
// Left Bumper + Move Backwards:		
		case STATE_WAIT_FOR_BUMPER_HIT_LEFT:
			// Blinking LED5:
			if(getStopwatch1() > 500)
			{
				statusLEDs.LED5 = !statusLEDs.LED5;
				updateStatusLEDs();
				setStopwatch1(0);
			}
			// Wait for left Bumper to be hit:
			if(bumper_left)
			{
				setLEDs(0b011001); 
				move(50, BWD, DIST_MM(150), NON_BLOCKING); // false --> NON-BLOCKING! 
				move_state = STATE_MOVE_BACKWARDS;
			}
		break;
		case STATE_MOVE_BACKWARDS:
			// Wait until the movement is complete:
			if(isMovementComplete())
			{
				setLEDs(0b000010); 
				move_state = STATE_WAIT_FOR_BUMPER_HIT_RIGHT;
			}
		break;
		
// Right Bumper + Move Forwards:		
		case STATE_WAIT_FOR_BUMPER_HIT_RIGHT:
			// Blinking LED2:
			if(getStopwatch1() > 500)
			{
				statusLEDs.LED2 = !statusLEDs.LED2;
				updateStatusLEDs();
				setStopwatch1(0);
			}
			// Wait for right Bumper to be hit:
			if(bumper_right)
			{
				setLEDs(0b100110); 
				move(50, FWD, DIST_MM(150), NON_BLOCKING); // false --> NON-BLOCKING! 
				move_state = STATE_MOVE_FORWARDS;
			}
		break;
		case STATE_MOVE_FORWARDS:
			// Wait until the movement is complete:
			if(isMovementComplete())
				move_state = STATE_START;
		break;
	}
}

/*****************************************************************************/
// Main:

int main(void)
{
	initRobotBase(); 
	setLEDs(0b111111);
	mSleep(1000);
	
	powerON();
	
	startStopwatch1();
	
	// Main loop
	while(true) 
	{		
		move_stateMachine(); // Call FSM task
		task_RP6System(); 
	}
	return 0;
}
