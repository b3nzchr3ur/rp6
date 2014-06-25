/* ****************************************************************************
 *                           _______________________
 *                           \| RP6  ROBOT SYSTEM |/
 *                            \_-_-_-_-_-_-_-_-_-_/           >>> RP6 M256 WIFI
 * ----------------------------------------------------------------------------
 * ------------------- [c]2011 / 2012 - AREXX ENGINEERING ---------------------
 * -------------------------- http://www.arexx.com/ ---------------------------
 * --------------------------- All rights reserved ----------------------------
 * ****************************************************************************
 * File: RP6M256Lib.c
 * Version: 1.1 - 16.07.2012
 * Target: RP6 CONTROL M256 WIFI - ATMEGA2560 @16.00MHz
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 * The RP6 CONTROL M256 function Library.
 *
 * ****************************************************************************
 * CHANGELOG AND LICENSING INFORMATION CAN BE FOUND AT THE END OF THIS FILE!
 * ****************************************************************************
 */

/*****************************************************************************/
// Includes:

#include "RP6M256Lib.h"



/*****************************************************************************/
// ADC:

/**
 * Read ADC channel (10 bit -> result is an integer from 0 to 1023).
 * The channels are defined in the RP6M256.h file.
 *
 * This is a blocking function, which means it waits until the conversion
 * is complete. 
 *
 * WARNING: The channel mux is more complex in MEGA2560 than MEGA32 - with 
 * differential and gain options. 
 *
 */
uint16_t readADC(uint8_t channel)
{
	if((ADCSRA & (1<<ADSC))) return 0; // check if ADC is buisy...
	if(channel >= 8) {
		ADCSRB = 0b1000; //(1 << MUX5);
	//	ADCSRB = (channel && 0b100000) <<MUX5;
	}
	else {
		ADCSRB = 0;
	}
	ADMUX = (1<<REFS0) | (0<<REFS1) | ((channel<<MUX0) & 0b111);
	ADCSRA = (0<<ADIE) | (1<<ADSC) | (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADIF);
	while ((ADCSRA & (1<<ADSC))); 
	ADCSRA |= (1<<ADIF);
	return ADC;
}

/*****************************************************************************/
// Hardware SPI Interface:
// DO NOT USE WHEN YOU USE THE microSD CARD!

/**
 * Writes a single Databyte to the SPI Interface.
 */
void writeSPI(uint8_t data) 
{     
	SPDR = data;    
	while(!(SPSR & (1<<SPIF)));
}

/**
 * Reads a single Databyte from the SPI Interface.
 */
uint8_t readSPI(void)
{
	writeSPI(0xFF);
	return SPDR;
}

/**
 * Reads TWO Bytes from the SPI Interface and returns them as
 * a 16 Bit value with first byte read in the upper 8 bits.
 */
uint16_t readWordSPI(void)
{
	uint16_t data = 0;
	data = readSPI() << 8;
	data |= readSPI();
	return data;
}

/**
 * Writes two Bytes contained in the 16 Bit parameter "data".
 * The first byte to be written needs to be in the upper 8 Bits.
 */
void writeWordSPI(uint16_t data)
{
	writeSPI((uint8_t)(data >> 8));
	writeSPI((uint8_t)data);
}

/** 
 * This function writes up to 255 Bytes to the SPI Interface.
 * The numer of bytes in the Buffer that shall be written is given 
 * by the parameter length.
 */
void writeBufferSPI(uint8_t *buffer, uint8_t length)
{
	uint8_t i = 0;
	for(; i < length; i++) {
		SPDR = buffer[i];    
		while(!(SPSR & (1<<SPIF)));
	}
}

/** 
 * Reads "length" Bytes from SPI Interface into the buffer.
 */
void readBufferSPI(uint8_t *buffer, uint8_t length)
{
	uint8_t i = 0;
	for(; i < length; i++) {
		SPDR = 0xFF;    
		while(!(SPSR & (1<<SPIF)));
		buffer[i] = SPDR;
	}
}


/*****************************************************************************/
// LEDs:


/** 
 * Set the 4 Status LEDs that are connected to the Microcontroller
 */
void setLEDs(uint8_t leds)
{
	PORTC &= ~0x3C; 
	PORTC |= ((leds << 2) & 0x3C);	
}	

