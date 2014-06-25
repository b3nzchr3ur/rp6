// Uncommented Version of RP6Base_SerialInterface_01.c
// written by Dominik S. Herwald
// ------------------------------------------------------------------------------------------
// 
// Again just like for RP6Base_LEDs.c, this is the uncommented version of 
// "RP6Base_SerialInterface_01.c" to show that the program looks a lot 
// shorter without comments.
//
// Remember:
// If you want to compile this uncommented program - you need to change the makefile or
// rename this file - or better copy it to a new folder together with the makefile
// and batch files, create a new project etc....
//
// ------------------------------------------------------------------------------------------

#include "RP6RobotBaseLib.h" 	

int main(void)
{
	initRobotBase();

	setLEDs(0b111111);
	mSleep(500);
	setLEDs(0b000000);

	writeString_P("\nJust a simple counter program\n\n");
	
	uint16_t counter = 0;

	while(true)
	{
		timer = 0; 
		if(counter < 100) 
		{
			writeString_P("Counter: ");
			writeInteger(counter, BIN);
			writeString_P("(BIN) | ");
			writeInteger(counter, OCT);
			writeString_P("(OCT) | ");
			writeInteger(counter, DEC);
			writeString_P("(DEC) | ");
			writeInteger(counter, HEX);
			writeString_P("(HEX) ");
		}
		else
		{
			writeString_P("Counter L: ");
			writeIntegerLength(counter, BIN, 16);  
			writeString_P("(BIN) | ");
			writeIntegerLength(counter, OCT, 6);
			writeString_P("(OCT) | ");
			writeIntegerLength(counter, DEC, 6);
			writeString_P("(DEC) | ");
			writeIntegerLength(counter, HEX, 4);
			writeString_P("(HEX) ");
		}
		
		writeChar(' ');
		writeInteger(timer,DEC); 
		writeString(" *100us");
		writeChar('\n');
		
		counter++;  
		mSleep(100);
	}
	return 0;
}
