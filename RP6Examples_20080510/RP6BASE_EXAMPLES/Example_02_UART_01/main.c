/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - ROBOT BASE EXAMPLES
 * ****************************************************************************
 * Example: Serial Interface Example - writing Numbers
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 * This sample program shows how to use some of the integer output serial 
 * interface functions of the RP6Library (s. also "RP6uart.c"). 
 * The writeInteger function is very important because you can
 * output sensor values over the serial interface with it.
 * This is very useful to solve problems in your programs or optimize some
 * parameters.
 *
 * Here we only output a counter value every 100ms.
 *
 *
 * NEW: 
 * A small timer example was added in Version 1.3 of the RP6Libary to measure
 * the time required for the output. 
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
					 
	setLEDs(0b111111); // Turn all LEDs on
	mSleep(500);       // delay 500ms
	setLEDs(0b000000); // All LEDs off

	// Write a text message to the UART:
	writeString_P("\nJust a simple counter program\n\n");
	
	// Define a counting variable:
	uint16_t counter = 0;
	
	// ---------------------------------------
	// Main loop:
	while(true)
	{
		timer = 0; // initialize universal timer variable for measuring how much
				   // time the output required! (this is not directly related to this
				   // example - it was added as a small example for the new System
				   // timer added in Version 1.3 of the RP6Lib)
				   // The timer has a resolution of 100µs and runs all the time. 
		
		// Now we check what value the counter has, ...
		if(counter < 100) // ... is it smaller than 100?
		{
			// Yes --> output the Counter value with the "writeInteger"
			// function:
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
		else 			  // ... or is it greater than or equal to 100?
		{
			// No, the counter >= 100 --> use "writeIntegerLength" instead.
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
		writeInteger(timer,DEC); // display time (in 100µs) we needed for this output!
								 // You will directly see that the time increases when
								 // the output gets longer.
		writeString(" *100us");
		
		writeChar('\n'); // New Line
		
		counter++;    // Increment counter
		
		mSleep(100); // delay 200ms = 0.2s
	}
	// End of main loop!
	// ---------------------------------------

	return 0;
}