/** 
 * Set ONLY LED1, don't change anything for the other LEDs.
 */
void setLED1(uint8_t led)
{
	PORTC &= ~SLED1; 
	PORTC |= (led > 0) ? SLED1 : 0;	
}	

/** 
 * Set ONLY LED2, don't change anything for the other LEDs.
 */
void setLED2(uint8_t led)
{
	PORTC &= ~SLED2; 
	PORTC |= (led > 0) ? SLED2 : 0;	
}	

/** 
 * Set ONLY LED3, don't change anything for the other LEDs.
 */
void setLED3(uint8_t led)
{
	PORTC &= ~SLED3; 
	PORTC |= (led > 0) ? SLED3 : 0;	
}	

/** 
 * Set ONLY LED4, don't change anything for the other LEDs.
 */
void setLED4(uint8_t led)
{
	PORTC &= ~SLED4; 
	PORTC |= (led > 0) ? SLED4 : 0;	
}	


/*****************************************************************************/
// LCD
// All LCD routines are prepared to control a 2x16 character LCD.
// If you want to connect a bigger LCD you need to change some things in 
// these routines! (especially in the initLCD, setCursorLCD and showScreenLCD
// routines)

char lcd_tmp_buffer[17];


/**
 * Sets the LCD ports without affecting the LEDs and also pulses the
 * enable line of the LCD to 'inform' the LCD about the new data.
 *
 */
void setLCDD(uint8_t lcdd)
{
	PORTA = lcdd; 
	sleep(5);
	PORTG |= DISP_EN_RD;
	delayCycles(150);
	PORTG &= ~DISP_EN_RD;
	delayCycles(20);
}

/**
 * Write a command to the LCD.
 */
void writeLCDCommand(uint8_t cmd)
{
	PORTG &= ~DISP_RS_ALE;
	setLCDD(cmd);
	delayCycles(500);
}

/**
 * Initialize the LCD. Always call this before using the LCD! 
 *
 */
void initLCD(void)
{
	delayCycles(34000);  //No need for Power ON delay as usually the
	// Bootloader should have been executed before...
	PORTG &= ~DISP_WR;
	PORTG &= ~DISP_RS_ALE;
	PORTG &= ~DISP_EN_RD;
	
	mSleep(5);
	
	writeLCDCommand(0b00110000);
	delayCycles(55500);
	mSleep(50);
	
	writeLCDCommand(0b00110000);
	delayCycles(55500);
	mSleep(5);
	
	writeLCDCommand(0b00110000);
	delayCycles(55500);
	
	mSleep(5);
	
	writeLCDCommand(0b00111000);
	delayCycles(55500);
	mSleep(1);
	
	writeLCDCommand(0b00001000);
	delayCycles(55500);
	mSleep(1);
	
	writeLCDCommand(0b00000001);
	delayCycles(55500);
	mSleep(1);
	
	writeLCDCommand(0b00000010);
	delayCycles(55500);
	mSleep(1);
		
	writeLCDCommand(0b00001100);
	delayCycles(55500);
	mSleep(1);

	mSleep(5);
	
	clearLCD();
}


/**
 * Clears the whole LCD!
 */
void clearLCD(void)
{
	writeLCDCommand(0b00000001);
	delayCycles(5500);
}


/**
 * Write a single character to the LCD.
 *
 * Example:
 *
 *			writeCharLCD('R');
 *			writeCharLCD('P');
 *			writeCharLCD('6');
 *			writeCharLCD(' ');
 *			writeCharLCD('0');
 *			writeCharLCD(48); // 48 is ASCII code for '0'
 *			writeCharLCD(49); // '1'
 *			writeCharLCD(50); // '2'
 *			writeCharLCD(51); // '3'
 *			//...
 *
 *			would output:
 *			RP6 00123
 *			at the current cursor position!
 *			use setCursorPos function to move the cursor to a 
 *			different location!
 */
void writeCharLCD(uint8_t ch)
{
	PORTG |= DISP_RS_ALE;
	setLCDD(ch);
	delayCycles(50);
}


/**
 * Writes a null terminated string from flash program memory to the LCD.
 * You can use the macro writeStringLCD_P(STRING); instead, this macro
 * ensures that the String is stored in program memory only!
 *
 * Example:
 *
 *			writeNStringLCD_P(PSTR("RP6 Control"));
 *
 *			// There is also a Macro that makes life easier and
 *			// you can simply write:
 *			writeStringLCD_P("RP6 Control");
 *
 */
