/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - RP6 CONTROL M256 Examples
 * ****************************************************************************
 * Example: LEDs, LCD, Serial and WIFI Interface Example
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 * A more complex "Hello World" program for RP6 Control M256 than the one that
 * you should know from RP6 Robot Base. 
 * It writes text to the PC with the serial and WIFI interfaces, shows a 
 * running light with the LEDs, shows a moving Text on the LCD (if available).
 *
 * It shows how to use the Stopwatch feature of the RP6M256Lib, which
 * is nearly the same as with RP6Libary.
 *
 * You can watch the output in BOTH Terminals, Serial and Network/WiFi. 
 *
 *
 * ATTENTION:
 * This and all other Examples for RP6-M256 assume that you did run the other 
 * example programs from the RP6 Robot Base before, understood those 
 * examples (most of it at least) and that you have read the RP6-BASE and 
 * RP6-M256 manuals. Some things are repeated here, but not everything!
 *
 * ############################################################################
 * The Robot does NOT move in this example! You can simply put it on a table
 * next to your PC and you should connect it to the PC via the USB Interface!
 * You should also connect to it via WIFI.
 * ############################################################################
 * ****************************************************************************
 */

/*****************************************************************************/
// Includes:

#include "RP6M256Lib.h" 					// The RP6 Control Library. 
											// Always needs to be included!

/*****************************************************************************/

/**
 * Here we do a small LED test 
 * (a running light with direction changing)!
 * For timing we use Stopwatch #1. 
 */
void runningLight(void)
{
	static uint8_t runLight = 1; 
	static uint8_t dir;  // Moving direction for running light
	// The "static" keyword in front of these two local variables means that they 
	// are NOT cleared when leaving this function. Their content is kept and they
	// act just like global variables (but they are only accessible within 
	// this function)! You already saw this on RP6 Robot Base Examples... 
	
	// We want a 100ms delay between the calls, which results in 10Hz refresh rate of
	// the running light.
	// ATTENTION: DO NOT USE "==" to compare to the stopwatch values, 
	// use only ">" or "<" or ">=" because you could miss the point where the stopwatch 
	// has the exact value!
	if(getStopwatch1() > 100) // 100ms
	{
		// Set status LEDs to the value of the variable testLEDs:
		setLEDs(runLight); 
	
		// Shift the LED bit left or right depending on direction:
		if(dir == 0)
			runLight <<= 1; 
		else
			runLight >>= 1;
			
		// Change the direction if we reached one of the two outer LEDs:
		if(runLight > 7 ) 
			dir = 1;			
		else if (runLight < 2 ) 
			dir = 0;
		
		// Reset Stopwatch1:
		setStopwatch1(0);
	}
}

/**
 * Two running LCD Texts with Stopwatch2 and 400ms refresh rate:
 */
void runLCDText(void)
{
	static uint8_t cnt = 0; // A counting variable for text position
	static uint8_t dir = 0; // Moving direction for LCD Text
	
	if(getStopwatch2() > 500) // 400ms
	{
		// Hint: This is just a simple example and you need to adjust 
		// some parameters if you want to change the moving text! 
		// Like it is now - both strings must have 5 characters!

		// Overwrite the two previous Strings:
		clearPosLCD(0, cnt, 5);
		clearPosLCD(1, (11 - cnt), 5);

		// Increase/Decrease the cursor position depending on
		// direction.
		if(dir == 0)
			cnt++;
		else
			cnt--;
		// Change the direction:
		if(cnt > 10)
			dir = 1;
		if(cnt < 1)
			dir = 0;
		
		// Write two Text Strings to the LCD...
		// ... the first starts at the first position on the LCD:
		setCursorPosLCD(0, cnt); // Sets Cursor position on the display
		writeStringLCD_P("HELLO"); // Writes a String from Flash to the LCD
		// ... and the second is on the second line and starts at the 
		// last position.
		setCursorPosLCD(1, (11 - cnt) ); 
		writeStringLCD_P("WORLD");
		
		// Reset Stopwatch #2:
		setStopwatch2(0);
		
	}
}

/**
 * This routine shows how to control the stopwatches.
 * It uses Stopwatch 3 to stop the other two stopwatches after some seconds, 
 * shows two text screens and continues operation afterwards.
 * 
 */
