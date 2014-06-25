// Uncommented Version of RP6Base_LightDetection.c
// written by Dominik S. Herwald
// ------------------------------------------------------------------------------------------

#include "RP6RobotBaseLib.h" 


#define RANGE 15     

void task_LDRinfo(void)
{
	uint16_t left, right;

	if(getStopwatch1() > 200)
	{
		writeString_P("LDR_L: ");
		writeIntegerLength(adcLSL, DEC, 4);
		writeString_P(" ; LDR_R: ");
		writeIntegerLength(adcLSR, DEC, 4);
		
		writeString_P(" ||");
		if(adcLSL >= RANGE && adcLSR >= RANGE)
		{
			int16_t dif = ((int16_t)(adcLSL - adcLSR));
			if(dif > 160) dif = 160;
			if(dif < -160) dif = -160;

			if(dif > 140)
				setLEDs(0b111000);
			else if(dif > 75)
				setLEDs(0b011000);
			else if(dif > 35)
				setLEDs(0b001000);
			else if(dif > -35)
				setLEDs(0b001001);
			else if(dif > -75)
				setLEDs(0b000001);
			else if(dif > -140)
				setLEDs(0b000011);
			else 
				setLEDs(0b000111);
				
			if(adcLSL < (adcLSR - RANGE)) 
				writeString_P("   L < R # ");
			else if(adcLSR < (adcLSL - RANGE)) 
				writeString_P(" # L > R   ");
			else 
				writeString_P("   L = R   ");
		}
		else  
		{
			writeString_P(" It is very dark!");
			if(adcLSL < adcLSR) 
			{
				setLEDs(0b010110);
				writeString_P("   L < R # ");
			}
			else if(adcLSR < adcLSL) 
			{
				setLEDs(0b110010);
				writeString_P(" # L > R   ");
			}
			else 
			{
				setLEDs(0b011011);
				writeString_P("   L = R   ");
			}
		}
		
		writeString_P(" || UBAT: ");
		writeIntegerLength(adcBat, DEC, 3);
		writeChar('\n');
		if(adcBat < 600) 
		{
			writeString_P("WARNING: BAT IS LOW!\n");
			writeChar('\n');
		}
		setStopwatch1(0);
	}
}

int main(void)
{
	initRobotBase();
    writeString_P("\nRP6Loader - LDR Testprogram\n");
	writeString_P("___________________________\n\n");

	setLEDs(0b111111);
	mSleep(1000);
	setLEDs(0b000000);

	startStopwatch1();

	while(true) 
	{
		task_LDRinfo();
	}
	return 0;
}
