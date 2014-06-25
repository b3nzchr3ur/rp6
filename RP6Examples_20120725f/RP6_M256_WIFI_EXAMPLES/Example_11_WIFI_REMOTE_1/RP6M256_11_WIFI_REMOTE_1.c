/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - RP6 CONTROL M256 Examples
 * ****************************************************************************
 * Example: I2C Master 11 - WIFI Remote Control
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:  
 * This is Example Program lets you remote control the Robot via a serial
 * Text commandline parser. 
 * It is implemented with the behaviour based operation in mind such that
 * it allows easy integration of new behaviours and commands. 
 * For more information about behaviour based robot control, see the example
 * programs of the Robot Base! There are lots of detail explanations. 
 * This program is just an adaption of this concept and is not further 
 * explained here!
 *
 * To control the Robot, just type the command and hit ENTER in the
 * Network Terminal. Make sure "send CR+LF" on Enter is enabled in the options. 
 * You can control speed and direction of the Robot, e.g. type:
 * 50
 * and hit Enter key, the Robot moves forward with speed = 50.
 * Type
 * bwd
 * and hit Enter key, the Robot moves backwards, still with speed = 50
 * Hit Enter without any text --> the Robot stops!
 * 
 * You can control the Robots' direction with the typical w-a-s-d keys. 
 *
 * ############################################################################
 * #+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+
 * 
 * ATTENTION: THE ROBOT MOVES AROUND IN THIS EXAMPLE! PLEASE PROVIDE ABOUT
 * 2m x 2m OR MORE FREE SPACE FOR THE ROBOT! 
 *
 * >>> DO NOT FORGET TO REMOVE THE FLAT CABLE CONNECTION TO THE USB INTERFACE
 * BEFORE YOU START THIS PROGRAM BY PRESSING THE START BUTTON ON THE ROBOT!
 *
 * Instead of the USB Connection, you can use the WIFI Interface to monitor
 * the outputs of the Robot. 
 *
 * #+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+
 * ############################################################################
 * ****************************************************************************
 */

/*****************************************************************************/
// Includes:

#include "RP6M256Lib.h" 

#include "RP6I2CmasterTWI.h"	// I2C Master Library


/*****************************************************************************/
/*****************************************************************************/
// Include the "RP6 Control M256 I2C Master library".
// This allows I2C control of the Robot base in a nearly identical API as
// on the Robot Base. 

#include "RP6M256_I2CMasterLib.h"  // If you want to get Details on how this
								   // works, please also check out the RP6-M32 
								   // examples where this is explained 
								   // step by step. 

/*****************************************************************************/

/*****************************************************************************/
// WIFI Line Parser:



/**
 * WIFI Terminal line parser. 
 * This checks if a whole input line has been received.
 *  
 */
uint8_t getInputLine_WIFI(void)
{ 
	if(getBufferLength_WIFI()) 						
		return parseLine_WIFI(readChar_WIFI());
	return 0;
}


/*****************************************************************************/

/**
 * Prints all Sensor Values on the Serial Interface.
 */
void printAllSensorValues(void)
{
	getAllSensors();		
	writeString_P("\nRead Sensor Values:\n");
	writeString_P("PL:");writeIntegerLength(mleft_power,DEC,3);
	writeString_P(" | PR:");writeIntegerLength(mright_power,DEC,3);
	writeString_P(" | VL:");writeIntegerLength(mleft_speed,DEC,3);
	writeString_P(" | VR:");writeIntegerLength(mright_speed,DEC,3);
	writeString_P(" | DL:");writeIntegerLength(mleft_des_speed,DEC,3);
	writeString_P(" | DR:");writeIntegerLength(mright_des_speed,DEC,3);
	writeChar('\n');
	writeString_P("DSTL:");writeIntegerLength(mleft_dist,DEC,5);
	writeString_P(" | DSTR:");writeIntegerLength(mright_dist,DEC,5);
	writeChar('\n');
	writeString_P("LSL:");writeIntegerLength(adcLSL,DEC,4);
	writeString_P(" | LSR:");writeIntegerLength(adcLSR,DEC,4);
	writeString_P(" | MCL:");writeIntegerLength(adcMotorCurrentLeft,DEC,4);
	writeString_P(" | MCR:");writeIntegerLength(adcMotorCurrentRight,DEC,4);
	writeString_P(" | BAT:");writeIntegerLength(adcBat,DEC,4);
	writeString_P(" | AD0:");writeIntegerLength(adc0,DEC,4);
	writeString_P(" | AD1:");writeIntegerLength(adc1,DEC,4);
	writeChar('\n');
}


