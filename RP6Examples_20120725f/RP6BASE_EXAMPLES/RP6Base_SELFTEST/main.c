/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - ROBOT BASE EXAMPLES
 * ****************************************************************************
 * Example: SELFTEST PROGRAM - NEW VERSION 1.4 with Display support
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 *
 * Test routines for all RP6 components. 
 *
 * ----------------------------------------------------------------------
 * UPDATE v. 1.4 - 05.11.2007 - Now the Selftest Program supports Sensor 
 * Displays for Encoder Adjustment. 
 * It uses a special RP6Library Version with some additional 
 * routines for generating the Encoder Waveforms. This version is NOT
 * useable for general RP6 Programs!
 * ----------------------------------------------------------------------
 *
 * ----------------------------------------------------------------------
 * UPDATE 25.09.2007 - The input routines have been changed to work with
 * new RP6 UART Library functions. Now the User input for the advanced
 * speed tests are much easier and there are some additional features. 
 * Please look at the text instructions you see before the tests start. 
 * ----------------------------------------------------------------------
 * 
 *
 * You can compile this in two different versions. The FACTORY Version is
 * more restrictive with the voltage sensor and tests if voltage is about 
 * 7.20V. 
 * You can not guarantee this with standard accumulators of course and thus
 * you would need a laboratory power supply - which most of the home users
 * will not have. 
 *
 * --------
 *
 * Yes I know - this program has not the most beautyful code on earth... 
 * To be honest it is quite ugly ;) 
 * All the Text output is just a waste of programspace and this is 
 * intentionally to get a large program ;) 
 *
 * It is a good demonstration how big your programs can get! (and this
 * program only fills 21KB of the 30KB)
 * Consider that every single character of a String consumes one byte 
 * of program space!
 * Also look at it in the hexfile viewer in RP6Loader - nearly half the 
 * program memory is full with ASCII Text.
 *  
 *
 * ****************************************************************************
 */
 
 
/*****************************************************************************/
// Includes:

#include "RP6RobotBaseLib_debug.h" 	

/*****************************************************************************/

// Uncomment only ONE of the following definitions:
//#define FACTORY 
#define HOME

/*****************************************************************************/

void done(void)
{
	writeString_P("Done!\n"); 
}

void bars(uint8_t number)
{
	uint8_t cnt;
	writeChar('\n');
	for(;number > 0; writeChar('\n'), number--)
		for(cnt = 69; cnt ; cnt--)
			writeChar('#');
}

void test(uint8_t number)
{
	bars(2);
	writeString_P("#### TEST #");
	writeInteger(number, DEC);
	writeString_P(" ####\n");
}

void printUBat(uint16_t uBat)
{
	writeIntegerLength((((uBat/102.4f)+0.1f)), DEC, 2);
	writeChar('.');
	writeIntegerLength((((uBat/1.024f)+10)), DEC, 2);
	writeChar('V');
}

/*****************************************************************************/
//

void startDisplayPacket(uint8_t displayID)
{
	writeString_P("<%(");
	writeInteger(displayID, DEC);
	writeChar(',');
}

#define transmitDisplayDataS(__DATA__) {writeString(__DATA__);writeChar(',');}
#define transmitDisplayDataS_P(__DATA__) {writeNStringP(PSTR(__DATA__));writeChar(',');}
#define transmitDisplayDataI(__DATA__) {writeInteger(__DATA__,DEC);writeChar(',');}

void endDisplayPacket(void)
{
	writeString_P("x)%>\n");
}

#define DISPLAY_DIGITAL  0
#define DISPLAY_ANALOG   1
#define DISPLAY_TEXT 	 2
#define DISPLAY_PLOTTER  3
#define DISPLAY_WAVEFORM 4

#define LOWSPEED 0
#define HIGHSPEED 1
uint8_t connection_speed = 0;

void connectToDisplays(uint8_t con_speed)
{
	writeString_P("<%(CONNECT)%>\n");
}

#define createNewDisplay(__DISPLAY_ID__, __NAME__, __TYPE__, __LOWER_LIMIT__, __UPPER_LIMIT__) \
__createNewDisplay_P(__DISPLAY_ID__, PSTR(__NAME__), __TYPE__, PSTR(__LOWER_LIMIT__), PSTR(__UPPER_LIMIT__))

void __createNewDisplay_P(uint8_t displayID, const char *name, uint8_t type, const char *lower_limit, const char *upper_limit)
{
	writeString_P("<%(CREATE,");
	transmitDisplayDataI(displayID);
	writeNStringP(name);writeChar(',');
	transmitDisplayDataI(type);
	writeNStringP(lower_limit);writeChar(',');
	writeNStringP(upper_limit);writeChar(',');
	endDisplayPacket();
}

void startDisplays(void)
{
	writeString_P("<%(START)%>\n");
}


/*****************************************************************************/
//

char receiveBuffer[UART_RECEIVE_BUFFER_SIZE];

uint8_t getInputLine(void)
{ 
	static uint8_t buffer_pos = 0;
	if(getBufferLength()) 
	{							
		receiveBuffer[buffer_pos] = readChar();

		if(receiveBuffer[buffer_pos]=='\n')
		{
			receiveBuffer[buffer_pos]='\0'; 
			buffer_pos = 0;
			return 1;
		}
		else if(buffer_pos >= (UART_RECEIVE_BUFFER_SIZE-1))
		{									
			receiveBuffer[(UART_RECEIVE_BUFFER_SIZE-1)]='\0';	
			buffer_pos = 0;
			return 2;
		}
		buffer_pos++;
	}
	return 0;
}

void enterString(void)
{
	while(!getInputLine());
}

uint8_t enterX(void)
{
	enterString();
	return receiveBuffer[0]=='x' || receiveBuffer[0]=='X';
}

/*****************************************************************************/
//

void testPowerOn(void)
{
	test(1);
	writeString_P("\n### POWER ON TEST ###\n");
	writeString_P("Please watch the yellow PowerOn LED and verify that it lights up!\n");
	writeString_P("(it will flash a few times!)\n");
	mSleep(1000);
	powerON();
	mSleep(1000);
	powerOFF();
	mSleep(1000);
	powerON();
	mSleep(2000);
	powerOFF();
	mSleep(500);
	powerON();
	mSleep(500);
	powerOFF();
	done();
}

