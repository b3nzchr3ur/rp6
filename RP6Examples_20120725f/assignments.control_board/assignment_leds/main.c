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
#include "level_check.h"
//#include "led_values_updater.h"

/*****************************************************************************/
// Main function - The program starts here:

#define MICROPHONE_THRESHOLD_LEVEL (100)


/** This method calculates the new settings of the led lights when an update is requested.
  * 
  * Examples: Input: led_value = 0b0000 (all lights are OFF) 
  *           Output: new_led_value = 0b0001.
  *
  *	      Input: led_value = 0b0001 (only one light is ON) 
  *           Output: new_led_value = 0b0010.
  *
  * \param led_values          : Input for the current value of the led lights
  * \param new_led_values_ptr  : Output for light value of the led lights after being updated
  *
  */
void led_values_update(uint8_t led_values, uint8_t* new_led_values_ptr)
{
	//TODO: create implementation
}

int main(void)
{
	initRP6Control(); // Always call this first!

	writeString_P("\n\nRP6Control Microphone Example Program!\n");

	sound(180,80,25);
	sound(220,80,0);

	setLEDs(0b0000); // All LEDs off!

	clearLCD();
	writeStringLCD_P("M:      PK:");
	uint8_t led_values = 0b0000;

	dischargePeakDetector();

	while(true)
	{
		uint16_t microphoneValue = getMicrophonePeak();
		setCursorPosLCD(0, 3);
		writeIntegerLengthLCD( microphoneValue, DEC, 4);

		bool peakDetected = is_level_detected(microphoneValue, MICROPHONE_THRESHOLD_LEVEL);

		if(peakDetected)
		{
			led_values_update(led_values, &led_values);
		}

		setLEDs(led_values);
	}
	return 0;
}
