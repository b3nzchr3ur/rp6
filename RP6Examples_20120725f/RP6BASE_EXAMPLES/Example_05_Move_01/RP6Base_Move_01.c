/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - ROBOT BASE EXAMPLES
 * ****************************************************************************
 * Example: Movement 1
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 * This Example Program shows how to control the Motors.
 *
 * The RP6 moves in a circle. Provide enough of free space, please!
 * If it hits an obstacle it will stop the motors and two LEDs will start 
 * to blink.
 * It will stay in this state and wait until you restart the program!
 *
 * ############################################################################
 * #+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+
 * 
 * ATTENTION: THE ROBOT MOVES AROUND IN THIS EXAMPLE! PLEASE PROVIDE ABOUT
 * 1m x 1m OR MORE FREE SPACE FOR THE ROBOT! 
 *
 * >>> DO NOT FORGET TO REMOVE THE FLAT CABLE CONNECTION TO THE USB INTERFACE
 * BEFORE YOU START THIS PROGRAM BY PRESSING THE START BUTTON ON THE ROBOT!
 *
 * (don't remove the USB Interface from the PC - only remove the flat cable
 * connection between the Interface and the Robot!)
 *
 * #+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+
 * ############################################################################
 * ****************************************************************************
 */

/*****************************************************************************/
// Includes:

#include "RP6RobotBaseLib.h" 	// The RP6 Robot Base Library. 
								// Always needs to be included!

/**
 * Here we react on any obstacle that we may hit. 
 * If any of the bumpers detects an obstacle, we stop the motors and start
 * Stopwatch1 which controls a small blink LED routine.
 */
void bumpersStateChanged(void)
{
	if(bumper_left || bumper_right) 
	{
		moveAtSpeed(0,0);  // stop moving!
		setLEDs(0b010000);
		startStopwatch1();
	}
}

/**
 * This function checks Stopwatch1 all the time. If stopwatch 1 is
 * not running, the function does not do anything. As soon as the
 * stopwatch is started, two LEDs begin to blink!
 */
void blink(void)
{
	if(getStopwatch1() > 500) // 500ms
	{
		statusLEDs.LED2 = !statusLEDs.LED2; // Toggle LED bit in LED shadow register... 
		statusLEDs.LED5 = !statusLEDs.LED5;
		updateStatusLEDs();
		setStopwatch1(0);
	}
}

/*****************************************************************************/
// Main:

int main(void)
{
	initRobotBase();
	setLEDs(0b111111);
	mSleep(1500);
	setLEDs(0b100001);

	// Set Bumpers state changed event handler:
	BUMPERS_setStateChangedHandler(bumpersStateChanged);

	
	powerON(); 	// Turn Encoders, Motor Current Sensors 
				// (and ACS IR Receiver and PWRON LED) on.
				// ATTENTION: Automatic Motor control will not work without this!
	
	// -------------------------
	// With the following two commands, the RP6 will start to move in a circle.
	// We simply set the direction to forwards:
	
	changeDirection(FWD);

    // and afterwards we set the speed of the left and right motors to different 
	// values (80 for left, and 30 for right in this example):
	
	
	moveAtSpeedDirection(-50, -50);
		
	
	//moveAtSpeed(80,30);  
 
	// The function 
	// void moveAtSpeed(uint8_t desired_speed_left, uint8_t desired_speed_right)
	// sets the desired speeds. The automatic motor control will try to maintain
	// this speed even if the motors get blocked or the robot has to move up a
	// ramp or drive over a small obstacle. 
	// At least this will be the case if you always call task_RP6System() frequently
	// out of the main loop!
	// Maximum speed is 200. And if you execute the command:
	// moveAtSpeed(0,0);
	// the Robot will stop and turn motors off.
	// Values below 8 will not work properly as this is just too slow to regulate
	// accurately. 
	// ATTENTION: If you use high speeds all the time, this will result in shorter
	// gears and motors lifetime. It is recommended to use only speed values
	// between 10 and 160! It is no problem to use 200 for some seconds, but 
	// you should not let the Robot drive at such high speed all the time!

	// Main loop:
	while(true) 
	{
		// If we hit an obstacle, the bumperStateChanged Handler will start
		// Stopwatch 1 and then this task will let two LEDs blink:
		blink();
	
		// In the main loop we need to call task_RP6System() all the time! 
		// This function calls the Motor control functions that automatically
		// regulate the motor speed:
		
		task_RP6System();
	}
	return 0;
}
