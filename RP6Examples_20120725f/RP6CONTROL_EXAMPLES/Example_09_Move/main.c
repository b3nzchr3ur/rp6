/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - RP6 CONTROL M32 Examples
 * ****************************************************************************
 * Example: I2C Master 9 - Lib2
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 * Now we add some movement functions to the new Library. We use them
 * to make the Robot move just like in one of the RP6Base Example programs.
 * It moves about 30cm forwards, turns about 180°, moves 30cm, turns 180°
 * and so on. 
 *
 * ############################################################################
 * #+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+
 * 
 * ATTENTION: THE ROBOT MOVES AROUND IN THIS EXAMPLE! PLEASE PROVIDE ABOUT
 * 2m x 2m OR MORE FREE SPACE FOR THE ROBOT! 
 *
 * >>> DO NOT FORGET TO REMOVE THE FLAT CABLE CONNECTION TO THE USB INTERFACE
 * BEFORE YOU START THIS PROGRAM BY PRESSING THE START BUTTON ON THE ROBOT!
 *
 * #+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+
 * ############################################################################
 * ****************************************************************************
 */

/*****************************************************************************/
// Includes:

#include "RP6ControlLib.h" 		// The RP6 Control Library. 
								// Always needs to be included!

#include "RP6I2CmasterTWI.h"	// I2C Master Library


/*****************************************************************************/
/*****************************************************************************/
// Include our new "RP6 Control I2C Master library":

#include "RP6Control_I2CMasterLib.h"

/*****************************************************************************/

/**
 * Timed Watchdog display only - the other heartbeat function
 * does not work in this example as we use blocked moving functions here.
 */
void watchDogRequest(void)
{
	static uint8_t heartbeat2 = false;
	if(heartbeat2)
	{
		clearPosLCD(0, 14, 1);
		heartbeat2 = false;
	}
	else
	{
		setCursorPosLCD(0, 14);
		writeStringLCD_P("#"); 
		heartbeat2 = true;
	}
}

/*****************************************************************************/
// I2C Error handler

/**
 * This function gets called automatically if there was an I2C Error like
 * the slave sent a "not acknowledge" (NACK, error codes e.g. 0x20 or 0x30).
 */
void I2C_transmissionError(uint8_t errorState)
{
	writeString_P("\nI2C ERROR - TWI STATE: 0x");
	writeInteger(errorState, HEX);
	writeChar('\n');
}

/*****************************************************************************/
// Main function - The program starts here:

int main(void)
{
	initRP6Control();  
	initLCD();
    
	writeString_P("\n\nRP6 CONTROL M32 I2C Master Example Program!\n"); 
        writeString_P("\nMoving...\n"); 

	// ---------------------------------------
	WDT_setRequestHandler(watchDogRequest); 
	
	// ---------------------------------------
	// Init TWI Interface:
	initI2C_RP6Lib();
        I2C_setTransmissionErrorHandler(I2C_transmissionError);

	sound(180,80,25);
	sound(220,80,25);

	setLEDs(0b1111);

	showScreenLCD("################", "################");
	mSleep(500);
	showScreenLCD("I2C-Master", "Movement...");
	mSleep(1000);
	setLEDs(0b0000);
	
	// ---------------------------------------
	I2CTWI_transmit3Bytes(I2C_RP6_BASE_ADR, 0, CMD_SET_ACS_POWER, ACS_PWR_MED);
	I2CTWI_transmit3Bytes(I2C_RP6_BASE_ADR, 0, CMD_SET_WDT, true);
	I2CTWI_transmit3Bytes(I2C_RP6_BASE_ADR, 0, CMD_SET_WDT_RQ, true);

	while(true) 
	{ 
		setLEDs(0b1001); 
		showScreenLCD("MOVE", "FWD");
		move(60, FWD, DIST_MM(300), BLOCKING);
		setLEDs(0b1000); 
		showScreenLCD("ROTATE", "LEFT");
		rotate(50, LEFT, 180, BLOCKING);
		setLEDs(0b1001); 
		showScreenLCD("MOVE", "FWD");
		move(60, FWD, DIST_MM(300), BLOCKING);
		setLEDs(0b0001); 
		showScreenLCD("ROTATE", "RIGHT");
		rotate(50, RIGHT, 180, BLOCKING);
	}
	return 0;
}