/**
 * Prints important Sensor Values via WIFI.
 */
void printAllSensorValues_WIFI(void)
{
	static uint16_t num = 0;
	getAllSensors();	
	writeString_P_WIFI("BAT:");writeIntegerLength_WIFI(adcBat,DEC,4);
	writeString_P_WIFI(" | DL:");writeIntegerLength_WIFI(mleft_des_speed,DEC,3);
	writeString_P_WIFI(" | DR:");writeIntegerLength_WIFI(mright_des_speed,DEC,3);	
	writeString_P_WIFI(" | PL:");writeIntegerLength_WIFI(mleft_power,DEC,3);
	writeString_P_WIFI(" | PR:");writeIntegerLength_WIFI(mright_power,DEC,3);
	writeString_P_WIFI(" | VL:");writeIntegerLength_WIFI(mleft_speed,DEC,3);
	writeString_P_WIFI(" | VR:");writeIntegerLength_WIFI(mright_speed,DEC,3);
	writeString_P_WIFI(" | MCL:");writeIntegerLength_WIFI(adcMotorCurrentLeft,DEC,3);
	writeString_P_WIFI(" | MCR:");writeIntegerLength_WIFI(adcMotorCurrentRight,DEC,3);
	writeString_P_WIFI(" | LSL:");writeIntegerLength_WIFI(adcLSL,DEC,4);
	writeString_P_WIFI(" | LSR:");writeIntegerLength_WIFI(adcLSR,DEC,4);
	writeString_P_WIFI(" | AD0:");writeIntegerLength_WIFI(adc0,DEC,4);
	writeString_P_WIFI(" | AD1:");writeIntegerLength_WIFI(adc1,DEC,4);
	writeString_P_WIFI(" | DSTL:");writeIntegerLength_WIFI(mleft_dist,DEC,5);
	writeString_P_WIFI(" | DSTR:");writeIntegerLength_WIFI(mright_dist,DEC,5);
	writeString_P_WIFI(" |||| NUM:");writeIntegerLength_WIFI(num++,DEC,5);
	writeString_P_WIFI("\n");
}




/*****************************************************************************/
// Behaviour command type:

#define IDLE  0

// The behaviour command data type:
typedef struct {
	uint8_t  speed_left;  // left speed (is used for rotation and 
						  // move distance commands - if these commands are 
						  // active, speed_right is ignored!)
	uint8_t  speed_right; // right speed
	unsigned dir:2;       // direction (FWD, BWD, LEFT, RIGHT)
	unsigned move:1;      // move flag
	unsigned rotate:1;    // rotate flag
	uint16_t move_value;  // move value is used for distance and angle values
	uint8_t  state;       // state of the behaviour
} behaviour_command_t;

behaviour_command_t STOP = {0, 0, FWD, false, false, 0, IDLE};



/*****************************************************************************/
/*****************************************************************************/
// Behaviour WIFI:

#define WIFI_ACTIVE 1
#define WIFI_EXECUTE_CMD 2

behaviour_command_t wifiControl = {0, 0, FWD, 
									false, false, 0, IDLE};


/**
 * This behaviour allows to remotely control the Robot with Text commands that
 * are sent over a WIFI connection (with a small modification also via standard serial interface).
 *
 * It basically receives a text line (with \r or \n or both at the end) and checks if 
 * it is a command or a number. If it is a number it sets the speed of the motors. 
 *
 * This behaviour first waits to be activated, then it surpresses all other behaviours 
 * (except for the low battery behaviour).
 * 
 */