void writeNStringLCD_P(const char *pstring)
{
    uint8_t c;
    for (;(c = pgm_read_byte_near(pstring++));writeCharLCD(c));
}

/**
 * Writes a String from SRAM to the LCD.
 */
void writeStringLCD(char *string)
{
	while(*string)
		writeCharLCD(*string++);
}

/**
 * Writes a string with specified length and offset from SRAM to the LCD.
 * If it is a null terminated string, output will be stopped at the
 * end. It does not need to be null terminated, but it is recommended
 * to use only null terminated strings/buffers, otherwise the function could
 * output any SRAM memory data stored after the string until it reaches a 0
 * or the specified length!
 *
 * Example:
 *
 *			writeStringLength("RP6 Robot Sytem",16,0);
 *			// would output: "RP6 Robot Sytem\n"
 *			writeStringLength("RP6 Robot Sytem",11,4);
 *			// would output: "Robot System"
 * 			writeStringLength("RP6 Robot Sytem",40,4);
 *			// would output: "Robot System"
 *			// No matter if the specified length is 40 characters!
 *
 */
void writeStringLengthLCD(char *string, uint8_t length, uint8_t offset)
{
	for(string = &string[offset]; *string && length; length--)
		writeCharLCD(*string++);
}

/**
 * Write a number (with specified base) to the LCD.
 *
 * Example:
 *
 *			// Write a hexadecimal number to the LCD:
 *			writeInteger(0xAACC,16);
 *			// Instead of 16 you can also write "HEX" as this is defined in the
 *			// RP6RobotBaseLib.h :
 *			writeInteger(0xAACC, HEX);
 *			// Other Formats:
 *			writeInteger(1024,DEC);  	// Decimal
 *			writeInteger(511,OCT);		// Ocal
 *			writeInteger(0b11010111,BIN); // Binary
 */
void writeIntegerLCD(int16_t number, uint8_t base)
{
	itoa(number, &lcd_tmp_buffer[0], base);
	writeStringLCD(&lcd_tmp_buffer[0]);
}

/**
 * Same as writeInteger, but with defined length.
 * This means this routine will add leading zeros to the number if length is
 * larger than the actual value or cut the upper digits if length is smaller
 * than the actual value.
 *
 * Example:
 *
 *			// Write a hexadecimal number to the LCD:
 *			writeIntegerLength(0xAACC, 16, 8);
 *			// Instead of 16 you can also write "HEX" as this is defined in the
 *			// RP6ControlLib.h :
 *			writeIntegerLength(0xAACC, HEX, 8);
 *			// Other Formats:
 *			writeIntegerLength(1024,DEC,6);  	// Decimal
 *			writeIntegerLength(511,OCT,4);		// Ocal
 *			writeIntegerLength(0b11010111,BIN,8); // Binary
 */
 
void writeIntegerLengthLCD(int16_t number, uint8_t base, uint8_t length)
{
	char buffer[17];
	itoa(number, &buffer[0], base);
	int8_t cnt = length - strlen(buffer);
	if(cnt > 0) {
		for(; cnt > 0; cnt--, writeCharLCD('0'));
		writeStringLCD(&buffer[0]);
	}
	else 
		writeStringLengthLCD(&buffer[0],length,-cnt);
}

/**
 * This function is useful for displaying text screens on the LCD.
 * It clears the whole LCD and writes the two Strings to line 1 and
 * line 2.
 */
void _showScreenLCD_P(const char *line1, const char *line2)
{
	clearLCD();
	writeNStringLCD_P(line1);
	setCursorPosLCD(1, 0);
	writeNStringLCD_P(line2);
}

/**
 * Sets the cursor position on LCD.
 */
void setCursorPosLCD(uint8_t line, uint8_t pos)
{
	pos |= 128;
	if(line==1) pos += 0x40;
	writeLCDCommand(pos);
}

/**
 * Clears some characters after the given position.
 */
void clearPosLCD(uint8_t line, uint8_t pos, uint8_t length)
{
	setCursorPosLCD(line,pos);
	while(length--)
		writeCharLCD(' ');
}

/*****************************************************************************/
// Keys / Buttons:


