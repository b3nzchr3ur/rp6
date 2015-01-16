/*****************************************************************************/
// Includes:

#include "RP6RobotBaseLib.h" 	
#include "RP6I2CmasterTWI.h"     // Include the I2C-Bus Slave Library

#define ARDUINO_DEVICE_ADDRESS (42)
#define ARDUINO_WRITE_ADDRESS (ARDUINO_DEVICE_ADDRESS << 1)
#define ARDUINO_READ_ADDRESS (ARDUINO_WRITE_ADDRESS + 1)

static uint8_t someByteToSend = 0; 

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

void sendByteAndReceiveByte(void)
{
	if(getStopwatch1() > 500) {
	
                writeString_P("\nSend: ");
		
		//TODO: send someByteToSend to the Arduino

		someByteToSend += 5;		
	
		if(someByteToSend > 255) 
		{
			someByteToSend = 0;
		}

		uint8_t someByteToRead = 0;
                
		//TODO: read someByteToSend from the Arduino

		writeString_P("\nReceived: ");
                writeInteger(someByteToRead,DEC);
		writeChar('\n');
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
	
	I2CTWI_setTransmissionErrorHandler(I2C_transmissionError);

	setLEDs(0b111111);
	mSleep(500);	   
	setLEDs(0b000000);
	
	powerON();

        writeString_P("\nReady for I2C master mode:\n");
	startStopwatch1();
	
	while(true)  
	{
		sendByteAndReceiveByte();
		task_I2CTWI(); // Call I2C Management routine
		task_RP6System();
	}
	return 0;
}