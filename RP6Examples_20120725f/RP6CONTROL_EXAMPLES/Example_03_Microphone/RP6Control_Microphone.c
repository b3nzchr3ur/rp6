/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - RP6 CONTROL M32 Examples
 * ****************************************************************************
 * Example: Microphone
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 * This example shows a small bargraph with Display and LEDs which indicate
 * the last peak noise measured with the Microphone sensor of the RP6-M32.
 *
 * Make noise or touch the Microphone with your fingers and see how the 
 * reaction is!
 *
 * ############################################################################
 * The Robot does NOT move in this example! You can simply put it on a table
 * next to your PC and you should connect it to the PC via the USB Interface!
 * ############################################################################
 * ****************************************************************************
 */

/*****************************************************************************/
// Includes:

#include "RP6ControlLib.h" 		// The RP6 Control Library. 
								// Always needs to be included!

/*****************************************************************************/
// Main function - The program starts here:

int main(void)
{
	initRP6Control(); // Always call this first! 

	writeString_P("\n\nRP6Control Microphone Example Program!\n"); 
	
	sound(180,80,25);
	sound(220,80,0);

	setLEDs(0b1111); // Turn all LEDs on!
	mSleep(50);
	initLCD(); // Initialize the LC-Display (LCD)
			   // Always call this before using the LCD!
	showScreenLCD("################", "################");
	mSleep(500);
	showScreenLCD("Microphone", "Example Program");
	mSleep(1000);
	showScreenLCD("MAKE LOTS OF", "NOISE PLEASE! ;)");
	mSleep(1500);
	// ---------------------------------------
	setLEDs(0b0000); // All LEDs off!

	clearLCD();
	writeStringLCD_P("M:      PK:");
	
	// Discharge the peak detector capacitor before we
	// start the measurements:
	dischargePeakDetector();

	uint16_t last_peak = 0;
	uint8_t peak_counter = 0;
	while(true) 
	{
		// Measure last Microphone peak:
		uint16_t tmp = getMicrophonePeak();
		// show last value on LCD:
		setCursorPosLCD(0, 3); 
		writeIntegerLengthLCD( tmp, DEC, 4);
		if(tmp > 4) 
		{
			if(tmp > last_peak) // Do we have a new peak?
			{
				last_peak = tmp;
				peak_counter = 0;
			}
			// Show last peak on LCD:
			setCursorPosLCD(0, 12);
			writeIntegerLengthLCD(last_peak, DEC, 4);
			
			// Display bargraph:
			clearPosLCD(1, 0, 16);
			setCursorPosLCD(1, 0);
			externalPort.LEDS = 0;
			uint16_t i;
			uint8_t j;
			for(i = 0, j = 2; i < tmp; i+= 68)
			{
				writeCharLCD('#');
				if(j > 2)
				{
					if(i < 240)
					{
						externalPort.LEDS++;
					}
					else
					{
						externalPort.LEDS <<=1;
						externalPort.LEDS++;
					}
					j = 0;
				}
				j++;
			}
			outputExt();
			// Show last peak cursor on Display:
			for(i = 0, j = 0; i < last_peak; i+=68)
				j++;
			if(last_peak >= 62)
			{
				setCursorPosLCD(1, j);
				writeCharLCD('#');
			}
		}
		// Clear display after some time with no peaks:
		else if(peak_counter > 12)
		{
			clearPosLCD(1, 0, 16);
			last_peak = tmp;
			peak_counter = 0;
			externalPort.LEDS = 0;
			outputExt();
		}
		else
			peak_counter++;
		mSleep(50);	
	}
	return 0;
}