/**
 * Checks which key is pressed - returns the key number, 1 or 2,
 * or 0 if no key is pressed.
 *
 */
uint8_t getPressedKeyNumber(void)
{
	if(!(PING & BUTTON_SW1))
	{
		// Debounce
		delayCycles(1000);
		if((PING & BUTTON_SW1))
			return 0;
		delayCycles(1000);
		if(!(PING & BUTTON_SW1))
			return 1;
	}
	if(!(PING & BUTTON_SW2))
	{
		// Debounce
		delayCycles(1000);
		if((PING & BUTTON_SW2))
			return 0;
		delayCycles(1000);
		if(!(PING & BUTTON_SW2))
			return 2;
	}
	return 0;
}

/**
 * This function has to be called frequently out of
 * the main loop and checks if a button is pressed! It only returns 
 * the key number a single time, DIRECTLY when the button is pressed.
 * 
 * This is useful for non-blocking check in the
 * main loop. You don't need something like 
 * "while(getPressedKeyNumber());" to wait for the button
 * to be released again!
 */
uint8_t checkPressedKeyEvent(void)
{
	static uint8_t pressed_key = 0;
	if(pressed_key) {
		if(!getPressedKeyNumber()) 
			pressed_key = 0;
	}
	else {
		pressed_key = getPressedKeyNumber();
		if(pressed_key)
			return pressed_key;
	}
	return 0;
}

/**
 * This function has to be called frequently out of
 * the main loop and checks if a button is pressed AND
 * released. It only returns the key number a single time, 
 * AFTER the button has been released.
 * 
 * This is useful for non-blocking key check in the
 * main loop. You don't need something like 
 * "while(getPressedKeyNumber());" to wait for the button
 * to be released again!
 */
uint8_t checkReleasedKeyEvent(void)
{
	static uint8_t released_key = 0;
	if(released_key) {
		if(!getPressedKeyNumber()) {
			uint8_t tmp = released_key;
			released_key = 0;
			return tmp;
		}
	}
	else
		released_key = getPressedKeyNumber();
	return 0;
}

/*****************************************************************************/
// Delays, Stopwatches and Beeper:


// ---------------------
// Internal status bits
volatile union {
	uint8_t byte;
	struct {
		unsigned beep:1;
		unsigned unused:7;
	};
} controlStatus;

volatile stopwatches_t stopwatches;
volatile uint8_t delay_timer;
volatile uint8_t ms_timer;
volatile uint16_t sound_timer;

volatile uint16_t timer; // You can use this timer for everything you like!


/**
 * Timer 0 Compare ISR - This timer is used for various
 * timing stuff: The delay timer for blocking delays, 
 * "Stopwatches" for non-blocking delays and the timing of
 * the sound generation with timer2...
 *
 * By default, it runs at 10kHz which means this ISR is called
 * every ~100탎! This is nice for timing stuff!
 *
 */
ISR (TIMER0_COMPA_vect)
{
	// 16bit timer (100탎 resolution)
	timer++;
	
	// Blocking delay (100탎):
	delay_timer++;
	
	// All 1ms based timing stuff
	if(ms_timer++ >= 9) { // 10 * 100탎 = 1ms, >= 9 because 0..9 = 10 counts
		// 16bit Stopwatches:
		if(stopwatches.watches & STOPWATCH1)
			stopwatches.watch1++;
		if(stopwatches.watches & STOPWATCH2)
			stopwatches.watch2++;
		if(stopwatches.watches & STOPWATCH3)
			stopwatches.watch3++;
		if(stopwatches.watches & STOPWATCH4)
			stopwatches.watch4++;
		if(stopwatches.watches & STOPWATCH5)
			stopwatches.watch5++;
		if(stopwatches.watches & STOPWATCH6)
			stopwatches.watch6++;
		if(stopwatches.watches & STOPWATCH7)
			stopwatches.watch7++;
		if(stopwatches.watches & STOPWATCH8)
			stopwatches.watch8++;
		
		ms_timer = 0;
	}
}

/**
 * Delay with the help of the 10kHz timer.
 * sleep(10) delays for *about* 1ms! Not exaclty, as we do not use assembly routines
 * anywhere in this library!
 *
 * This is a blocking routine, which means that the processor
 * will loop in this routine and (except for interrupts) the
 * normal program flow is stopped!
 * Thus you should use the Stopwatch functions wherever you can!
 *
 * Example:
 *		sleep(1); // delay 1 * 100us = 100us = 0.1ms
 *		sleep(10); // delay 10 * 100us = 1000us = 1ms
 *		sleep(100); // delay 100 * 100us = 10000us = 10ms
 *		// The maximum delay is:
 *		sleep(255); // delay 255 * 100us = 25500us = 25.5ms
 */
