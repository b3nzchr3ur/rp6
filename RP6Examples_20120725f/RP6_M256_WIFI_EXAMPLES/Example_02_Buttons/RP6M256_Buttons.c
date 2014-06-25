/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - RP6 CONTROL M256 Examples
 * ****************************************************************************
 * Example: Buttons
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 * 
 * This Example shows how to use the keys (=Buttons SW1, SW2) on the RP6-M256.
 * We simply poll for pressed keys and show key number on the
 * LCD and WiFi interface. SW1 starts/stops a running light task. 
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

#include "RP6M256Lib.h" 		// The RP6 M256 Library. 
								// Always needs to be included!

/*****************************************************************************/


/**
 * Show a running light...
 */
void runningLight(void)
{
	static uint8_t runLight = 1; 
	static uint8_t dir;  
	
	if(getStopwatch1() > 100)
	{
		setLEDs(runLight); 
	
		if(dir == 0)
			runLight <<= 1; 
		else
			runLight >>= 1;
			
		if(runLight > 7 ) 
			dir = 1;			
		else if (runLight < 2 ) 
			dir = 0;
		
		setStopwatch1(0);
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

	// Write some text messages via WIFI
	writeString_P_WIFI("\n\nRP6M256 Button Example Program!\n"); 

	writeString_P("Check WIFI Terminal output!\n"); 

	setLEDs(0b1111);
	showScreenLCD("################", "################");
	mSleep(500);
	showScreenLCD("Buttons", "Example Program");
	mSleep(1000);
	setLEDs(0b0000); 

	writeString_P_WIFI("\nSTART!\n"); 

	
	showScreenLCD("Pressed Button", "--> ");
	while(true) 
	{
		// Check if a key is pressed:
		uint8_t key = getPressedKeyNumber(); 
		// This function returns a 0 if no key is pressed and
		// the key number 1 or 2 otherwise.

		if(key) // If a key is pressed... (key != 0)
		{
			// ... we update the values
			// on the LCD display:

			setCursorPosLCD(1, 4);
			writeIntegerLCD(key, DEC);
			// Please note that we did not output the whole screen again - we just 
			// overwrote the old numbers!
			
			// Output key and ADC Value on serial Interface:
			writeString_P_WIFI(" --> Pressed key:");
			writeInteger_WIFI(key,DEC);
			writeChar_WIFI('\n');
			
			 // ... wait until the key is released again...
			while(getPressedKeyNumber());
			writeString_P_WIFI("Key released!\n");
			// ... and depending on which key was pressed, we 
			// toggle the running light on/off or output some text via
			// WIFI Interface. 
			if(key == 1)
			{
				writeString_P_WIFI("SW1 was pressed and released!\n"); 
				if(!isStopwatch1Running()) {
					setCursorPosLCD(1, 9);
					writeStringLCD_P("RUN! "); // Note the space to overwrite previous content.
					startStopwatch1();
				}
				else {
					setCursorPosLCD(1, 9);
					writeStringLCD_P("STOP!");
					stopStopwatch1();
				}
			}
			else if(key == 2)
			{
				writeString_P_WIFI("SW2 was pressed and released!\n"); 
				writeString_P_WIFI("\n\n   _______________________\n");
				writeString_P_WIFI("   \\| RP6  ROBOT SYSTEM |/\n");
				writeString_P_WIFI("    \\_-_-_-_-_-_-_-_-_-_/\n\n");
			}
		}
		
		// As the above routine is blocking while a Button is pressed down, 
		// the running light will STOP during that time (also with SW2 not only SW1).
		runningLight();
	}
	
	return 0;
}
