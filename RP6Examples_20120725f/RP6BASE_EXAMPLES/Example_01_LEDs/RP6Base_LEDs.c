/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - ROBOT BASE EXAMPLES
 * ****************************************************************************
 * Example: LEDs and Serial Interface Example
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 * A typical "Hello World" program for the RP6. Writes text to the PC with the
 * Serial Interface and shows a running Light with the LEDs afterwards.
 *
 * You can watch the text output when RP6 is connected to the USB interface
 * with the Terminal in RP6Loader.
 * In the RP6Loader Software switch to the tab "Terminal" and use the
 * menu item RP6Loader->Start Target or whatever the text is for the 
 * language you selected. 
 * (or press strg+s on your keyboard)
 *
 * Also take a look at "RP6Base_LEDs_uncommented.c"! There you can see how
 * this file looks like without any comments. Much shorter and for some of you
 * maybe better to understand.
 *
 * ############################################################################
 * The Robot does NOT move in this example! You can simply put it on a table
 * next to your PC and you should connect it to the PC via the USB Interface!
 * ############################################################################
 * ****************************************************************************
 */

/*****************************************************************************/
// Includes:

#include "RP6RobotBaseLib.h" 	// The RP6 Robot Base Library.
								// Always needs to be included!
								
/*****************************************************************************/
// Main function - The program starts here:

int main(void)
{
	initRobotBase(); // Always call this first! The Processor will not work
					 // correctly otherwise.

	// ---------------------------------------
	// Write messages to the Serial Interface
	// (here it is a RP6 text logo):
	writeString_P("\n\n   _______________________\n");
	writeString_P("   \\| RP6  ROBOT SYSTEM |/\n");
	writeString_P("    \\_-_-_-_-_-_-_-_-_-_/\n\n");

	// Explanation of special chars:
	// '\n' = new line
	// '\\' = '\'
	// These are "escape sequences" for characters you can not
	// use directly within strings.

	// Write "Hello World" to the Serial Interface:
	writeString_P("Hello World! My name is Robby!\n");
	writeString_P("Let's go! :)\n");

	// ---------------------------------------
	// LEDs:
	setLEDs(0b111111); // Turn all LEDs on!

	// 0b111111 is a binary value and is the same as
	// 63 in the decimal system.
	// For this routine, the binary value is better to read, because each bit
	// represents one of the LEDs.
	// e.g. this:
	// setLEDs(0b000001); would set only LED1
	// setLEDs(0b000010); LED2
	// setLEDs(0b000100); LED3
	// setLEDs(0b101001); LED6, LED4, LED1 - and so on!

	mSleep(1000); // delay 1000ms = 1s
	setLEDs(0b000000); // All LEDs off!
	mSleep(500); // delay 500ms = 0.5s


	// ---------------------------------------

	uint8_t runningLight = 1; // This defines the local unsigned 8 bit variable "runningLight".
						      // It can be accessed everywhere _below_ in this function.
						      // And ONLY within this function!

	// ---------------------------------------
	// Main loop - the program will loop here forever!
	// In this program, it only runs a small LED chaselight.
	while(true)
	{
		// Here we do a small LED test:
		// ---------------------------------------

		setLEDs(runningLight); 	// Set status LEDs to the value of the variable
							// testLEDs.
							// In the first loop iteration it has the value 1,
							// and thus the StatusLED1 will be switched on.

		runningLight <<= 1; // shift the bits of "runningLight" one step to the left.
						// As there is only one bit set in this variable,
						// only one LED is on at the same time.
						// This results is a moving light dot like this:
						// 1: 0b000001
						// 2: 0b000010
						// 3: 0b000100
						// 4: 0b001000
						// 5: 0b010000
						// 6: 0b100000 
						//
						// In decimal format that would be the numbers:
						// 1, 2, 4, 8, 16, 32

		// When we have reached a value > 32 (32 means Status LED6 is on), 
		// we need to reset the value of runningLight to 1 to start again
		// from the beginning...
		// Instead of "32" we could also write "0b100000".
		if(runningLight > 32)
			runningLight = 1; 	// reset runningLight to 1 (StatusLED1) 

		// If we want to see the running Light, we need to
		// add a delay here - otherwise our human eyes would not see
		// the moving light dot: 
		mSleep(100); // delay 100ms = 0.1s 

		// ---------------------------------------

	}
	// End of main loop
	// ---------------------------------------

	// ---------------------------------------
	// The Program will NEVER get here!
	// (at least if you don't perform a "break" in the main loop, which
	// you should not do usually!)

	return 0; 
}
