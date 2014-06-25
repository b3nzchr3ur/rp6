/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - RP6 CONTROL M256 Examples
 * ****************************************************************************
 * Example: I2C Master 12 - WIFI Remote Control 2
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:  
 * Same as the last example, but here we added a few more behaviours like 
 * Cruise, Avoid, Escape and Low Battery. These can be activated and 
 * deactivated with the command interpreter. 
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
// Cruise Behaviour:

#define CRUISE_SPEED_FWD 100 // Default speed 

#define MOVE_FORWARDS 1
behaviour_command_t cruise = {CRUISE_SPEED_FWD, CRUISE_SPEED_FWD, FWD, 
								false, false, 0, MOVE_FORWARDS};

/**
 * Cruise Behaviour
 */
void behaviour_cruise(void)
{
}

/*****************************************************************************/
// Escape Behaviour:

#define ESCAPE_SPEED_BWD    100
#define ESCAPE_SPEED_ROTATE 80

#define ESCAPE_FRONT		1
#define ESCAPE_FRONT_WAIT 	2
#define ESCAPE_LEFT  		3
#define ESCAPE_LEFT_WAIT	4
#define ESCAPE_RIGHT	    5
#define ESCAPE_RIGHT_WAIT 	6
#define ESCAPE_WAIT_END		7
behaviour_command_t escape = {0, 0, FWD, false, false, 0, IDLE}; 

/**
 * This is the Escape behaviour for the Bumpers.
 */
void behaviour_escape(void)
{
	static uint8_t bump_count = 0;
	
	switch(escape.state)
	{
		case IDLE: 
		break;
		case ESCAPE_FRONT:
			escape.speed_left = ESCAPE_SPEED_BWD;
			escape.dir = BWD;
			escape.move = true;
			if(bump_count > 3)
				escape.move_value = 400;
			else
				escape.move_value = 200;
			escape.state = ESCAPE_FRONT_WAIT;
			bump_count+=2;
		break;
		case ESCAPE_FRONT_WAIT:			
			if(!escape.move)
			{	
				escape.speed_left = ESCAPE_SPEED_ROTATE;
				if(bump_count > 3)
				{
					escape.move_value = 110;
					escape.dir = RIGHT;
					bump_count = 0;
				}
				else 
				{
					escape.dir = LEFT;
					escape.move_value = 75;
				}
				escape.rotate = true;
				escape.state = ESCAPE_WAIT_END;
			}
		break;
		case ESCAPE_LEFT:
			escape.speed_left = ESCAPE_SPEED_BWD;
			escape.dir 	= BWD;
			escape.move = true;
			if(bump_count > 3)
				escape.move_value = 300;
			else
				escape.move_value = 200;
			escape.state = ESCAPE_LEFT_WAIT;
			bump_count++;
		break;
		case ESCAPE_LEFT_WAIT:
			if(!escape.move)
			{
				escape.speed_left = ESCAPE_SPEED_ROTATE;
				escape.dir = RIGHT;
				escape.rotate = true;
				if(bump_count > 3)
				{
					escape.move_value = 100;
					bump_count = 0;
				}
				else
					escape.move_value = 65;
				escape.state = ESCAPE_WAIT_END;
			}
		break;
		case ESCAPE_RIGHT:	
			escape.speed_left = ESCAPE_SPEED_BWD ;
			escape.dir 	= BWD;
			escape.move = true;
			if(bump_count > 3)
				escape.move_value = 300;
			else
				escape.move_value = 200;
			escape.state = ESCAPE_RIGHT_WAIT;
			bump_count++;
		break;
		case ESCAPE_RIGHT_WAIT:
			if(!escape.move)
			{ 
				escape.speed_left = ESCAPE_SPEED_ROTATE;		
				escape.dir = LEFT;
				escape.rotate = true;
				if(bump_count > 3)
				{
					escape.move_value = 100;
					bump_count = 0;
				}
				else
					escape.move_value = 65;
				escape.state = ESCAPE_WAIT_END;
			}
		break;
		case ESCAPE_WAIT_END:
			if(!(escape.move || escape.rotate))
				escape.state = IDLE;
		break;
	}
}

