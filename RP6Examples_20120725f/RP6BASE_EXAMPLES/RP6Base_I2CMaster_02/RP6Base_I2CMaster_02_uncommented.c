// Uncommented Version of RP6Base_I2C_Master_02.c!
// written by Dominik S. Herwald
// ------------------------------------------------------------------------------------------

#include "RP6RobotBaseLib.h" 	
#include "RP6I2CmasterTWI.h"

void I2C_transmissionError(uint8_t errorState)
{
	writeString_P("\nI2C ERROR --> TWI STATE IS: 0x");
	writeInteger(errorState, HEX);
	writeChar('\n');
}

#define PCF8574_8LEDS_ADR  0x70
#define PCF8591_4LDRS_ADR  0x90

void task_ext_LDRs(void)
{
	if(getStopwatch2() > 200) 
	{
		I2CTWI_transmitByte(PCF8591_4LDRS_ADR, 0b01000100 ); 
		 
		uint8_t results[6];
		I2CTWI_readBytes(PCF8591_4LDRS_ADR,results,5);
		writeString_P(" | LDR1:"); writeInteger(results[1], DEC); 
		writeString_P(" | LDR2:"); writeInteger(results[2], DEC); 
		writeString_P(" | LDR3:"); writeInteger(results[3], DEC); 
		writeString_P(" | LDR4:"); writeInteger(results[4], DEC); 
		writeChar('\n');
		
		setStopwatch2(0);
	}
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
