/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - RP6 CONTROL M32 Examples
 * ****************************************************************************
 * Example: External EEPROM
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 * The External EEPROM can be used for Datalogging or as program memory for
 * Bytecode Interpreters (Java VMs for example NanoVM).
 * This program shows basical write and read operations.
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

// Our read and write Buffers: 
uint8_t write_buffer[64];
uint8_t read_buffer[128];

/*****************************************************************************/
// Main function - The program starts here:

int main(void)
{
	initRP6Control(); // Always call this first! The Processor will not work
					  // correctly otherwise. 

	writeString_P("\n\nRP6Control SPI EEPROM Example Program!\n"); 
	setLEDs(0b1111);
	mSleep(50);
	initLCD(); 
	showScreenLCD("################", "################");

	// Play a sound to indicate that our program starts:
	sound(100,40,64);
	sound(170,40,0);
	mSleep(400);
	setLEDs(0b0000);

	showScreenLCD("SPI EEPROM", "Example Program");
	mSleep(1000);

	writeString_P("\nReading memory before write operation:\n");
	
	// We read two pages of the EEPROM to show what the EEPROM Values
	// were before we wrote any data to them:
	SPI_EEPROM_readBytes(0, &read_buffer[0], 127);
	
	
	// And now we output the read data on the serial interface:
	uint8_t cnt;
	for(cnt = 0; cnt < 127; cnt++)
	{
		writeInteger(read_buffer[cnt],DEC);
		writeChar(',');
	}
	writeString_P("\nDone!\n"); 


	writeString_P("\n\nWriting EEPROM memory:\n");
	
	// Prepare buffer with some test data and output
	// this on the serial interface: 
	for(cnt = 0; cnt < 64; cnt++)
	{
		write_buffer[cnt] = cnt;
		writeInteger(write_buffer[cnt],DEC);
		writeChar(',');
	}
	
	// Write the Buffer to the EEPROM:
	SPI_EEPROM_writeBytes(0, &write_buffer[0],64);
	// You can only write 64 Bytes at once. 64 Bytes
	// is the page size. You can not write across the
	// page boundaries in a single write - then you need to
	// split the writes up. 
	// s. EEPROM Datasheet for more details on this!
	
	// Wait until writing is finished:
	while(SPI_EEPROM_getStatus() & SPI_EEPROM_STAT_WIP);
	// (you do not need to wait for this - we only do this
	// here to see when it is finished. This check is 
	// also performed at the beginning of the write and
	// read EEPROM functions...) 

	writeString_P("\nDone!\nReading written memory:\n");

	// To verify that we have really written the data correctly,
	// we read a bigger area of the memory:
	SPI_EEPROM_readBytes(0, &read_buffer[0], 127);
	// (You can read as many databytes from the EEPROM as you 
	// like - there is no need to observe page boundaries like it
	// is neccessary for write operations...)
	
	// And now we output the read data on the serial interface:
	for(cnt = 0; cnt < 127; cnt++)
	{
		writeInteger(read_buffer[cnt],DEC);
		writeChar(',');
	}
	writeString_P("\nDone!\n"); 

	// -------------------------------------------

	// We can also write one single byte:
	writeString_P("\nWriting 128 to byte with address 4:\n");
	SPI_EEPROM_writeByte(4, 128);
	while(SPI_EEPROM_getStatus() & SPI_EEPROM_STAT_WIP);
	writeString_P("\nDone!\n"); 
	
	// And we can read single bytes directly - step by step
    // with the function SPI_EEPROM_readByte:
	writeString_P("\nReading written memory - step by step:\n");
	for(cnt = 0; cnt < 127; cnt++)
	{
		writeInteger(SPI_EEPROM_readByte(cnt), DEC);
		writeChar(',');
	}
	writeString_P("\nDone!\n"); 

	// Play a sound to indicate that we finished our small test:
	sound(150,20,64);
	sound(150,40,0);
	showScreenLCD("EERPOM TEST", "DONE!");
	
	
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