/**
 * Bumpers Event handler
 */
void bumpersStateChanged(void)
{
	if(bumper_left && bumper_right)
	{
		escape.state = ESCAPE_FRONT;
	}
	else if(bumper_left) 
	{
		if(escape.state != ESCAPE_FRONT_WAIT) 
			escape.state = ESCAPE_LEFT;
	}
	else if(bumper_right)
	{
		if(escape.state != ESCAPE_FRONT_WAIT)
			escape.state = ESCAPE_RIGHT;
	}
}

/*****************************************************************************/
// Avoid Behaviour:

#define AVOID_SPEED_L_ARC_LEFT  20
#define AVOID_SPEED_L_ARC_RIGHT 80
#define AVOID_SPEED_R_ARC_LEFT  80
#define AVOID_SPEED_R_ARC_RIGHT 20

#define AVOID_SPEED_ROTATE 	60

#define AVOID_OBSTACLE_RIGHT 		1
#define AVOID_OBSTACLE_LEFT 		2
#define AVOID_OBSTACLE_MIDDLE	    3
#define AVOID_OBSTACLE_MIDDLE_WAIT 	4
#define AVOID_END 					5
behaviour_command_t avoid = {0, 0, FWD, false, false, 0, IDLE};

/**
 * Avoid behaviour with ACS IR Sensors.
 */
void behaviour_avoid(void)
{
	static uint8_t last_obstacle = LEFT;
	static uint8_t obstacle_counter = 0;
	switch(avoid.state)
	{
		case IDLE: 
			if(obstacle_right && obstacle_left)
				avoid.state = AVOID_OBSTACLE_MIDDLE;
			else if(obstacle_left) 
				avoid.state = AVOID_OBSTACLE_LEFT;
			else if(obstacle_right)
				avoid.state = AVOID_OBSTACLE_RIGHT;
		break;
		case AVOID_OBSTACLE_MIDDLE:
			avoid.dir = last_obstacle;
			avoid.speed_left = AVOID_SPEED_ROTATE;
			avoid.speed_right = AVOID_SPEED_ROTATE;
			if(!(obstacle_left || obstacle_right))
			{
				if(obstacle_counter > 3)
				{
					obstacle_counter = 0;
					setStopwatch4(0);
				}
				else
					setStopwatch4(400);
				startStopwatch4();
				avoid.state = AVOID_END;
			}
		break;
		case AVOID_OBSTACLE_RIGHT:
			avoid.dir = FWD;
			avoid.speed_left = AVOID_SPEED_L_ARC_LEFT;
			avoid.speed_right = AVOID_SPEED_L_ARC_RIGHT;
			if(obstacle_right && obstacle_left)
				avoid.state = AVOID_OBSTACLE_MIDDLE;
			if(!obstacle_right)
			{
				setStopwatch4(500);
				startStopwatch4();
				avoid.state = AVOID_END;
			}
			last_obstacle = RIGHT;
			obstacle_counter++;
		break;
		case AVOID_OBSTACLE_LEFT:
			avoid.dir = FWD;
			avoid.speed_left = AVOID_SPEED_R_ARC_LEFT;
			avoid.speed_right = AVOID_SPEED_R_ARC_RIGHT;
			if(obstacle_right && obstacle_left)
				avoid.state = AVOID_OBSTACLE_MIDDLE;
			if(!obstacle_left)
			{
				setStopwatch4(500);
				startStopwatch4();
				avoid.state = AVOID_END;
			}
			last_obstacle = LEFT;
			obstacle_counter++;
		break;
		case AVOID_END:
			if(getStopwatch4() > 1000)
			{
				stopStopwatch4();
				setStopwatch4(0);
				avoid.state = IDLE;
			}
		break;
	}
}

