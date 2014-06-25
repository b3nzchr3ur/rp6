// Uncommented Version of RP6Control_LCD.c
// written by Dominik S. Herwald
// ------------------------------------------------------------------------------------------
// Uncommented Version of the LCD Demo Program.
// Good template for your own programs and also good to see the structure of
// the program. 
//
// ------------------------------------------------------------------------------------------


#include "RP6ControlLib.h" 

void runningLight(void)
{
	static uint8_t runLight = 1; 
	static uint8_t dir; 
	
	if(getStopwatch1() > 100)
	{
		setLEDs(runLight); 
	
		if(dir == 0)
			runLight <<= 1; 
		else
			runLight >>= 1;
			
		if(runLight > 7 ) 
			dir = 1;			
		else if (runLight < 2 ) 
			dir = 0;

		setStopwatch1(0);
	}
}

void runLCDText(void)
{
	static uint8_t cnt = 0;
	static uint8_t dir = 0;
	
	if(getStopwatch2() > 400)
	{
		clearPosLCD(0, cnt, 5);
		clearPosLCD(1, (11 - cnt), 5);

		if(dir == 0)
			cnt++;
		else
			cnt--;
	
		if(cnt > 10)
			dir = 1;
		if(cnt < 1)
			dir = 0;
		
		setCursorPosLCD(0, cnt);
		writeStringLCD_P("HELLO"); 

		setCursorPosLCD(1, (11 - cnt) ); 
		writeStringLCD_P("WORLD");
	
		setStopwatch2(0);
	}
}

void takeABreakAfterSomeTime(void)
{
	static uint8_t putScreenOnceOnly; 

	if(getStopwatch3() > 24000)
	{
		clearLCD();
		startStopwatch1();
		startStopwatch2();
		setStopwatch3(0); 
		sound(160,20,20);
		sound(220,40,0);
	}
	else if(getStopwatch3() > 22000)
	{
		if(!putScreenOnceOnly)
		{
			showScreenLCD("OK - let's", "go on!");
			putScreenOnceOnly = 1;
		}
	}
	else if(getStopwatch3() > 16000)
	{
		if(isStopwatch1Running()) 
		{
			stopStopwatch1();
			stopStopwatch2();
			showScreenLCD("I'll take a", "short break now!");
			putScreenOnceOnly = 0;
			sound(220,40,20);
			sound(160,40,0);
		}
	}
}

int main(void)
{
	initRP6Control();
	initLCD(); 
	
	writeString_P("\n\n   _______________________\n");
	writeString_P("   \\| RP6  ROBOT SYSTEM |/\n");
	writeString_P("    \\_-_-_-_-_-_-_-_-_-_/\n\n");
	
	writeString_P("Hello World from RP6 CONTROL!\n"); 
	writeString_P("Let's move it! :)\n"); 

	setLEDs(0b1111);
	mSleep(500);
	setLEDs(0b0000);
	
	sound(180,80,25);
	sound(220,80,0);

	showScreenLCD("################", "################");
	mSleep(1500);
	showScreenLCD("<<RP6  Control>>", "<<LC - DISPLAY>>");
	mSleep(2500); 
	showScreenLCD("Hello World", "Example Program");
	mSleep(2500);
	clearLCD(); 
	
	startStopwatch1();
	startStopwatch2();
	startStopwatch3();
	
	while(true) 
	{
		runLCDText();
		runningLight();
		takeABreakAfterSomeTime();
	}
	return 0;
}
