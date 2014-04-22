/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - ROBOT BASE EXAMPLES
 * ****************************************************************************
 * Example: I2C Master - Single SRF08/SRF10
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 *
 * This Example shows how to use a Devantech SRF08 or SRF10 with RP6Library
 * (both sensors work nearly the same way, but SRF08 offers some additional 
 * features and seems to be more accurate sometimes - of course it is
 * larger, but there's enough space on RP6). 
 *
 * You may notice that the measurements fluctuate and that there are some error 
 * readings - this seems to be "normal" and you maybe need to filter these errors 
 * if you want to do obstacle avoiding with it. 
 * Simplest way is to calculate the average distance...
 * ****************************************************************************
 */

/*****************************************************************************/
// Includes:

#include "RP6RobotBaseLib.h" 	


// IMPORTANT:

#include "RP6I2CmasterTWI.h"     // Include the I²C-Bus Slave Library



/*****************************************************************************/
// SRF08/SRF10:

// The I2C slave address of the SRF ranger - default is 0xE0, but you can
// change this when you use the function changeSRFAddr() below.
// Here we changed it to 0xE6 already:
#define SRF_ADR  0xE6

/**
 * Use this function to change the address of a SRF  ONCE ONLY!!
 * You only need to do this ONCE - the sensor keeps its address in EEPROM.
 * ONLY CONNECT ONE SINGLE SENSOR TO THE BUS WHEN DOING THIS!!!
 *
 * READ THE SRF DOCUMENTATION FOR MORE DETAILS!!!
 *
 * Default address is 0xE0, so to change it to 0xE6, perform:
 * changeSRFAddr(0xE0,0xE6);
 * and then delete this line out of your program! Don't forget
 * to delete it!
 * Once you changed the address of each sensor to suit your needs, 
 * you can remove the whole function.
 */
void changeSRFAddr(uint8_t old_addr, uint8_t new_addr)
{
	I2CTWI_transmit2Bytes(old_addr, 0, 0xA0);
	I2CTWI_transmit2Bytes(old_addr, 0, 0xAA);
	I2CTWI_transmit2Bytes(old_addr, 0, 0xA5);
	I2CTWI_transmit2Bytes(old_addr, 0, new_addr);
}


/*****************************************************************************/
// I2C Event handlers:


// I2C Data request IDs:
#define MEASURE_US_LOW 	0
#define MEASURE_US_HIGH 1

// The measured distance:
uint16_t distance;

/**
 * This Function gets called automatically if all requested data has been received.
 * You can identify your requests by giving them a unique dataRequestID for each
 * device or even register you are reading from. 
 * Of course you don't need to use it like this - just look at the PCF8591 examples!
 * But this allows you to do other things while the TWI Module fetches the
 * data for you in the Background.
 */
void I2C_requestedDataReady(uint8_t dataRequestID)
{
	uint8_t messageBuf[8];
	static uint8_t dist_tmp;
	switch(dataRequestID)
	{
		case MEASURE_US_HIGH: // High data register
		    // get received data ...
			I2CTWI_getReceivedData(messageBuf, 2);
			dist_tmp = (messageBuf[0]);
			// ... and request low data byte:
			I2CTWI_transmitByte(SRF_ADR, 3);
			I2CTWI_requestDataFromDevice(SRF_ADR, MEASURE_US_LOW, 1);
		break;
		case MEASURE_US_LOW: // low data byte:
			I2CTWI_getReceivedData(messageBuf, 2);
			distance = messageBuf[0] + (dist_tmp << 8);
		
			// ----------------------------------
			// Output measured distance:
			writeString_P("Distance: ");
			writeInteger(distance, DEC);
			writeString_P(" cm | ");
			// show a simple text bargraph:
			uint16_t dist_count;
			for(dist_count = distance; dist_count > 4; dist_count-=2)
				writeChar('#');
			writeChar('\n');
			// ... of course you can also use "distance" as a normal
			// variable everywhere else in your program! 
			// ----------------------------------
		break;
	}
}

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

/**
 * The SRF needs some time to perform the measurements - here we use Stopwatch1
 * to time the measurement actions.
 */
void task_SRF(void)
{
	static uint8_t measureInProgress = false;
	if(!measureInProgress) // Start measurement ONCE only
	{
		if(TWI_operation == I2CTWI_NO_OPERATION) // If there is no request in progress...
		{
			I2CTWI_transmit2Bytes(SRF_ADR, 0, 81); // 81 means return distance in cm
			measureInProgress = true;
			setStopwatch1(0);
		}
	}
	else if(getStopwatch1() > 70)  // 70ms (measurement delay)
	{
		I2CTWI_transmitByte(SRF_ADR, 2); // range register high byte
		I2CTWI_requestDataFromDevice(SRF_ADR, MEASURE_US_HIGH, 1); // receive it
		measureInProgress = false; 	// allow to start new measurement
		setStopwatch1(0);
	}
}


/*****************************************************************************/
// Main - The program starts here:

int main(void)
{
	initRobotBase();
	
	I2CTWI_initMaster(100);
	I2CTWI_setRequestedDataReadyHandler(I2C_requestedDataReady);
	I2CTWI_setTransmissionErrorHandler(I2C_transmissionError);

	setLEDs(0b111111);
	mSleep(500);	   
	setLEDs(0b000000);
	
	// Use this to change the address of a SINGLE SRF with this
	// address on the bus:
	// changeSRF_addr(0xE0,0xE6);
	// do not forget to delete/outcomment this again after you performed this once!
	
	powerON();

	startStopwatch1();
	
	while(true)  
	{
		task_SRF();
		task_I2CTWI();
		task_RP6System();
	}
	return 0;
}
