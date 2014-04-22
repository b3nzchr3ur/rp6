/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - RP6 CONTROL M32 Examples
 * ****************************************************************************
 * Example: I2C Master
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 * In this example we show how to react on interrupt requests from the 
 * Microcontroller on the RP6 Mainboard. It sets the signal INT1 (that's what
 * it is called on the RP6 hardware...) which is connected to the external 
 * interrupt input 0 of the Microcontroller. We will NOT use the interrupt
 * routine to react on the interrupts. Instead we simply poll the interrupt
 * pin and check if there is an request. This works better together with
 * the I2C Bus Master routines of the RP6Library. They are also interrupt
 * based and thus we can not directly start a new transmission out of the
 * interrupt service routine of external Interrupt 0. 
 * It would not make any difference in this case, but we could get new
 * problems when there is a I2C Bus transfer in progress when we get
 * an interrupt event. Then it could get cancelled and replaced by the 
 * new request, which would cause problems sometimes.
 *
 * When an Interrupt Event occurs on the Slave Controller, the program 
 * initiates an I2C-Bus request for the first 3 Status Registers. 
 * In the Status registers some bits change depending on what has caused 
 * the interrupt Event (e.g. ACS has detected an obstacle, or the way is 
 * free again...) so we can determine what happened and can react on it 
 * accordingly.
 * 
 * 
 * As you can see - this program got already quite long. The next examples 
 * would get a lot longer. In order to make it easier to understand the 
 * Software, we will put all commonly used I2C remote control functions in a 
 * seperate file in the next example. 
 * We build a RP6Lib equivalent that can be used nearly the same way, with
 * only some small differences. 
 *
 * But first - you need to understand how this general example works!
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

#include "RP6I2CmasterTWI.h"	// I2C Bus Master routines

/*****************************************************************************/

#define I2C_RP6_BASE_ADR 10		// The default address of the Slave Controller 
								// on the RP6 Mainboard

/*****************************************************************************/
// These are the same command definitions as you can find them in the 
// I2C Bus Slave Example program for RP6Base:

#define I2C_REG_STATUS1 		 0
#define I2C_REG_STATUS2 		 1
#define I2C_REG_MOTION_STATUS 	 2
#define I2C_REG_POWER_LEFT 		 3
#define I2C_REG_POWER_RIGHT 	 4
#define I2C_REG_SPEED_LEFT 		 5
#define I2C_REG_SPEED_RIGHT 	 6
#define I2C_REG_DES_SPEED_LEFT 	 7
#define I2C_REG_DES_SPEED_RIGHT  8
#define I2C_REG_DIST_LEFT_L 	 9
#define I2C_REG_DIST_LEFT_H 	 10
#define I2C_REG_DIST_RIGHT_L     11
#define I2C_REG_DIST_RIGHT_H 	 12
#define I2C_REG_ADC_LSL_L 		 13
#define I2C_REG_ADC_LSL_H 		 14
#define I2C_REG_ADC_LSR_L 		 15
#define I2C_REG_ADC_LSR_H 		 16
#define I2C_REG_ADC_MOTOR_CURL_L 17
#define I2C_REG_ADC_MOTOR_CURL_H 18
#define I2C_REG_ADC_MOTOR_CURR_L 19
#define I2C_REG_ADC_MOTOR_CURR_H 20
#define I2C_REG_ADC_UBAT_L 		 21
#define I2C_REG_ADC_UBAT_H 		 22
#define I2C_REG_ADC_ADC0_L 		 23
#define I2C_REG_ADC_ADC0_H 		 24
#define I2C_REG_ADC_ADC1_L 		 25
#define I2C_REG_ADC_ADC1_H 		 26
#define I2C_REG_RC5_ADR	 		 27
#define I2C_REG_RC5_DATA	 	 28
#define I2C_REG_LEDS	 		 29

#define CMD_POWER_OFF 		0
#define CMD_POWER_ON 		1
#define CMD_CONFIG 			2
#define CMD_SETLEDS 		3
#define CMD_STOP   			4
#define CMD_MOVE_AT_SPEED   5
#define CMD_CHANGE_DIR	    6
#define CMD_MOVE 			7
#define CMD_ROTATE 			8
#define CMD_SET_ACS_POWER	9 
#define CMD_SEND_RC5		10 
#define CMD_SET_WDT			11
#define CMD_SET_WDT_RQ		12

#define ACS_PWR_OFF  0
#define ACS_PWR_LOW  1
#define ACS_PWR_MED  2
#define ACS_PWR_HIGH 3

