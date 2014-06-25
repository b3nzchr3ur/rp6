// Uncommented Version of RP6Base_Stopwatches.c
// written by Dominik S. Herwald
// ------------------------------------------------------------------------------------------
// Another uncommented version - this time of RP6Base_ACS.c.
// ------------------------------------------------------------------------------------------

#include "RP6RobotBaseLib.h" 

void acsStateChanged(void)
{
	writeString_P("ACS state changed   L: ");
	
	if(obstacle_left)
		writeChar('o');
	else
		writeChar(' ');
		
	writeString_P(" | R: ");
	
	if(obstacle_right)
		writeChar('o');
	else
		writeChar(' ');	
		
	if(obstacle_right && obstacle_left)
		writeString_P("   MIDDLE!");
		
	writeChar('\n');
	
	if(obstacle_left && obstacle_right)
		statusLEDs.byte = 0b100100;
	else
		statusLEDs.byte = 0b000000;
	statusLEDs.LED5 = obstacle_left;
	statusLEDs.LED4 = (!obstacle_left);
	statusLEDs.LED2 = obstacle_right;
	statusLEDs.LED1 = (!obstacle_right);
	
	updateStatusLEDs();	
}

void bumpersStateChanged(void)
{
	writeString_P("Bumpers state changed!   BPL: ");
	
	if(bumper_left)
		writeChar('o');
	else
		writeChar(' ');
		
	writeString_P(" | BPR: ");
	
	if(bumper_right)
		writeChar('o');
	else
		writeChar(' ');
		
	writeChar('\n');
}

int main(void)
{
	initRobotBase();
	
    writeString_P("\nRP6 ACS - Testprogram\n");
	writeString_P("_____________________\n\n");

	setLEDs(0b111111);
	mSleep(1000);
	setLEDs(0b001001);

	ACS_setStateChangedHandler(acsStateChanged);
	
	BUMPERS_setStateChangedHandler(bumpersStateChanged);

	powerON(); 
	setACSPwrMed();

	while(true) 
	{		
		task_RP6System();
	}
	return 0;
}
