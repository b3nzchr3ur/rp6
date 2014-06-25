/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - ROBOT BASE EXAMPLES
 * ****************************************************************************
 * Example: I2C Master - PCF8574 6 Bit I/O Expander
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 *
 * This example program shows how to create a "Knight Rider" style running light 
 * on 8 LEDs connected to a PCF8574 I2C-Bus 8-Bit I/O Expander.
 *
 * You need to build a suitable Expansionboard by yourself of course! 
 * This is only to demonstrate how you can interface parts from other 
 * manufacturers - in the next example we will also show how to interface
 * the PCF8591 for A/D Conversion.
 *
 * Controlling other devices works very similar and thus this is a general
 * example that can be reused for many many other I2C Bus devices.
 * You need to read the datasheets of the devices you want to interface!
 * (there are thousands of other devices with I2C Bus interface, let it
 * be complete Ultrasonic sensor modules, Displays, EEPROM Memories, 
 * Temperature Sensors, Realtime clocks, etc. pp. )
 *
 * ****************************************************************************
 */

/*****************************************************************************/
// Includes:

#include "RP6RobotBaseLib.h" 	

// ---------------------------------

#include "RP6I2CmasterTWI.h"     // Include the I2C-Bus Slave Library

/*****************************************************************************/
// I2C

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

// Define the addresses of our devices - in this simple example we only have
// our PCF8574:
#define PCF8574_8LEDS_ADR  0x70


/**
 * Here we have a special running light routine for 8 LEDs connected to a
 * PCF8574 - it gives the typical Knight Rider scanning effect.
 *
 * There are two moving light dots now - not only one!
 * And the running direction is changed each time the light dots reach
 * the outer LEDs.
 */
void task_knightRider(void)
{
	static uint8_t runningLight = 3; 
	static uint8_t dir = 0;
	if(getStopwatch1() > 70) {
	
		// With this routine we write the data byte to the PCF8574 port:
		I2CTWI_transmitByte(PCF8574_8LEDS_ADR, (~runningLight) );
		
		// We need to invert the bits with "~" because the LEDs are
		// switched on when the port is LOW. The PCF8574 can only sink
		// up to 25mA of current. It is only able to source about 300µA
		// of current. Thus you need to connect the cathode of the LED
		// to the ports - in series with a 1K or 2K2 resistor connected to
		// VDD.
	
		if(runningLight == 128) 
		{
			dir = 1;
			runningLight = 192;
		}
		else if(runningLight < 3) 
		{
			dir = 0;
			runningLight = 3;
		}
		else if(dir)
			runningLight >>= 1;
		else
			runningLight <<= 1;
			
		setStopwatch1(0);	
	}
}

/*****************************************************************************/
// Main - The program starts here:

int main(void)
{
	initRobotBase();
	
	I2CTWI_initMaster(100); // Initialize the TWI Module for Master operation
							// with 100kHz SCL Frequency
							// PCF8574 and PCF8591 are only specified for
							// up to 100kHz SCL freq - not 400kHz HighSpeed mode!
	
	// Register the event handlers:
	I2CTWI_setTransmissionErrorHandler(I2C_transmissionError);

	setLEDs(0b111111);
	mSleep(500);	   
	setLEDs(0b000000);
	
	powerON();

	startStopwatch1();
	
	while(true)  
	{
		task_knightRider();
		task_I2CTWI(); // Call I2C Management routine
		task_RP6System();
	}
	return 0;
}
