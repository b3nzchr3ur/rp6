/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - ROBOT BASE EXAMPLES
 * ****************************************************************************
 * Example: Movement 4 - Excursion: Finite State Machines
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 *
 * Before we move on to the examples with Subsumption Architechture, we need to
 * introduce an important piece of computing theory - the 
 * finite state machine (FSM).
 * We can not dive deep into this topic, but two small example programs 
 * (s. also the example project "Example_05_Move_04_StateMachine2" ) 
 * will show you the principle.
 *
 * For more infos in FSMs s. article in Wikipedia for example as a 
 * starting point to get more information: 
 * (English:) http://en.wikipedia.org/wiki/Finite_state_machine 
 * (German:)  http://de.wikipedia.org/wiki/Endlicher_Automat
 *
 * 
 * In this program the robot does not move - but finite state machines are
 * used in the following programs and thus you need to have a basic idea
 * of what's going on in those programs.
 *
 * We don't need to write long text here to explain what this program does.
 * Just let it run and press down the bumpers two times (slowly!) and watch 
 * the LEDs and the text output on the terminal!
 *
 * Also read the example programs section in the manual which gives some
 * additional details!
 *
 * ############################################################################
 * The Robot does NOT move in this example! You can simply put it on a table
 * next to your PC and you should connect it to the PC via the USB Interface!
 * ############################################################################
 * ****************************************************************************
 */

/*****************************************************************************/
// Includes:

#include "RP6RobotBaseLib.h" 	

/*****************************************************************************/
//Bumper finite State machine:

// The 8 possible states of our small and simple FSM are defined here:
#define STATE_START				0
#define STATE_WAIT_FOR_BUMPERS	1
#define STATE_LEFT_PRESSED		2
#define STATE_LEFT_RELEASED 	3
#define STATE_LEFT_PRESSED_2	4
#define STATE_RIGHT_PRESSED		5
#define STATE_RIGHT_RELEASED	6
#define STATE_RIGHT_PRESSED_2	7
// Yes - just simple integers - each number reprents one of our states! 
// We only define speaking names for them here so we can directly guess
// what the purpose of the state is.

// We store our current state in this variable:
uint8_t bump_state = STATE_START;

/**
 * This is the demo function that contains the FSM. The FSM is pretty simple and
 * checks for one bumper to be hit two times! Most of it is just text output.
 * This function has to be called frequently out of the main loop!
 */
void bumpers_finiteStateMachine(void)
{
	switch(bump_state) // Which state is active?
	{
// Start:
		case STATE_START: 
			writeString_P("\nHey you! Yes you over there! Hit a bumper TWO TIMES please!\n\n");
			bump_state = STATE_WAIT_FOR_BUMPERS; // Set state to STATE_WAIT_FOR_BUMPERS
		break;
		
// Wait for Bumpers to be pressed down:
		case STATE_WAIT_FOR_BUMPERS:
			if(bumper_left)  				// Left Bumper pressed down?
			{
				writeString_P("Left Bumper is pressed down!\n");
				writeString_P("\t\tSTATE_START -> STATE_LEFT_PRESSED\n");
				setLEDs(0b100001);
				bump_state = STATE_LEFT_PRESSED; // Set state to STATE_LEFT_PRESSED
			}
			else if(bumper_right)			// Right Bumper pressed down?
			{ 
				writeString_P("Right Bumper is pressed down!\n");
				writeString_P("\t\tSTATE_START -> STATE_RIGHT_PRESSED\n");
				setLEDs(0b001100);
				bump_state = STATE_RIGHT_PRESSED; // Set state to STATE_RIGHT_PRESSED
			}
		break;
		
// Left Bumper was pressed first:	
		case STATE_LEFT_PRESSED:
			if(!bumper_left)  				// Left Bumper released?
			{
				writeString_P("Left Bumper has been released! Now press it down once again!\n");
				writeString_P("\t\tSTATE_LEFT_PRESSED -> STATE_LEFT_RELEASED\n");
				setLEDs(0b011001);
				bump_state = STATE_LEFT_RELEASED;
			}
		break;
		case STATE_LEFT_RELEASED:
			if(bumper_left)  				// Left Bumper pressed a second time?
			{
				writeString_P("Left Bumper has been pressed down a second time!\n");
				writeString_P("\t\tSTATE_LEFT_RELEASED -> STATE_LEFT_PRESSED_2\n");
				setLEDs(0b110001);
				bump_state = STATE_LEFT_PRESSED_2; 
			}
		break;
		case STATE_LEFT_PRESSED_2:
			if(!bumper_left)  				// Left Bumper released a second time?
			{
				writeString_P("Left Bumper has been released a second time!\n");
				writeString_P("We go back to the start now!\n");
				writeString_P("\t\tSTATE_LEFT_PRESSED_2 -> STATE_START\n");
				setLEDs(0b001001);
				bump_state = STATE_START; // Go back to the start!
			}
		break;
		
// Right Bumper was pressed first:	
		case STATE_RIGHT_PRESSED:
			if(!bumper_right)  				// Right Bumper released?
			{
				writeString_P("Right Bumper has been released! Now press it down once again!\n");
				writeString_P("\t\tSTATE_RIGHT_PRESSED -> STATE_RIGHT_RELEASED\n");
				setLEDs(0b001011);
				bump_state = STATE_RIGHT_RELEASED;
			}
		break;
		case STATE_RIGHT_RELEASED:
			if(bumper_right)  				// Right Bumper pressed a second time?
			{
				writeString_P("Right Bumper has been pressed down a second time!\n");
				writeString_P("\t\tSTATE_RIGHT_RELEASED -> STATE_RIGHT_PRESSED_2\n");
				setLEDs(0b001110);
				bump_state = STATE_RIGHT_PRESSED_2; 
			}
		break;
		case STATE_RIGHT_PRESSED_2:
			if(!bumper_right)  				// Right Bumper released a second time?
			{
				writeString_P("Right Bumper has been released a second time!\n");
				writeString_P("We go back to the start now!\n");
				writeString_P("\t\tSTATE_RIGHT_PRESSED_2 -> STATE_START\n");
				setLEDs(0b001001);
				bump_state = STATE_START; // Go back to the start!
			}
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
	setLEDs(0b001001); 
	writeString_P("\n\nFinite State Machine DEMO\n");
	writeString_P("_________________________\n\n");
	
	// Main loop
	while(true) 
	{		
		bumpers_finiteStateMachine(); // Call FSM function
		task_RP6System(); 	// required -> updates bumper values every 50ms!
	}
	return 0;
}
