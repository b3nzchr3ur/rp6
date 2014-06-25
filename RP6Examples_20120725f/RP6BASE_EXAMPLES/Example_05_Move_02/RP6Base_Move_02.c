/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - ROBOT BASE EXAMPLES
 * ****************************************************************************
 * Example: Movement 2
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 *
 * The RP6 moves about 30cm forwards, turns 180° left, moves forwards 30cm, 
 * turns 180° right and repeats this forever.
 * IT DOES NOT REACT ON ANY OBSTACLES!
 * Of course you need to calibrate the Encoders to make this work properly.
 * Read the appropriate section in the RP6 Manual for more details on this!
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
// Main:

int main(void)
{
	initRobotBase(); 
	setLEDs(0b111111);
	mSleep(2500);
	
	powerON(); // Turn on Encoders, Current sensing, ACS and Power LED.
	
	
	/*
	In the following code segment we use the functions
	
	void move(uint8_t desired_speed, uint8_t dir, uint16_t distance, uint8_t blocking)
	and
	void rotate(uint8_t desired_speed, uint8_t dir, uint16_t angle, uint8_t blocking)
	
	The parameters have the following purposes:
	move:
		uint8_t desired_speed: The Speed the robot should try to maintain all the time
							   during the movement.
							   (but it will slow down a bit before it reaches the desired
							   distance)
							   
		uint8_t dir:		   The Direction the robot should move - this can be
							   FWD for Forwards or BWD for Backwards. 
							   You can also use LEFT or RIGHT here, but this function
							   is not optimized for rotation - you should use rotate
							   instead!
							   
		uint16_t distance:	   The distance we want to move. The distance needs to be
							   given in encoder counts. You can use the macro DIST_MM()
							   to convert the distance from mm to encoder counts if you 
							   have calibrated your Encoder sensors resoultion. 
							   
							   Maximum distance is 65000 encoder counts!
							   (not 65535 like you might have thought. There needs to be
							   a bit space to the top value... anyways - who wants to
							   move about 15m? ;) )
							   
		uint8_t blocking:	   This parameter sets if the function shall block the normal
							   program flow during the movement. 
							   If the parameter is "true", the function will NOT quit 
							   until the robot has moved the given distance!
							   ACS, Bumper, RC5 and other events are still processed and
							   not affected, but the main program will be blocked (s. below). 
							   You can use "BLOCKING" instead of "true" and
							   "NON_BLOCKING" instead of false to make the code more readable!
	
	rotate:
		uint8_t desired_speed: Same as above.
		uint8_t dir:		   Nearly the same as above, but here you should only 
							   use LEFT or RIGHT as parameters even if you can pass
							   BWD or FWD to this function.
		uint16_t angle		   The angle to turn. Attention: This rotation function is
							   not optimal for very small angles - it is optimized to 
							   turn angles over 25°!
							   If you want smaller angles, you need to write your
							   own function!
							   And even for large angles this function is far away
							   from beeing perfect as the encoder errors for rotation
							   are rather big. You need to do some experiments to
							   optimize this for the surfaces you want to use the
							   Robot on. The encoders are mainly intended for very good
							   speed regulation and don't work too well for tracked
							   vehicles' dead reckoning. Not only for RP6 but for EVERY
							   tracked vehicle out there - even for those 
							   with $10.000 price tag.
							   Measuring distances when moving straight forward works
							   OK - but not when rotating as there is a lot of tracks
							   slippery and you always don't move as far as you measured.
							   Solution: Use external sensors for rotation! Like
							   electronic compass, gyroscopes, optical mouse sensor or similar. 
							   Also s. comments in next example!
		
	To block or not to block:	
	If you set the "blocking" parameter to false, then the function will only set all 
	necessary parameters and quit - it will not wait until the movement has finished.
	This code:
	
		move(60, FWD, DIST_MM(300), false); // <-- non blocking
		rotate(50, LEFT, 180, false);		// <-- blocking
		
		mSleep(5000);
		
	would NOT work as intended! The Robot would NOT move 300mm and then turn 180°, instead
	it would only turn 180°! And, if you remove the mSleep(5000) at the end, it would maybe 
	not even do this.
	
	You always need to check if the movement has finished and if you can initiate the 
	next movement if you set the blocking parameter to false!
	
	This:
		move(60, FWD, DIST_MM(300), true); // <-- blocking movement
		rotate(50, LEFT, 180, true);	   // <-- blocking rotation
		
	would work like intended and the robot would move 300mm, turn 180° and stop.	
		
	This is equivalent to this:
	
		move(60, FWD, DIST_MM(300), false); // <-- non-blocking movement
		while(!isMovementComplete())		// Now we need to wait until the movement
				task_RP6System();			// is finished before we go on. 
											// If you set blocking to true, the function
											// does exactly this automatically.
	
		rotate(50, LEFT, 180, false);       // <-- non-blocking rotation
		while(!isMovementComplete())		// Again, we need to wait for the
				task_RP6System();			// movement (rotation) to be finished...
	
	Of course, you do not need to wait for the movement to finish in a while-loop - you can 
	also check this in several different ways and you can do other things while the movement is 
	in progress. You will see some other ways in the next examples.
	You can use the "isMovementComplete()" function for checking if the last movement is
	complete. 
	There is also a function "stop()" that can be used to abort a movement, e.g. if one
	of the sensors has detected an obstacle. But you do not need to use stop, you can also
	initiate the next movement without performing "stop" before.

	*/
	
	// Ok, now back to the example program. If you want this to work accurate, you
	// need to calibrate the encoders (s. RP6 manual)!
	
	// Main loop
	while(true) 
	{		
		setLEDs(0b100100); 
		move(60, FWD, DIST_MM(300), true); // Move 300mm forwards 
										   // at Speed 60*5 ~= 300 counts/second
										   // = 7.5cm/s with 0.25mm resolution
										   // (6.9cm/s with 0.23mm resolution...)
		setLEDs(0b100000); 
		rotate(50, LEFT, 180, true); 	   // rotate 180° left
		setLEDs(0b100100); 
		move(60, FWD, DIST_MM(300), true); // move 30cm forwards
		setLEDs(0b000100); 
		rotate(50, RIGHT, 180, true); 	   // rotate 180° right
	}
	return 0;
}