/*****************************************************************************/
//

void testLEDs(void)
{
	test(2);
	writeString_P("\n### LED Test ###\n");
	writeString_P("Please watch the LEDs and verify that they all work!\n");
	mSleep(1000);
	setLEDs(0b111111);
	mSleep(500);
	setLEDs(0b000000);
	mSleep(500);
	setLEDs(0b111111);
	mSleep(500);
	setLEDs(0b000000);
	mSleep(250);
	uint8_t testLEDs = 1;
	uint8_t i;
	for(i = 0; i < 12; i++){
		setLEDs(testLEDs); 
		testLEDs <<= 1; 
		if(testLEDs > 63)
			testLEDs = 1; 	
		mSleep(400);
	}
	setLEDs(0b111111);
	mSleep(400);
	setLEDs(0b000000);
	done();
}

/*****************************************************************************/
//

void testVoltageSensor(void)
{
	test(3);
	writeString_P("\n### Voltage Sensor Test ###\n");
#ifdef FACTORY
	writeString_P("Please adjust the **laboratory power supply** to 7.20V!\n");
#endif
#ifdef HOME
	writeString_P("Be sure that you are using good accumulators!\n");
	writeString_P("\n### Enter \"x\" and hit return to START this test!\n\n");
	enterX();
#endif
	writeString_P("Performing 10 measurements:\n");
	uint16_t ubat;
	uint8_t i;
	for(i = 1; i <= 10; i++)
	{
		writeString_P("Measurement #"); 
		writeInteger(i, DEC);
		writeString_P(": ");
		ubat = readADC(ADC_BAT);
		printUBat(ubat);
#ifdef FACTORY
		if(ubat >= 700 && ubat <= 770)
		{
			writeString_P(" --> OK!\n");
		}
		else 
		{
			writeString_P("\n####### ERROR: VOLTAGE IS TOO ");
			if(ubat < 700) 
			{
				writeString_P("LOW");
			}
			else if(ubat > 770)
			{
				writeString_P("HIGH");
			}
			writeString_P(" (should be min. 6.90V to max. 7.60V)!\n");
		}
#endif
#ifdef HOME
		if(ubat >= 570 && ubat <= 970)
		{
			writeString_P(" --> OK!\n");
		}
		else 
		{
			writeString_P("\nWARNING: VOLTAGE IS TOO ");
			if(ubat < 570) 
			{
				writeString_P("LOW");
			}
			else if(ubat > 970)
			{
				writeString_P("HIGH");
			}
			writeString_P(" (should be min. 5.50V to max. 9.50V)!\n");
		}
#endif
		mSleep(200);
	}
	done();
}

/*****************************************************************************/
//

void testBumpers(void)
{
	test(4);
	writeString_P("\nBumper Test\n");
	writeString_P("Please hit both bumpers and verify \n");
	writeString_P("that both Bumpers are working properly!\n\n");
	writeString_P("### The Test is running now.\n### Enter \"x\" and hit return to STOP this test!\n\n");
	clearReceptionBuffer();
	while(true)
	{
		if(getInputLine())
			break;
		
		static uint8_t info_left = false;
		static uint8_t info_right = false;
		uint8_t bumper_left = getBumperLeft();
		uint8_t bumper_right = getBumperRight();
		
		if(bumper_left) 
		{
			if(!info_left) 
			{			 
				writeString_P("OBSTACLE: LEFT!\n");
				info_left = true;
			}
		}
		else if(info_left) 
		{
			writeString_P("FREE: LEFT!\n");
			info_left = false;
		}
		if(bumper_right) 
		{
			if(!info_right) 
			{
				writeString_P("OBSTACLE: RIGHT!\n");
				info_right = true;
			}
		}
		else if(info_right) 
		{
			writeString_P("FREE: RIGHT!\n");
			info_right = false;
		}
	    statusLEDs.LED4 = (!bumper_left);
		statusLEDs.LED1 = (!bumper_right);
		updateStatusLEDs();
		mSleep(50);
	}
	setLEDs(0b000000);
	done();
}

/*****************************************************************************/
// 

void testLightSensors(void)
{
	test(5);
	writeString_P("\n### Light Sensor Test ###\n");
	writeString_P("Please get yourself a small flashlight!\n");
	writeString_P("While the test runs, move it in front of the Robot\n");
	writeString_P("and watch if the values change accordingly!\n\n");
#ifdef HOME
	writeString_P("### Enter \"x\" and hit return when you are ready!\n");
	enterX();
#endif
	writeString_P("### The Test is running now.\n### Enter \"x\" and hit return to STOP this test!\n\n");
	mSleep(1000);
	startStopwatch1();
	startStopwatch2();
	writeString_P("Performing measurements...:\n");
	uint16_t sens_l;
	uint16_t sens_r;
	clearReceptionBuffer();
	while(true)
	{
		if(getInputLine())
			break;
		
		if(getStopwatch1() > 50) 
		{		
			sens_l = readADC(ADC_LS_L);
			sens_r = readADC(ADC_LS_R);
			
			int16_t dif = ((int16_t)(sens_l - sens_r));
			if(dif > 160) dif = 160;
			if(dif < -160) dif = -160;
	
			// Set LEDs - a small bargraph display that shows which sensor has detected
			// brighter light - and how much brighter it is:
			if(dif > 140)
				setLEDs(0b111000);
			else if(dif > 75)
				setLEDs(0b011000);
			else if(dif > 35)
				setLEDs(0b001000);
			else if(dif > -35)
				setLEDs(0b001001);
			else if(dif > -75)
				setLEDs(0b000001);
			else if(dif > -140)
				setLEDs(0b000011);
			else 
				setLEDs(0b000111);
			setStopwatch1(0);
		}
		
		if(getStopwatch2() > 250) 
		{	
			sens_l = readADC(ADC_LS_L);
			sens_r = readADC(ADC_LS_R);

			writeString_P("Left: ");
			writeIntegerLength(sens_l, DEC, 4);
			writeString_P(",  Right: ");
			writeIntegerLength(sens_r, DEC, 4);
			writeChar('\n');
			setStopwatch2(0);
		}
	}
	stopStopwatch1();
	stopStopwatch2();
	done();
} 

