/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - ROBOT BASE EXAMPLES
 * ****************************************************************************
 * Example: RP6 Remote Controlled
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 *
 * This demo program shows how to use a RC5 TV Remote Control to control your 
 * RP6 like a RC-Car.
 * It also implements acceleration and deceleration functionality. 
 * (yes it is intentionally that the robot accelerates and decelerates so
 * slow ;)  Of course you can change this if you want!)
 *
 * First you need to get a Remote Control that transmits RC5 Code - like
 * Conrad Promo8 or EuroSky Universal remote controls - which are very cheap 
 * --> about 10 Euro. Then you may need to try out some different codes - look
 * at the manual of these remote controls how to change the transmit code!
 * 
 * Attention: The codes listed in this sample program will not match even 
 * when you use the same remote control - it depends on specific protocol
 * that it uses.
 *
 * You can test if the remote transmits RC5 code when you start this program
 * and look at the Terminal output. If it receives a RC5 code, it will display
 * the keycode, address and the toggle bit.
 *
 * MAKE SURE THAT THE RP6 CAN NOT MOVE,
 * or uncomment the line #define DO_NOT_MOVE!
 *
 * As soon as this works, you can change the key mapping - look at your
 * remote control and decide yourself which keys are best for move left/right,
 * forwards/backwards, drive left/right curve fwd/bwd, only left/right motor 
 * forwards/backwards and stop.
 * Typically there are some keys with ideal layout for this 
 * (usually intended for cursor movement and stuff like that)!
 * Press them and look what their keycode is. Then enter the keyvalues in the
 * source code - like in this example:
 * 
 * #ifdef RC_YOUR_OWN		// Standard Numbers on every Remote control
 *	 #define RC5_KEY_LEFT 		4		
 *	 #define RC5_KEY_RIGHT 		6		
 *	 #define RC5_KEY_FORWARDS 	2  	
 *	 #define RC5_KEY_BACKWARDS 	8		
 *	 #define RC5_KEY_STOP 		5		
 * //...
 * #endif
 *
 * Outcomment the #define RC_PROMO8
 * like this: //#define RC_PROMO8
 * and Uncomment //#define RC_YOUR_OWN	
 * like this: #define RC_YOUR_OWN	
 *
 * Then recompile the program, upload it, start it again and test if
 * the RP6 reacts when you press those keys on your remote control.
 * You need to HOLD DOWN the key - then the RP6 will accelerate slowly to the 
 * MAX_SPEED values defined in this program. If you release the key again, 
 * it starts to deccelerate slowly.
 * You can control the RP6 also with 
 * press key - release key - press key - release key - press key - release key 
 * ....
 * This will result in a slower movement than just holding the key down
 * continously.
 *
 * ****************************************************************************
 */

/*****************************************************************************/
// Includes:

#include "RP6RobotBaseLib.h" 

/*****************************************************************************/
/*****************************************************************************/


// If you only want to look at the RC5 Keycodes received from the Remote
// Control, uncomment this line:
//#define DO_NOT_MOVE
// The RP6 will not move then!



/*****************************************************************************/
/*****************************************************************************/
// The TV Remote Control definitions

// ###########################
// Uncomment __one__ (only one!) of the following definitions:
//#define RC_EURO_SKY
#define RC_PROMO8
//#define RC_YOUR_OWN
// ...

// ###########################
// Change the keymapping here:

#ifdef RC_EUROSKY		// RC Type:  Conrad - EuroSky	
	#define RC5_KEY_LEFT 				22		
	#define RC5_KEY_RIGHT 				21		
	#define RC5_KEY_FORWARDS 			16  	
	#define RC5_KEY_BACKWARDS 			17		
	#define RC5_KEY_STOP 				23		
	#define RC5_KEY_CURVE_LEFT 			1	
	#define RC5_KEY_CURVE_RIGHT 		3	
	#define RC5_KEY_CURVE_BACK_LEFT 	7	
	#define RC5_KEY_CURVE_BACK_RIGHT 	9	
	#define RC5_KEY_LEFT_MOTOR_FWD 		1
	#define RC5_KEY_LEFT_MOTOR_BWD 		7
	#define RC5_KEY_RIGHT_MOTOR_FWD 	3
	#define RC5_KEY_RIGHT_MOTOR_BWD 	9