// The Status Struct - here we write the data of the main status register.
// It is the same definition as it can be found in the RP6Slave example!
union {
 	uint8_t byte;
	struct {
		uint8_t batLow:1;
		uint8_t bumperLeft:1;
		uint8_t bumperRight:1;
		uint8_t RC5reception:1;
		uint8_t RC5transmitReady:1;
		uint8_t obstacleLeft:1;
		uint8_t obstacleRight:1;
		uint8_t driveSystemChange:1;
	};
} interrupt_status;


/*****************************************************************************/
// INT0 

#define INT0_STATUS_CHECK 0
uint8_t block = false;

/** 
 * This function and task_I2CTWI have to be called VERY frequently out of the 
 * main loop.
 * Bigger delays result in slower reaction to Interrupt requests of the 
 * Slave. 
 * This function initiates a request of the first 3 Registers of the I2C Bus
 * Slave Controller - these Bytes contain status bits, which tell us what
 * caused the Interrupt request. 
 * They are checked in the requested data ready handler below. 
 */
void task_checkINT0(void)
{
	if(!block && (PIND & EINT1)) 
	{
		block = true; // Block further requests and wait until 
					  // this request has been processed.
		I2CTWI_requestRegisterFromDevice(I2C_RP6_BASE_ADR, INT0_STATUS_CHECK, 0, 3);
	}
}

uint8_t messageBuf[8]; // Buffer for I2C Data

/**
 * You already know this Event Handler from the RP6 Base Examples. 
 * Here on RP6Control it works a little bit different. This Event Handler is 
 * very nice for reacting on an interrupt request from the Slave controller and 
 * read all the data from it! 
 *
 * In this example we output the ACS Status. We show it with the
 * LEDs, on the LCD, make a small sound each time an ACS Channel has detected 
 * an obstacle and output it on the serial Interface... 
 * In principle, this program works just like the ACS Example you know from
 * the RP6Base examples... but with some additional outputs...
 *
 * Beneath this, this routine also checks the Bumpers and for 
 * RC5 Receptions. 
 * 
 */
void I2C_requestedDataReady(uint8_t dataRequestID)
{
	if(dataRequestID == INT0_STATUS_CHECK) // We only have one request source, so
	{                                      // we leave out the switch-case from the
                                           // other example that you already know...
	    // get received data: 
        I2CTWI_getReceivedData(messageBuf, 3); 
		
		// We want to check if the ACS and Bumper status bits have changed, so we XOR 
		// them with the old value for comparison and later mask them in the if 
		// conditions below...
		uint8_t compare = messageBuf[0] ^ interrupt_status.byte;
		interrupt_status.byte = messageBuf[0]; // Update local register
		
		// First, we check if the ACS status bits have changed, if not
		// we can just ignore the change as it was something else...
		if(compare & 0b01100000) // 0b01100000 are the ACS Status bits... 
		{
			writeString_P("- ACS state changed L: ");
			if(interrupt_status.obstacleLeft)
			{
				writeChar('o');
				setCursorPosLCD(1, 12);
				writeStringLCD_P("LEFT"); 
			}
			else
			{
				writeChar(' ');
				clearPosLCD(1, 12, 4);
			}
			writeString_P(" | R: ");
			if(interrupt_status.obstacleRight)
			{
				writeChar('o');
				setCursorPosLCD(1, 0);
				writeStringLCD_P("RIGHT"); 
			}
			else
			{
				writeChar(' ');	
				clearPosLCD(1, 0, 5);
			}
			if(interrupt_status.obstacleRight && interrupt_status.obstacleLeft)
			{
				externalPort.LEDS = 0b0110;
				writeString_P("   MIDDLE!");
				setCursorPosLCD(1, 7);
				writeStringLCD_P("MID");
			}
			else
			{
				externalPort.LEDS = 0b0000;
				clearPosLCD(1, 7, 3);
			}
			writeChar('\n');
			if(interrupt_status.obstacleLeft)
				externalPort.LED1 = true;
			if(interrupt_status.obstacleRight)
				externalPort.LED4 = true;
			outputExt();
			
			// Play a small sound with the Beeper depending on which sensor has
			// detected something:
			if(interrupt_status.obstacleRight && interrupt_status.obstacleLeft)
			{
				sound(140,10,0);
			}
			else
			{
				if(interrupt_status.obstacleLeft)
					sound(100,5,0);
				if(interrupt_status.obstacleRight)
					sound(120,5,0);
			}
		}
		
		// ------------------------------------
		// Check if Bumpers status has changed:
		if(compare & 0b00000110)
		{
			// Bumper status changed, output current state and play sounds:
			writeString_P(" - Bumpers changed: ");
			if(interrupt_status.bumperRight && interrupt_status.bumperLeft)
			{
				writeString_P("MIDDLE!\n");
				sound(200,100,0);
			}
			else
			{
				if(interrupt_status.bumperLeft)
				{
					writeString_P("LEFT!\n");
					sound(200,50,10);
					sound(150,20,0);
				}
				else if(interrupt_status.bumperRight)
				{
					writeString_P("RIGHT!\n");
					sound(200,50,10);
					sound(150,20,0);
				}
				else
				{
					writeString_P("FREE!\n");
				}
			}
		}
		
		// ------------------------------------
		// Check if there was a RC5 Reception:
		if(interrupt_status.RC5reception)
		{
			uint8_t readBuf[2];
			writeString_P("Received RC5 Transmission: ");
			I2CTWI_transmitByte(I2C_RP6_BASE_ADR,I2C_REG_RC5_ADR);
			I2CTWI_readBytes(I2C_RP6_BASE_ADR, readBuf, 2);
			writeString_P("ADR:");writeInteger(readBuf[0],DEC);
			writeString_P(" | DATA:");writeInteger(readBuf[1],DEC);
			writeString_P("\n");
		}
		
		// ------------------------------------
		// IMPORTANT - reset the block flag:
		block = false;
	}
}