/**
 * ACS Event Handler
 */
void acsStateChanged(void)
{
	if(obstacle_left && obstacle_right)
		statusLEDs.byte = 0b100100;
	else
		statusLEDs.byte = 0b000000;
	statusLEDs.LED5 = obstacle_left;
	statusLEDs.LED4 = (!obstacle_left);
	statusLEDs.LED2 = obstacle_right;
	statusLEDs.LED1 = (!obstacle_right);
	updateStatusLEDs();
}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
// Behaviour WIFI:

#define WIFI_ACTIVE 1
#define WIFI_EXECUTE_CMD 2

behaviour_command_t wifiControl = {0, 0, FWD, 
									false, false, 0, IDLE};



/**
 * The Behaviour command interpreter - has been pulled out of the behaviour_wifiControl 
 * function to reuse the Code in two different states. 
 * 
 */
uint8_t behaviour_command_interpreter(void)
{
	if(strcmp(receiveBuffer_WIFI,"avoid")==0)
	{
		writeString_P_WIFI("\nActivate AVOID Behaviour / ACS!\n"); 
		I2CTWI_transmit3Bytes(I2C_RP6_BASE_ADR, 0, CMD_SET_ACS_POWER, ACS_PWR_MED);
		avoid.state = IDLE;
	}
	else if(strcmp(receiveBuffer_WIFI,"disable_avoid")==0)
	{
		writeString_P_WIFI("\nDisable AVOID Behaviour / ACS!\n"); 
		I2CTWI_transmit3Bytes(I2C_RP6_BASE_ADR, 0, CMD_SET_ACS_POWER, ACS_PWR_OFF);
		avoid.state = IDLE;
	}
	else if(strcmp(receiveBuffer_WIFI,"cruise")==0)
	{
		writeString_P_WIFI("\nActivate CRUISE Behaviour!\n"); 
		cruise.state = MOVE_FORWARDS;
		
	}
	else if(strcmp(receiveBuffer_WIFI,"disable_cruise")==0)
	{
		writeString_P_WIFI("\nDisable CRUISE Behaviour!\n"); 
		cruise.state = IDLE;
	}
	else
		return 0;
	return 1; 
}

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
		case IDLE: 
			if(getInputLine_WIFI())
			{
				if(strcmp(receiveBuffer_WIFI,"cmd")==0 || strcmp(receiveBuffer_WIFI,"command")==0)
				{
					writeString_P_WIFI("\nWIFI COMMAND MODE ACTIVE\n"); 
					wifiControl.state = WIFI_ACTIVE;
					break;
				}
				else if(!behaviour_command_interpreter()) // Call behaviour control interpreter code
					writeString_P_WIFI("\nYou must enter cmd or command to enable command mode!\n"); 
			}
		break;
		case WIFI_ACTIVE: 
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
						wifiControl.speed_left = speedl / 2;
						wifiControl.speed_right = speedr / 2;
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
				else if(!behaviour_command_interpreter()) // Call behaviour control interpreter code
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
// Behaviour check low Battery:

#define BATTERY_LOW 1
behaviour_command_t checkLowBattery = {0, 0, FWD, 
									false, false, 0, IDLE};

/**
 * In this behaviour routine, we have nothing to do
 */
void behaviour_checkLowBattery(void)
{
}

/**
 * This is a new Event Handler and it gets called when the Battery Voltage
 * is getting low! The Parameter isVoltageLow is true, when the voltage
 * is low and false, when the voltage is OK.
 */
