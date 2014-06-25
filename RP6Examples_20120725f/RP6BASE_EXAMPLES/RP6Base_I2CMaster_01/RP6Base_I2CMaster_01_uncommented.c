// Uncommented Version of RP6Base_I2C_Master_01.c
// written by Dominik S. Herwald
// ------------------------------------------------------------------------------------------

#include "RP6RobotBaseLib.h" 	
#include "RP6I2CmasterTWI.h"   

#define PCF8574_8LEDS_ADR  0x70

void I2C_requestedDataReady(uint8_t dataRequestID)
{
}

void I2C_transmissionError(uint8_t errorState)
{
	writeString_P("\nI2C ERROR - TWI STATE: 0x");
	writeInteger(errorState, HEX);
	writeChar('\n');
}

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
		task_knightRider();
		task_I2CTWI();
		task_RP6System();
	}
	return 0;
}
