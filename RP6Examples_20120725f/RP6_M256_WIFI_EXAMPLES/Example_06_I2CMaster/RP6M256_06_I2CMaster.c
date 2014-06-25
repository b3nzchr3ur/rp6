/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - RP6 CONTROL M256 Examples
 * ****************************************************************************
 * Example: I2C Master
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description: 
 * Now we will use the I2C Bus Interface to send commands to the Controller
 * on the Mainboard and read sensor values. 
 *
 * You need to program the Controller on the Mainboard with the I2C Bus Slave
 * Example program from the RP6Base examples! Otherwise it will not work!
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

#include "RP6M256Lib.h"   	
#include "RP6I2CmasterTWI.h"

/*****************************************************************************/

#define I2C_RP6_BASE_ADR 10		// The default address of the Slave Controller 
								// on the RP6 Mainboard

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
	writeString_P_WIFI("\nI2C ERROR - TWI STATE: 0x");
	writeInteger_WIFI(errorState, HEX);
	writeChar_WIFI('\n');
}


/*****************************************************************************/
// Read all registers function

uint8_t RP6data[32]; // This array will contain all register values of RP6 
					 // after you called readAllRegisters()
					 
					 // It is better to keep such big arrays GLOBAL as 
					 // they otherwise fill up the stack in memory...

/**
 * This function reads ALL registers available in the standard I2C Bus Slave
 * Example program for the Robot Base and outputs their values on the serial interface. 
 * You will see a lot of zeros when the Motors are not running. The main values that are not
 * zero are the two Light Sensors and the two free ADC Channels.
 */
void readAllRegisters(void)
{
	I2CTWI_transmitByte(I2C_RP6_BASE_ADR, 0); // Start with register 0...
	I2CTWI_readBytes(I2C_RP6_BASE_ADR,RP6data, 30); // and read all 30 registers up to
													// register Number 29 !

	// Now we output the Data we have just read on the serial interface:
	writeString_P_WIFI("\nREADING ALL RP6 REGISTERS:");
	uint8_t i = 0;
	for(i = 0; i < 30; i++)
	{
		if(i % 8 == 0) 		  // add some newline chars otherwise everything 
			writeChar_WIFI('\n');  // is printed on ONE single line...
		else
			writeString_P_WIFI(" | ");
		writeChar_WIFI('#');
		writeIntegerLength_WIFI(i,DEC,2);
		writeChar_WIFI(':');
		writeIntegerLength_WIFI(RP6data[i],DEC,3);
	}
	writeChar_WIFI('\n');
}

/** 
 * Here we demonstrate how to read a few specific registers. 
 * It is just the same as above, but we only read 4 registers and
 * start with register Number 13.
 * We also show how to combine values from high and low registers 
 * back together to a 16 Bit value.
 */
void readLightSensors(void)
{
	uint8_t lightSens[4];

	I2CTWI_transmitByte(I2C_RP6_BASE_ADR, 13); // Start with register 13 (LSL_L)...
	I2CTWI_readBytes(I2C_RP6_BASE_ADR, lightSens, 4); // and read all 4 registers up to
													// register Number 16 (LSR_H) !
	writeString_P_WIFI("Light Sensor registers:\n");
	writeString_P_WIFI(" | LSL_L:"); writeInteger_WIFI(lightSens[0], DEC); 
	writeString_P_WIFI(" | LSL_H:"); writeInteger_WIFI(lightSens[1], DEC); 
	writeString_P_WIFI(" | LSR_L:"); writeInteger_WIFI(lightSens[2], DEC); 
	writeString_P_WIFI(" | LSR_H:"); writeInteger_WIFI(lightSens[3], DEC); 

	writeString_P_WIFI("\n\n Light Sensor Values:");
	writeString_P_WIFI(" | LSL:"); writeInteger_WIFI(lightSens[0] + (lightSens[1]<<8), DEC); 
	writeString_P_WIFI(" | LSR:"); writeInteger_WIFI(lightSens[2] + (lightSens[3]<<8), DEC); 
	writeChar_WIFI('\n');
}


/*****************************************************************************/
// Main function - The program starts here:

int main(void)
{
	initRP6M256(); 
	initLCD(); 
			   
	writeString_P_WIFI("\n\nRP6 CONTROL M256 I2C Master Example Program!\n"); 

	// IMPORTANT:
	I2CTWI_initMaster(100); // Initialize the TWI Module for Master operation
							// with 100kHz SCL Frequency
							
	// Register the event handlers:
	I2CTWI_setTransmissionErrorHandler(I2C_transmissionError);

	setLEDs(0b1111); // Turn all LEDs on!

	showScreenLCD("################", "################");
	mSleep(500);
	showScreenLCD("I2C-Master", "Example Program 1");
	mSleep(1000);
	// ---------------------------------------
	setLEDs(0b0000); // All LEDs off!
	
	uint8_t counter = 1;
	
	
	// The command and register used here - in the next example we define them all. 
	#define CMD_SET_ACS_POWER	9 
	#define ACS_PWR_MED  2
	
	// Set ACS to medium power (you can see the ACS value changes in the raw registers):
	I2CTWI_transmit3Bytes(I2C_RP6_BASE_ADR, 0, CMD_SET_ACS_POWER, ACS_PWR_MED);


	while(true) 
	{
		// Increment a counter and send value to LEDs of the
		// Slave Controller:
		setLEDs(0b0001);
		showScreenLCD("INCREMENT", "COUNTER");
		setCursorPosLCD(1, 11);
		writeIntegerLengthLCD(counter, DEC, 3);
		I2CTWI_transmit3Bytes(I2C_RP6_BASE_ADR, 0, 3, counter);

		counter++;
	
		// Read and display ALL registers of the slave controller:
		setLEDs(0b0010);

		readAllRegisters();
	
		// Read the light sensors:
		setLEDs(0b0100); 
		readLightSensors();
		
		mSleep(250);
	}
	return 0;
}

