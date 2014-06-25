/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - ROBOT BASE EXAMPLES
 * ****************************************************************************
 * Example: Stopwatches Example
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 * A small demo program, to show how to use the stopwatches
 * feature of the RP6Library for timing purposes.
 *
 * Just to show that we can do several things "in parallel", we have four 
 * "tasks" here. "task" is really ment as task, this is __NOT__ multitasking!
 * Not at all. 
 * --> We are not really performing these things in parallel, but it looks 
 * like this for someone looking at the mikrocontroller from the outside. 
 *
 * These four tasks can do rather complex things - not only these simple
 * things like in this program. This will be shown later in the more complex
 * example programs.
 *
 * You can understand better what this program does when you just let it run 
 * and watch the LEDs and the text output in the terminal!
 *
 * There are eight stopwatches in total, which you can use for whatever you 
 * want. You can even implement more stopwatches with one of those stopwatches
 * e.g. with 50ms resolution for other things you need to do.   
 * Please keep in mind that this is no accurate timing! It depends on how many 
 * stopwatches you use and how complex the tasks you need to perform are and
 * what else your program does. 
 * You should not use blocking delays like mSleep when you use stopwatches! 
 * Short delays e.g. sleep(50) (5ms) are usually OK but you should not use 
 * it too often.
 * As you will see, there are other reasons not to use mSleep and sleep
 * functions. Also the ACS, the motion control and bumper tasks in the 
 * RP6Library work similar to the stopwatches.
 *
 * ############################################################################
 * The Robot does NOT move in this example! You can simply put it on a table
 * next to your PC and you should connect it to the PC via the USB Interface!
 * ############################################################################
 * ****************************************************************************
 */

/*****************************************************************************/
// Includes:

#include "RP6RobotBaseLib.h" // Always needs to be included!

/*****************************************************************************/

/**
 * A small running light that uses Stopwatch 1 for timing.
 * The running light is updated every 100ms (= refresh frequency of 10Hz)
 */
void task_LEDs(void)
{
	static uint16_t runningLight = 1; 
	if(getStopwatch1() > 100) // have we reached AT LEAST 100ms = 0.1s?
	{
		// Update chaselight:
		setLEDs(runningLight); 	
		runningLight <<= 1; 
		if(runningLight > 32) 
			runningLight = 1;
			
		 // Don't forget to reset the stopwatch to 0 again:
		setStopwatch1(0);   
	}
}

/**
 * A simple counter that outputs it's value on the
 * Serialport each time it is incremented. 
 * It is incremented every 400ms.
 */
void task_counter1(void)
{
	static uint8_t counter;
	if(getStopwatch2() > 400) // 400ms = 0.4s
	{
		writeString_P("CNT1 : ");
		writeInteger(counter, DEC);
		writeChar('\n');
		counter++;
		setStopwatch2(0); // Reset stopwatch
	}
}

/**
 * A second counter, with different interval (0.8s) and
 * binary output format. 
 */
void task_counter2(void)
{
	static uint8_t counter2;
	if(getStopwatch3() > 800) // 800ms = 0.8s
	{
		writeString_P("\t  CNT2 : ");
		writeInteger(counter2, BIN);
		writeChar('\n');
		counter2++;
		setStopwatch3(0); // Reset stopwatch
	}
}

/**
 * A third counter, with different interval (1.2s) and
 * hexadecimal output format. 
 */
void task_counter3(void)
{
	static uint8_t counter2;
	if(getStopwatch4() > 1200) // 1200ms = 1.2s
	{
		writeString_P("\t\t        CNT3 : ");
		writeIntegerLength(counter2, HEX, 2);
		writeChar('\n');
		counter2++;
		setStopwatch4(0); // Reset stopwatch
	}
}

/*****************************************************************************/
// Main - The program starts here:

int main(void)
{
	initRobotBase(); // Always call this first! The Processor will not work
					 // correctly otherwise. 
	
    writeString_P("\nRP6 Stopwatch Demo Program\n");
	writeString_P("__________________________\n\n");

	// Start all used Stopwatches:
	startStopwatch1();
	startStopwatch2();
	startStopwatch3();
	startStopwatch4();

	// Set a stopwatch to a specific initial value:
	setStopwatch2(1600);
	
	// Main loop
	while(true) 
	{
		// Here we call the four "tasks" we have.
		// You should poll each stopwatch
		// in it's own function like it is done here, this keeps the sourcecode 
		// a bit more tidy. (In the RP6 Manual you can find an example
		// which has this within the main method)
		task_LEDs();
		task_counter1();
		task_counter2();
		task_counter3();
	}
	return 0;
}
