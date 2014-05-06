/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - RP6 C-Code examples
 * ****************************************************************************
 */

/*****************************************************************************/
// Includes:

#include <RP6ControlLib.h> 		// The RP6 Control Library. 
#include "test_codestuff.h"

/*****************************************************************************/

int main(void)
{
	initRP6Control(); // Always call this first! The Processor will not work
					  // correctly otherwise. 

	initLCD(); // Initialize the LC-Display (LCD)
		   // Always call this before using the LCD!
			   
	writeString_P("\n\nRP6 CONTROL: C-code examples\n"); 
	showScreenLCD("C Code examples", "     ....");

	run_testCases();
	
	return 0;
}

