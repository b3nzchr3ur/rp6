/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - RP6 CONTROL M256 Examples
 * ****************************************************************************
 * Example: I2C Master 8 - Lib
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 * To make it easier to understand, we use a seperate Library now that
 * contains all I2C Master functions for controlling the RP6 Base. 
 * We made the library like the RP6Lib - all variable names are just the
 * same as in the RP6Lib. Also some functions have the same names 
 * and parameters.
 * There are also all the event handlers like the ACS and Bumpers event 
 * Handler - plus some new ones. 
 * 
 * This makes it easier to reuse parts of programs written for the RP6Lib on
 * RP6-M256.
 *
 * In this example we do almost the same as in the last program, but we also
 * format the sensor values on the WIFI Interface a bit different and enable the 
 * timed watchdog of the slave, which generates an interrupt every 500ms and
 * forces the Master to respond. These requests are shown with a second 
 * blinking character on the LC-Display. If the Master does not respond within
 * 3 seconds, the Robot gets stopped.
 *
 * Some Sensor values are transmitted via the WIFI Interface and
 * the status of the ACS is shown on the serial terminal!
 * This means you can watch both Terminals at the same time and view
 * different output there (this is nice if you have different Sensors and
 * want to watch the output of each Sensor individually - gives 
 * better overview. Of course this only works when you leave the Robot
 * connected to the host PC but can be nice for debugging).
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

#include "RP6M256Lib.h" 	
#include "RP6I2CmasterTWI.h"


/*****************************************************************************/
/*****************************************************************************/
// Include the "RP6 M256 I2C Master library":

#include "RP6M256_I2CMasterLib.h"

// This Library has a lot of functions for controlling the Robot Base unit 
// with the RP6 CONTROL M256 via the I2C Bus. 
// It features a nearly identical API as on the Robot Base. 
// Different from the RP6-M32 examples, here we directly use the final
// library and do not build it up any further. 
// See the comments in the RP6-M32 examples for more details. 

/*****************************************************************************/

/**
 * This is just the same as in the last example - but inside of the 
 * ACS Event Handler that you already know from the Base Unit. 
 * You can even use the same variables - obstacle_left and obstacle_right!
 */
void acsStateChanged(void)
{
	writeString_P("ACS state changed L: ");
	if(obstacle_left)
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
	if(obstacle_left && obstacle_right)
	{
		setLEDs(0b0110);
		writeString_P("   MIDDLE!");
		setCursorPosLCD(1, 7);
		writeStringLCD_P("MID");
	}
	else
	{
		setLEDs(0b0000);
		clearPosLCD(1, 7, 3);
	}
	
	setLED1(obstacle_right);
	setLED4(obstacle_left);

	
	writeChar('\n');
}

/**
 * The same as for the ACS Event Handler above applies for the Bumpers 
 * Event Handler!
 */
void bumpersStateChanged(void)
{
	// Bumper status changed, output current state and play sounds:
	writeString_P("Bumpers changed: ");
	if(bumper_right && bumper_left)
	{
		writeString_P("MIDDLE!");
	}
	else
	{
		if(bumper_left)
		{
			writeString_P("LEFT!");
		}
		else if(bumper_right)
		{
			writeString_P("RIGHT!");
		}
		else
		{
			writeString_P("FREE!");
		}
	}
	writeChar('\n');
}

/**
 * And the RC5 Event Handler is the same as on the Robot Base, too. 
 */
void receiveRC5Data(RC5data_t rc5data)
{
	// Output the received data:
	writeString_P("RC5 Reception --> Toggle Bit:");
	writeChar(rc5data.toggle_bit + '0');
	writeString_P(" | Device Address:");
	writeInteger(rc5data.device, DEC);
	writeString_P(" | Key Code:");
	writeInteger(rc5data.key_code, DEC);
	writeChar('\n');
}

/**
 * This is a new Event Handler and it gets called when the Battery Voltage
 * is getting low! The Parameter isVoltageLow is true, when the voltage
 * is low and false, when the voltage is OK.
 */
void batteryVoltageLow(uint8_t isVoltageLow)
{
	if(isVoltageLow)
	{
		writeString_P("\nBattery Voltage low: ");
		// Send Battery voltage to UART:
		writeIntegerLength((((adcBat/102.4f)+0.1f)), DEC, 2);
		writeChar('.');
		writeIntegerLength((((adcBat/1.024f)+10)), DEC, 2);
		writeString_P("V\n");
	}
	else
	{
		writeString_P("\nBattery Voltage is OK!\n");
	}
}

/**
 * This function prints out all ADC values and motor parameters:
 * power, desired speed, measured speed and driven distance.
 *
 * It first calls "getAllSensors()" from the library which reads all
 * the sensor values we use here from the Slave Controller. 
 * Then you can use just the same variables as on the RP6Base to get
 * the ADC values.
 */
