/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - RP6 CONTROL M256 Examples
 * ****************************************************************************
 * Example: I2C Master 9 - Lib2
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 * Now we test some movement functions of the Library. We use them
 * to make the Robot move just like in one of the RP6Base Example programs.
 * It moves about 30cm forwards, turns about 180°, moves 30cm, turns 180°
 * and so on. 
 *
 * Different from the other RP6 Example programs, you can now view the
 * status outputs on your computer while the USB Cable is not connected.
 * 
 * This is very useful to monitor the current status of the robot. 
 * Of course in this simple program the output is only very limited, 
 * but you can output any number of sensor values at a quite decent update 
 * rate if required. 
 *
 * If you did not notice it yet, you can start and stop the Robot
 * with the WIFI Interface: just hit the RESET or START Button in the
 * RobotLoader Software WIFI Loader Tab! 
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
 * Instead of the USB Connection, you can use the WIFI Interface to monitor
 * the outputs of the Robot. 
 *
 * #+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+
 * ############################################################################
 * ****************************************************************************
 */

/*****************************************************************************/
// Includes:

#include "RP6M256Lib.h" 
#include "RP6I2CmasterTWI.h"

/*****************************************************************************/
/*****************************************************************************/
// Include the "RP6 Control M256 I2C Master library".
// This allows I2C control of the Robot base in a nearly identical API as
// on the Robot Base. 

#include "RP6M256_I2CMasterLib.h"  // If you want to get Details on how this
								   // works, please also check out the RP6-M32 
								   // examples where this is explained 
								   // step by step. 

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
// I2C Requests: 

/**
 * The I2C_requestedDataReady Event Handler
 */
void I2C_requestedDataReady(uint8_t dataRequestID)
{
	checkRP6Status(dataRequestID);
}

/*****************************************************************************/
// I2C Error handler

/**
 * This function gets called automatically if there was an I2C Error like
 * the slave sent a "not acknowledge" (NACK, error codes e.g. 0x20 or 0x30).
 */
void I2C_transmissionError(uint8_t errorState)
{
	writeString_P_WIFI("\nI2C ERROR - TWI STATE: 0x");
	writeInteger(errorState, HEX);
	writeChar_WIFI('\n');
}

/*****************************************************************************/
// Main function - The program starts here:

int main(void)
{
	initRP6M256();  
	initLCD();
    
	writeString_P_WIFI("\n\nRP6 CONTROL M32 I2C Master Example Program!\n"); 
    writeString_P_WIFI("\nMoving...\n"); 

	// ---------------------------------------
	WDT_setRequestHandler(watchDogRequest); 
	
	// ---------------------------------------
	// Init TWI Interface:
	I2CTWI_initMaster(100);  
	I2CTWI_setRequestedDataReadyHandler(I2C_requestedDataReady);
	I2CTWI_setTransmissionErrorHandler(I2C_transmissionError);


	setLEDs(0b1111);

	showScreenLCD("################", "################");
	mSleep(1000);
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
		writeString_P_WIFI("\nMoving Forwards...\n"); 
		move(70, FWD, DIST_MM(300), BLOCKING);
		setLEDs(0b1000); 
		showScreenLCD("ROTATE", "LEFT");
		writeString_P_WIFI("\nRotating Left...\n"); 
		rotate(60, LEFT, 180, BLOCKING);
		setLEDs(0b1001); 
		showScreenLCD("MOVE", "FWD");
		writeString_P_WIFI("\nMoving Forwards...\n"); 
		move(70, FWD, DIST_MM(300), BLOCKING);
		setLEDs(0b0001); 
		showScreenLCD("ROTATE", "RIGHT");
		writeString_P_WIFI("\nRotating Right...\n"); 
		rotate(60, RIGHT, 180, BLOCKING);
	}
	return 0;
}

