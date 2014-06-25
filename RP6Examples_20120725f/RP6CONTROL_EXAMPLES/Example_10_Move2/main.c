/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - RP6 CONTROL M32 Examples
 * ****************************************************************************
 * Example: I2C Master 10 - Behaviour based Robot
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 * This is a rahter large example. We ported one of the behaviour based Robot 
 * Examples for the RP6 CONTROL M32 and added some new Behaviours.
 * Now there is a Behaviour that lets the Robot wait until you clapped your 
 * hands three times or made some other noises. 
 * There is also a behaviour that checks if the Battery voltage is too low.
 * If this is the case, the robot is stopped. 
 *
 * Of course the Robot still drives around and tries to avoid collisions. 
 * The only difference is that it is controlled by the RP6 CONTROL M32.  
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
 * #+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+
 * ############################################################################
 * ****************************************************************************
 */

/*****************************************************************************/
// Includes:

#include "RP6ControlLib.h" 		// The RP6 Control Library. 
								// Always needs to be included!

#include "RP6I2CmasterTWI.h"	// I2C Master Library


/*****************************************************************************/
/*****************************************************************************/
// Include our new "RP6 Control I2C Master library":

#include "RP6Control_I2CMasterLib.h"

/*****************************************************************************/


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

#define CRUISE_SPEED_FWD 80 // Default speed 

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

#define ESCAPE_SPEED_BWD    80
#define ESCAPE_SPEED_ROTATE 60

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
				escape.move_value = 200;
			else
				escape.move_value = 140;
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
				escape.move_value = 160;
			else
				escape.move_value = 100;
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
				escape.move_value = 160;
			else
				escape.move_value = 100;
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
		sound(200,100,0);
		escape.state = ESCAPE_FRONT;
	}
	else if(bumper_left) 
	{
		sound(200,25,10);
		sound(150,25,0);
		if(escape.state != ESCAPE_FRONT_WAIT) 
			escape.state = ESCAPE_LEFT;
	}
	else if(bumper_right)
	{
		sound(200,25,10);
		sound(150,25,0);
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
	if(obstacle_left && obstacle_right)
	{
		sound(160,20,0);
	}
	else
	{
		if(obstacle_left)
			sound(120,10,0);
		if(obstacle_right)
			sound(140,10,0);
	}
}


/*****************************************************************************/
// Behaviour waitForStart:

#define PREPARE 1
#define WAIT 2
behaviour_command_t waitForStart = {0, 0, FWD, 
									false, false, 0, PREPARE};

/**
 * Wait for start Behaviour. 
 * You need to clap your hands (or make other noise) three times in order
 * to start the Robot!
 */
void behaviour_waitForStart(void)
{
	static uint8_t peak_count = 3;
	if(waitForStart.state == PREPARE)
	{
		if(getStopwatch2() > 250)
		{
			setCursorPosLCD(1, 6); 
			writeIntegerLengthLCD( peak_count, DEC, 1);
			dischargePeakDetector();
			waitForStart.state = WAIT;
			setStopwatch2(0);
		}
	}
	else if(waitForStart.state == WAIT)
	{
		uint8_t key = checkReleasedKeyEvent(); 
		if(key)
			waitForStart.state = IDLE;
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
					waitForStart.state = PREPARE;
					peak_count--;
				}
				if(peak_count == 0)
					waitForStart.state = IDLE;
			}
			else
				setLEDs(0b0000);
			setStopwatch2(0);
		}
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
			case 5: writeStringLCD_P("WAIT"); setLEDs(0b0000); break;
			case 4: writeStringLCD_P("ESCAPE"); setLEDs(0b0110); break;
			case 3: writeStringLCD_P("AVOID"); setLEDs(0b1001); break;
			case 2: writeStringLCD_P("CRUISE"); setLEDs(0b0000); break;
			case 1: writeStringLCD_P("STOP"); setLEDs(0b0000); break;
		}
	}
	if(behave == 2) // If Cruise behaviour is active, show a running light...
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
	if(behave == 6) // If Battery is low - beep all 3 seconds!
	{
		if(getStopwatch2() > 3000)  // We can use Stopwatch2 here and 
		{							// for several other things because only
			sound(200,20,20);		// one of these things can be active at 
			sound(225,20,60);		// the same time! You could not do this if
			sound(200,20,20);		// there were things that could be active
			sound(225,20,0);		// at the same time!
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
	behaviour_waitForStart();
	behaviour_cruise();
	behaviour_avoid();
	behaviour_escape();

    // Execute the commands depending on priority levels:
	if(checkLowBattery.state != IDLE) // Highest priority - 6
	{
		displayBehaviour(6);
		moveCommand(&checkLowBattery);
	}
	else if(waitForStart.state != IDLE) // Priority - 5
	{
		displayBehaviour(5);
		moveCommand(&waitForStart);
	}
	else if(escape.state != IDLE) // Priority - 4
	{
		displayBehaviour(4);
		moveCommand(&escape);
	}
	else if(avoid.state != IDLE) // Priority - 3
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
			printAllSensorValues();
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

// ---------------------------------------

/*****************************************************************************/
// Main function - The program starts here:

int main(void)
{
	initRP6Control();  
	initLCD();
    
	writeString_P("\n\nRP6 CONTROL M32 I2C Master Example Program!\n"); 
    writeString_P("\nMoving...\n"); 

	// ---------------------------------------
	WDT_setRequestHandler(watchDogRequest); 
	BUMPERS_setStateChangedHandler(bumpersStateChanged);
	ACS_setStateChangedHandler(acsStateChanged);
	BATTERY_setLowVoltageHandler(batteryVoltageLow);
        
	initI2C_RP6Lib();
        I2C_setTransmissionErrorHandler(I2C_transmissionError);

	sound(180,80,25);
	sound(220,80,25);

	setLEDs(0b1111);

	showScreenLCD("################", "################");
	mSleep(500);
	showScreenLCD("I2C-Master", "Behaviours");
	mSleep(1000);
	setLEDs(0b0000);
	
	// ---------------------------------------
	// Setup ACS power:
	I2CTWI_transmit3Bytes(I2C_RP6_BASE_ADR, 0, CMD_SET_ACS_POWER, ACS_PWR_MED);
	// Enable Watchdog for Interrupt requests:
	I2CTWI_transmit3Bytes(I2C_RP6_BASE_ADR, 0, CMD_SET_WDT, true);
	// Enable timed watchdog requests:
	I2CTWI_transmit3Bytes(I2C_RP6_BASE_ADR, 0, CMD_SET_WDT_RQ, true);

	startStopwatch1();
	startStopwatch2();

	showScreenLCD("Active Behaviour", "");

	while(true) 
	{ 
		task_LCDHeartbeat();
		task_checkINT0();
	    task_I2CTWI();
		behaviourController();
	}
	return 0;
}