void printAllSensorValues(void)
{
	getAllSensors();		
	writeString_P_WIFI("\nRead Sensor Values:\n");
	writeString_P_WIFI("PL:");writeIntegerLength_WIFI(mleft_power,DEC,3);
	writeString_P_WIFI(" | PR:");writeIntegerLength_WIFI(mright_power,DEC,3);
	writeString_P_WIFI(" | VL:");writeIntegerLength_WIFI(mleft_speed,DEC,3);
	writeString_P_WIFI(" | VR:");writeIntegerLength_WIFI(mright_speed,DEC,3);
	writeString_P_WIFI(" | DL:");writeIntegerLength_WIFI(mleft_des_speed,DEC,3);
	writeString_P_WIFI(" | DR:");writeIntegerLength_WIFI(mright_des_speed,DEC,3);
	writeChar_WIFI('\n');
	writeString_P_WIFI("DSTL:");writeIntegerLength_WIFI(mleft_dist,DEC,5);
	writeString_P_WIFI(" | DSTR:");writeIntegerLength_WIFI(mright_dist,DEC,5);
	writeChar_WIFI('\n');
	writeString_P_WIFI("LSL:");writeIntegerLength_WIFI(adcLSL,DEC,4);
	writeString_P_WIFI(" | LSR:");writeIntegerLength_WIFI(adcLSR,DEC,4);
	writeString_P_WIFI(" | MCL:");writeIntegerLength_WIFI(adcMotorCurrentLeft,DEC,4);
	writeString_P_WIFI(" | MCR:");writeIntegerLength_WIFI(adcMotorCurrentRight,DEC,4);
	writeString_P_WIFI(" | BAT:");writeIntegerLength_WIFI(adcBat,DEC,4);
	writeString_P_WIFI(" | AD0:");writeIntegerLength_WIFI(adc0,DEC,4);
	writeString_P_WIFI(" | AD1:");writeIntegerLength_WIFI(adc1,DEC,4);
	writeChar_WIFI('\n');
}

/**
 * Heartbeat function
 */
void task_LCDHeartbeat(void)
{
	if(getStopwatch1() > 500)
	{
		static uint8_t heartbeat = false;
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
			printAllSensorValues(); // Print all Sensor Values
		}
		setStopwatch1(0);
	}
}

/**
 * Now we have a second "Heartbeat" display
 * which shows if the Controller still reacts on
 * the Watchdog requests from the Slave Controller!
 * And it also shows if the slave Controller is
 * still up and running. 
 *
 * It will blink with a rate of about 2Hz when 
 * the watchdog requests are still active. 
 */
void watchDogRequest(void)
{
	static uint8_t heartbeat2 = false;
	if(heartbeat2)
	{
		clearPosLCD(0, 14, 1);
		heartbeat2 = false;
	}
	else
	{
		setCursorPosLCD(0, 14);
		writeStringLCD_P("#"); 
		heartbeat2 = true;
	}
}


/*****************************************************************************/
// I2C Requests: 

/**
 * The I2C_requestedDataReady Event Handler is now a lot smaller. 
 * It is free for your own request routines. 
 * You can put them in the Block of the if condition.
 * (similar to the RP6 Base Example programs...)
 */
void I2C_requestedDataReady(uint8_t dataRequestID)
{
	// We need to check if this is an INT Request from the Robot Base unit.
	// The Function call inside the if condition returns true if it was an
	// interrupt request, so we have to negate it here and if it was NOT
	// an interrupt request from the Robot base we can check any other sensors
	// from which you may have requested data...
	if(!checkRP6Status(dataRequestID)) 
	{
		// Here you can Check other sensors/microcontrollers with their own
		// request IDs - if there are any... 
	}
}

/*****************************************************************************/
// I2C Error handler

/**
 * This function gets called automatically if there was an I2C Error like
 * the slave sent a "not acknowledge" (NACK, error codes e.g. 0x20 or 0x30).
 */
void I2C_transmissionError(uint8_t errorState)
{
	writeString_P("\nI2C ERROR - TWI STATE: 0x");
	writeInteger(errorState, HEX);
	writeChar('\n');
}

/*****************************************************************************/
// Main function - The program starts here:

int main(void)
{
	initRP6M256();  
	initLCD();
    
	writeString_P("\n\nRP6 CONTROL M256 I2C Master Example Program!\n"); 
    writeString_P("\nInterrupts - part 2...\n"); 
	writeString_P("\nNow with different output in BOTH Terminals! SERIAL AND WIFI!\n"); 
	
	  
	writeString_P_WIFI("\n\nRP6 CONTROL M256 I2C Master Example Program!\n"); 
    writeString_P_WIFI("\nInterrupts - part 2...\n"); 
	writeString_P_WIFI("\nNow with different output in BOTH Terminals! SERIAL AND WIFI!\n"); 
	
	// ---------------------------------------
	// The Event Handlers can be set the same way as with the 
	// RP6Lib:
	ACS_setStateChangedHandler(acsStateChanged);
	BUMPERS_setStateChangedHandler(bumpersStateChanged);
	IRCOMM_setRC5DataReadyHandler(receiveRC5Data);
	// New LowBat Event Handler:
	BATTERY_setLowVoltageHandler(batteryVoltageLow);
	// New Watchdog Request Event Handler:
	WDT_setRequestHandler(watchDogRequest); 
	
	// ---------------------------------------
	// Init TWI Interface:
	I2CTWI_initMaster(100);  
	I2CTWI_setRequestedDataReadyHandler(I2C_requestedDataReady);
	I2CTWI_setTransmissionErrorHandler(I2C_transmissionError);

	setLEDs(0b1111);

	showScreenLCD("################", "################");
	mSleep(500);
	showScreenLCD("I2C-Master", "Example Program 3");
	mSleep(1000);
	setLEDs(0b0000);
	
	// ---------------------------------------
	// Setup ACS power:
	I2CTWI_transmit3Bytes(I2C_RP6_BASE_ADR, 0, CMD_SET_ACS_POWER, ACS_PWR_MED);
	// Enable Watchdog for Interrupt requests:
	I2CTWI_transmit3Bytes(I2C_RP6_BASE_ADR, 0, CMD_SET_WDT, true);
	// Enable timed watchdog requests:
	I2CTWI_transmit3Bytes(I2C_RP6_BASE_ADR, 0, CMD_SET_WDT_RQ, true);

    showScreenLCD("ACS Status:", "");
    
	startStopwatch1();
	startStopwatch2();

	while(true) 
	{ 
		task_LCDHeartbeat();
		task_checkINT();
	    task_I2CTWI();
	}
	return 0;
}