/*****************************************************************************/
//

uint8_t RC5_data;
uint8_t RC5_error;
uint8_t RC5_test;

void receiveRC5Data(RC5data_t rc5data)
{
	writeString_P("RX RC5 Packet --> Toggle Bit:");
	writeChar(rc5data.toggle_bit + '0');
	writeString_P(" | Device Address:");
	writeInteger(rc5data.device, DEC);
	writeString_P(" | Key Code:");
	writeInteger(rc5data.key_code, DEC);
	
	if(RC5_test==2)
	{
		writeString_P(" --> ");
		if(rc5data.key_code == RC5_data 
			&& rc5data.device == (RC5_data & 0b11111) 
			&& rc5data.toggle_bit == ((RC5_data>>5)&1))
		{
			writeString_P("OK!");
			RC5_error = false;
			RC5_test = 10;
		}
		else
		{
			writeString_P("### ERROR!!! ###");
			RC5_error = true;
			RC5_test = 5;
		}
	}
	writeChar('\n');
}


void testACS(void)
{
	test(6);
	writeString_P("\nACS Test\n");
	writeString_P("Please move your hand or other obstacles in front of the Robot\n");
	writeString_P("and verify that both ACS channels are working properly!\n\n");
	writeString_P("ACS is set to MAXIMUM power/range!\n\n");
	
	writeString_P("You can also send RC5 Codes with a TV Remote Control\n");
	writeString_P("to the RP6 - it will display the Toggle Bit, Device Adress\n");
	writeString_P("and Keycode of the RC5 Transmission!\n");
	writeString_P("Make sure your remote control transmits in RC5 and not\n");
	writeString_P("SIRCS or RECS80 etc.! There are several other formats that will NOT work!\n\n");
	writeString_P("### The Test is running now.\n### Enter \"x\" and hit return to STOP this test!\n\n");

	powerON();
	setACSPwrHigh();

	clearReceptionBuffer();
	while(true)
	{
		if(getInputLine())
			break;
			
		task_ACS();

		static uint8_t info_left = false;
		static uint8_t info_right = false;
	
		if(isObstacleLeft()) 
		{
			if(!info_left) 
			{			 
				writeString_P("OBSTACLE: LEFT!\n");
				info_left = true;
			}
		}
		else if(info_left) 
		{
			writeString_P("FREE: LEFT!\n");
			info_left = false;
		}
		if(isObstacleRight()) 
		{
			if(!info_right) 
			{
				writeString_P("OBSTACLE: RIGHT!\n");
				info_right = true;
			}
		}
		else if(info_right) 
		{
			writeString_P("FREE: RIGHT!\n");
			info_right = false;
		}

		statusLEDs.byte = 0b000000;
		if(isObstacleLeft() && isObstacleRight())
			statusLEDs.byte = 0b100100;
		statusLEDs.LED5 = isObstacleLeft();
		statusLEDs.LED2 = isObstacleRight();
		statusLEDs.LED4 = (!isObstacleLeft());
		statusLEDs.LED1 = (!isObstacleRight());
		updateStatusLEDs();
	}
	setACSPwrOff();
	powerOFF();
	setLEDs(0b000000);
	done();
}

/*****************************************************************************/
//

void testRC5(void)
{
	test(7);
	writeString_P("\nIRCOMM Test\n");
	writeString_P("This test will transmit several RC5 Codes with the IRCOMM and check\n");
	writeString_P("if they are received back properly!\n");
	writeString_P("Please make sure that there is a white ceiling or some reflective\n");
	writeString_P("Object directly over the IRCOMM LEDs on the front of the RP6!\n\n");
	writeString_P("Please DO NOT send anything with TV Remote Controls or similar\n");
	writeString_P("as this will disturb the test and it will fail!\n\n");
	writeString_P("The ACS is active and the Robot will detect obstacles that you\nmove in front of it!\n\n");
#ifdef HOME
	writeString_P("\n### Enter \"x\" and hit return to START this test!\n\n");
	enterX();
#endif
	mSleep(500);
	powerON();
	setACSPwrOff();
	startStopwatch1();
	setLEDs(0b001000);
	RC5_data = 0;
	RC5_error = 0;
	RC5_test = 0;
	uint8_t errors = 0;
	uint8_t i = 0;
	while(true)
	{
		if(getStopwatch1() > 500) 
		{		
			if(RC5_test==2)
			{
				writeString_P("\nTIMEOUT!!!\n\n");
				errors = true;
				break;
			}
			writeString_P("TX RC5 Packet: ");
			writeInteger(i,DEC);
			writeChar('\n');
			
			IRCOMM_sendRC5(i, i);
			RC5_data = i;
			RC5_test = 2;
			i+=3;
			setStopwatch1(0);
		}
		if(RC5_test==10)
		{
			RC5_test = 0;
			setStopwatch1(450);
			if(i > 63)
			{
				writeString_P("\nTest finished!\n");
				break;
			}
		}
		if(RC5_test==5)
		{
			RC5_test = 0;
			writeString_P("\nRECEPTION ERROR!!!\n\n");
			errors = true;
			break;
		}
		
		task_ACS();
	}
	
	if(errors)
	{
		setLEDs(0b110110);
		writeString_P("####### !!!  WARNING WARNING WARNING  !!! #######\n");
		writeString_P("####### !!! TEST FINISHED WITH ERRORS !!! #######\n");
		writeString_P("####### !! Please check IRCOMM assembly ! #######\n");
		writeString_P("\n\nEnter any character to continue!\n");
		setACSPwrOff();
		powerOFF();
		enterString();
	}
	
	setACSPwrOff();
	stopStopwatch1();
	powerOFF();
	mSleep(1000);
	setLEDs(0b000000);
	done();
}

/*****************************************************************************/
//

uint16_t tENCL_min_l_time = 0;
uint16_t tENCL_min_h_time = 0;
uint16_t tENCL_max_l_time = 0;
uint16_t tENCL_max_h_time = 0;

uint16_t tENCR_min_l_time = 0;
uint16_t tENCR_min_h_time = 0;
uint16_t tENCR_max_l_time = 0;
uint16_t tENCR_max_h_time = 0;

