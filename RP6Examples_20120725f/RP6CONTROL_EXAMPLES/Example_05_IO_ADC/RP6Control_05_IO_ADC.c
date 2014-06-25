/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - RP6 CONTROL M32 Examples
 * ****************************************************************************
 * Example: I/Os and ADCs
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 * This Demo Program show how you can use the free ADCs and I/Os 
 * on the RP6 CONTROL M32 Board. 
 *
 * 		THIS PROGRAM DOES NOT DO ANYTHING USEFUL! 
 *		You need to change it and add your own routines
 *		for the specific hardware you want to control!
 *		It only shows how this works in general... 
 *		... and outputs the values of ADC2 and ADC7 at the end 
 *      in an infinite loop.
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

/*****************************************************************************/
// Main function - The program starts here:


int main(void)
{
	initRP6Control(); // Always call this first! The Processor will not work
					  // correctly otherwise. 

	initLCD(); 
	
	writeString_P("\n\nRP6Control I/O and ADC Example Program!\n"); 
	setLEDs(0b1111);

	showScreenLCD("################", "################");

	// Play a sound to indicate that our program starts:
	sound(100,40,64);
	sound(170,40,0);
	mSleep(400);
	setLEDs(0b0000);

	showScreenLCD("I/O and ADC", "Example Program");
	mSleep(1000);

	/*
		Here we will show how to set and read I/O pins. 
		
		You need to change this and add your own routines
		for the specific hardware you want to control!
		
		The 14 free I/O Pins are the following ones (definitions from RP6Control.h):
		
		ADC7 	(1 << PINA7)
		ADC6	(1 << PINA6)
		ADC5 	(1 << PINA5)
		ADC4 	(1 << PINA4)
		ADC3 	(1 << PINA3)
		ADC2 	(1 << PINA2)
		IO_PC7 	(1 << PINC7)
		IO_PC6 	(1 << PINC6)
		IO_PC5 	(1 << PINC5)
		IO_PC4 	(1 << PINC4)
		IO_PC3 	(1 << PINC3)
		IO_PC2 	(1 << PINC2)
		IO_PD6 	(1 << PIND6)
		IO_PD5 	(1 << PIND5)

		ADC2 - 7 are useable as I/Os or as Analog/Digital Converter Channels.
		IO_PC2 - 7 and IO_PD5 and IO_PD6 are only useable as I/Os.
		
		So you have free pins on PORTA, C and D.
		
		Please note the small difference in spelling ADC Channels and I/Os 
		(ADC_7 vs. ADC7)
	*/
	
	
	// When you want to use a port pin as output, you have to set the 
	// DDRx register bit belonging to this port to 1. 
	//
	// For example - if you want to use PORTC 7 as output, you can write:
	
	DDRC |= IO_PC7;  // PC7 is output
	
	// And then you can set the Port to high or low:
	
	
	PORTC |= IO_PC7;  // High
	writeString_P("\nPC7 is set to HIGH!\n\n");
	mSleep(1000);	  // wait 1s for example... 
	PORTC &= ~IO_PC7; // Low
	writeString_P("\nPC7 is set to LOW!\n\n");
	
	// When you want to use the Port as input to read its value,
	// you need to clear the DDRx register bit. 
	
	DDRC &= ~IO_PC6;  // PC6 is input
	
	PORTC |= IO_PC6;     // enable internal pullup resistor of PC6  OR ALTERNATIVELY:
	// PORTC &= ~IO_PC6  // disable pullup resistor of PC6
	// You need this when external sensors only pull the signal low
	// for example or if you disconnect the sensors or ... 
	
	// Now we want to output something depending on if this port pin is 
	// high or low:
	writeString_P("\nCheck PC6:");
	if(PINC & IO_PC6) // Check if PC6 is high
		writeString_P("\n\nPC6 is HIGH!\n\n");
	else
		writeString_P("\n\nPC6 is LOW!\n\n");
	
	
	// Hints for DDRx and PORTx Registers:
	// DDRx = 0 and PORTx = 0 ==> Input without internal Pullup
	// DDRx = 0 and PORTx = 1 ==> Input with internal Pullup
	// DDRx = 1 and PORTx = 0 ==> Output low
	// DDRx = 1 and PORTx = 1 ==> Output high
	// "=1" indicates that the appropriate bit is set.


	// To read the ADC channels, you can use the readADC() function. 
	// First you have to make the pins INPUTs, of course:
	DDRA &= ~ADC7;
	DDRA &= ~ADC2;
	

	// When you run this program with nothing connected
	// to the ADCs, you will most likely measure only junk
	// data - for example the ADC could show 210 or 623 or
	// anything else randomly. 
	
	// -------------------------------------------

	while(true) {
		writeString_P("\nADC7: ");
		uint16_t adc7 = readADC(ADC_7); // Read ADC Channel 7
		writeInteger(adc7, DEC);
		writeString(" | ADC2: ");
		uint16_t adc2 = readADC(ADC_2); // Read ADC Channel 2
		writeInteger(adc2, DEC);
		writeChar('\n');
		mSleep(500);
	}
	return 0;
}
