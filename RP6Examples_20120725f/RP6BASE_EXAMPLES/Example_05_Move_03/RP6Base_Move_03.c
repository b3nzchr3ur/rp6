/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - ROBOT BASE EXAMPLES
 * ****************************************************************************
 * Example: Movement 3
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 *
 * The Robot moves in a Rectangle and changes direction after each cycle.
 * It does not react on any obstacles!
 *
 * You need to calibrate the RP6 Encoder resolution and rotation factors 
 * and set them in the File "RP6Config.h" in the RP6Library folder.
 * Otherwise the angles will not even be roughly accurate.
 *
 * Even with calibration the robot will not drive in an accurate rectangle, 
 * there will always be errors - more or less. On the first cycle you will not 
 * notice too big errors, but latest after the third cycle you will see that 
 * the robot is several cm away from the intended path and the angle is 
 * not accurate.
 *
 * THIS IS NORMAL! Tracked drive systems always have some wheel/tracks slippery
 * and thus the driven distance is often not the distance the encoders have
 * measured. This depends on surface - it will not work that well on any 
 * slippery surfaces!  
 *
 * This is an accumulative error - if one rotation is wrong by 1°, ALL
 * subsequent rotations will have an absolute error of 1° (seen from the 
 * startpoint and the intended waypath) and if one more of the subsequent 
 * rotations is wrong by 2°, all subsequent rotations will have 3° absolute error, 
 * even if the relative rotation is accurate.
 * Now image 5 rotations out of 10 are wrong by 3°, then you have an absolute 
 * error of 15°.
 *
 * The Encoders are very good for speed regulation and when you move straight
 * forwards - well - and for roughly accurate rotations with 1 to 10° error.
 * When moving forwards you don't have much wheel or track slippery and thus
 * you can drive quite accurate distances. 
 * The rotational errors could be corrected by using additional sensors like
 * electronics compasses or optical floor sensors that can be found in 
 * every optical mice for example.
 * 
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
 * #+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+#+
 * ############################################################################
 * ****************************************************************************
 */

/*****************************************************************************/
// Includes:

#include "RP6RobotBaseLib.h" 	

/*****************************************************************************/
// Speed definitions:

#define MOVE_SPEED 50
#define TURN_SPEED 40
	
/*****************************************************************************/
// Main:

int main(void)
{
	initRobotBase(); 
	setLEDs(0b111111);
	mSleep(2500);
	
	powerON(); // Turn on Encoders, Current sensing, ACS and Power LED.

	uint8_t move_count = 0;	// Counter for driven rectangle edges
	uint8_t turn_direction = LEFT; // This variable stores turning direction 
								   // to allow direction changing
	
	// Main loop
	while(true) 
	{		
		setLEDs(0b100100); 
		
		// Move 300mm forwards at "MOVE_SPEED":
		move(MOVE_SPEED, FWD, DIST_MM(300), BLOCKING);
		
		// Set LEDs to show turning direction:
		statusLEDs.LED6 = (turn_direction==LEFT);
		statusLEDs.LED3 = (turn_direction==RIGHT);
		updateStatusLEDs();
		
		// Rotate 90° 
		rotate(TURN_SPEED, turn_direction, 90, BLOCKING);
		// This is not 100% accurate in most
		// cases - deviations of -5 to +5° are
		// normal even with good calibration. This depends on surface!
		
		if(move_count >= 3) // are we at the startpoint again?
		{
			setLEDs(0b101101); 
			mSleep(1000);
			
			// Set LEDs to show turning direction:
			statusLEDs.LED6 = (turn_direction==LEFT);
			statusLEDs.LED3 = (turn_direction==RIGHT);
			updateStatusLEDs();
			
			// rotate 90° again:
			rotate(TURN_SPEED, turn_direction, 90, BLOCKING);
			
			// Change turn direction:
			if(turn_direction == LEFT)
				turn_direction = RIGHT;
			else
				turn_direction = LEFT;
				
			// Reset counter to start again:
			move_count = 0;
		}
		else // We are not at the startpoint, increment counter:
			move_count++; 
	}
	return 0;
}
