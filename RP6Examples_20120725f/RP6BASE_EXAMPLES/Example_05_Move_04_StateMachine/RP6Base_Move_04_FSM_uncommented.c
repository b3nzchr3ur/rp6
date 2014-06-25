// Uncommented Version of RP6Base_Move_04_FSM.c
// written by Dominik S. Herwald
// ------------------------------------------------------------------------------------------

#include "RP6RobotBaseLib.h" 	

#define STATE_START				0
#define STATE_WAIT_FOR_BUMPERS	1
#define STATE_LEFT_PRESSED		2
#define STATE_LEFT_RELEASED 	3
#define STATE_LEFT_PRESSED_2	4
#define STATE_RIGHT_PRESSED		5
#define STATE_RIGHT_RELEASED	6
#define STATE_RIGHT_PRESSED_2	7

uint8_t bump_state = STATE_START;

void bumpers_finiteStateMachine(void)
{
	switch(bump_state)
	{
		case STATE_START: 
			writeString_P("\nHey you! Yes you over there! Hit a bumper TWO TIMES please!\n\n");
			bump_state = STATE_WAIT_FOR_BUMPERS; 
		break;
		case STATE_WAIT_FOR_BUMPERS:
			if(bumper_left)
			{
				writeString_P("Left Bumper is pressed down!\n");
				writeString_P("\t\tSTATE_START -> STATE_LEFT_PRESSED\n");
				setLEDs(0b100001);
				bump_state = STATE_LEFT_PRESSED;
			}
			else if(bumper_right)
			{ 
				writeString_P("Right Bumper is pressed down!\n");
				writeString_P("\t\tSTATE_START -> STATE_RIGHT_PRESSED\n");
				setLEDs(0b001100);
				bump_state = STATE_RIGHT_PRESSED;
			}
		break;
		
		case STATE_LEFT_PRESSED:
			if(!bumper_left)
			{
				writeString_P("Left Bumper has been released! Now press it down once again!\n");
				writeString_P("\t\tSTATE_LEFT_PRESSED -> STATE_LEFT_RELEASED\n");
				setLEDs(0b011001);
				bump_state = STATE_LEFT_RELEASED;
			}
		break;
		case STATE_LEFT_RELEASED:
			if(bumper_left) 
			{
				writeString_P("Left Bumper has been pressed down a second time!\n");
				writeString_P("\t\tSTATE_LEFT_RELEASED -> STATE_LEFT_PRESSED_2\n");
				setLEDs(0b110001);
				bump_state = STATE_LEFT_PRESSED_2; 
			}
		break;
		case STATE_LEFT_PRESSED_2:
			if(!bumper_left)
			{
				writeString_P("Left Bumper has been released a second time!\n");
				writeString_P("We go back to the start now!\n");
				writeString_P("\t\tSTATE_LEFT_PRESSED_2 -> STATE_START\n");
				setLEDs(0b001001);
				bump_state = STATE_START;
			}
		break;
		
		case STATE_RIGHT_PRESSED:
			if(!bumper_right) 
			{
				writeString_P("Right Bumper has been released! Now press it down once again!\n");
				writeString_P("\t\tSTATE_RIGHT_PRESSED -> STATE_RIGHT_RELEASED\n");
				setLEDs(0b001011);
				bump_state = STATE_RIGHT_RELEASED;
			}
		break;
		case STATE_RIGHT_RELEASED:
			if(bumper_right) 
			{
				writeString_P("Right Bumper has been pressed down a second time!\n");
				writeString_P("\t\tSTATE_RIGHT_RELEASED -> STATE_RIGHT_PRESSED_2\n");
				setLEDs(0b100011);
				bump_state = STATE_RIGHT_PRESSED_2; 
			}
		break;
		case STATE_RIGHT_PRESSED_2:
			if(!bumper_right)  
			{
				writeString_P("Right Bumper has been released a second time!\n");
				writeString_P("We go back to the start now!\n");
				writeString_P("\t\tSTATE_RIGHT_PRESSED_2 -> STATE_START\n");
				setLEDs(0b001001);
				bump_state = STATE_START; 
			}
		break;
	}
}

int main(void)
{
	initRobotBase(); 
	setLEDs(0b111111);
	mSleep(1000);
	setLEDs(0b001001); 
	writeString_P("\n\nFinite State Machine DEMO\n");
	writeString_P("_________________________\n\n");
	
	while(true) 
	{		
		bumpers_finiteStateMachine();
		task_RP6System(); 
	}
	return 0;
}