extern volatile uint16_t ENCL_min_l_time;
extern volatile uint16_t ENCL_min_h_time;
extern volatile uint16_t ENCL_max_l_time;
extern volatile uint16_t ENCL_max_h_time;

extern volatile uint16_t ENCR_min_l_time;
extern volatile uint16_t ENCR_min_h_time;
extern volatile uint16_t ENCR_max_l_time;
extern volatile uint16_t ENCR_max_h_time;



void transmitEncoderWaveform(void)
{
	uint8_t k = 0;
	if(ENCL_trig == TRIG_PROCESS_DATA)
	{
		startDisplayPacket(16);
		transmitDisplayDataI(51);
		for(k = 0; k < 51; k++) {
			transmitDisplayDataI(ENCL_buffer_values[k]);
			transmitDisplayDataI(ENCL_buffer_time[k]);
		}
		endDisplayPacket();
		
		tENCL_min_l_time = ENCL_min_l_time;
		tENCL_min_h_time = ENCL_min_h_time;
		tENCL_max_l_time = ENCL_max_l_time;
		tENCL_max_h_time = ENCL_max_h_time;
		ENCL_trig = TRIG_WAIT;
		mSleep(10);
	}
	if(ENCR_trig == TRIG_PROCESS_DATA)
	{
		startDisplayPacket(17);
		transmitDisplayDataI(51);
		for(k = 0; k < 51; k++) {
			transmitDisplayDataI(ENCR_buffer_values[k]);
			transmitDisplayDataI(ENCR_buffer_time[k]);
		}
		endDisplayPacket();
		tENCR_min_l_time = ENCR_min_l_time;
		tENCR_min_h_time = ENCR_min_h_time;
		tENCR_max_l_time = ENCR_max_l_time;
		tENCR_max_h_time = ENCR_max_h_time;
		ENCR_trig = TRIG_WAIT;
		mSleep(10);
	}
}

void transmitSpeedAndBatteryVoltage(void)
{
	startDisplayPacket(1);
	transmitDisplayDataI(getLeftSpeed());
	transmitDisplayDataI(100);
	endDisplayPacket();
	startDisplayPacket(2);
	transmitDisplayDataI(getRightSpeed());
	transmitDisplayDataI(100);
	endDisplayPacket();
	startDisplayPacket(9);
	transmitDisplayDataI(adcBat);
	transmitDisplayDataI(100);
	endDisplayPacket();
	startDisplayPacket(3);
	transmitDisplayDataI(getLeftDistance());
	transmitDisplayDataI(100);
	endDisplayPacket();
	startDisplayPacket(4);
	transmitDisplayDataI(getRightDistance());
	transmitDisplayDataI(100);
	endDisplayPacket();
}