#endif

#ifdef RC_PROMO8		// RC Type:  Conrad - Promo8			
	#define RC5_KEY_LEFT 				21		
	#define RC5_KEY_RIGHT 				22		
	#define RC5_KEY_FORWARDS 			32  	
	#define RC5_KEY_BACKWARDS 			33		
	#define RC5_KEY_STOP 				11	
	#define RC5_KEY_CURVE_LEFT 			29	
	#define RC5_KEY_CURVE_RIGHT 		13	
	#define RC5_KEY_CURVE_BACK_LEFT 	10	
	#define RC5_KEY_CURVE_BACK_RIGHT 	62		
	#define RC5_KEY_LEFT_MOTOR_FWD 		1
	#define RC5_KEY_LEFT_MOTOR_BWD 		7
	#define RC5_KEY_RIGHT_MOTOR_FWD 	3
	#define RC5_KEY_RIGHT_MOTOR_BWD 	9
#endif

#ifdef RC_YOUR_OWN		// Your own RC!
	#define RC5_KEY_LEFT 				4		
	#define RC5_KEY_RIGHT 				6		
	#define RC5_KEY_FORWARDS 			2  	
	#define RC5_KEY_BACKWARDS 			8		
	#define RC5_KEY_STOP 				5		
	#define RC5_KEY_CURVE_LEFT 			1	
	#define RC5_KEY_CURVE_RIGHT 		3	
	#define RC5_KEY_CURVE_BACK_LEFT 	7	
	#define RC5_KEY_CURVE_BACK_RIGHT 	9		
	#define RC5_KEY_LEFT_MOTOR_FWD		10
	#define RC5_KEY_LEFT_MOTOR_BWD 		11
	#define RC5_KEY_RIGHT_MOTOR_FWD 	12
	#define RC5_KEY_RIGHT_MOTOR_BWD 	13
#endif

//... you can add more Remote control keymappings or implement something
// better than this if you like...



/*****************************************************************************/
// Speed values:

#define MAX_SPEED_MOVE 200
#define MAX_SPEED_TURN 100 

#define MAX_SPEED_CURVE 120 
#define MAX_SPEED_CURVE2 40 
#define ACCELERATE_CURVE 10
#define ACCELERATE_CURVE2 4
#define DECELERATE_CURVE 4
#define DECELERATE_CURVE2 2

#define MAX_SPEED_1_MOTOR 120 

#define ACCELERATE_VALUE 8
#define DECELERATE_VALUE 4

uint8_t max_speed_left; // Maximum speed variable left
uint8_t max_speed_right; // Maximum speed variable right
uint8_t acl_left;
uint8_t acl_right;
uint8_t decl_left;
uint8_t decl_right;

/*****************************************************************************/

/**
 * Just a small helper function to set speed params.
 */
void setDefaultSpeedParameters(void)
{
	max_speed_left = MAX_SPEED_MOVE;
	max_speed_right = max_speed_left;
	acl_left = ACCELERATE_VALUE;
	acl_right = ACCELERATE_VALUE;
	decl_left = DECELERATE_VALUE;
	decl_right = DECELERATE_VALUE;
	uint16_t tmp = (getDesSpeedLeft() + getDesSpeedRight())/2;
	moveAtSpeed(tmp , tmp);
}

/**
 * RC5 Data reception handler - this function is called automatically from the 
 * RP6lib if new RC5 Data has been received.
 */
