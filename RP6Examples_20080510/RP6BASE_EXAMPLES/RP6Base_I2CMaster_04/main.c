/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - ROBOT BASE EXAMPLES
 * ****************************************************************************
 * Example: I2C Master - Double SRF08/SRF10
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 *
 * This Example has two SRF08/SRF10 Sensors connected to the bus. 
 * It measures each distance alternating.
 *
 * And we also have the PCF8574 with 8 LEDs and the PCF8591 connected to the
 * Bus like in example 02. 
 * 
 * The Text output of this program is maybe a bit hard to read because
 * there are many outputs. 
 * This program has not been optimized for useability ;) 
 * It just shows that you can use many sensors at the same time without
 * any big problems. 
 * ****************************************************************************
 */

/*****************************************************************************/
// Includes:

#include "RP6RobotBaseLib.h" 	

#include "RP6I2CmasterTWI.h"     // Include the I²C-Bus Slave Library

/*****************************************************************************/
// I2C

#define SRF_LEFT_ADR  0xE6
#define SRF_RIGHT_ADR 0xE8

#define MEASURE_US_LEFT_LOW 	0
#define MEASURE_US_LEFT_HIGH 	1
#define MEASURE_US_RIGHT_LOW 	2
#define MEASURE_US_RIGHT_HIGH 	3

uint16_t distance_left = 0;
uint16_t distance_right = 0;

/**
 * Same like in the last example - but here we have two more registers.
 * You can also put several register request together with a few global
 * variables that store which SRF is active.......
 */
void I2C_requestedDataReady(uint8_t dataRequestID)
{
	uint8_t messageBuf[8];
	static uint8_t dist_tmp;
	switch(dataRequestID)
	{
		case MEASURE_US_LEFT_HIGH:
			I2CTWI_getReceivedData( messageBuf, 2 );
			dist_tmp = messageBuf[0];
			I2CTWI_transmitByte(SRF_LEFT_ADR, 3);
			I2CTWI_requestDataFromDevice(SRF_LEFT_ADR, MEASURE_US_LEFT_LOW, 1);
		break;
		case MEASURE_US_LEFT_LOW:
			I2CTWI_getReceivedData( messageBuf, 2 );
			distance_left = messageBuf[0] + (dist_tmp << 8);
			writeString_P("DistanceL: ");
			writeInteger(distance_left, DEC);
			writeString_P(" cm ");
		break;
		case MEASURE_US_RIGHT_HIGH:
			I2CTWI_getReceivedData( messageBuf, 2 );
			dist_tmp = messageBuf[0];
			I2CTWI_transmitByte(SRF_RIGHT_ADR, 3);
			I2CTWI_requestDataFromDevice(SRF_RIGHT_ADR, MEASURE_US_RIGHT_LOW, 1);
		break;
		case MEASURE_US_RIGHT_LOW:
			I2CTWI_getReceivedData( messageBuf, 2 );
			distance_right = messageBuf[0] + (dist_tmp << 8);
			writeString_P("\t\tDistanceR: ");
			writeInteger(distance_right, DEC);
			writeString_P(" cm\n");
		break;
	}
}

/**
 * 
 */
void I2C_transmissionError(uint8_t errorState)
{
	writeString_P("\nI2C ERROR --> TWI STATE IS: 0x");
	writeInteger(errorState, HEX);
	writeChar('\n');
}

/*****************************************************************************/

/**
 * Same as in the last example - but now we fire two SRF Sensors alternating.
 */
void task_SRF(void)
{
	static uint8_t measureInProgress = false;
	static uint8_t channel = 0;
	if(!measureInProgress) // Start measurement ONCE only
	{
		if(TWI_operation == I2CTWI_NO_OPERATION) // If there is no request in progress...
		{
			if(channel == 0)
				I2CTWI_transmit2Bytes(SRF_LEFT_ADR, 0, 81);
			else if(channel == 1)
				I2CTWI_transmit2Bytes(SRF_RIGHT_ADR, 0, 81);
			measureInProgress = true;
			setStopwatch1(0);
		}
	}
	else if(getStopwatch1() > 70) // 70ms (measurement delay)
	{
		if(channel == 0)
		{
			I2CTWI_transmitByte(SRF_LEFT_ADR, 2);
			I2CTWI_requestDataFromDevice(SRF_LEFT_ADR, MEASURE_US_LEFT_HIGH, 1);
			channel = 1;
		}
		else if(channel == 1)
		{
			I2CTWI_transmitByte(SRF_RIGHT_ADR, 2);
			I2CTWI_requestDataFromDevice(SRF_RIGHT_ADR, MEASURE_US_RIGHT_HIGH, 1);
			channel = 0;
		}
		measureInProgress = false;
		setStopwatch1(0);
	}
}

/*****************************************************************************/


#define PCF8574_8LEDS_ADR  0x70
#define PCF8591_4LDRS_ADR  0x90

/**
 * Read 4x PCF8591 LDRs - we do not take advantage of the method we use for
 * the SRF Sensors above!
 */
void task_ext_LDRs(void)
{
	if(getStopwatch2() > 200) 
	{
		I2CTWI_transmitByte(PCF8591_4LDRS_ADR, 0b01000100 );  // 64 + 4); 
		
		uint8_t results[6];
		I2CTWI_readBytes(PCF8591_4LDRS_ADR,results,5);
		// Byte 0 contains last conversion result - thus we skip it and
		// begin with results[1]:
		writeString_P(" | LDR1:"); writeInteger(results[1], DEC); 
		writeString_P(" | LDR2:"); writeInteger(results[2], DEC); 
		writeString_P(" | LDR3:"); writeInteger(results[3], DEC); 
		writeString_P(" | LDR4:"); writeInteger(results[4], DEC); 
		writeChar('\n');
		setStopwatch2(0);
	}
}

/**
 * The Knight Rider running Light once again:
 */
void task_knightRider(void)
{
	static uint8_t runningLight = 3; 
	static uint8_t dir = 0;
	if(getStopwatch3() > 70) {
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
		setStopwatch3(0);	
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
	
	powerON();

	startStopwatch1();
	startStopwatch2();
	startStopwatch3();
	
	while(true)  
	{
		task_knightRider();
		task_ext_LDRs();
		task_SRF();
		task_I2CTWI();
		task_RP6System();
	}
	return 0;
}