void testMotorsAndEncoders(void)
{
	test(8);
	writeString_P("\nAutomatic speed speed regulation test\n\n");
	writeString_P("#####################################################################\n");
	writeString_P("### ATTENTION!!! DANGER!!! WARNING!!!\n");
	writeString_P("Make sure that the RP6 can __NOT__ move!\n");
	writeString_P("The caterpillar tracks should __NOT__ touch the ground!\n(hold it in your hands for example...)\n");
	writeString_P("THE RP6 WILL START MOVING FAST! YOU CAN DAMAGE IT IF YOU DO NOT\n");
	writeString_P("MAKE SURE THAT IT CAN __NOT__ MOVE!\n");
	writeString_P("Make sure both crawler tracks are FREE RUNNING! DO NOT BLOCK THEM!\n");
	writeString_P("--> OTHERWISE THE TEST WILL FAIL!\n");
	writeString_P("#####################################################################\n\n");
#ifdef HOME
	writeString_P("### Enter \"x\" and hit return when TO START this test!\n### Make sure the RP6 can not move!\n\n");
	enterX();
	if(receiveBuffer[0]!='x')
		return;
#endif
	task_RP6System();

	powerON();
	setMotorDir(FWD,FWD);
	setMotorPower(0,0);
	moveAtSpeed(0,0);
	setStopwatch1(0);
	setStopwatch2(0);
	startStopwatch1();
	startStopwatch2();
	uint8_t state = 0;
	uint8_t cnt = 0;
	uint8_t speed_test = 0;
	uint8_t errors = 0;
	
	task_RP6System();
	while(true)
	{
		if(getStopwatch2() > 200)
		{

			transmitSpeedAndBatteryVoltage();
			
			writeString_P("T: ");
			writeIntegerLength(speed_test,DEC,3);
			writeString_P(" |VL: ");
			writeIntegerLength(getLeftSpeed(),DEC,3);
			writeString_P(" |VR: ");
			writeIntegerLength(getRightSpeed(),DEC,3);
			writeString_P(" |PL: ");
			writeIntegerLength(getMotorLeft(),DEC,3);
			writeString_P(" |PR: ");
			writeIntegerLength(getMotorRight(),DEC,3);
			writeString_P(" |IL: ");
			writeIntegerLength(adcMotorCurrentLeft,DEC,3);
			writeString_P(" |IR: ");
			writeIntegerLength(adcMotorCurrentRight,DEC,3);
			writeString_P(" |UB: ");
			printUBat(adcBat);
			writeChar('\n');
			if((getLeftSpeed() < 130 && adcMotorCurrentLeft > 120)
			   || (getLeftSpeed() >= 130 && adcMotorCurrentLeft > 150))
			{
				writeString_P("####### Left: CURRENT CONSUMPTION IS TOO HIGH! ########\n");
				errors++;
			}
			if((getRightSpeed() < 130 && adcMotorCurrentRight > 120)
			   || (getRightSpeed() >= 130 && adcMotorCurrentRight > 150))
			{
				writeString_P("####### Right: CURRENT CONSUMPTION IS TOO HIGH! ########\n");
				errors++;
			}
			
			if(getLeftSpeed() > 25 && getMotorLeft() >= 30 && adcMotorCurrentLeft < 7)
			{
				writeString_P("####### Left: Current Sensor is defect!!! Current measurement is too low! ########\n");
				errors++;
			}
			if(getRightSpeed() > 25 && getMotorRight() >= 30 && adcMotorCurrentRight < 7)
			{
					writeString_P("####### Right: Current Sensor is defect!!! Current measurement is too low! ########\n");
				errors++;
			}
#ifdef HOME
			if(adcBat < 550)
			{
				writeString_P("####### ERROR: Supply Voltage is too low (<5.50V)! Please check power supply! #######\n");
				errors++;
			}
#endif
#ifdef FACTORY
			if(adcBat < 650)
			{
				writeString_P("####### ERROR: Supply Voltage is too low (<6.50V)! Please check power supply! #######\n");
				errors++;
			}
#endif
			if(errors)
				break;

			setStopwatch2(0);
		}
		if(getStopwatch1() > 1000)
		{
				moveAtSpeed(speed_test,speed_test);
				if(cnt > 2)
				{
					if((getLeftSpeed() >= ((speed_test > 5) ? (speed_test - 10) : speed_test)) 
						&& (getLeftSpeed() <= (speed_test+10)))
					{
						writeString_P("Speed Left: OK\n");
					}
					else
					{
						writeString_P("####### ERROR Left ########\n");
						errors++;
					}

					if((getRightSpeed() >= ((speed_test > 5) ? (speed_test - 10) : speed_test)) 
						&& (getRightSpeed() <= (speed_test+10)))
					{
						writeString_P("Speed Right: OK\n");
					}
					else
					{
						writeString_P("####### ERROR Right #######\n");
						errors++;
					}
#ifdef FACTORY
					if(getMotorLeft() > (3*getLeftSpeed()))
					{
						writeString_P("####### ERROR: LEFT PWM Value is too high! Check motor assembly! #######\n");
						errors++;
						break;
					}
					if(getMotorRight() > (3*getRightSpeed()))
					{
						writeString_P("####### ERROR: LEFT PWM Value is too high! Check motor assembly! #######\n");
						errors++;
						break;
					}
#endif
					if(errors) break;

					if(state == 1)
					{
						if(speed_test <= 120)
						{
							if(speed_test >= 80)
								changeDirection(BWD);
							else if(speed_test >= 60)
								changeDirection(FWD);
							else if(speed_test >= 40)
								changeDirection(BWD);
							else if(speed_test < 40)
								state = 2;
						}
						speed_test -= 20;
#ifdef FACTORY
						if(speed_test >= 120)
							speed_test -= 20;
#endif
					}

					if(state == 0)
					{
#ifdef FACTORY
						if(speed_test >= 160)
							state = 1;
						if(speed_test >= 80)
							speed_test += 20;
#endif
#ifdef HOME
						if(speed_test >= 80)
							state = 1;
#endif
						speed_test += 20;
					}
					cnt = 0;
				}
				else
					cnt++;
			setStopwatch1(0);
		}
		
		transmitEncoderWaveform();

		task_RP6System();

		if(state==2)
		{
			moveAtSpeed(0,0);
			setMotorPower(0,0);
			break;
		}
		task_RP6System();
	}

	stopStopwatch1();
	setMotorPower(0,0);
	moveAtSpeed(0,0);
	setMotorDir(FWD,FWD);
	powerOFF();
	if(errors)
	{
		setLEDs(0b110110);
		writeString_P("####### !!!  WARNING WARNING WARNING  !!! #######\n");
		writeString_P("####### !!! TEST FINISHED WITH ERRORS !!! #######\n");
		writeString_P("#### Please check motor and encoder assembly! ###\n");
		writeString_P("#### And also check Sensors for defects!      ###\n");
		writeString_P("\n\nEnter any character to continue!\n");
		powerOFF();
		enterString();
	}
	else
	{
		writeString_P("\n***** MOTOR AND ENCODER TEST OK! *****\n");
		mSleep(1000);
	}
	setLEDs(0b000000);
}

void printMotorValues(void)
{
	writeString_P("PL: ");
	writeIntegerLength(getMotorLeft(),DEC,3);
	writeString_P(" | PR: "); 
	writeIntegerLength(getMotorRight(),DEC,3);
	writeString_P(" | IL: ");
	writeIntegerLength(adcMotorCurrentLeft,DEC,3);
	writeString_P(" | IR: ");
	writeIntegerLength(adcMotorCurrentRight,DEC,3);
	writeString_P(" | VL: ");
	writeIntegerLength(getLeftSpeed(),DEC,3);
	writeString_P(" | VR: ");
	writeIntegerLength(getRightSpeed(),DEC,3);
	writeChar('\n');
	writeString_P(" | DSTR: ");
	writeInteger(getRightDistance(),DEC);
	writeString_P(" | DSTL: ");
	writeInteger(getLeftDistance(),DEC);
	writeString_P(" ###  | UBAT: "); 
	printUBat(adcBat);
	writeChar('\n');
}