void receiveRC5Data(RC5data_t rc5data)
{
	// Output the received data:
	writeString_P("Toggle Bit:");
	writeChar(rc5data.toggle_bit + '0');
	writeString_P(" | Device Address:");
	writeInteger(rc5data.device, DEC);
	writeString_P(" | Key Code:");
	writeInteger(rc5data.key_code, DEC);
	writeChar('\n');
	
#ifndef DO_NOT_MOVE    	// used to disable movement if you want to 
						// look at the received RC5 codes only (s. above).
	
	uint8_t movement_command = false; // used to store if we have received 
									  // a movement command.
									  // Any other key is ignored!
	
	// Check which key is pressed:
	switch(rc5data.key_code)
	{
		case RC5_KEY_LEFT: 	 		// Turn left:
			writeString_P("LEFT\n");
			setDefaultSpeedParameters();
			max_speed_left = MAX_SPEED_TURN;
			max_speed_right = max_speed_left;
			changeDirection(LEFT);
			setLEDs(0b100000);
			movement_command = true;	// Store that we have received a movement command!
		break;
		case RC5_KEY_RIGHT: 		// Turn right:
			writeString_P("RIGHT\n");
			setDefaultSpeedParameters();
			max_speed_left = MAX_SPEED_TURN;
			max_speed_right = max_speed_left;
			changeDirection(RIGHT);
			setLEDs(0b000100);
			movement_command = true;
		break;
		case RC5_KEY_FORWARDS: 		// Move forwards
			writeString_P("FORWARDS\n");
			setDefaultSpeedParameters();
			changeDirection(FWD);
			setLEDs(0b100100);
			movement_command = true;
		break;
		case RC5_KEY_BACKWARDS: 	// Move backwards
			writeString_P("BACKWARDS\n");
			setDefaultSpeedParameters();
			changeDirection(BWD);
			setLEDs(0b001001);
			movement_command = true;
		break;
		case RC5_KEY_STOP: 			// Stop!
			writeString_P("STOP\n");
			max_speed_left = 0;
			max_speed_right = max_speed_left;
			moveAtSpeed(0,0);
			setLEDs(0b011011);
			movement_command = true;
		break;
		case RC5_KEY_CURVE_LEFT: 	// Drive curve left - forwards
			writeString_P("CURVE LEFT FWD\n");
			max_speed_left = MAX_SPEED_CURVE2;
			max_speed_right = MAX_SPEED_CURVE;
			acl_left = ACCELERATE_CURVE2;
			acl_right = ACCELERATE_CURVE;
			decl_left = DECELERATE_CURVE2;
			decl_right = DECELERATE_CURVE;
			changeDirection(FWD);
			setLEDs(0b110100);
			movement_command = true;
		break;
		case RC5_KEY_CURVE_RIGHT: 	// Drive curve right - forwards
			writeString_P("CURVE RIGHT FWD\n");
			max_speed_left = MAX_SPEED_CURVE;
			max_speed_right = MAX_SPEED_CURVE2;
			acl_left = ACCELERATE_CURVE;
			acl_right = ACCELERATE_CURVE2;
			decl_left = DECELERATE_CURVE;
			decl_right = DECELERATE_CURVE2;
			changeDirection(FWD);
			setLEDs(0b100110);
			movement_command = true;
		break;
		case RC5_KEY_CURVE_BACK_LEFT: 	// Drive curve left - backwards
			writeString_P("CURVE LEFT BWD\n");
			max_speed_left = MAX_SPEED_CURVE2;
			max_speed_right = MAX_SPEED_CURVE;
			acl_left = ACCELERATE_CURVE2;
			acl_right = ACCELERATE_CURVE;
			decl_left = DECELERATE_CURVE2;
			decl_right = DECELERATE_CURVE;
			changeDirection(BWD);
			setLEDs(0b011001);
			movement_command = true;
		break;
		case RC5_KEY_CURVE_BACK_RIGHT: 	// Drive curve right - backwards
			writeString_P("CURVE RIGHT BWD\n");
			max_speed_left = MAX_SPEED_CURVE;
			max_speed_right = MAX_SPEED_CURVE2;
			acl_left = ACCELERATE_CURVE;
			acl_right = ACCELERATE_CURVE2;
			decl_left = DECELERATE_CURVE;
			decl_right = DECELERATE_CURVE2;
			changeDirection(BWD);
			setLEDs(0b001011);
			movement_command = true;
		break;
		case RC5_KEY_LEFT_MOTOR_FWD: 	// Only left motor on - forwards
			writeString_P("MOTOR LEFT FWD\n");
			max_speed_left = 0;
			max_speed_right = MAX_SPEED_1_MOTOR;
			acl_left = 4;
			acl_right = 4;
			decl_left = 4;
			decl_right = 4;
			changeDirection(FWD);
			setLEDs(0b110000);
			movement_command = true;
		break;
		case RC5_KEY_LEFT_MOTOR_BWD: 	// Only left motor on - backwards
			writeString_P("MOTOR LEFT BWD\n");
			max_speed_left = 0;
			max_speed_right = MAX_SPEED_1_MOTOR;
			acl_left = 4;
			acl_right = 4;
			decl_left = 4;
			decl_right = 4;
			changeDirection(BWD);
			setLEDs(0b101000);
			movement_command = true;
		break;
		case RC5_KEY_RIGHT_MOTOR_FWD: // Only right motor on - forwards
			writeString_P("MOTOR RIGHT FWD\n");
			max_speed_left = MAX_SPEED_1_MOTOR;
			max_speed_right = 0;
			acl_left = 4;
			acl_right = 4;
			decl_left = 4;
			decl_right = 4;
			changeDirection(FWD);
			setLEDs(0b000110);
			movement_command = true;
		break;
		case RC5_KEY_RIGHT_MOTOR_BWD: 	// Only right motor on - backwards
			writeString_P("MOTOR RIGHT BWD\n");
			max_speed_left = MAX_SPEED_1_MOTOR;
			max_speed_right = 0;
			acl_left = 4;
			acl_right = 4;
			decl_left = 4;
			decl_right = 4;
			changeDirection(BWD);
			setLEDs(0b000101);
			movement_command = true;
		break;
	}
	
	if(movement_command) // Did we receive a move command?
	{
		// Accelerate if neccessary:
		if(getDesSpeedLeft() < max_speed_left) // If we have not reached the left maximum speed...
		{								// ... accelerate!
			moveAtSpeed(getDesSpeedLeft()+acl_left, getDesSpeedRight());
			if(getDesSpeedLeft() < 10)
				moveAtSpeed(10, getDesSpeedRight());
		}
		if(getDesSpeedRight() < max_speed_right) // If we have not reached the right maximum speed...
		{
										// ... accelerate!
			moveAtSpeed(getDesSpeedLeft(), getDesSpeedRight()+acl_right);
			if(getDesSpeedRight() < 10)
				moveAtSpeed(getDesSpeedLeft(), 10);
		}

		// Start Stopwatch 1 - it starts decceleration after 250ms of no RC5 reception! (s. below)
		setStopwatch1(0);
		startStopwatch1();
	}
#endif

}