/**
 * A small useful routine, to show that the Program is running and not locked up.
 * It lets a '*' character blink with 1Hz on the LC-Display.
 * When you change the program and it seems to lock up under certain conditions, you
 * can see if at least the main loop is still working or if only the I2C Bus Interface
 * is locked up. 
 */
void task_LCDHeartbeat(void)
{
	static uint8_t heartbeat = false;
	if(getStopwatch1() > 500)
	{
		if(heartbeat)
		{
			clearPosLCD(0, 15, 1);
			heartbeat = false;
		}
		else
		{
			setCursorPosLCD(0, 15);
			writeStringLCD_P("*"); 
			heartbeat = true;
		}
		setStopwatch1(0);
	}
}

/*****************************************************************************/
// I2C Error handler

/**
 * This function gets called automatically if there was an I2C Error like
 * the slave sent a "not acknowledge" (NACK, error codes e.g. 0x20 or 0x30).
 * The most common mistakes are: 
 *   - using the wrong address for the slave
 *   - slave not active or not connected to the I2C-Bus
 *   - too fast requests for a slower slave
 * Be sure to check this if you get I2C errors!
 */
void I2C_transmissionError(uint8_t errorState)
{
	writeString_P("\nI2C ERROR - TWI STATE: 0x");
	writeInteger(errorState, HEX);
	writeChar('\n');
	block = false;
}


/*****************************************************************************/
// Main function - The program starts here:

int main(void)
{
	initRP6Control(); // Always call this first! The Processor will not work
					  // correctly otherwise. 

	initLCD();
    
	writeString_P("\n\nRP6 CONTROL M32 I2C Master Example Program!\n"); 
    writeString_P("\nInterrupts...\n"); 

	// IMPORTANT:
	I2CTWI_initMaster(100); // Initialize the TWI Module for Master operation
							// with 100kHz SCL Frequency
							
    // Also very important in this example:                        
	// Register the event handlers:
	I2CTWI_setTransmissionErrorHandler(I2C_transmissionError);
	I2CTWI_setRequestedDataReadyHandler(I2C_requestedDataReady);

	sound(180,80,25);
	sound(220,80,25);

	setLEDs(0b1111);

	showScreenLCD("################", "################");
	mSleep(500);
	showScreenLCD("I2C-Master", "Example Program 2");
	mSleep(1000);
	setLEDs(0b0000);
    showScreenLCD("ACS Status:", "");
	
	// ---------------------------------------
	
	// Set ACS to medium power:
	I2CTWI_transmit3Bytes(I2C_RP6_BASE_ADR, 0, CMD_SET_ACS_POWER, ACS_PWR_MED);
	// Enable Software Watchdog timer on Slave:
	I2CTWI_transmit3Bytes(I2C_RP6_BASE_ADR, 0, CMD_SET_WDT, true);
	// (This timer stops all operations of the Slave if the Master Controller 
	// does not react on interrupt requests after a few seconds. This can
	// prevent damage if the Master controller locks up but the Slave has still
	// the order to drive at high speed... it would maybe crash against a wall
	// at high speeds or worser things... )
	
	// ---------------------------------------
	
	startStopwatch1(); // For LCDHeartbeat function
	
	while(true) 
	{
        // You need to call task_I2CTWI frequently out of the
        // main loop - otherwise the I2C Bus request functions don't work.  
		task_checkINT0();
        task_I2CTWI();
		task_LCDHeartbeat();
	}
	return 0;
}