void takeABreakAfterSomeTime(void)
{
	static uint8_t putScreenOnceOnly; 
	// We use this static local variable as a flag to determine 
	// if the LCD text has already been written (if we would write it everytime 
	// this function gets called, we would get ugly artefacts on the LCD...)

	// Because we are cheking size with ">" (greater than) we need to 
	// use a if-else-if-... construct with biggest number on top!
	// This means we will FIRST get into the LAST if-condition (after 16s ...!)
	// When we reached 24 seconds the stopwatch gets cleared.
	if(getStopwatch3() > 24000) // 24s
	{
		clearLCD();
		startStopwatch1();
		startStopwatch2();
		startStopwatch4();
		setStopwatch3(0); // Reset stopwatch 3

	}
	else if(getStopwatch3() > 22000) // 22s
	{
		if(!putScreenOnceOnly) // Have we already written this to the LCD already?
		{
			// No - write the Text:
			showScreenLCD("OK - let's", "go on!");
			writeString_P_WIFI("\nGO ON!\n");
			putScreenOnceOnly = 1; // Remember that we have written it!
		}
	}
	else if(getStopwatch3() > 16000) // 16s
	{
		if(isStopwatch1Running()) // No need for a seperate flag here!
		{					      // We just check if we already stopped
			stopStopwatch1();	  // the Stopwatch #1 !
			stopStopwatch2();
			stopStopwatch4();
			showScreenLCD("I'll take a", "short break now!");
			writeString_P_WIFI("\nPause for some seconds... \n");
			putScreenOnceOnly = 0;

		}
	}
}


/*****************************************************************************/
// Main function - The program starts here:

int main(void)
{
	initRP6M256();    // Always call this first! The Processor will not work
					  // correctly otherwise. 

	initLCD(); // Initialize the LC-Display (LCD)
			   // Always call this before using the LCD!

	// Write some text messages to the UART - just like on RP6Base:
	writeString_P("\n\n   _______________________\n");
	writeString_P("   \\| RP6  ROBOT SYSTEM |/\n");
	writeString_P("    \\_-_-_-_-_-_-_-_-_-_/\n\n");
	
	writeString_P("Hello World from RP6 CONTROL M256 WIFI!\n"); 
	writeString_P("Let's move it!  :)\n"); 
	writeString_P("This message was sent via ***Serial Port***!\n"); 


	// Now write Text via the WIFI Interace:
	writeString_P_WIFI("\n\n   _______________________\n");
	writeString_P_WIFI("   \\| RP6  ROBOT SYSTEM |/\n");
	writeString_P_WIFI("    \\_-_-_-_-_-_-_-_-_-_/\n\n");
	// As you can see it is exactly the same as with the UART just with an added _WIFI
	// at the end. 
	
	writeString_P_WIFI("Hello World from RP6 CONTROL M256 WIFI!\n"); 
	writeString_P_WIFI("Let's move it!  :)\n"); 
	writeString_P_WIFI("This message was sent via ***WLAN***!\n"); 



	// Set the four Status LEDs:
	setLEDs(0b1111);
	mSleep(500);
	setLEDs(0b0000);
	
	
	// The following presents three LCD Screens 
	// with a short delay in between.
	// These routines clear the whole display and write the 
	// Text to the two LCD lines. If you just want to write
	// something on one line, you can use other functions
	// like writeStringLCD_P(STRING) (s. moving text example above)!
	showScreenLCD("################", "################");
	mSleep(1500);
	showScreenLCD("RP6v2-M256-WIFI ", "<<LC - DISPLAY>>");
	mSleep(1500); 
	showScreenLCD("Hello World", "Example Program");
	mSleep(1500);
	clearLCD(); // Clear the whole LCD Screen
	
	// Start the stopwatches:
	startStopwatch1();
	startStopwatch2();
	startStopwatch3();
	startStopwatch4();
	
	while(true) 
	{
		// Run the examples:
		runLCDText();
		runningLight();
		takeABreakAfterSomeTime();

		// You can add more stuff here if you like!
		// For example you could output some sensor values with the 
		// serial port every second, or play a melody every 10 seconds or 
		// make the stopwatches controllable with the buttons - or the
		// microphone or ...
		// (s. other example programs first for examples how to use these
		// hardware features)
		
	}
	return 0;
}