/*****************************************************************************/

/**
 * This function is called frequently out of the main loop and checks if 
 * Stopwatch1 has counted at least 250ms. If this is the case, decceleration is started
 * and the Stopwatch is resetted and the progtam waits for next 250ms to pass by.
 * Stopwatch1 ist set to 0 and started from the RC5 reception handler after
 * each reception of a valid keycode. (s. above)
 */
void deccelerate(void)
{
	if(getStopwatch1() > 250) // After 250ms with no reception...
	{
		if(getDesSpeedLeft() <= 10) // If left speed is less or equal than 10...
			moveAtSpeed(0, getDesSpeedRight()); // ... stop the left motor
		else				  // Otherwise continue to deccelerate:
			moveAtSpeed(getDesSpeedLeft()-decl_left, getDesSpeedRight());
			
		if(getDesSpeedRight() <= 10) // If right speed is less or equal than 10...
			moveAtSpeed(getDesSpeedLeft(), 0); // ... stop the right motor
		else				  // Otherwise continue to deccelerate:
			moveAtSpeed(getDesSpeedLeft(), getDesSpeedRight()-decl_right);
			
		if (getDesSpeedRight() == 0 && getDesSpeedLeft() == 0)	
			stopStopwatch1(); // Decceleration has finished!
			
		max_speed_left = getDesSpeedLeft(); // Update max_speed value
		max_speed_right = getDesSpeedRight(); // Update max_speed value
		setLEDs(0b000000);			   // and clear LEDs
		setStopwatch1(0);
	}
	
	// Make sure we don't move after Direction has changed and key is released too fast. 
	// This prevents the RP6 from moving when the direction has just changed and temporary saved
	// speed value is written back again in the task_motionControl function.
	if(getDesSpeedLeft() > max_speed_left) 
	{
		if(getDesSpeedLeft() <= 10) // If left speed is less or equal than 10...
			moveAtSpeed(0, getDesSpeedRight()); 		// ... stop the left motor
		else    // decelerate:
			moveAtSpeed(getDesSpeedLeft()-decl_left, getDesSpeedRight()); 
	}
	if(getDesSpeedRight() > max_speed_right) 
	{
		if(getDesSpeedRight() <= 10) // If right speed is less or equal than 10...
				moveAtSpeed(getDesSpeedLeft(), 0); 		// ... stop the right motor
			else    // decelerate:
				moveAtSpeed(getDesSpeedLeft(), getDesSpeedRight()-decl_right); 
	}
}

