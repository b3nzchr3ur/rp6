// Uncommented Version of RP6Base_Stopwatches.c
// written by Dominik S. Herwald
// ------------------------------------------------------------------------------------------
// This is the uncommented version of "RP6Base_Stopwatches.c".
// ------------------------------------------------------------------------------------------

#include "RP6RobotBaseLib.h"

void task_LEDs(void)
{
	static uint16_t testLEDs = 1;
	if(getStopwatch1() > 100)
	{
		setLEDs(testLEDs); 	
		testLEDs <<= 1; 
		if(testLEDs > 32) 
			testLEDs = 1;
		setStopwatch1(0);
	}
}

void task_counter(void)
{
	static uint8_t counter;
	if(getStopwatch2() > 400)
	{
		writeString_P("CNT:");
		writeInteger(counter, DEC);
		writeChar('\n');
		counter++;
		setStopwatch2(0);
	}
}

void task_counter2(void)
{
	static uint8_t counter2;
	if(getStopwatch3() > 800)
	{
		writeString_P("        CNT 2: ");
		writeInteger(counter2, BIN);
		writeChar('\n');
		counter2++;
		setStopwatch3(0);
	}
}

void task_counter3(void)
{
	static uint8_t counter2;
	if(getStopwatch4() > 1200)
	{
		writeString_P("\t\t        CNT3 : ");
		writeIntegerLength(counter2, HEX, 2);
		writeChar('\n');
		counter2++;
		setStopwatch4(0);
	}
}

int main(void)
{
	initRobotBase();
	
    writeString_P("\nRP6 Stopwatch Demo Program\n");
	writeString_P("__________________________\n\n");

	startStopwatch1();
	startStopwatch2();
	startStopwatch3();
	startStopwatch4();
	setStopwatch2(2000);
	
	while(true) 
	{
		task_LEDs();
		task_counter();
		task_counter2();
		task_counter3();
	}
	return 0;
}
