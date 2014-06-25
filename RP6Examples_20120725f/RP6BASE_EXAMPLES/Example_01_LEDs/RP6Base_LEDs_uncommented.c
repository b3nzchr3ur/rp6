// Uncommented Version of RP6Base_LEDs.c
// ------------------------------------------------------------------------------------------
// Just to show how short this program really is - here it is again (s. also RP6Base_LEDs.c)
// WITHOUT any comments (except for these few lines at the top).
//
// As you can see, the comments make this program *look* much larger than it is.
// They are neccessary to explain what the program does, but you don't need to add that much 
// comments to your own programs and you can leave them short like this one here! 
//
// Maybe this is better to understand for some of you.
// And of course it is easier to use the examples as templates for your own programs 
// like this.
//
// Remember:
// If you want to compile this uncommented program - you need to change the makefile or
// rename this file - or better copy it to a new folder together with the makefile
// and batch files, create a new project etc....
// ------------------------------------------------------------------------------------------

#include "RP6RobotBaseLib.h" 

int16_t main(void)
{
	initRobotBase(); 
	
	writeString_P("\n\n   _______________________\n");
	writeString_P("   \\| RP6  ROBOT SYSTEM |/\n");
	writeString_P("    \\_-_-_-_-_-_-_-_-_-_/\n\n");

	writeString_P("Hello World! My name is Robby!\n");
	writeString_P("Let's go! :)\n");

	setLEDs(0b111111); 
	mSleep(1000); 
	setLEDs(0b000000);
	mSleep(500);

	uint8_t runningLight = 1;
	
	while(true)
	{
		setLEDs(runningLight); 
		runningLight <<= 1; 
		if(runningLight > 32)
			runningLight = 1;
		mSleep(100); 
	}
	return 0; 
}
