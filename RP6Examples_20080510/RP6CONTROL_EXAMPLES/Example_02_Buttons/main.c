/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - RP6 CONTROL M32 Examples
 * ****************************************************************************
 * Example: Buttons and Beeper
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 * This Example shows how to use the keys (and the beeper) on the RP6 Control 
 * board.
 * We simply poll for pressed keys and output ADC value and key number on the
 * LCD and serial interface, set some LEDs and play sounds with the beeper.
 * You can use this program to get the real ADC values for your keypad. 
 * (They are different on each board, because of the 5% tolerance resistors.
 * You may need to adjust some values in the RP6ControlLib in the 
 * getPressedKeyNumber() Function)
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
	initRP6Control(); // Always call this first! The Processor will not work
					  // correctly otherwise. 

	writeString_P("\n\nRP6Control Button Example Program!\n"); 

	// Play two sounds:
	sound(180,80,25);
	sound(220,80,25);

	setLEDs(0b1111); // Turn all LEDs on!
	initLCD(); // Initialize the LC-Display (LCD)
			   // Always call this before using the LCD!
	showScreenLCD("################", "################");
	mSleep(500);
	showScreenLCD("Buttons", "Example Program2");
	mSleep(1000);
	// ---------------------------------------

	setLEDs(0b0000); // All LEDs off!
	// This text is fixed on the LCD. Only the
	// key and ADC values are changed later on!
	showScreenLCD("ADC Value:", "Button: T");
	while(true) 
	{
		// Check if a key is pressed:
		uint8_t key = getPressedKeyNumber(); 
		// This function returns a 0 if no key is pressed and
		// the key number from 1 to 5 otherwise.

		if(key) // If a key is pressed... (key != 0)
		{
			// ... we update the values
			// on the LCD display:
			uint16_t key_adc = readADC(ADC_KEYPAD);
			setCursorPosLCD(0, 11); 
			writeIntegerLengthLCD(key_adc, DEC, 4);
			setCursorPosLCD(1, 9);
			writeIntegerLCD(key, DEC);
			// Please note that we did not output the whole screen again - we just 
			// overwrote the old numbers!
			
			// Output key and ADC Value on serial Interface:
			writeString_P(" ADC Value:");
			writeInteger(key_adc,DEC);
			writeString_P(" --> Pressed key:");
			writeInteger(key,DEC);
			writeChar('\n');
			
			 // ... wait until the key is released again...
			while(getPressedKeyNumber());
			writeString_P("Key released - playing sound!\n");
			// ... and depending on which key was pressed, we 
			// turn some LEDs on and play different sounds:
			switch(key)
			{
				case 1: 
					setLEDs(0b0001);
					sound(200,10,10);
					sound(175,10,10);
					sound(215,10,10);
					sound(135,50,40);
					sound(225,20,25);
					sound(205,50,50);
					sound(135,120,120);
					sound(95,50,10);
					sound(145,40,10);
					sound(200,30,15);
					sound(175,30,20);
					sound(135,50,25);
				break;
				case 2: 
					setLEDs(0b0010);
					sound(235,11,1);
					sound(225,12,2);
					sound(215,13,3);
					sound(205,14,4);
					sound(195,15,5);
					sound(185,16,6);
					sound(175,17,7);
					sound(165,18,8);
				break;
				case 3: 
					setLEDs(0b0100); 
					sound(245,10,0);
					sound(235,20,0);
					sound(225,40,0);
					sound(215,60,0);
					sound(205,80,0);
					sound(215,60,0);
					sound(225,40,0);
					sound(235,20,0);
					sound(245,10,0);
				break;
				case 4: 
				// Simple "Alarm" Sound using setBeeperFrequency function:
				{ 
					uint8_t i;
					uint8_t j;
					for(i = 1; i < 4; i++)
					{
						setLEDs(0b1111); 
						for(j = 230; j > 46; j-=2)
						{
							setBeeperPitch(j);
							mSleep(5);
						}
						setLEDs(0b0000); 
						for(; j < 230; j+=2)
						{
							setBeeperPitch(j);
							mSleep(5);
						}
						// Turn beeper off:
						setBeeperPitch(0);
					}
				}
				break;
				case 5: 
					setLEDs(0b1001);
					sound(220,80,10);
					sound(175,20,20);
					sound(220,80,10);
					sound(175,20,20);
				break;
			}
		}
	}
	return 0;
}
