// Uncommented Version of RP6Base_SerialInterface_02.c
// written by Dominik S. Herwald
// ------------------------------------------------------------------------------------------
// 
// This is the uncommented version of "RP6Base_SerialInterface_02.c".
//
// Remember:
// If you want to compile this uncommented program - you need to change the makefile or
// rename this file. Or better copy it to a new folder together with the makefile
// and batch files, create a new project etc....
//
// ------------------------------------------------------------------------------------------

#include "RP6RobotBaseLib.h" 

void chaseLight(void)
{
	static uint8_t testLEDs = 1; 
	
	setLEDs(testLEDs); 	
	testLEDs <<= 1;
	if(testLEDs > 32)
		testLEDs = 1; 	
	mSleep(100); 
	
	if(statusLEDs.LED3 || statusLEDs.LED6 )
	{
		setLEDs(0b000000);
		mSleep(200);
	}
}

void outputTextStuff(void)
{
	uint8_t x,y;
	for(x = 0; x < 53; x++)
	{
		writeStringLength("1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ",10,x);
		writeChar(' ');
		for(y = 0; y < x; y++)
			writeChar('#');
		mSleep(200); 
		writeChar('\n');
	}
}

int8_t question = 0;

void askAQuestion(void)
{
	uint8_t charsToReceive = 1;
	
	switch(question)
	{
		case 0:
			writeString_P("What's your name? (max. 32 characters)\n"); 
			charsToReceive = 32;  
		break;
		case 1:
			writeString_P("How old are you? (max. 3 digits please)\n"); 
			charsToReceive = 3;
		break;
		case 2: 
			writeString_P("Do you want to see some Text output? (\"y\" or \"n\")\n"); 
		break;
		case 3: 
			writeString_P("Do you want to see blinking LEDs? (\"y\" or \"n\")\n"); 
		break;
		case 4: 
			writeString_P("Do you want to do all this again? (\"y\" or \"n\")\n"); 
		break;
	}
	
	char receiveBuffer[charsToReceive+1];

	clearReceptionBuffer(); 
	
	uint8_t buffer_pos = 0;	
	while(true) 
	{ 
		if(getBufferLength()) 
		{		
			receiveBuffer[buffer_pos] = readChar();
			if(receiveBuffer[buffer_pos]=='\n')
			{
				receiveBuffer[buffer_pos]='\0'; 
				buffer_pos = 0; 
				break;
			}
			else if(buffer_pos >= charsToReceive)
			{		
				receiveBuffer[charsToReceive]='\0';
				writeString_P("\n\nYou entered more characters than possible!\n");
				break;
			}
			buffer_pos++;
		}										 
	}

	switch(question)
	{
		case 0: 
			writeString_P("Hello \"");
			writeString(receiveBuffer);
			writeString_P("\" !\n");
		break;
		case 1:
			{
				uint8_t cnt;
				uint8_t not_a_number = 0;
				for(cnt = 0;cnt < 3 && receiveBuffer[cnt] != '\0'; cnt++)
				{
					if(!(receiveBuffer[cnt] >= '0' && receiveBuffer[cnt] <= '9'))
					{
						not_a_number = 1;
						break;
					}
				}
				
				if(!not_a_number)
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
					question=0;
				}
			}
		break;
		case 2:
			if(receiveBuffer[0] == 'y')
			{
				writeString_P("Here we go!\n");
				outputTextStuff();
				writeString_P("\n\nThat's it!\n\n"); 
			}
			else 
				writeString_P("OK then... let's move on!\n"); 
		break;
		case 3:
			if(receiveBuffer[0] == 'y')
			{
				writeString_P("Now you get the ultimative running light!\n");
				uint8_t i = 0;
				for(;i < 36; i++)
					runningLight();
				writeString_P("Oh ... it's over!\n");
			}
			else
				writeString_P("Well, if you don't want that... :*( \n"); 
		break;
		case 4:
			if(receiveBuffer[0] == 'y')
			{
				writeString_P("Great! Here we go again...\n"); 
				question = -1; 
			}
			else
			{
				writeString_P("OK good bye! I will rest in an infinite running light loop\n");
				writeString_P("until someone resets me!\n"); 
				question = -2; 
			}
		break;
	}
	question++;
}

int main(void)
{
	initRobotBase(); 

	setLEDs(0b111111); 
	mSleep(500); 
	setLEDs(0b000000);

	writeString_P("\nHello! My name is RP6!\n\n");
	
	while(true)
	{
		if(question != -1)
			askAQuestion();
		else 
			chaseLight();
	}
	return 0;
}
