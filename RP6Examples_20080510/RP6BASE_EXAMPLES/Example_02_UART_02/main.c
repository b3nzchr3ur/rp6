/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - ROBOT BASE EXAMPLES
 * ****************************************************************************
 * Example: Serial Interface Example - Question and Answer
 * Author(s): Dominik S. Herwald
 * Version 2.0 --> Updated 10.09.2007 to support new UART Reception routines
 * 				   in the RP6 Libary! The program now also has some new 
 *				   features - e.g. it now checks if user really enteres a 
 *				   number... 
 * ****************************************************************************
 * Description:
 * This sample program shows how to use the RP6Library functions to receive
 * data with the serial interface. It is a "Question and Answer" program. 
 * It is a bit longer, but don't be afraid - the next programs are a lot
 * shorter again. Also have a look at the uncommented version of this program!
 *
 * ############################################################################
 * The Robot does NOT move in this example! You can simply put it on a table
 * next to your PC and you should connect it to the PC via the USB Interface!
 * ############################################################################
 * ****************************************************************************
 */

/*****************************************************************************/
// Includes:

#include "RP6RobotBaseLib.h" 	// The RP6 Robot Base Library.
								// Always needs to be included!


/*****************************************************************************/
// Chaselight

/**
 * This is a small chaselight function. This has nothing to do with the 
 * UART, it is just a demo application. 
 * Btw. this is nearly the same code as in example 1 - just without the comments!
 * And this one has a small "blanking" effect.
 */
void runningLight(void)
{
	static uint8_t runningLight = 1; // static variable used to store 	
								   // running light value. A static variable 
								   // does NOT "forget" about its value when
								   // the function ends. It is like a global
								   // variable, but only accessible within this
								   // Function!
								   // runningLight is initialized with 1 
								   // ONCE only!
								   
	// The running-light code fragment you already know from example 1:
	setLEDs(runningLight); 	
	runningLight <<= 1;
	if(runningLight > 32)
		runningLight = 1; 	
	mSleep(100); 
	
	// A small blanking effekt...
	// If StatusLED3 or StatusLED6 are on, 
	// turn off all LEDs for 200ms:
	if(statusLEDs.LED3 || statusLEDs.LED6 )
	{
		setLEDs(0b000000);
		mSleep(200);
	}
}

/**
 * Another demo function that will be called as a response to one of your answers.
 * It uses the "writeStringLength" function to output some text - well this is hard to 
 * describe - just look at the output on the serial interface!
 */
void outputTextStuff(void)
{
	uint8_t x, y; // define TWO local variables x and y. (In comparison to the static
				  // variable in the function above, these DO "forget" about their values
				  // when this function ends)
				 
	for(x = 0; x < 53; x++)
	{
		writeStringLength("1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ",10,x);
		writeChar(' ');
		for(y = 0; y < x; y++) // output x times "#" on the serial interface...
			writeChar('#');
		mSleep(200); 
		writeChar('\n');
	}
}

/*****************************************************************************/
// Question game

// A global variable that stores the current question:
int8_t question = 0;

/**
 * Here we ask a question and receive the answer from the user. 
 * The function uses some of the reception routines of the RP6Library.
 * It is called from the main loop (s. below).
 */