void behaviour_wifiControl(void)
{
	static uint8_t speedl = 0;
	static uint8_t speedr = 0;
	switch(wifiControl.state)
	{
		case IDLE:  // Wait for start with "cmd"
			if(getInputLine_WIFI())
			{
				if(strcmp(receiveBuffer_WIFI,"cmd")==0 || strcmp(receiveBuffer_WIFI,"command")==0)
				{
					writeString_P_WIFI("\nWIFI COMMAND MODE ACTIVE\n"); 
					wifiControl.state = WIFI_ACTIVE;
					break;
				}
				else
				{
					writeString_P_WIFI("\nYou must enter cmd or command to enable command mode!\n"); 
				}
			}
		break;
		case WIFI_ACTIVE: // now accept all other commands...
			if(getInputLine_WIFI())
			{
				if(strcmp(receiveBuffer_WIFI,"z")==0 || strcmp(receiveBuffer_WIFI,"quit")==0)
				{
					writeString_P_WIFI("\nWIFI COMMAND MODE EXIT!\n"); 
					wifiControl.state = IDLE;
				}
				else if( strcmp(receiveBuffer_WIFI,"fwd")==0 || strcmp(receiveBuffer_WIFI,"f")==0 || strcmp(receiveBuffer_WIFI,"w")==0)
				{
					wifiControl.speed_left = speedl;
					wifiControl.speed_right = speedr;
					writeString_P_WIFI("\nFWD!\n"); 
					wifiControl.dir = FWD;
				}
				else if(strcmp(receiveBuffer_WIFI,"bwd")==0 || strcmp(receiveBuffer_WIFI,"b")==0 || strcmp(receiveBuffer_WIFI,"s")==0)
				{
					wifiControl.speed_left = speedl;
					wifiControl.speed_right = speedr;
					writeString_P_WIFI("\nBWD!\n"); 
					wifiControl.dir = BWD;
				}
				else if(strcmp(receiveBuffer_WIFI,"left")==0 || strcmp(receiveBuffer_WIFI,"l")==0 || strcmp(receiveBuffer_WIFI,"a")==0)
				{
					if(speedl/2 >= 30) 
					{
						wifiControl.speed_left = speedl / 2;   // reduce speed for rotation otherwise
						wifiControl.speed_right = speedr / 2;  // it can be quite fast
					}
					else if(speedl > 10)
					{
						wifiControl.speed_left = 30;
						wifiControl.speed_right = 30;
					}
					writeString_P_WIFI("\nLEFT!\n"); 
					wifiControl.dir = LEFT;
				}
				else if(strcmp(receiveBuffer_WIFI,"right")==0 || strcmp(receiveBuffer_WIFI,"r")==0 || strcmp(receiveBuffer_WIFI,"d")==0)
				{
					if(speedl/2 >= 30) 
					{
						wifiControl.speed_left = speedl / 2;
						wifiControl.speed_right = speedr / 2;
					}
					else if(speedl > 10)
					{
						wifiControl.speed_left = 30;
						wifiControl.speed_right = 30;
					}
					writeString_P_WIFI("\nRIGHT!\n"); 
					wifiControl.dir = RIGHT;
				}
				else if(strcmp(receiveBuffer_WIFI,"fl")==0 || strcmp(receiveBuffer_WIFI,"q")==0)
				{
					if(speedl/2 >= 30) 
					{
						wifiControl.speed_left = speedl / 2;
						wifiControl.speed_right = speedr;
					}
					else if(speedl > 10)
					{
						wifiControl.speed_left = 20;
						wifiControl.speed_right = 40;
					}
					writeString_P_WIFI("\nCURVE FWD LEFT!\n"); 
					wifiControl.dir = FWD;
				}
				else if(strcmp(receiveBuffer_WIFI,"fr")==0 || strcmp(receiveBuffer_WIFI,"e")==0)
				{
					if(speedr/2 >= 30) 
					{
						wifiControl.speed_left = speedl;
						wifiControl.speed_right = speedr / 2;
					}
					else if(speedr > 10)
					{
						wifiControl.speed_left = 40;
						wifiControl.speed_right = 20;
					}
					writeString_P_WIFI("\nCURVE FWD LEFT!\n"); 
					wifiControl.dir = FWD;
				}
				else if(strcmp(receiveBuffer_WIFI,"stp")==0 || strcmp(receiveBuffer_WIFI,"x")==0)
				{
					writeString_P_WIFI("\nSTOP!\n"); 
					wifiControl.dir = FWD;
					wifiControl.speed_left = 0;
					wifiControl.speed_right = 0;
				}
				else
				{
					uint8_t pwm_tmp = atoi(receiveBuffer_WIFI);
					if(pwm_tmp == 0)
					{
						uint8_t not_a_number = 0;
						int8_t i;
						for(i = strlen(receiveBuffer_WIFI)-1; i >= 0 ; i--)
							if((receiveBuffer_WIFI[i] < '0') || (receiveBuffer_WIFI[i] > '9'))
							{
								not_a_number = 1;
								writeString_P_WIFI("\n### NOT A NUMBER! ###\n");
								
							}
						if(not_a_number) {
							pwm_tmp = speedl;
							break;	
						}
					}
					else if(pwm_tmp > 160){pwm_tmp = 160; writeString_P("\n--> Power limited to 160!");}
					else if(pwm_tmp < 0) {pwm_tmp = 0;}
					
					writeString_P_WIFI("\n--> Change speed to:");
					writeInteger_WIFI(pwm_tmp,DEC);
					writeChar_WIFI('\n');
								
					
					wifiControl.speed_left = pwm_tmp;
					wifiControl.speed_right = pwm_tmp;
					speedl = pwm_tmp;
					speedr = pwm_tmp;
				}
			}
		break;
		case WIFI_EXECUTE_CMD:
			wifiControl.state = WIFI_ACTIVE;
		break;
		default:
			wifiControl.state = IDLE;
		break;
	}
}


