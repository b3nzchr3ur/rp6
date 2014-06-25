/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - RP6 CONTROL M256 Examples
 * ****************************************************************************
 * Example: ADC, I/O Ports and WIFI Interface Example
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 * This Demo Program shows how to use the I/Os and ADCs as inputs and transmit
 * their values via WIFI to the PC. 
 * It also showcases that you can transmit rather large messages via the WIFI
 * Module. 
 *
 *
 * The Program sets the packet size to 1340 by default as this is the
 * exact size of the message that is sent out. 
 * It is also recommended to set the flush timer 
 * value between 10 and 25ms. 
 *
 * To change this manually for all other programs, run the following in 
 * wifi command mode (on serial interface): 
 *
 * set comm size 1460
 * set comm time 10
 * save
 * reboot
 *
 * The commands in the main function override these settings
 * until the module is rebooted. 
 *
 *
 *
 * ############################################################################
 * The Robot does NOT move in this example! You can simply put it on a table
 * next to your PC and you should connect it to the PC via the USB Interface!
 * You should also connect to it via WIFI.
 * ############################################################################
 * ****************************************************************************
 */

/*****************************************************************************/
// Includes:

#include "RP6M256Lib.h" 					// The RP6 M256 Library. 
											// Always needs to be included!

/*****************************************************************************/

/**
 * Show a running light...
 */
void runningLight(void)
{
	static uint8_t runLight = 1; 
	static uint8_t dir;  
	
	if(getStopwatch1() > 100)
	{
		setLEDs(runLight); 
	
		if(dir == 0)
			runLight <<= 1; 
		else
			runLight >>= 1;
			
		if(runLight > 7 ) 
			dir = 1;			
		else if (runLight < 2 ) 
			dir = 0;
		
		setStopwatch1(0);
	}
}


uint16_t sample_count = 0; 

/**
 * This routine shows how to sample all ADC Channels and I/O Ports and output their value
 * via the WIFI Interface. 
 * It also outputs some other stuff in order to create a large (~ 1370 Bytes) Text message.
 * This text message is send out at a defined refresh rate. 
 * In your own application the Text could be replaced by sensor values.
 * There are a lot of space fillers and 
 * 
 * ATTENTION: Make sure the packet size is larger than all Text that is sent at once. 
 * Otherwise data loss may occur!
 *
 */
void sampleAllADCs(void)
{
	
	if(getStopwatch4() > 100)  
	{	
	/*
		You can change this to make the update rate faster.			  
		Down to 25ms (= 40Hz update rate) is feasible with good connection quality and
		if you make the message size SIGNIFICANTLY smaller than in this example.
		The quite large message below takes about 30ms to transmit @500kBit/s. 
		And afterwards you need to make a small break to allow the data to be sent out. 
		
		With faster update rate, data loss and larger latencies may occur. 
		It is recommended to keep update rate well below 40Hz (>25ms) even with small
		data size. 
		
		Please note that the network traffic can take some time to
		reach the destination. Also the WLAN module collects data for
		typically 10ms or until the buffer is full and then sends one packet
		for all buffered data. 
		 
		So this behaves a bit different than a normal serial port which just
		sends out data as it comes in. 
		When you write your own Applications it is a good idea to
		send out all data at once - like it is done here - and add at least
		15ms Pause afterwards (depending on flush timer value also longer). 
		Keep the data size lower than the packet size!
		Max packet size is 1460 chars. 

		To change this run the following in wifi command mode (on serial interface): 
		set comm size 1460
		save
		reboot

		You can also change the flush timer size:
		
		set comm time 10
		save
		reboot
		
		Hint: You can also configure different bitrates e.g. 
		to 250 or 100kBit/s in RobotLoader.
		It stores these settings in the EEPROM and the Library automatically 
		sets the baudrate of the UART connected to the WIFI Module accordingly. 

	*/
	
		setStopwatch8(0);
		
		uint8_t chan;
		for(chan = 0; chan < 16; chan++) {
			uint16_t result = readADC(chan); // Read the ADC value of a specific channel
			writeIntegerLength_WIFI(chan,DEC,2);
			writeString_WIFI("=> ");
			writeIntegerLength_WIFI(result,DEC,4);
			writeString_P_WIFI("  ");
			
			// Display moving lines of stars: 
			uint8_t pos = 15 - (sample_count % 15);
			uint8_t rl = 0;
			for(; rl < pos; rl++)
				writeChar_WIFI(' ');
			writeChar_WIFI('*');		
			for(; rl < 15; rl++)
				writeChar_WIFI(' ');
				
			// Just to see some more changing output, display a counter 
			// as DECimal and BINary:
			writeString_P_WIFI(" CNT:");
			writeIntegerLength_WIFI(sample_count,DEC,5);
			writeString_P_WIFI("  0b");
			writeIntegerLength_WIFI(sample_count,BIN,16);
			writeString_P_WIFI(" #");
			writeChar_WIFI('\n');
			
			sample_count++;
		}
		
		
		// Output I/O Port values as HEX Strings,  note that also outputs are included so the 
		// running light will change value of PINC 
		writeString_P_WIFI("###########################################################\n");
		writeString_P_WIFI("PORTS: ");
		writeIntegerLength_WIFI(PINA,HEX, 2);writeChar_WIFI(' ');
		writeIntegerLength_WIFI(PINB,HEX, 2);writeChar_WIFI(' ');
		writeIntegerLength_WIFI(PINC,HEX, 2);writeChar_WIFI(' ');
		writeIntegerLength_WIFI(PIND,HEX, 2);writeChar_WIFI(' ');
		writeIntegerLength_WIFI(PINE,HEX, 2);writeChar_WIFI(' ');
		writeIntegerLength_WIFI(PINF,HEX, 2);writeChar_WIFI(' ');
		writeIntegerLength_WIFI(PING,HEX, 2);writeChar_WIFI(' ');
		writeIntegerLength_WIFI(PINH,HEX, 2);writeChar_WIFI(' ');
		writeIntegerLength_WIFI(PINJ,HEX, 2);writeChar_WIFI(' ');
		writeIntegerLength_WIFI(PINK,HEX, 2);writeChar_WIFI(' ');
		writeIntegerLength_WIFI(PINL,HEX, 2);writeChar_WIFI(' ');
		
		writeString_P_WIFI("      #############");
		writeChar_WIFI('\n');
		
		// Display Stopwatch values: 
		writeString_P_WIFI("###########################################################\n");
		writeString_P_WIFI("STOPWATCH: ");   
		writeIntegerLength_WIFI(getStopwatch1(), DEC, 5);writeChar_WIFI(' ');
		writeIntegerLength_WIFI(getStopwatch2(), DEC, 5);writeChar_WIFI(' ');
		writeIntegerLength_WIFI(getStopwatch3(), DEC, 5);writeChar_WIFI(' ');
		// Stopwatch 4 has constant value here thus not displayed. 
		writeIntegerLength_WIFI(getStopwatch5(), DEC, 5);writeChar_WIFI(' ');
		writeIntegerLength_WIFI(getStopwatch6(), DEC, 5);writeChar_WIFI(' ');
		writeIntegerLength_WIFI(getStopwatch7(), DEC, 5);writeChar_WIFI(' ');
		writeIntegerLength_WIFI(getStopwatch8(), DEC, 5);writeChar_WIFI(' ');
		writeString_P_WIFI("  ####");
		writeChar_WIFI('\n');
		writeString_P_WIFI("###########################################################\n");
		setStopwatch4(0);
	}
}


