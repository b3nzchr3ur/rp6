/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - ROBOT BASE EXAMPLES
 * ****************************************************************************
 * Example: ACS Example
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 * Short Description:
 * A small ACS (Anti Collision System) example. It shows the ACS status with 
 * the LEDs and writes messages to the UART, which indicate whether an obstacle 
 * has been detected left, right or in the middle.
 *
 * As this is very similar to check - also the Bumper status is shown in 
 * this program.
 * 
 * When the program is running, move a hand or any other object in front of 
 * the Robot and press the Bumpers and watch the Text output!
 *
 * You can use this program to test the ACS sensivity a little bit.
 * It is good to know what obstacles the ACS can not detect properly.
 * You should try out different power settings (s. main function below)!
 *
 * ############################################################################
 * The Robot does _NOT_ move in this example! You can simply put it on a table
 * next to your PC and you should connect it to the PC via the USB Interface!
 * ############################################################################
 * ****************************************************************************
 */

/*****************************************************************************/
// Includes:

#include "RP6RobotBaseLib.h" // Always needs to be included!

/*****************************************************************************/
// ACS Info:

/**
 * This function is called AUTOMATICALLY by the task_ACS() function in the 
 * RP6Library everytime the sensor reports __different__ sensor status.
 * This means that it is called if left/right sensor detects an obstacle AND if
 * it reports "no obstacle" again.
 * This function can be called automatically because it has been registered
 * as an event handler in the main function:
 * ACS_setStateChangedHandler(acsStateChanged);
 * (s. main function below)
 *
 * Everytime it is called, it prints out infos about current ACS status 
 * and sets status LEDs.
 *
 * In a real application program, you could initiate an obstacle avoiding 
 * movement that causes the robot to move in a different direction...
 * (s. other example programs)
 * 
 * Btw. you don't need to use the event handler method to check for 
 * ACS Obstacles - you can also use a simple "if(obstacle_left)" condition
 * everywhere else in your programs. But it can be more comfortable to let the
 * RP6Library check this for you. 
 */
void acsStateChanged(void)
{
	// Print messages to UART:
	writeString_P("ACS state changed   L: ");
	
	if(obstacle_left)  // Left "sensor" has detected something
		writeChar('o');
	else
		writeChar(' ');
		
	writeString_P(" | R: ");
	
	if(obstacle_right) // Right "sensor" has detected something
		writeChar('o');
	else
		writeChar(' ');	
		
	if(obstacle_right && obstacle_left) // left AND right sensor have detected something...
		writeString_P("   MIDDLE!");
		
	writeChar('\n');
	
	// -----------------------------------------------------------
	// Set LEDs
	
	// Here you can see another method to set the LEDs. 
	// You can use a shadow register and set the LEDs like:
	// statusLEDs.LED1 = true;
	// updateStatusLEDs();
	// This would turn on LED1 - and leaves all other LEDs like they are.
	
	if(obstacle_left && obstacle_right) // Obstacle in the middle?
		statusLEDs.byte = 0b100100;
	else
		statusLEDs.byte = 0b000000;
	statusLEDs.LED5 = obstacle_left;
	statusLEDs.LED4 = (!obstacle_left);  // Inverted LED5!
	statusLEDs.LED2 = obstacle_right;
	statusLEDs.LED1 = (!obstacle_right);  // Inverted LED2!
	
	// Update the LED values:
	updateStatusLEDs(); // The LEDs are turned on/off here! __Not__ in the lines above!	
}

/**
 * This function works similar to the ACS function and gets called everytime the
 * Bumpers are hit (the Bumpers need to be pressed down for at least 100ms to make
 * sure that it is registered by the program!)
 */
void bumpersStateChanged(void)
{
	writeString_P("Bumpers state changed!   BPL: ");
	
	if(bumper_left)  // Left Bumper
		writeChar('o');
	else
		writeChar(' ');
		
	writeString_P(" | BPR: ");
	
	if(bumper_right) // Right Bumper
		writeChar('o');
	else
		writeChar(' ');
		
	writeChar('\n');
}


/*****************************************************************************/
// Main - The program starts here:

int main(void)
{
	initRobotBase(); // Always call this first! The Processor will not work
					 // correctly otherwise. 

	// Write Message to UART:
    writeString_P("\nRP6 ACS - Testprogram\n");
	writeString_P("_____________________\n\n");

	setLEDs(0b111111);
	mSleep(1000);
	setLEDs(0b001001);
	
	// ---------------------------
	// Register Event Handlers:
	
	// Set ACS state changed event handler:
	ACS_setStateChangedHandler(acsStateChanged);
	
	// Set Bumpers state changed event handler:
	BUMPERS_setStateChangedHandler(bumpersStateChanged);

	// ---------------------------
	
	powerON(); // Activate all important Systems and turn power LED on.
			   // You always need to perform this command before you can
			   // use the ACS!
			   // Usually some of the RP6 Systems like the encoders, the ACS
			   // (and thus also IRCOMM reception),
			   // the current sensors and the power on LED are turned off to
			   // save Battery power (saves about 10mA).
			   // You can use powerOFF() to disable all those systems.

	// ---------------------------

	setACSPwrMed(); // Set ACS to medium power/range
	
	// These are the __alternative__ settings:
	// setACSPwrLow(); // Low power
	// setACSPwrHigh(); // High Power
	// and of course you can use: 
	// setACSPwrOff(); to turn it off completely!
	// You can uncomment (= remove the "//" in front of a line) ONE of these 
	// lines and test how it affects the maximum range of the ACS and which
	// Obstacles it can detect.

	// ---------------------------
	
	// Main loop
	while(true) 
	{		
		// This very very important function:
		task_RP6System();
		// handles a lot of things for you. For this example, it controls the
		// ACS and sets the obstacle_right and obstacle_left values if it detects
		// an obstacle. If you registered an event handler (s. above), it calls this 
		// event handler automatically as soon as one of these two values changes. 
		// Nearly the same applies for the bumpers - you can check the state
		// of each bumper with bumper_left and bumper_right and everytime the
		// state changes the event handler is called automatically. 
		
		// In some of the next examples you will see that this function also
		// controls the motors and some motion functionality. 
		
		// But be careful with delay functions or other things that take a long
		// time (gets problematic if there are delays greater than 50ms) if you 
		// use this! The delays may disturb the automatic control of all those 
		// systems mentioned above. 
		
		// Why do we need such a function? 
		// Well, several things that the MEGA32 needs to take care about are not
		// that easy to understand (look at the task_ACS function 
		// and some of the other functions in the RP6RobotBaseLib.c!) 
		// and this makes it a lot easier for you! 
		// The only thing you really need to take care about is to call
		// task_RP6System frequently! Then you will not have big problems usually.
	}
	return 0;
}
