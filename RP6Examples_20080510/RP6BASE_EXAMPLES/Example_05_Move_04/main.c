/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - ROBOT BASE EXAMPLES
 * ****************************************************************************
 * Example: Movement 4 - Subsumption based Robot
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 *
 * This is a rather complex example - here we use the principle published
 * by Rodney Brooks in the 1980's called "Subsumption Architechture" and
 * the principle of Behaviour based robots.
 *
 * s. (English original publication):
 * http://people.csail.mit.edu/brooks/papers/AIM-864.pdf
 *
 * s. (German, short summary):
 * http://www.igi.tugraz.at/STIB/WS98/Bergmann/einleitung.htm
 *
 * Basically, this is the way we can make our robot act like a simple
 * insect - e.g. like a Moth or an Ant.
 *
 * We start things off with the two "tentacles" our simple insect has - the
 * bumpers at the front of the robot.
 * 
 * In the previous example, we have seen how Finite State Machines work. 
 * Now we can use them to implement different behaviours.
 * 
 * In this example we only have the "Cruise" and the "Escape" behaviours.
 * The Cruise behaviour tells the robot to move straight forward all the time.
 * The Escape behaviour gets active when the robot hits an obstacle with its 
 * bumpers. Then this behaviour initiates a reaction: it stops the robot, 
 * moves back a few cm, turns left or right by a few degrees, depending on which 
 * bumper was hit and returns control to the Cruise behaviour afterwards.
 *
 * The reaction of the Escape behaviour also depends on how often the bumpers were 
 * hit - this allows the robot to escape from "hang up" situations when it loops 
 * around between two obstacles that are close to each other.
 *
 * Well - just let this program run, then you will see what it does.
 *
 *
 * A small Exercise:
 * There is a little problem with this program! 
 * The Robot can easily get into dead lock situations like this and does
 * not find the way out anymore. 
 * You only need to change TWO lines of code to solve this problem...
 * Think about it!
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
// Here we define a Behaviour command data type - this makes life easier. 
// These structures contain all required information for movement request and 
// current state of our behaviours. 
// If you want to learn more about structs, unions and typedefs, read some of 
// the C-Tutorials we suggest in the manual!

#define IDLE  0

// The behaviour command data type:
typedef struct {
	uint8_t  speed_left;  // left speed (is used for rotation and 
						  // move distance commands - if these commands are 
						  // active, speed_right is ignored!)
	uint8_t  speed_right; // right speed (if required...)
	unsigned dir:2;       // direction (FWD, BWD, LEFT, RIGHT)
	unsigned move:1;      // move flag
	unsigned rotate:1;    // rotate flag
	uint16_t move_value;  // move value is used for distance and angle values
	uint8_t  state;       // state of the behaviour
} behaviour_command_t;


/*****************************************************************************/
// Cruise Behaviour:

#define CRUISE_SPEED_FWD    80  // Default speed when no obstacles are detected!

#define MOVE_FORWARDS 1
behaviour_command_t cruise = {CRUISE_SPEED_FWD, CRUISE_SPEED_FWD, FWD, 
								false, false, 0, MOVE_FORWARDS};

/**
 * We don't have anything to do here - this behaviour has only
 * a constant value for moving forwards - s. above!
 * Of course you can change this and add some random or timed movements 
 * in here...
 */
void behaviour_cruise(void)
{
}

/*****************************************************************************/
// Escape Behaviour:

// Some constants:
#define ESCAPE_SPEED_BWD    60
#define ESCAPE_SPEED_ROTATE 50

// States for the FSM of this behaviour:
#define ESCAPE_FRONT_START 	1
#define ESCAPE_FRONT_WAIT 	2
#define ESCAPE_LEFT_START  	3
#define ESCAPE_LEFT_WAIT	4
#define ESCAPE_RIGHT_START  5
#define ESCAPE_RIGHT_WAIT 	6
#define ESCAPE_WAIT_END		7
behaviour_command_t escape = {0, 0, FWD, false, false, 0, IDLE}; 

