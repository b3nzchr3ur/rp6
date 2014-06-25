/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - RP6 CONTROL M32 Examples
 * ****************************************************************************
 * Example: I2C Master
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 * Now we will use the I2C Bus Interface to send commands to the Controller
 * on the Mainboard and read sensor values. 
 * This program does not do anything directly, it checks for pressed buttons
 * and then runs a small I2C Bus example depending on which button has
 * been pressed. 
 *
 * You need to program the Controller on the Mainboard with the I2C Bus Slave
 * Example program from the RP6Base examples! Otherwise it will not work!
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
	writeString_P("\nI2C ERROR - TWI STATE: 0x");
	writeInteger(errorState, HEX);
	writeChar('\n');
}


/*****************************************************************************/
// Rotate function

uint8_t transmit_buffer[10]; // temporary transmit buffer
							 // A global variable saves space on the heap... 

#define CMD_ROTATE 8

#define LEFT 2
#define RIGHT 3

/**
 * Rotate function - you can define nearly the same functions as you have on 
 * the RP6 and just forward the commands with I2C Bus transfers... 
 * We will make an improved version of this and other functions in another example!
 */
void RP6_rotate(uint8_t desired_speed, uint8_t dir, uint16_t angle)
{
	transmit_buffer[0] = 0;
	transmit_buffer[1] = CMD_ROTATE;
	transmit_buffer[2] = desired_speed;
	transmit_buffer[3] = dir;
	transmit_buffer[4] = ((angle>>8) & 0xFF);
	transmit_buffer[5] = (angle & 0xFF);
	I2CTWI_transmitBytes(I2C_RP6_BASE_ADR, transmit_buffer, 6 );
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
	writeString_P("\nREADING ALL RP6 REGISTERS:");
	uint8_t i = 0;
	for(i = 0; i < 30; i++)
	{
		if(i % 8 == 0) 		  // add some newline chars otherwise everything 
			writeChar('\n');  // is printed on ONE single line...
		else
			writeString_P(" | ");
		writeChar('#');
		writeIntegerLength(i,DEC,2);
		writeChar(':');
		writeIntegerLength(RP6data[i],DEC,3);
	}
	writeChar('\n');
}

/** 
 * Here we demonstrate how to read a few specific registers. 
 * It is just the same as above, but we only read 4 registers and
 * start with register Number 13...
 * We also show how to combine values from high and low registers 
 * back together to a 16 Bit value... 
 */
void readLightSensors(void)
{
	uint8_t lightSens[4];

	I2CTWI_transmitByte(I2C_RP6_BASE_ADR, 13); // Start with register 13 (LSL_L)...
	I2CTWI_readBytes(I2C_RP6_BASE_ADR, lightSens, 4); // and read all 4 registers up to
													// register Number 16 (LSR_H) !
	writeString_P("Light Sensor registers:\n");
	writeString_P(" | LSL_L:"); writeInteger(lightSens[0], DEC); 
	writeString_P(" | LSL_H:"); writeInteger(lightSens[1], DEC); 
	writeString_P(" | LSR_L:"); writeInteger(lightSens[2], DEC); 
	writeString_P(" | LSR_H:"); writeInteger(lightSens[3], DEC); 

	writeString_P("\n\n Light Sensor Values:");
	writeString_P(" | LSL:"); writeInteger(lightSens[0] + (lightSens[1]<<8), DEC); 
	writeString_P(" | LSR:"); writeInteger(lightSens[2] + (lightSens[3]<<8), DEC); 
	writeChar('\n');
	
	setCursorPosLCD(1, 3);
	writeIntegerLengthLCD(lightSens[0] + (lightSens[1]<<8), DEC, 4);
	setCursorPosLCD(1, 11);
	writeIntegerLengthLCD(lightSens[2] + (lightSens[3]<<8), DEC, 4);
}

/*****************************************************************************/
// Main function - The program starts here:

int main(void)
{
	initRP6Control(); // Always call this first! The Processor will not work
					  // correctly otherwise. 

	initLCD(); // Initialize the LC-Display (LCD)
			   // Always call this before using the LCD!
			   
	writeString_P("\n\nRP6 CONTROL M32 I2C Master Example Program!\n"); 

	// IMPORTANT:
	I2CTWI_initMaster(100); // Initialize the TWI Module for Master operation
							// with 100kHz SCL Frequency
							
	// Register the event handlers:
	I2CTWI_setTransmissionErrorHandler(I2C_transmissionError);

	// Play two sounds:
	sound(180,80,25);
	sound(220,80,25);

	setLEDs(0b1111); // Turn all LEDs on!

	showScreenLCD("################", "################");
	mSleep(500);
	showScreenLCD("I2C-Master", "Example Program 1");
	mSleep(1000);
	// ---------------------------------------
	setLEDs(0b0000); // All LEDs off!
	
	uint8_t counter = 1;

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
				case 1: // Increment a counter and send value to LEDs of the
						// Slave Controller:
					setLEDs(0b0001);
					showScreenLCD("INCREMENT", "LED COUNTER");
					I2CTWI_transmit3Bytes(I2C_RP6_BASE_ADR, 0, 3, counter);
					counter++;
				break;
				case 2: // Read and display ALL registers of the slave controller:
					setLEDs(0b0010);
					showScreenLCD("READ ALL RP6", "REGISTERS");
					readAllRegisters();
				break;
				case 3: // Read the light sensors and show value on display:
					setLEDs(0b0100); 
					showScreenLCD("LIGHT SENSORS:", "L:      R:");
					readLightSensors();
				break;
				case 4: // Rotate a very small distance to the left:
					setLEDs(0b1000); 
					showScreenLCD("ROTATE A BIT...", "... LEFT!");
					RP6_rotate(35,LEFT,40);
				break;
				case 5: // Rotate a very small distance to the right:
					setLEDs(0b1001);
					showScreenLCD("ROTATE A BIT...", "... RIGHT!");
					RP6_rotate(35,RIGHT,40);
				break;
			}
		}
	}
	return 0;
}

