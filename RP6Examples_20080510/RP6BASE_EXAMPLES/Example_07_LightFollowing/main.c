/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - ROBOT BASE EXAMPLES
 * ****************************************************************************
 * Example: Movement 5
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 *
 * Now we integrate the LDRs to our subsumption architechture. Just like in the 
 * previous examples, the other behaviours are not changed (at least not much). 
 * We only add a new behaviour to implement light following. 
 * 
 * This is the last subsumption architechture example. Now you could
 * add other behaviours for other sensors that you can add with expansion
 * Modules or for example also a behaviour that integrates remote control
 * into the subsumption ... and low battery behaviour or - whatever ;)
 *
 * It would also be a good idea to make this a bit more universal and
 * create a small library with different behaviours and the control 
 * routines...  ... ... ... this is up to you!
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

#include "RP6RobotBaseLib.h" 	

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
	if(avoid.state != IDLE)
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
}

/*****************************************************************************/
// Follow Behaviour:

#define FOLLOW 3

#define LIGHT_MIN 100

behaviour_command_t follow = {0, 0, FWD, false, false, 0, IDLE};

/**
 * The new Light following behaviour. 
 */
void behaviour_follow(void)
{
	switch(follow.state)
	{
		case IDLE: 
			if(adcLSL >= LIGHT_MIN || adcLSR >= LIGHT_MIN) // Is the light bright enough?
			{
				setStopwatch6(0);
				startStopwatch6();
				follow.state = FOLLOW;
			}
		break;
		case FOLLOW:
			if(getStopwatch6() > 100)
			{
				if(adcLSL >= LIGHT_MIN || adcLSR >= LIGHT_MIN) // Is the light bright enough?
				{
					// Now we calculate the speed of the motors - the robot will always
					// move into the direction of the brighter sensor.
					// You can adjust the constant values (40 and 60 here) for different
					// speeds.
					int16_t dif = ((int16_t)(adcLSL - adcLSR))>>1;
					if(dif > 40) dif = 40;
					if(dif < -40) dif = -40;
					follow.speed_left = 60 - dif;
					follow.speed_right = 60 + dif;

					// Set LEDs - a small bargraph display that shows which sensor has detected
					// brighter light - and how much brighter it is:
					if(dif > 30)
						setLEDs(0b111000);
					else if(dif > 25)
						setLEDs(0b011000);
					else if(dif > 5)
						setLEDs(0b001000);
					else if(dif > -5)
						setLEDs(0b001001);
					else if(dif > -25)
						setLEDs(0b000001);
					else if(dif > -30)
						setLEDs(0b000011);
					else 
						setLEDs(0b000111);
				}
				else  // The light is not bright enough - it is very dark!
				{
					stopStopwatch6();
					follow.state = IDLE;
				} 
				if((avoid.state || escape.state)) // Update LEDs with ACS values if there is an
				{								  // Obstacle...
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
				setStopwatch6(0);
			}
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
	if(cmd->move_value > 0)
	{
		if(cmd->rotate)
			rotate(cmd->speed_left, cmd->dir, cmd->move_value, false); 
		else if(cmd->move)
			move(cmd->speed_left, cmd->dir, DIST_MM(cmd->move_value), false); 
		cmd->move_value = 0; 
	}
	else if(!(cmd->move || cmd->rotate)) 
	{
		changeDirection(cmd->dir);
		moveAtSpeed(cmd->speed_left,cmd->speed_right);
	}
	else if(isMovementComplete())
	{
		cmd->rotate = false;
		cmd->move = false;
	}
}

/**
 * The behaviourController task controls the subsumption architechture. 
 * It implements the priority levels of the different behaviours. 
 */
void behaviourController(void)
{
    // Call all the behaviour tasks:
	behaviour_cruise();
	behaviour_follow();
	behaviour_avoid();
	behaviour_escape();

    // Execute the commands depending on priority levels:
	if(escape.state != IDLE) // Highest priority - 4
		moveCommand(&escape);
	else if(avoid.state != IDLE) // Priority - 3
		moveCommand(&avoid);
	else if(follow.state != IDLE) // Priority - 2
		moveCommand(&follow);
	else if(cruise.state != IDLE) // Priority - 1
		moveCommand(&cruise); 
	else                     // Lowest priority - 0
		moveCommand(&STOP);  // Default command - do nothing! 
							 // In the current implementation this never 
							 // happens.
}

/*****************************************************************************/
// Main:

int main(void)
{
	initRobotBase(); 
	setLEDs(0b111111);
	mSleep(2500);
	setLEDs(0b001001); 

	// Set Bumpers state changed event handler:
	BUMPERS_setStateChangedHandler(bumpersStateChanged);
	
	// Set ACS state changed event handler:
	ACS_setStateChangedHandler(acsStateChanged);
	
	powerON(); // Turn on Encoders, Current sensing, ACS and Power LED.
	setACSPwrMed(); 
	
	// Main loop
	while(true) 
	{		
		behaviourController();
		task_RP6System();
	}
	return 0;
}