/*****************************************************************************/
// Behaviour control:

/**
 * This function processes the movement commands that the behaviours generate. 
 * Depending on the values in the behaviour_command_t struct, it sets motor
 * speed, moves a given distance or rotates.
 */
void moveCommand(behaviour_command_t * cmd)
{
	if(cmd->move_value > 0)  // move or rotate?
	{
		if(cmd->rotate)
			rotate(cmd->speed_left, cmd->dir, cmd->move_value, false); 
		else if(cmd->move)
			move(cmd->speed_left, cmd->dir, DIST_MM(cmd->move_value), false); 
		cmd->move_value = 0; // clear move value - the move commands are only
		                     // given once and then runs in background.
	}
	else if(!(cmd->move || cmd->rotate)) // just move at speed? 
	{
		changeDirection(cmd->dir);
		moveAtSpeed(cmd->speed_left,cmd->speed_right);
	}
	else if(isMovementComplete()) // movement complete? --> clear flags!
	{
		cmd->rotate = false;
		cmd->move = false;
	}
}



/**
 * A small helper function to display the current behaviour on the
 * LCD. It only prints out the active behaviour ONCE, otherwise the
 * text would flicker on the LCD!
 */
void displayBehaviour(uint8_t behave)
{
	static uint8_t compare = 0;
	if(compare != behave)
	{
		compare = behave;
		clearPosLCD(1, 0, 13);
		setCursorPosLCD(1, 0); 
		switch(behave)
		{
			case 2: writeStringLCD_P("WIFI CTRL"); setLEDs(0b0000); break;
			case 1: writeStringLCD_P("STOP"); setLEDs(0b0000); break;
		}
	}
	if(wifiControl.state == WIFI_ACTIVE) // If WIFI behaviours is active, show a running light...
	{
		static uint8_t runLEDs = 1;
		static uint8_t dir = 0;
		if(getStopwatch2() > 100) 
		{
			setLEDs(runLEDs); 
			if(dir == 0)
				runLEDs <<= 1; 
			else
				runLEDs >>= 1;
			if(runLEDs > 7 ) 
				dir = 1;			
			else if (runLEDs < 2 ) 
				dir = 0;
			setStopwatch2(0);
		}
		
		if(getStopwatch3() > 100) // ... and send all Sensorvalues 10x per Second... 
		{
			printAllSensorValues_WIFI();
			setStopwatch3(0);
		}
	}
}

/**
 * The behaviourController task controls the subsumption architechture. 
 * It implements the priority levels of the different behaviours. 
 *
 * Here we also show which behaviour is active on the LC-Display!
 *
 */
void behaviourController(void)
{
    // Call all the behaviour tasks:
	behaviour_wifiControl();

    // Execute the commands depending on priority levels:
	if(wifiControl.state != IDLE) // Priority - 1
	{
		displayBehaviour(2);
		moveCommand(&wifiControl);
	}
	else                     // Lowest priority - 0
	{
		displayBehaviour(1);
		moveCommand(&STOP);  // Default command - do nothing! 
							 // In the current implementation this never 
							 // happens.
	}
}


