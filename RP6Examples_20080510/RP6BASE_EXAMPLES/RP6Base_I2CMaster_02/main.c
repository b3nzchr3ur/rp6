/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - ROBOT BASE EXAMPLES
 * ****************************************************************************
 * Example: I2C Master - PCF8591 & PCF8574
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 *
 * This example is just the same as the previous example, but additionally
 * controls a PCF8591 4 channel A/D Convertor + 1 ch D/A convertor. 
 *
 * Of course you need to build your own board for this. 
 * You can use one of the RP6 Experimentation Boards. 
 *
 * We write about 4 LDRs connected to the PCF8591 - but this does not matter!
 * You can connect any analog output sensor with voltage output range
 * from 0-5V or with range matching voltage divider and protection diodes 
 * also Sensors with bigger output voltages... read the PCF8591 datasheet that 
 * you can find on the CD! 
 *
 * ****************************************************************************
 */

/*****************************************************************************/
// Includes:

#include "RP6RobotBaseLib.h" 	


// IMPORTANT:

#include "RP6I2CmasterTWI.h"     // Include the I2C-Bus Slave Library


/*****************************************************************************/
// I2C Event handlers:

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
	writeString_P("\nI2C ERROR --> TWI STATE IS: 0x");
	writeInteger(errorState, HEX);
	writeChar('\n');
}

/*****************************************************************************/

// Define the addresses of our devices:
#define PCF8574_8LEDS_ADR  0x70
#define PCF8591_4LDRS_ADR  0x90

/**
 * This function reads out the four ADCs of the PCF8591 and outputs their
 * values on the serial interface. We assume you connected four LDRs to the
 * ports, but you can connect anything else there!
 */
void task_ext_LDRs(void)
{
	if(getStopwatch2() > 200) 
	{
	    // Configure PCF8591: Ext. Output on + auto increment: 
		I2CTWI_transmitByte(PCF8591_4LDRS_ADR, 0b01000100 ); 
		
		// Read the data: 
		uint8_t results[6];
		I2CTWI_readBytes(PCF8591_4LDRS_ADR,results,5);
		
		// Byte 0 contains last conversion result - thus we read 5 bytes,
		// skip byte 0 and output only results[1] to results[4]:
		writeString_P(" | LDR1:"); writeInteger(results[1], DEC); 
		writeString_P(" | LDR2:"); writeInteger(results[2], DEC); 
		writeString_P(" | LDR3:"); writeInteger(results[3], DEC); 
		writeString_P(" | LDR4:"); writeInteger(results[4], DEC); 
		writeChar('\n');
		
		setStopwatch2(0);
	}
}

/**
 * The "Knight Rider" running Light once again!
 */
void task_knightRider(void)
{
	static uint8_t runningLight = 3; 
	static uint8_t dir = 0;
	if(getStopwatch1() > 70) {
		I2CTWI_transmitByte(PCF8574_8LEDS_ADR, (~runningLight) );
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
	
	// Register the event handlers:
	I2CTWI_setTransmissionErrorHandler(I2C_transmissionError);

	setLEDs(0b111111);
	mSleep(500);	   
	setLEDs(0b000000);
	
	powerON();

	startStopwatch1();
	startStopwatch2();
	
	while(true)  
	{
		task_knightRider();
		task_ext_LDRs();
		task_I2CTWI();
		task_RP6System();
	}
	return 0;
}
