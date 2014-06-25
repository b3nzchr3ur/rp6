/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - RP6 CONTROL M32 Examples
 * ****************************************************************************
 * Example: External EEPROM
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 *
 * Simple test program for RP6 Control M32. You can use it to verify that
 * everything works fine. (at the start you have to press all buttons and
 * at the end you need to hit the microphone 3 times...)
 *
 * Just as the RP6Base_SELFTEST, also this program is not meant to be nice...
 *
 * ############################################################################
 * The Robot does NOT move in this example! You can simply put it on a table
 * next to your PC and you should connect it to the PC via the USB Interface!
 * ############################################################################
 * ****************************************************************************
 */
 
/*****************************************************************************/
// Includes:

#include "RP6ControlLib.h" 		// The RP6 Control Library. 
								// Always needs to be included!

/*****************************************************************************/

#include "RP6I2CmasterTWI.h"

// define the RP6 Base address here:
#define I2C_RP6_BASE_ADR 10

uint8_t errors = 0;

char receiveBuffer[UART_RECEIVE_BUFFER_SIZE];

void done(void)
{
	writeString_P("Done!\n"); 
}

void bars(uint8_t number)
{
	uint8_t cnt;
	writeChar('\n');
	for(;number > 0; writeChar('\n'), number--)
		for(cnt = 69; cnt ; cnt--)
			writeChar('#');
}

void test(uint8_t number)
{
	bars(2);
	writeString_P("#### TEST #");
	writeInteger(number, DEC);
	writeString_P(" ####\n");
}


/**
 * This function gets called automatically if there was an I2C Error like
 * the slave sent a "not acknowledge" (NACK, error codes e.g. 0x20 or 0x30).
 */
void I2C_transmissionError(uint8_t errorState)
{
	writeString_P("\n############ I2C ERROR!!!!! - TWI STATE: 0x");
	writeInteger(errorState, HEX);
	writeChar('\n');
	errors++;
}

/*****************************************************************************/
// Main function - The program starts here:

int main(void)
{
	initRP6Control(); // Always call this first! The Processor will not work
					  // correctly otherwise. 

	bars(2);
	writeString_P("\n\nRP6Control Selftest!\n\n"); 
	bars(2);
	setLEDs(0b1111);
	mSleep(50);
	initLCD(); 
	showScreenLCD("################", "################");

	mSleep(400);
	showScreenLCD("################", "################");
	showScreenLCD("RP6Control M32", "SELFTEST");

	mSleep(1000);
	
	uint8_t keynumber = 0;
	while(keynumber < 6)
	{
		uint8_t key = checkReleasedKeyEvent(); 
		if(key == keynumber)
		{
			keynumber++;
			showScreenLCD("PRESS BUTTON", "NUMBER ");
			writeIntegerLCD(keynumber,DEC);
			setLEDs(0b0000);
			writeString_P("### PRESS BUTTON NUMBER ");
			writeInteger(keynumber,DEC);
			writeString_P("!\n");
		}
	}
	
	
	showScreenLCD("Testing", "BEEPER & LEDS");
	mSleep(250);
	// Play a sound to indicate that our program starts:
	sound(50,50,100); setLEDs(0b0000);
	sound(80,50,100); setLEDs(0b0001);
	sound(100,50,100);setLEDs(0b0010);
	sound(120,50,100);setLEDs(0b0100);
	sound(140,50,100);setLEDs(0b1000);
	sound(160,50,100);setLEDs(0b1001);
	sound(180,50,100);setLEDs(0b1011);
	sound(200,50,100);setLEDs(0b1111);
	mSleep(400);
	setLEDs(0b0000);

	showScreenLCD("Testing", "EERPOM");
	
	test(1);
	writeString_P("\nEEPROM TEST\n");
	writeString_P("\nErasing 250 Bytes...\n");
	
	uint8_t cnt;
	for(cnt = 0; cnt < 250; cnt++)
	{
		SPI_EEPROM_writeByte(cnt, 0xFF);
		while(SPI_EEPROM_getStatus() & SPI_EEPROM_STAT_WIP);
	}

	writeString_P("...Done!\nWriting 250 Bytes to EEPROM:\n");
	for(cnt = 0; cnt < 250; cnt++)
	{
		writeIntegerLength(cnt, DEC, 3);
		SPI_EEPROM_writeByte(cnt, cnt);
		while(SPI_EEPROM_getStatus() & SPI_EEPROM_STAT_WIP);
		writeChar(',');
		if(cnt % 10 == 0) writeChar('\n');
	}
	
	mSleep(400);
	setLEDs(0b1111);
	writeString_P("\nReading and verifying:\n");
	
	for(cnt = 0; cnt < 250; cnt++)
	{
		uint8_t result = SPI_EEPROM_readByte(cnt);
		if(result != cnt)
		{
			writeString_P("\nEEPROM VERIFY ERROR!!!! EEPROM DAMAGED!!!\n");
			writeString_P("Data read: "); writeInteger(result,DEC);
			writeString_P(", should be: "); writeInteger(cnt,DEC); writeChar('\n');
			errors++;
		}
		else
			writeIntegerLength(result,DEC,3);
		writeChar(',');
		if(cnt % 10 == 0) writeChar('\n');
	}
	
	writeString_P("\nErasing 250 Bytes...\n");
	for(cnt = 0; cnt < 250; cnt++)
	{
		SPI_EEPROM_writeByte(cnt, 0xFF);
		while(SPI_EEPROM_getStatus() & SPI_EEPROM_STAT_WIP);
	}
	
	mSleep(400);
	setLEDs(0b0000);
	writeString_P("\nEEPROM TEST DONE!\n");
	writeString_P("\nI2C TWI TEST:\n");
	showScreenLCD("I2C TWI", "TEST");
	
	
	I2CTWI_initMaster(100);  
	I2CTWI_setTransmissionErrorHandler(I2C_transmissionError);
	
	uint8_t runningLight = 1;
	for(cnt = 0; cnt < 24; cnt++)
	{
		writeIntegerLength(cnt,DEC,3);
		writeChar(':');
		writeIntegerLength(runningLight,DEC,3);
		writeChar(',');
		writeChar(' ');

		I2CTWI_transmit3Bytes(I2C_RP6_BASE_ADR, 0, 3, runningLight);
		I2CTWI_transmitByte(I2C_RP6_BASE_ADR, 29);
		uint8_t result = I2CTWI_readByte(I2C_RP6_BASE_ADR);
		if(result != runningLight) 
		{
			writeString_P("\nTWI TEST ERROR!\n");
			errors++;
		}
		runningLight <<= 1; 
		if(runningLight > 32) 
			runningLight = 1;
	
		if((cnt+1) % 6 == 0) writeChar('\n');
		mSleep(100);
	}
	I2CTWI_transmit3Bytes(I2C_RP6_BASE_ADR, 0, 3, 0);
	
	writeString_P("\nTWI TEST DONE!\n");
	writeString_P("\nMicrophone Test\n");
	writeString_P("Hit the Microphone three times with your finger!\n");
	showScreenLCD("MIC TEST:", "");
	
	#define PREPARE 1
	#define WAIT 2
	
	uint8_t state = PREPARE;

	startStopwatch2();
	while(true)
	{
		static uint8_t peak_count = 3;
		if(state == PREPARE)
		{
			if(getStopwatch2() > 250)
			{
				setCursorPosLCD(1, 6); 
				writeIntegerLengthLCD( peak_count, DEC, 1);
				dischargePeakDetector();
				state = WAIT;
				setStopwatch2(0);
			}
		}
		else if(state == WAIT)
		{
			uint8_t key = checkReleasedKeyEvent(); 
			if(key)
			{
				break;
			}
			if(getStopwatch2() > 50)
			{
				uint16_t tmp = getMicrophonePeak();
				if(tmp > 4)
				{
					externalPort.LEDS = 0;
					uint16_t i;
					uint8_t j;
					for(i = 0, j = 2; i < tmp; i+= 40)
					{
						if(i < 40)
						{
							externalPort.LEDS++;
						}
						else
						{
							externalPort.LEDS <<=1;
							externalPort.LEDS++;
						}
					}
					outputExt();
					if(tmp > 120)
					{
						state = PREPARE;
						peak_count--;
					}
					if(peak_count == 0)
						break;
				}
				else
					setLEDs(0b0000);
				setStopwatch2(0);
			}
		}
	}

	writeString_P("\nMICROPHONE TEST DONE!\n");
	showScreenLCD("ALL TESTS", "DONE!");
	
	writeString_P("\n\n\n\n");
	bars(2);
	writeString_P("\n\nALL TESTS DONE!\n\n");
	
	if(errors)
	{
		bars(4);
		writeString_P("\nERROR ERROR ERROR ERROR ERROR ERROR ERROR\n");
		writeString_P("\nATTENTION: TESTS FINISHED WITH ERRORS!!!\n");
		writeString_P("PLEASE CHECK RP6-M32 ASSEMBLY!!!\n\n");
		bars(4);
		writeString_P("\n\n");
	}
	
	// Now we just show a running light...
	startStopwatch1();
	
	uint8_t runLEDs = 1; 
	uint8_t dir = 0;
	
	while(true) 
	{
		if(getStopwatch1() > 100) {
			setLEDs(runLEDs); 
			if(dir == 0)
				runLEDs <<= 1; 
			else
				runLEDs >>= 1;
			if(runLEDs > 7 ) 
				dir = 1;			
			else if (runLEDs < 2 ) 
				dir = 0;
			setStopwatch1(0);
		}
	}
	return 0;
}