/*****************************************************************************/
// Main function - The program starts here:

int main(void)
{
	initRP6M256();    // Always call this first! The Processor will not work
					  // correctly otherwise. 
	
	initLCD(); // Initialize the LC-Display (LCD)
			   // Always call this before using the LCD!

	// Write some text messages to the UART - just like on RP6Base:
	writeString_P("\n\n   _______________________\n");
	writeString_P("   \\| RP6  ROBOT SYSTEM |/\n");
	writeString_P("    \\_-_-_-_-_-_-_-_-_-_/\n\n");
	
	writeString_P("Hello World from RP6 CONTROL M256 WIFI!\n"); 
	writeString_P("Let's move it!  :)\n"); 
	writeString_P("This message was sent via ***Serial Port***!\n");
	

	// Now write Text via the WIFI Interace:
	writeString_P_WIFI("\n\n   _______________________\n");
	writeString_P_WIFI("   \\| RP6  ROBOT SYSTEM |/\n");
	writeString_P_WIFI("    \\_-_-_-_-_-_-_-_-_-_/\n\n");
	// As you can see it is exactly the same as with the UART just with an added _WIFI
	// at the end. 
	
	writeString_P_WIFI("Hello World from RP6 CONTROL M256 WIFI!\n"); 
	writeString_P_WIFI("Let's move it!  :)\n"); 
	writeString_P_WIFI("This message was sent via ***WLAN***!\n\n"); 


	writeString_P("\nCheck WIFI Terminal output!\n"); 


	// Set the four Status LEDs:
	setLEDs(0b1111);
	mSleep(100);
	
	showScreenLCD("################", "################");
	mSleep(500);
	setLEDs(0b0000);
	showScreenLCD("RP6v2-M256-WIFI ", "<<ADC / IO>>");
	mSleep(1000); 
	showScreenLCD("ADC / IO Input", "Example Program");


	// Make sure WLAN Module Packet size / buffer size and flush timer is OK
	enter_cmd_mode_WIFI();   
	issueCMD_WIFI("set comm size 1340","AOK"); 
	issueCMD_WIFI("set comm time 10","AOK"); 
	leave_cmd_mode_WIFI();  
	
	
	// Start all the stopwatches for the output example:
	startStopwatch1();
	startStopwatch2();
	startStopwatch3();
	startStopwatch4();
	startStopwatch5();
	startStopwatch6();
	startStopwatch7();
	startStopwatch8();
	
	while(true) 
	{
//		runningLight();
		sampleAllADCs();
	}
	return 0;
}