void speed_control(void)
{
	bars(2);
	writeString_P("\nManual speed regulation test\n\n");
	writeString_P("#####################################################################\n");
	writeString_P("### ATTENTION!!! DANGER!!! WARNING!!!\n");
	writeString_P("Make sure that the RP6 can __NOT__ move!\n");
	writeString_P("The caterpillar tracks should __NOT__ touch the ground!\n(hold it in your hands for example...)\n");
	writeString_P("THE RP6 CAN START MOVING VERY FAST! YOU CAN DAMAGE IT IF YOU DO NOT\n");
	writeString_P("MAKE SURE THAT IT CAN __NOT__ MOVE!\n");
	writeString_P("#####################################################################\n\n");
	writeString_P("\n\nUsage: Enter Speed values like: \n");
	writeString_P("0, 25, 94, 100, 155, 200 (max!) and hit Enter!\n");
	writeString_P("Enter \"end\" or \"x\" and hit return when you want to stop the test!\n");
	writeString_P("Enter \"fwd\", \"bwd\", \"l\" or \"r\" to change direction!\n");
	writeString_P("Enter \"x\" and hit return when you are ready to START!\n");
	enterX();
	clearReceptionBuffer();
	powerON();
	setMotorDir(FWD,FWD);
	setMotorPower(0,0);
	moveAtSpeed(0,0);
	uint8_t speed_value = 0;
	int16_t speed_value_tmp = 0;
	startStopwatch1();
	startStopwatch2();
	while(true)
	{	
		if(getInputLine())
		{
			if(strcmp(receiveBuffer,"end")==0 || strcmp(receiveBuffer,"x")==0)
			{
			    writeString_P("\nEND\n");
				break;
			}
			else if(strcmp(receiveBuffer,"fwd")==0)
			{
			    writeString_P("\nChange direction: FWD\n");
				changeDirection(FWD);
			}
			else if(strcmp(receiveBuffer,"bwd")==0)
			{
			    writeString_P("\nChange direction: BWD\n");
				changeDirection(BWD);
			}
			else if(strcmp(receiveBuffer,"l")==0)
			{
			    writeString_P("\nChange direction: Left\n");
				changeDirection(LEFT);
			}
			else if(strcmp(receiveBuffer,"r")==0)
			{
			    writeString_P("\nChange direction: Right\n");
				changeDirection(RIGHT);
			}
			else
			{
				speed_value_tmp = atoi(receiveBuffer);
				if(speed_value_tmp == 0)
				{
					uint8_t not_a_number = 0;
					int8_t i;
					for(i = strlen(receiveBuffer)-1; i >= 0 ; i--)
						if((receiveBuffer[i] < '0') || (receiveBuffer[i] > '9'))
						{
							not_a_number = 1;
							writeString_P("\n### NOT A NUMBER! ###\n");
							break;
						}
					if(not_a_number)
						speed_value_tmp = speed_value;	
				}
				else if(speed_value_tmp > 200){speed_value_tmp = 200;}
				else if(speed_value_tmp < 0) {speed_value_tmp = 0;}
				speed_value = speed_value_tmp;
				
				writeString_P("\n--> Change speed to:");
				writeInteger(speed_value_tmp,DEC);
				writeChar('\n');
							
				moveAtSpeed(speed_value,speed_value);
			}
		}
		if(getStopwatch1() > 250)
		{
			printMotorValues();
			setStopwatch1(0);
		}
		if(getStopwatch2() > 100)
		{
			transmitSpeedAndBatteryVoltage();
			setStopwatch2(0);
		}
		
		transmitEncoderWaveform();

		task_RP6System();
	}
	stopStopwatch1();
	stopStopwatch2();
	mSleep(500);
	setMotorPower(0,0);
	moveAtSpeed(0,0);
	setMotorDir(FWD,FWD);
	powerOFF();
	done();
}

void testMoveDistance(void)
{
	bars(2);
	writeString_P("\nMove Distance test\n");
	writeString_P("#####################################################################\n");
	writeString_P("### ATTENTION!!! DANGER!!! WARNING!!!\n");
	writeString_P("Make sure that the RP6 can __NOT__ move!\n");
	writeString_P("The caterpillar tracks should __NOT__ touch the ground!\n(hold it in your hands for example...)\n");
	writeString_P("THE RP6 CAN START MOVING VERY FAST! YOU CAN DAMAGE IT IF YOU DO NOT\n");
	writeString_P("MAKE SURE THAT IT CAN __NOT__ MOVE!\n");
	writeString_P("#####################################################################\n\n");
	writeString_P("This program can be used to calibrate the Encoder Resolution.\n");
	writeString_P("You can let the Robot drive specific distances with it and it\n");
	writeString_P("will output the driven Encoder counts.\n");
	writeString_P("\nUsage: Enter Distance values like: \n");
	writeString_P("150, 500, 1580, 4000, 30000 (max!)\n");
	writeString_P("Enter \"end\" or \"x\" and hit return when you want to stop the test!\n");
	writeString_P("Enter \"fwd\", \"bwd\", \"l\" or \"r\" to change direction!\n");
	writeString_P("The test is running already, you can Enter Distance values now!\n");

	powerON();
	setMotorDir(FWD,FWD);
	setMotorPower(0,0);
	moveAtSpeed(0,0);

	uint16_t dist = 0;
	int16_t dist_tmp = 0;

	while(true)
	{
		if(getInputLine())
		{
			if(strcmp(receiveBuffer,"end")==0)
			{
			    writeString_P("\nEND\n");
				break;
			}
			else if(strcmp(receiveBuffer,"x")==0)
			{
			    writeString_P("\nEND\n");
				break;
			}
			else if(strcmp(receiveBuffer,"fwd")==0)
			{
			    writeString_P("\nChange direction: FWD\n");
				changeDirection(FWD);
			}
			else if(strcmp(receiveBuffer,"bwd")==0)
			{
			    writeString_P("\nChange direction: BWD\n");
				changeDirection(BWD);
			}
			else if(strcmp(receiveBuffer,"l")==0)
			{
			    writeString_P("\nChange direction: Left\n");
				changeDirection(LEFT);
			}
			else if(strcmp(receiveBuffer,"r")==0)
			{
			    writeString_P("\nChange direction: Right\n");
				changeDirection(RIGHT);
			}
			else
			{
				dist_tmp = atoi(receiveBuffer);
				if(dist_tmp == 0)
				{
					uint8_t not_a_number = 0;
					int8_t i;
					for(i = strlen(receiveBuffer)-1; i >= 0 ; i--)
						if((receiveBuffer[i] < '0') || (receiveBuffer[i] > '9'))
						{
							not_a_number = 1;
							writeString_P("\n### NOT A NUMBER! ###\n");
							break;
						}
				}
				else if(dist_tmp < 0) {dist_tmp = 0;}
				dist = dist_tmp;
			
				writeString_P("\n--> Move:");
				writeInteger(dist_tmp,DEC);
				writeChar('\n');
				setStopwatch1(250);
				startStopwatch1();
				move(40,FWD,dist,false);
			}
		}
		if(getStopwatch1() > 250)
		{
			printMotorValues();
			transmitSpeedAndBatteryVoltage();
			if(isMovementComplete())
			{
				stopStopwatch1();
				mSleep(500);
				task_RP6System();
				printMotorValues();
				writeString("\n--> Movement is complete!\n");
			}
			setStopwatch1(0);
		}
		
		transmitEncoderWaveform();
		
		task_RP6System();
	}
	stopStopwatch1();
	mSleep(500);
	setMotorPower(0,0);
	moveAtSpeed(0,0);
	setMotorDir(FWD,FWD);
	powerOFF();
	done();
}