void sleep(uint8_t time)
{
	delay_timer = 0;
	while (delay_timer <= time+1);
}

/**
 * The same as sleep() but this delays for time*1ms.
 *
 * Example:
 *      mSleep(100); // delay 100 * 1ms = 100ms = 0.1s
 *		mSleep(1000); // delay 1000 * 1ms = 1000ms = 1s
 *
 */
void mSleep(uint16_t time)
{
	while (time--) sleep(10);
}

/**
 * Delay a number of instruction cycles. 
 * No exact delay function! And interrupts can still occur and 
 * add a lot of extra cycles.
 * This function only guarantees to delay for a MINIMAL number
 * of cycles! 
 *
 * Example:
 * 	delayCycles(1000); // Delays for *about* 1000 instruction cycles
 */
void delayCycles(volatile uint16_t dly)
{
	//volatile uint16_t delay2 = dly;
	//while(delay2--) nop();	
	while(dly--) nop();
}


/*****************************************************************************/
// Internal EEPROM:

/**
 * Write a byte to the specified address of the internal EEPROM.
 * WARNING #1: The EEPROM has a limited lifetime of a few million cycles. 
 * Only write to it when required and only when data actually changes. 
 * See Datasheet for more details. 
 * 
 * WARNING #2: DO NOT WRITE TO THE FIRST 32 BYTES IN THE EEPROM!!! 
 * These are used for configuration of the Bootloader.
 */
void writeINTEE(uint8_t adr, uint8_t data)
{
	eeprom_busy_wait();
	EEAR = adr;
	EEDR = data;
	EECR |= (1<<EEMPE);
	EECR |= (1<<EEPE);
}

/**
 * Reads a Byte from the internal EEPROM.
 */
uint8_t readINTEE(uint8_t adr)
{
	eeprom_busy_wait();	
	EEAR = adr;
	EECR |= (1<<EERE);
	return EEDR;
	return 0;
}


/*****************************************************************************/
// Some helper functions for Bootloader EEPROM Settings

#define EE_CFG_CHECK1 0
#define EE_CFG_WIFI_BAUD_L 7
#define EE_CFG_WIFI_BAUD_H 8
#define EE_CFG_WIFI_CMD_GPIO14 22
uint8_t use_gpio14 = false;


/**
 * Can be used to change the baudrate of the UART that is used for the
 * WIFI Module to whatever the Bootloader has been configured to use. 
 * This allows you to change the Baudrate from RobotLoader without 
 * changing and recompiling your program. 
 * This function is right now directly called in initRP6M256().
 * If you need a fixed different Baudrate, set UBBR3 directly in your
 * program right after calling initRP6M256().
 */
void init_wifi_bootloader_baudrate(void)
{
	if(readINTEE(EE_CFG_CHECK1) == 42)
	{
		UBRR3H = readINTEE(EE_CFG_WIFI_BAUD_H);	// Setup Baudrate
		UBRR3L = readINTEE(EE_CFG_WIFI_BAUD_L);
		
		UCSR3A = 0x00;
		UCSR3C = (0<<UMSEL30) | (0<<UMSEL31) | (1<<UCSZ31) | (1<<UCSZ30);
		UCSR3B = (1 << TXEN3) | (1 << RXEN3) | (1 << RXCIE3);
	}
}


/**
 * Read config setting from EEPROM if the WIFI lib should use GPIO14 to enter CMD Mode. 
 */ 
void init_gpio_wifi_cmd_mode(void)
{
	if(readINTEE(EE_CFG_CHECK1) == 42) {
		uint8_t setting = readINTEE(EE_CFG_WIFI_CMD_GPIO14);
		 
		if((setting & 1) == 1)
			use_gpio14 = true;
		else
			use_gpio14 = false;
	}
	else
		use_gpio14 = false;
}


/*****************************************************************************/
// Initialisation:

/**
 * Initialize the Controller - ALWAYS CALL THIS FIRST!
 * The Processor will not work correctly otherwise.
 * (If you don't implement your own init routine...)
 *
 * Example:
 *
 *			int main(void)
 *			{
 *				initRP6Control_M256_WIFI(); // CALL THIS FIRST!
 *
 *				// ... your application code
 *
 *				while(true);
 *				return 0;
 *			}
 *
 */
void initRP6M256(void)
{
	cli();			// Disable global interrupts.
	
	DDRG |= DISP_WR;
	PORTG &= ~DISP_WR;
	DDRG |= DISP_EN_RD;
	PORTG &= ~DISP_EN_RD;

	portInit();		// Setup port directions and initial values.
					// This is the most important step!


	// UART:
	UBRR0H = UBRR_BAUD_LOW >> 8;	// Setup UART: Baud is Low Speed
	UBRR0L = (uint8_t) UBRR_BAUD_LOW;
	UCSR0A = 0x00;
    UCSR0C = (0<<UMSEL00) | (0<<UMSEL01) | (1<<UCSZ01) | (1<<UCSZ00);
    UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);


	// WIFI:
	UBRR3H = UBRR_BAUD_WIFI >> 8;	// Setup Baudrate as default, but this is changed 
	UBRR3L = (uint8_t) UBRR_BAUD_WIFI; // dynamically based on EEPROM settings later
    UCSR3A = 0x00;
    UCSR3C = (0<<UMSEL30) | (0<<UMSEL31) | (1<<UCSZ31) | (1<<UCSZ30);
    UCSR3B = (1 << TXEN3) | (1 << RXEN3) | (1 << RXCIE3);

	clearReceptionBuffer();
	clearReceptionBuffer_WIFI();
	

	// Initialize External interrupts - all disabled:
//	MCUCR = (1 << ISC11) | (1 << ISC10) | (1 << ISC01) | (1 << ISC00);
//	GICR = (0 << INT2) | (0 << INT1) | (0 << INT0);
//	MCUCSR = (0 << ISC2);

	
	// 10kHz Timer 0:
	TCCR0A =  (0 << WGM00) 
			| (1 << WGM01) 
			| (0 << COM0A0) 
			| (0 << COM0A1) 
			| (0 << COM0B0) 
			| (0 << COM0B1) 
			| (0 << CS02)  
			| (1 << CS01) 
			| (0 << CS00);
	TCCR0B =  (0 << WGM02) 
			| (0 << FOC0A) 
			| (0 << FOC0B) 
			| (0 << CS02)  
			| (1 << CS01) 
			| (0 << CS00);		
				
	OCR0A  = 199;
	OCR0B  = 255;
	
	
	// Enable timer interrupts:
	TIMSK0 =   (1 << OCIE0A); 


	/* 
	All other Timers are free to use for your own projects!
	*/
	
	
	// Set WIFI Baudrate from Bootloader settings in the EEPROM:
	init_wifi_bootloader_baudrate();
	
	// Optionally enable use of GPIO14 for entering WIFI Command mode: 
	init_gpio_wifi_cmd_mode();

	
	sei(); // Enable Global Interrupts
}

/******************************************************************************
 * Additional info
 * ****************************************************************************
 * Changelog:
 * - v. 1.1 16.07.2012 by Dominik S. Herwald
 *		- NEW: added EEPROM functions and Baudrate is now set like
 *  	  the Bootloader  has been configured by RobotLoader
 *		- NEW: added configurable use of GPIO14 to enter CMD Mode 
 *		  (this is a lot faster than wating 250ms, send $$$, wait 250ms...)
 *		  Can be enabled in RobotLoader configuration, 
 *        setting is stored in EEPROM. 
 * - v. 1.0 (initial release) 24.02.2012 by Dominik S. Herwald
 *
 * ****************************************************************************
 * Bugs, feedback, questions and modifications can be posted on the AREXX Forum
 * on http://www.arexx.com/forum/ !
 * Of course you can also write us an e-mail to: info@arexx.nl
 * AREXX Engineering may publish updates from time to time on AREXX.com!
 * ****************************************************************************
 * - LICENSE -
 * GNU GPL v2 (http://www.gnu.org/licenses/gpl.txt, a local copy can be found
 * on the RP6 CD in the RP6 sorce code folders!)
 * This program is free software. You can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 * ****************************************************************************
 */

/*****************************************************************************/
// EOF
