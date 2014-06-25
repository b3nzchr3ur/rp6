/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - ROBOT BASE EXAMPLES
 * ****************************************************************************
 * Example: Light Sensors
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 *
 * A LDR test program. Shows which sensor detects brighter light with the LEDs
 * and also prints info about this to the UART.
 *
 * In the next example we will integrate this into our Behaviour controlled
 * Robot. Together with the small bargraph difference display with the LEDs. 
 *
 * ############################################################################
 * The Robot does NOT move in this example! You can simply put it on a table
 * next to your PC and you should connect it to the PC via the USB Interface!
 * ############################################################################
 * ****************************************************************************
 */
 
#include "RP6RobotBaseLib.h" 

/*****************************************************************************/
// LDRs

#define RANGE 15       // The range within the sensor values 
					   // are considered to be "equal" 

/**
 * Print out some infos about current LDR status and sets status LEDs.
 */
void task_LDRinfo(void)
{
	if(getStopwatch1() > 200)  // 200ms
	{
		// Write LDR values to UART:
		writeString_P("LDR_L: ");
		writeIntegerLength(adcLSL, DEC, 4);
		writeString_P(" ; LDR_R: ");
		writeIntegerLength(adcLSR, DEC, 4);
		
		// Which one is bigger or are both "equal" (within a specified range)?
		writeString_P(" ||");
		if(adcLSL >= RANGE && adcLSR >= RANGE) // Is the light bright enough?
		{
			int16_t dif = ((int16_t)(adcLSL - adcLSR));
			if(dif > 160) dif = 160;
			if(dif < -160) dif = -160;

			// Set LEDs - a small bargraph display that shows which sensor has detected
			// brighter light - and how much brighter it is:
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
				
			if(adcLSL < (adcLSR - RANGE))  // Is Left < Right?
				writeString_P("   L < R # ");
			else if(adcLSR < (adcLSL - RANGE)) 
				writeString_P(" # L > R   ");
			else  // Is Left = Right - within specified range?
				writeString_P("   L = R   ");
			
		}
		else  // The light is not bright enough - it is very dark!
			  // Here we do not use the "range" parameter and just check if one
			  // sensor value is bigger than the other...
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
		
		// Show Battery voltage:
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


/*****************************************************************************/
// Main:

int main(void)
{
	initRobotBase();
    writeString_P("\nRP6Loader - LDR Testprogram\n");
	writeString_P("___________________________\n\n");

	setLEDs(0b111111);
	mSleep(1000);
	setLEDs(0b000000);

	startStopwatch1();

	// Main loop 
	while(true) 
	{
		task_LDRinfo();
		task_RP6System();
	}
	return 0;
}