void testEncoderDutyCycle(void)
{
	bars(2);
	writeString_P("\nEncoder Duty Cycle test\n\n");
	writeString_P("#####################################################################\n");
	writeString_P("### ATTENTION!!! DANGER!!! WARNING!!!\n");
	writeString_P("Make sure that the RP6 can __NOT__ move!\n");
	writeString_P("The caterpillar tracks should __NOT__ touch the ground!\n(hold it in your hands for example...)\n");
	writeString_P("THE RP6 CAN START MOVING VERY FAST! YOU CAN DAMAGE IT IF YOU DO NOT\n");
	writeString_P("MAKE SURE THAT IT CAN __NOT__ MOVE!\n");
	writeString_P("#####################################################################\n\n");
	
	writeString_P("Speed regulation is disabled - the motors get constant Power!\n");
	writeString_P("This allows you to adjust the small potis on the Encoders\n");
	writeString_P("to set the duty cycle properly.");
	writeString_P("Optimal is a a duty cycle \n");
	writeString_P("of 50% - check this with the waveform displays in the RP6Loader Software.\n");
	writeString_P("On/Off time of the signal should be as equal as possible.\n");
	writeString_P("\nUsage: Enter PWM values like: \n");
	writeString_P("0, 25, 94, 100, 120 (LIMITED MAXIMUM!!!) and hit Enter!\n");
	writeString_P("Enter \"end\" or \"x\" and hit return when you want to stop the test!\n");
	writeString_P("Enter \"fwd\", \"bwd\", \"l\" or \"r\" to change motor turning direction!\n");
	writeString_P("The test is running already, you can Enter PWM values now!\n");

//	enterX();

	uint16_t pwm_tmp = 0;
	uint8_t pwm = 0;

	powerON();
	setMotorDir(FWD,FWD);
	moveAtSpeed(0,0);
	startStopwatch1();
	startStopwatch2();
	setMotorPower(pwm,pwm);
	mSleep(500);
	clearReceptionBuffer();
	while(true)
	{
		if(getInputLine())
		{
			if(strcmp(receiveBuffer,"end")==0 || strcmp(receiveBuffer,"x")==0)
			{
			    writeString_P("\nEND\n");
				break;
			}
			else if(strcmp(receiveBuffer,"fwd")==0)
			{
			    writeString_P("\nChange direction: FWD\n");
				setMotorDir(FWD,FWD);
			}
			else if(strcmp(receiveBuffer,"bwd")==0)
			{
			    writeString_P("\nChange direction: BWD\n");
				setMotorDir(BWD,BWD);
			}
			else if(strcmp(receiveBuffer,"l")==0)
			{
			    writeString_P("\nChange direction: Left\n");
				setMotorDir(BWD,FWD);
			}
			else if(strcmp(receiveBuffer,"r")==0)
			{
			    writeString_P("\nChange direction: Right\n");
				setMotorDir(FWD,BWD);
			}
			else
			{
				pwm_tmp = atoi(receiveBuffer);
				if(pwm_tmp == 0)
				{
					uint8_t not_a_number = 0;
					int8_t i;
					for(i = strlen(receiveBuffer)-1; i >= 0 ; i--)
						if((receiveBuffer[i] < '0') || (receiveBuffer[i] > '9'))
						{
							not_a_number = 1;
							writeString_P("\n### NOT A NUMBER! ###\n");
							break;
						}
					if(not_a_number)
						pwm_tmp = pwm;	
				}
				else if(pwm_tmp > 120){pwm_tmp = 120; writeString_P("\n--> Power limited to 120!");}
				else if(pwm_tmp < 0) {pwm_tmp = 0;}
				pwm = pwm_tmp;
				
				writeString_P("\n--> Change speed to:");
				writeInteger(pwm_tmp,DEC);
				writeChar('\n');
							
				setMotorPower(pwm,pwm);
			}
		}

		if(getStopwatch1() > 100)
		{
			if(pwm > 0)
			{
				writeString_P("P: ");
				writeIntegerLength(pwm,DEC,3);
				writeString_P(" |VL: ");
				writeIntegerLength(getLeftSpeed(),DEC,3);
				writeString_P(" |VR: ");
				writeIntegerLength(getRightSpeed(),DEC,3);
				writeString_P(" |IL: ");
				writeIntegerLength(adcMotorCurrentLeft,DEC,3);
				writeString_P(" |IR: ");
				writeIntegerLength(adcMotorCurrentRight,DEC,3);
				
				uint16_t tmp1 = (tENCL_min_l_time + tENCL_max_l_time) / 2;
				uint16_t tmp2 = (tENCL_min_h_time + tENCL_max_h_time) / 2;
				uint16_t tmp3 = tmp1 + tmp2;
				
				writeString_P(" |DCL: ");
				
				writeIntegerLength((tmp1*100)/tmp3,DEC,2);
				writeChar(':');
				writeIntegerLength((tmp2*100)/tmp3,DEC,2);
				
				tmp1 = (tENCR_min_l_time + tENCR_max_l_time) / 2;
				tmp2 = (tENCR_min_h_time + tENCR_max_h_time) / 2;
				tmp3 = tmp1 + tmp2;
				writeString_P(" |DCR: ");
				writeIntegerLength((tmp1*100)/tmp3,DEC,2);
				writeChar(':');
				writeIntegerLength((tmp2*100)/tmp3,DEC,2);
				
				writeString_P(" |UB: ");
				printUBat(adcBat);
				writeChar('\n');
				if(getLeftSpeed() < 5)
				{
					writeString_P("SPEED LEFT IS TOO LOW!!!!\n");
				}
				if(getRightSpeed() < 5)
				{
					writeString_P("SPEED RIGHT IS TOO LOW!!!!\n");
				}
			}
			setStopwatch1(0);
		}
		if(getStopwatch2() > 100)
		{
			if(pwm > 0)
			{
				transmitSpeedAndBatteryVoltage();
			}
			setStopwatch2(0);
		}
		
		if(pwm > 0)
			transmitEncoderWaveform();
		task_ADC();
	}
	stopStopwatch1();
	stopStopwatch2();
	mSleep(500);
	setMotorPower(0,0);
	setMotorDir(FWD,FWD);
	powerOFF();
	done();
}