/*****************************************************************************/

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
			clearPosLCD(1, 15, 1);
			heartbeat = false;
		}
		else
		{
			setCursorPosLCD(1, 15);
			writeStringLCD_P("*"); 
			heartbeat = true;
		}
		setStopwatch1(0);
	}
}


/**
 * Timed Watchdog display - the other heartbeat function
 * does not work in this example as we use blocked moving functions here.
 */
void watchDogRequest(void)
{
	static uint8_t heartbeat2 = false;
	if(heartbeat2)
	{
		clearPosLCD(1, 14, 1);
		heartbeat2 = false;
	}
	else
	{
		setCursorPosLCD(1, 14);
		writeStringLCD_P("#"); 
		heartbeat2 = true;
	}
}

/*****************************************************************************/
// I2C Requests: 

/**
 * The I2C_requestedDataReady Event Handler
 */
void I2C_requestedDataReady(uint8_t dataRequestID)
{
	checkRP6Status(dataRequestID);
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

   
	writeString_P("\n\nRP6 CONTROL M256 WIFI Remote Control Example Program!\n"); 
    writeString_P("\nWAITING for commands over WIFI Connection!\n"); 
    writeString_P("\nSerial connection is for debug use only in this example.\n"); 


	// Show a help message to explain usage: 
	writeString_P_WIFI("\n\nRP6 CONTROL M256 WIFI Remote Control Example Program!\n"); 
    writeString_P_WIFI("\nWAITING for commands over WIFI Connection!\n"); 
	writeString_P_WIFI("\n#######################################################################################\n"); 
	writeString_P_WIFI("#######################################################################################\n"); 
    writeString_P_WIFI("Command Reference:\n"); 
	writeString_P_WIFI("#######################################################################################\n"); 
	writeString_P_WIFI("command   | cmd        - enter WIFI Remote Control mode\n"); 
	writeString_P_WIFI("quit      | z          - quit WIFI Remote Control mode\n"); 
	writeString_P_WIFI("fwd       | f | w      - move forwards\n"); 
	writeString_P_WIFI("left      | l | a      - move left\n"); 
	writeString_P_WIFI("bwd       | b | s      - move backwards\n");
	writeString_P_WIFI("right     | r | d      - move right\n"); 
	writeString_P_WIFI("fr        | e          - move curve right\n"); 
	writeString_P_WIFI("fl        | q          - move curve left\n"); 
	writeString_P_WIFI("stp       | x | ENTER  - STOP\n"); 
	writeString_P_WIFI("#######################################################################################\n");
	writeString_P_WIFI("#######################################################################################\n");  
	writeString_P_WIFI("\nWAITING for commands over WIFI!\n"); 
	

	// ---------------------------------------
	WDT_setRequestHandler(watchDogRequest); 

	// ---------------------------------------
	I2CTWI_initMaster(100);  
	I2CTWI_setRequestedDataReadyHandler(I2C_requestedDataReady);
	I2CTWI_setTransmissionErrorHandler(I2C_transmissionError);


	setLEDs(0b1111);

	showScreenLCD("################", "################");
	mSleep(500);
	showScreenLCD("WIFI DEMO", "REMOTE 1");
	mSleep(1000);
	setLEDs(0b0000);
	
	// ---------------------------------------
	// Setup ACS power:
	I2CTWI_transmit3Bytes(I2C_RP6_BASE_ADR, 0, CMD_SET_ACS_POWER, ACS_PWR_OFF);
	// Enable Watchdog for Interrupt requests:
	I2CTWI_transmit3Bytes(I2C_RP6_BASE_ADR, 0, CMD_SET_WDT, true);
	// Enable timed watchdog requests:
	I2CTWI_transmit3Bytes(I2C_RP6_BASE_ADR, 0, CMD_SET_WDT_RQ, true);

	startStopwatch1();
	startStopwatch2();
	startStopwatch3();
	
	showScreenLCD("Active Behaviour", "");

	while(true) 
	{ 
		task_LCDHeartbeat();
		task_checkINT();
	    task_I2CTWI();
		behaviourController();
	}
	return 0;
}