void batteryVoltageLow(uint8_t isVoltageLow)
{
	if(isVoltageLow)
		checkLowBattery.state = BATTERY_LOW;
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


uint16_t packets = 0;

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
			case 6: writeStringLCD_P("LOW BATTERY!"); setLEDs(0b0000); break;
			case 5: writeStringLCD_P("WIFI CTRL"); setLEDs(0b0000); break;
			case 4: writeStringLCD_P("ESCAPE"); setLEDs(0b0110); break;
			case 3: writeStringLCD_P("AVOID"); setLEDs(0b1001); break;
			case 2: writeStringLCD_P("CRUISE"); setLEDs(0b0000); break;
			case 1: writeStringLCD_P("STOP"); setLEDs(0b0000); break;
		}
	}
	if(behave == 2 || wifiControl.state == WIFI_ACTIVE) // If Cruise or WIFI behaviours are active, show a running light...
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
	}
	if(behave > 1)
	{
		if(getStopwatch3() > 25)  // Send all Sensorvalues 10x per Second... 
		{
			printAllSensorValues_WIFI();
			setStopwatch3(0);
		}
	}
	if(behave == 6) // If Battery is low, send a message via WIFI every 3 seconds.
	{
		if(getStopwatch2() > 3000) 
		{
			writeString_P_WIFI("\n#### WARNING: BATTERY VOLTAGE IS LOW!\n");
			setStopwatch2(0);
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
	behaviour_checkLowBattery();
	behaviour_wifiControl();
	behaviour_cruise();
	behaviour_avoid();
	behaviour_escape();

    // Execute the commands depending on priority levels:
	if(checkLowBattery.state != IDLE) // Highest priority - 5
	{
		displayBehaviour(6);
		moveCommand(&checkLowBattery);
	}
	else if(wifiControl.state != IDLE) // Priority - 4
	{
		displayBehaviour(5);
		moveCommand(&wifiControl);
	}
	else if(escape.state != IDLE) // Priority - 3
	{
		displayBehaviour(4);
		moveCommand(&escape);
	}
	else if(avoid.state != IDLE) // Priority - 2
	{
		displayBehaviour(3);
		moveCommand(&avoid);
	}
	else if(cruise.state != IDLE) // Priority - 1
	{
		displayBehaviour(2);
		moveCommand(&cruise); 
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
	writeString_P_WIFI("\n\nRP6 CONTROL M256 WIFI Remote Control Example Program!\n"); 
	
	// ---------------------------------------
	WDT_setRequestHandler(watchDogRequest); 
	BUMPERS_setStateChangedHandler(bumpersStateChanged);
	ACS_setStateChangedHandler(acsStateChanged);
	BATTERY_setLowVoltageHandler(batteryVoltageLow);

	// ---------------------------------------
	I2CTWI_initMaster(100);  
	I2CTWI_setRequestedDataReadyHandler(I2C_requestedDataReady);
	I2CTWI_setTransmissionErrorHandler(I2C_transmissionError);


	setLEDs(0b1111);

	showScreenLCD("################", "################");
	mSleep(800);
	showScreenLCD("WIFI DEMO", "REMOTE");
	mSleep(800);
	setLEDs(0b0000);
	
	
	cruise.state = IDLE;
	avoid.state = IDLE;
	escape.state = IDLE;
	
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
	writeString_P_WIFI("cruise                 - Enable cruise behaviour when not in wifi control mode\n"); 
	writeString_P_WIFI("disable_cruise         - Disable cruise behaviour when not in wifi control mode\n");
	writeString_P_WIFI("avoid                  - Enable avoid behaviour when not in wifi control mode\n"); 
	writeString_P_WIFI("disable_avoid          - Disable avoid behaviour when not in wifi control mode\n"); 
	writeString_P_WIFI("Note: Escape behaviour is essential for the robot so it is always active.\n"); 
	writeString_P_WIFI("#######################################################################################\n");
	writeString_P_WIFI("#######################################################################################\n");  
	writeString_P_WIFI("\nWAITING for commands over WIFI!\n"); 
	
	
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
	
	
	
	// Make sure WLAN Module Packet size / buffer size and flush timer is OK
	enter_cmd_mode_WIFI();   
	issueCMD_WIFI("set comm size 1024","AOK"); 
	issueCMD_WIFI("set comm time 10","AOK"); 
	leave_cmd_mode_WIFI();  
	
	
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