/*****************************************************************************/
// Main:

int main(void)
{
	initRobotBase(); 
	
	uint8_t test = 0;

	setLEDs(0b111111);
	mSleep(500);
	setLEDs(0b000000);
	mSleep(250);
	
	IRCOMM_setRC5DataReadyHandler(receiveRC5Data);
	
	
	
	connectToDisplays(HIGHSPEED);

	createNewDisplay(1, "SPEED LEFT", DISPLAY_DIGITAL, "0", "255");
	createNewDisplay(2, "SPEED RIGHT", DISPLAY_DIGITAL, "0", "255" );
	createNewDisplay(3, "DISTANCE LEFT", DISPLAY_DIGITAL, "0", "65535");
	createNewDisplay(4, "DISTANCE RIGHT", DISPLAY_DIGITAL, "0", "65535");
//	createNewDisplay(5, "POWER LEFT", DISPLAY_ANALOG, "0" , "210");
//	createNewDisplay(6, "POWER RIGHT", DISPLAY_ANALOG, "0", "210");
//	createNewDisplay(7, "CURRENT LEFT", DISPLAY_ANALOG, "0", "1023");
//	createNewDisplay(8, "CURRENT RIGHT", DISPLAY_ANALOG, "0", "1023");
	createNewDisplay(9, "BATTERY VOLTAGE", DISPLAY_DIGITAL, "0", "1023" );
	createNewDisplay(16, "ENCODER LEFT", DISPLAY_WAVEFORM, " ", " ");
	createNewDisplay(17, "ENCODER RIGHT", DISPLAY_WAVEFORM, " ", " ");
	startDisplays();
	
	writeString_P("\n\nRP6 Robot Base Selftest\n\n");
#ifdef FACTORY
	writeString_P("\n\nATTENTION: FACTORY VERSION! \n"); 
	writeString_P("You need to use a laboratory Power supply set to 7.20V!\n\n"); 
#endif
	writeString_P("#####################################################################\n");
	writeString_P("### ATTENTION!!! DANGER!!! WARNING!!!\n");
	writeString_P("Make sure that the RP6 CAN __NOT__ MOVE!\n");
	writeString_P("The caterpillar tracks should NOT touch the ground!\n(hold it in your hands for example...)\n");
	writeString_P("THE RP6 WILL START MOVING IN TEST 8! YOU CAN DAMAGE IT IF YOU DO NOT\n");
	writeString_P("MAKE SURE THAT IT CAN __NOT__ MOVE!\n");
	writeString_P("Make sure both crawler tracks are FREE RUNNING! DO NOT BLOCK THEM!\n");
	writeString_P("#####################################################################\n\n");
	writeString_P("Please enter \"x\" and hit Enter to continue!\n");
	
	writeString_P("Hint: This program requires a newline character (\"\\n\") at the end of\n");
	writeString_P("all input lines!\n");
	clearReceptionBuffer(); 
	enterX();
	clearReceptionBuffer();
	
	while(true)
	{
		// This menu is mainly a program space filler - good for serial interface (speed) testing ;)
		// The same applies for ALL other text information in this program!
		// All this is just to have a BIG program download that fills nearly the complete Memory.
		writeChar('\n');
		writeString_P("#####################################################################\n"); 
		writeString_P("#########            RP6 Robot Base Selftest                #########\n"); 
#ifdef FACTORY
		writeString_P("######### FACTORY VERSION (!! 7.2V !!)  v. 1.4 - 05.11.2007 #########\n"); 
#endif
#ifdef HOME
		writeString_P("######### HOME VERSION                  v. 1.4 - 05.11.2007 #########\n"); 
#endif
	    writeString_P("#####################################################################\n"); 
		writeString_P("#####       Main Menu         #########      Advanced Menu      #####\n"); 
		writeString_P("#                                 #                                 #\n"); 
		writeString_P("# 0 - Run ALL Selftests (0-8)     # s - Move at speed Test          #\n"); 
		writeString_P("# 1 - PowerOn Test                # d - Move distance Test          #\n"); 
		writeString_P("# 2 - LED Test                    # c - Encoder Duty-Cycle Test     #\n"); 
		writeString_P("# 3 - Voltage Sensor Test         #                                 #\n"); 
		writeString_P("# 4 - Bumper Test                 #                                 #\n"); 
		writeString_P("# 5 - Light Sensor Test           #                                 #\n"); 
		writeString_P("# 6 - ACS (and RC5 receive) Test  #                                 #\n"); 
		writeString_P("# 7 - IRCOMM/RC5 Test             # System voltage is:              #\n"); 
		writeString_P("# 8 - Motors and Encoders Test    #      "); 
		printUBat(readADC(ADC_BAT));
		writeString_P("                     #\n"); 
		writeString_P("#                                 #                                 #\n"); 
		writeString_P("#####################################################################\n"); 
		writeString_P("# Please enter your choice (1-8, s, d, c)!                          #\n"); 
		writeString_P("#####################################################################\n"); 

		enterString();
	
		test = receiveBuffer[0] - 48;
		
		if(receiveBuffer[0] == 's')
		{
			speed_control();
		}
		else if(receiveBuffer[0] == 'd')
		{
			testMoveDistance();
		}
		else if(receiveBuffer[0] == 'c')
		{
			testEncoderDutyCycle();
		}
		else if(test > 8)
		{
			writeString_P("You need to enter a single number from 0 to 8, s, d or c!");
			continue;
		}
		else
		{
			switch(test)
			{
				case 0: 
					testPowerOn();
					testLEDs();
					testVoltageSensor(); 
					testBumpers(); 
					testLightSensors(); 
					testACS(); 
					testRC5(); 
					testMotorsAndEncoders();
				break; 	
				case 1: testPowerOn(); 				break; 
				case 2: testLEDs(); 				break; 
				case 3: testVoltageSensor();		break; 		
				case 4: testBumpers(); 				break;	
				case 5: testLightSensors(); 		break;
				case 6: testACS(); 			 		break;
				case 7: testRC5(); 		 	 		break;
				case 8: testMotorsAndEncoders(); 	break;	
			}
		}
	}
	return 0;
}