void askAQuestion(void)
{
	uint8_t charsToReceive = 1; // By default, we only want to receive 1 character
	
	// ------------------------------------------
	// Ask a question (the variable "question" contains the number of the current question):
	
	switch(question)
	{
		case 0: // Question 0
			writeString_P("What's your name? (max. 32 characters)\n"); 
			charsToReceive = 32;  // here we want to receive maximum 32 characters
								  // and not only the default 1 character
		break;
		case 1: // Question 1 ...
			writeString_P("How old are you? (max. 3 digits please)\n"); 
			charsToReceive = 3; // receive 3 characters
		break;
		case 2: // 2 ...
			writeString_P("Do you want to see some Text output? (\"y\" or \"n\")\n"); 
			// bytesToReceive has the value 1 by default, so we don't need to 
			// set it to 1 to receive one character.
		break;
		case 3: 
			writeString_P("Do you want to see blinking LEDs? (\"y\" or \"n\")\n"); 
		break;
		case 4: 
			writeString_P("Do you want to do all this again? (\"y\" or \"n\")\n"); 
		break;
	}
	
	// ------------------------------------------
	// Receive Answer (UPDATED - this was changed for compatibility with new RP6Lib):
	
	char receiveBuffer[charsToReceive+1]; // our reception buffer is one byte larger
										  // than the data we want to receive, because
										  // we also need to receive the "Newline" character!
	
	
	
	clearReceptionBuffer(); // Make sure reception Buffer is empty and no junk data 
							// is left in it.
	
	uint8_t cnt;
	for(cnt = 0; cnt < charsToReceive; cnt++) {
		receiveBuffer[cnt]=0;
	}
	
	uint8_t buffer_pos = 0;
	while(true) // Loop until we received one line of Data!
	{ 
		if(getBufferLength())    // Check if we still have data (means getBufferLength() 
		{						 // is not zero)	
			receiveBuffer[buffer_pos] = readChar(); // get next character from reception buffer
			if(receiveBuffer[buffer_pos]=='\n') // End of line detected!
			{
				receiveBuffer[buffer_pos]='\0'; // Terminate String with a 0, so other routines.
				buffer_pos = 0;                 // can determine where it ends!
												// We also overwrite the Newline character here.
				break; // We are done and can leave reception loop!
			}
			else if(buffer_pos >= charsToReceive) // IMPORTANT: We can not receive more 
			{								 	  // characters than "charsToReceive" because
												  // our buffer wouldn't be large enough!
				receiveBuffer[charsToReceive]='\0';	// So if we receive more characters, we just 
												     // stop reception and terminate the String.
				writeString_P("\n\nYou entered more characters than possible!\n");
				break; // We are done and can leave reception loop!
			}
			buffer_pos++;
			
		}										 
	}
	writeChar('\n');
	// ------------------------------------------
	// Do something with the received answer:
	
	writeString_P("Your input was (Bytes):\n");

	for(cnt = 0; cnt < charsToReceive; cnt++) {
		writeInteger(receiveBuffer[cnt],DEC);
		writeChar(',');
	}
	writeInteger(receiveBuffer[charsToReceive],DEC);
	writeString_P("\n");
	
	switch(question) // select the question...
	{
		case 0: // Question 0: What's your name?
			writeString_P("Hello \"");
			writeString(receiveBuffer); // Output the received data as a String
			writeString_P("\" !\n");
		break;
		case 1: // Your age?
			{ // we need to declare a block here to use local variables!
			
				// Check if a number was entered:
				uint8_t cnt;
				uint8_t not_a_number = 0;
				for(cnt = 0;cnt < 3 && receiveBuffer[cnt] != '\0'; cnt++)
				{
					if(!(receiveBuffer[cnt] >= '0' && receiveBuffer[cnt] <= '9'))
					{
						not_a_number = 1; // set flag to show that the user entered 
						break;			  // characters not between 0..9.
					}
				}
				
				if(!not_a_number) // Check flag...
				{
					writeString_P("So your age is: \""); 
					writeString(receiveBuffer);
					writeString_P("\" ! Guess what? Nobody cares... ;-)\n");
				}
				else
				{
					writeString_P("You entered:\""); 
					writeString(receiveBuffer);
					writeString_P("\"...\n... and this is clearly not a number! ;->\n");
					writeString_P("I will give you another chance to enter a number...\n\n");
					question=0; // do it again if entered data is not a number!
				}
			}
		break;
		case 2: // Text output?
			// Check if the answer was "y":
			if(receiveBuffer[0] == 'y') // Answer is 'y' ?
			{
				// Output some text. Here we use the writeStringLength
				// function to demonstrate a bit better how this one works. 
				writeString_P("Here we go!\n");
				outputTextStuff(); 	// Call output function
				writeString_P("\n\nThat's it!\n\n"); 
			}
			else // No the answer was not 'y' ... most likely it was 'n', but it does
			     // not have to be 'n' - we don't need to care about that.
				writeString_P("OK then... let's move on!\n"); 
		break;
		case 3: // Chaselight?
			if(receiveBuffer[0] == 'y') // Answer is 'y' ?
			{
				writeString_P("Now you get the ultimative running light!\n");
				uint8_t i = 0;
				for(;i < 36; i++) // run the running-light function 36 times
					runningLight();
				writeString_P("Oh ... it's over!\n");
			}
			else
				writeString_P("Well, if you don't want that... :*( \n"); 
		break;
		case 4: // Again?
			if(receiveBuffer[0] == 'y') // Answer was 'y' ?
			{
				writeString_P("Great! Here we go again...\n"); 
				question = -1; // If "question" is set to -1, the question/answer game
							   // will jump back to the first question as the variable "question"
							   // is incremented at the end of this function (s. line // Next question...)
							   // it gets the value 0
			}
			else
			{
				writeString_P("OK good bye! I will rest in an infinite running light loop\n");
				writeString_P("until someone resets me!\n"); 
				question = -2; // If "question" is set to -2, the increment at the end
							   // of this function only sets it to -1 and thus the main
							   // loop will quit the question answer game as it checks for this!
							   // (s. below)
			}
		break;
	}
	
	question++; // Next question...
}


/*****************************************************************************/
// Main function - The program starts here:

int main(void)
{
	initRobotBase(); // Always call this first! The Processor will not work
					 // correctly otherwise.

	setLEDs(0b111111); // Turn all LEDs on
	mSleep(500);       // delay 500ms
	setLEDs(0b000000); // All LEDs off

	// Write a message to UART:
	writeString_P("\nHello! My name is RP6!\n\n");
	
	// ---------------------------------------
	// Main loop:
	while(true)
	{
		if(question != -1) // Ask a question? This gets false if "question" has the value -1.
			askAQuestion(); // Yes!
		else              // No we are done! (question = -1 ...)
			runningLight(); // END! Show chaselight, forever!
	}
	// End of main loop
	// ---------------------------------------

	return 0;
}

/*****************************************************************************/
// EOF