/**
 * This is the Escape behaviour for the Bumpers - remember the last FSM examples!
 * This is also a FSM - just with some more transitions between the states.
 */
void behaviour_escape(void)
{
	static uint8_t bump_count = 0;
	switch(escape.state)
	{
		case IDLE: 
			// We have no obstacles detected. Obstacles are detected in the
			// bumpersStateChanged function below!
			// Alternatively you could also put that code in here, but
			// when it is in the event handler below, it can also change this
			// behaviour when it is not in the idle state!
			// (when the robot is running, just hit a bumper several times with
			// your hand while it is moving backwards!)
		break;
		case ESCAPE_FRONT_START:
			escape.speed_left = ESCAPE_SPEED_BWD;
			escape.dir = BWD;
			escape.move = true;
			if(bump_count > 3)
				escape.move_value = 180;
			else
				escape.move_value = 120;
			escape.state = ESCAPE_FRONT_WAIT;
			bump_count+=2;
		break;
		case ESCAPE_FRONT_WAIT:			
			if(!escape.move) 
			{
				escape.speed_left = ESCAPE_SPEED_ROTATE;
				if(bump_count > 3)
				{
					escape.move_value = 90;
					escape.dir = RIGHT;
					bump_count = 0;
				}
				else 
				{
					escape.dir = LEFT;
					escape.move_value = 50;
				}
				escape.rotate = true;
				escape.state = ESCAPE_WAIT_END;
			}
		break;
		case ESCAPE_LEFT_START:
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
					escape.move_value = 80;
					bump_count = 0;
				}
				else
					escape.move_value = 45;
				escape.state = ESCAPE_WAIT_END;
			}
		break;
		case ESCAPE_RIGHT_START:
			escape.speed_left = ESCAPE_SPEED_BWD;
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
					escape.move_value = 80;
					bump_count = 0;
				}
				else
					escape.move_value = 45;
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
	if(bumper_left && bumper_right) // Both Bumpers were hit
	{
		escape.state = ESCAPE_FRONT_START;
	}
	else if(bumper_left)  			// Left Bumper was hit
	{
		if(escape.state != ESCAPE_FRONT_WAIT) 
			escape.state = ESCAPE_LEFT_START;
	}
	else if(bumper_right) 			// Right Bumper was hit
	{
		if(escape.state != ESCAPE_FRONT_WAIT)
			escape.state = ESCAPE_RIGHT_START;
	}
}

/*****************************************************************************/
// Behaviour control:

/**
 * This function processes the movement commands that the behaviours generate. 
 * Depending on the values in the behaviour_command_t struct, it sets motor
 * speed, moves a given distance or rotates.
 *
 * A small remark: with "->" you can access fields in 
 * structs of which you only have a pointer like it is here ( * cmd ).  
 * You can NOT use "." there - like "avoid.state" for example!
 * Read one of the C Tutorials suggested in the manual for more details!
 */
void moveCommand(behaviour_command_t * cmd)
{
	if(cmd->move_value > 0)  // any move/rotate commands?
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

// Define a default movement command:
behaviour_command_t STOP = {0, 0, FWD, false, false, 0, IDLE};

/**
 * The behaviourController task controls the subsumption architechture. 
 * It implements the priority levels of the different behaviours. 
 * In this Example we only have two behaviours, but of course there can be
 * a lot more! 
 */
void behaviourController(void)
{
    // Call all the behaviour tasks:
	behaviour_cruise();
	behaviour_escape();

    // Execute the commands depending on priority levels:
	if(escape.state != IDLE) // Highest priority (2)
		moveCommand(&escape);
	else if(cruise.state != IDLE) // Lower priority (1)
		moveCommand(&cruise); 
	else                     // Lowest priority (0)
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
	setLEDs(0b100100); 

	// Set Bumpers state changed event handler:
	BUMPERS_setStateChangedHandler(bumpersStateChanged);
	
	powerON(); // Turn on Encoders, Current sensing, ACS and Power LED.

	// Main loop
	while(true) 
	{		
		behaviourController();
		task_RP6System();
	}
	return 0;
}