/*****************************************************************************/
// Main - The program starts here:

int main(void)
{
	initRobotBase(); 
	
	setLEDs(0b111111);
	writeChar('\n');
    writeString_P("RP6 controlled by RC5 TV Remote\n");
	writeString_P("___________________________\n");
	mSleep(500);	 
	setLEDs(0b000000); 
	powerON();
	
	// Set the RC5 Receive Handler:
	IRCOMM_setRC5DataReadyHandler(receiveRC5Data);
	
	//Output small usage instructions and the RC5 Codes:
	writeString_P("\nYou can control your RP6 with the following RC5 Keycodes:");
	writeString_P("\n-----------------------");
	writeString_P("\n * Turn Left: "); 					writeInteger(RC5_KEY_LEFT, DEC);
	writeString_P("\n * Turn Right: "); 				writeInteger(RC5_KEY_RIGHT, DEC);
	writeString_P("\n * Move Forwards: "); 				writeInteger(RC5_KEY_FORWARDS, DEC);
	writeString_P("\n * Move Backwards: "); 			writeInteger(RC5_KEY_BACKWARDS, DEC);
	writeString_P("\n * Stop: "); 						writeInteger(RC5_KEY_STOP, DEC);
	writeString_P("\n * Move curve left forwards: "); 	writeInteger(RC5_KEY_CURVE_LEFT, DEC);
	writeString_P("\n * Move curve right forwards: "); 	writeInteger(RC5_KEY_CURVE_RIGHT, DEC);
	writeString_P("\n * Move curve left backwards: "); 	writeInteger(RC5_KEY_CURVE_BACK_LEFT, DEC);
	writeString_P("\n * Move curve right backwards: "); writeInteger(RC5_KEY_CURVE_BACK_RIGHT, DEC);
	writeString_P("\n * Motor left forwards: "); 		writeInteger(RC5_KEY_LEFT_MOTOR_FWD, DEC);
	writeString_P("\n * Motor left backwards: "); 		writeInteger(RC5_KEY_LEFT_MOTOR_BWD, DEC);
	writeString_P("\n * Motor right forwards: "); 		writeInteger(RC5_KEY_RIGHT_MOTOR_FWD, DEC);
	writeString_P("\n * Motor right backwards: "); 		writeInteger(RC5_KEY_RIGHT_MOTOR_BWD, DEC);
	writeString_P("\n-----------------------\n");
	writeString_P("To change the key mapping, read the comments in the program source code!\n");
	writeString_P("_________\nPlease make sure that your IR Remote Control really transmits RC5 code!\n");
	
	startStopwatch2();
	
	// Main loop 
	while(true) 
	{
		deccelerate(); // Call the deceleration function.
		task_RP6System(); // Motion Control tasks etc.
	}
	return 0;
}
