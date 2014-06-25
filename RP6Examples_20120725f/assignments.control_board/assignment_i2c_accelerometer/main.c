/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - RP6 CONTROL M32 Examples
 * ****************************************************************************
 * Example: I2C Master - 
 * Communicatie met 3D accelleratiesensor MMA7455 van Parallax (#28526)
 * Author(s): Corne Govers
 * Datum: 21 maart 2011
 * ****************************************************************************
 * Description:
 * De I2C bus wordt gebruikt om de #D accelleratie semsor uit te lezen.
 * De sensor is bevestigd op het breadboard op de RP6 robot. 
 * Adres van de sensor: 0x1D
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

#include "RP6I2CmasterTWI.h"	// Needed because we're using the I2C 

#include "MMA7455L.h"


/*****************************************************************************/

/*****************************************************************************/
// I2C Error handler

/**
 * This function gets called automatically if there was an I2C Error like
 * the slave sent a "not acknowledge" (NACK, error codes e.g. 0x20 or 0x30).
 * The most common mistakes are: 
 *   - using the wrong address for the slave
 *   - slave not active or not connected to the I2C-Bus
 *   - too fast requests for a slower slave
 * Be sure to check this if you get I2C errors!
 */
void I2C_transmissionError(uint8_t errorState)
{
	writeString_P("\nI2C ERROR - TWI STATE: 0x");
	writeInteger(errorState, HEX);
	writeChar('\n');
}


/*****************************************************************************/

void showRawSensData(Meetdata rdata)
{
	// Now we output the Data we have just read on the serial interface:
	writeString_P("\nSHOWING ALL sensor accellerationvalues:\n");
	writeChar('X');
	writeChar(':');
	writeIntegerLength(rdata.Xacc, DEC, 3);
	writeChar('\n');
	writeChar('Y');
	writeChar(':');
	writeIntegerLength(rdata.Yacc, DEC, 3);
	writeChar('\n');
	writeChar('Z');
	writeChar(':');
	writeIntegerLength(rdata.Zacc, DEC, 3);
	writeChar('\n');
}


/*****************************************************************************/
// Main function - The program starts here:

int main(void)
{
	initRP6Control(); // Always call this first! The Processor will not work
					  // correctly otherwise. 

	initLCD(); // Initialize the LC-Display (LCD)
			   // Always call this before using the LCD!
			   
	// IMPORTANT:
	I2CTWI_initMaster(100); // Initialize the TWI Module for Master operation
							// with 100kHz SCL Frequency
	initMMA7455();
	
	I2CTWI_setTransmissionErrorHandler(I2C_transmissionError);

	writeString_P("\n\nRP6 CONTROL M32 I2C Master Example Program - reading 3D accelerometer!\n"); 
							
	// Register the event handlers:

	// Play two sounds:
	sound(180,80,25);
	sound(220,80,25);

	setLEDs(0b1111); // Turn all LEDs on!

	showScreenLCD("################", "################");
	mSleep(500);
	showScreenLCD("I2C-Master", "MMA7455L sensor");
	mSleep(1000);
	// ---------------------------------------
	setLEDs(0b0000); // All LEDs off!
	
	Meetdata mdata;

	while(true) 
	{
		// We check the buttons - just like the buttons example - but here we 
		// generate several I2C Bus requests and commands when a key 
		// is pressed AND released again...
		uint8_t key = checkReleasedKeyEvent(); 
		
		if(key)
		{
			switch(key)
			{
				case 1:
					setLEDs(0b0001);
					showScreenLCD("INTO STANDBY", "MODE");
					MMA7455_set_to_standby_mode();
				break;
				case 2: 
					showScreenLCD("INTO MEASUREMENT", "MODE");
					MMA7455_set_to_measurement_mode();
				break;
				case 3: // Read the light sensors and show value on display:
					setLEDs(0b0100); 
					showScreenLCD("MEASURE", "");
					MMA7455_measure(&mdata);
					showRawSensData(mdata);
				break;
			}
		}
	}
	return 0;
}

