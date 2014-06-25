// Uncommented Version of RP6Base_I2C_Master_03.c!
// written by Dominik S. Herwald
// ------------------------------------------------------------------------------------------

#include "RP6RobotBaseLib.h" 	
#include "RP6I2CmasterTWI.h"  

#define SRF_ADR  0xE6


void changeSRFAddr(uint8_t old_addr, uint8_t new_addr)
{
	I2CTWI_transmit2Bytes(old_addr, 0, 0xA0);
	I2CTWI_transmit2Bytes(old_addr, 0, 0xAA);
	I2CTWI_transmit2Bytes(old_addr, 0, 0xA5);
	I2CTWI_transmit2Bytes(old_addr, 0, new_addr);
}


#define MEASURE_US_LOW 	0
#define MEASURE_US_HIGH 1

uint16_t distance;

void I2C_requestedDataReady(uint8_t dataRequestID)
{
	uint8_t messageBuf[8];
	static uint8_t dist_tmp;
	switch(dataRequestID)
	{
		case MEASURE_US_HIGH: 
			I2CTWI_getReceivedData(messageBuf, 2);
			dist_tmp = (messageBuf[0]);
			I2CTWI_transmitByte(SRF_ADR, 3);
			I2CTWI_requestDataFromDevice(SRF_ADR, MEASURE_US_LOW, 1);
		break;
		case MEASURE_US_LOW:
			I2CTWI_getReceivedData(messageBuf, 2);
			distance = messageBuf[0] + (dist_tmp << 8);
		
			writeString_P("Distance: ");
			writeInteger(distance, DEC);
			writeString_P(" cm | ");
			uint16_t dist_count;
			for(dist_count = distance; dist_count > 4; dist_count-=2)
				writeChar('#');
			writeChar('\n');
		break;
	}
}

void I2C_transmissionError(uint8_t errorState)
{
	writeString_P("\nI2C ERROR --> TWI STATE IS: 0x");
	writeInteger(errorState, HEX);
	writeChar('\n');
}

void task_SRF(void)
{
	static uint8_t measureInProgress = false;
	if(!measureInProgress) 
	{
		if(TWI_operation == I2CTWI_NO_OPERATION)
		{
			I2CTWI_transmit2Bytes(SRF_ADR, 0, 81);
			measureInProgress = true;
			setStopwatch1(0);
		}
	}
	else if(getStopwatch1() > 70)
	{
		I2CTWI_transmitByte(SRF_ADR, 2);
		I2CTWI_requestDataFromDevice(SRF_ADR, MEASURE_US_HIGH, 1);
		measureInProgress = false;
		setStopwatch1(0);
	}
}

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
	
	while(true)  
	{
		task_SRF();
		task_I2CTWI();
		task_RP6System();
	}
	return 0;
}
