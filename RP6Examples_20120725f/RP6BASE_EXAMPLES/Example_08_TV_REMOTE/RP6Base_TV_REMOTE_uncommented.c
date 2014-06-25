// (nearly) uncommented Version of RP6Base_TV_REMOTE.c
// written by Dominik S. Herwald
// ------------------------------------------------------------------------------------------


#include "RP6RobotBaseLib.h" 

// If you only want to look at the RC5 Keycodes received from the Remote
// Control, uncomment this line:
//#define DO_NOT_MOVE
// The RP6 will not move then!


// Uncomment one of the following definitions:
//#define RC_EURO_SKY
#define RC_PROMO8
//#define RC_YOUR_OWN

#ifdef RC_EUROSKY	
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

#ifdef RC_PROMO8	
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

#ifdef RC_YOUR_OWN
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

uint8_t max_speed_left;
uint8_t max_speed_right;
uint8_t acl_left;
uint8_t acl_right;
uint8_t decl_left;
uint8_t decl_right;


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


void receiveRC5Data(RC5data_t rc5data)
{
	writeString_P("Toggle Bit:");
	writeChar(rc5data.toggle_bit + '0');
	writeString_P(" | Device Address:");
	writeInteger(rc5data.device, DEC);
	writeString_P(" | Key Code:");
	writeInteger(rc5data.key_code, DEC);
	writeChar('\n');
	
#ifndef DO_NOT_MOVE  
	
	uint8_t movement_command = false; 

	switch(rc5data.key_code)
	{
		case RC5_KEY_LEFT: 	
			writeString_P("LEFT\n");
			setDefaultSpeedParameters();
			max_speed_left = MAX_SPEED_TURN;
			max_speed_right = max_speed_left;
			changeDirection(LEFT);
			setLEDs(0b100000);
			movement_command = true;
		break;
		case RC5_KEY_RIGHT:
			writeString_P("RIGHT\n");
			setDefaultSpeedParameters();
			max_speed_left = MAX_SPEED_TURN;
			max_speed_right = max_speed_left;
			changeDirection(RIGHT);
			setLEDs(0b000100);
			movement_command = true;
		break;
		case RC5_KEY_FORWARDS:
			writeString_P("FORWARDS\n");
			setDefaultSpeedParameters();
			changeDirection(FWD);
			setLEDs(0b100100);
			movement_command = true;
		break;
		case RC5_KEY_BACKWARDS:
			writeString_P("BACKWARDS\n");
			setDefaultSpeedParameters();
			changeDirection(BWD);
			setLEDs(0b001001);
			movement_command = true;
		break;
		case RC5_KEY_STOP:
			writeString_P("STOP\n");
			max_speed_left = 0;
			max_speed_right = max_speed_left;
			moveAtSpeed(0,0);
			setLEDs(0b011011);
			movement_command = true;
		break;
		case RC5_KEY_CURVE_LEFT:
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
		case RC5_KEY_CURVE_RIGHT:
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
		case RC5_KEY_CURVE_BACK_LEFT:
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
		case RC5_KEY_CURVE_BACK_RIGHT:
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
		case RC5_KEY_LEFT_MOTOR_FWD:
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
		case RC5_KEY_LEFT_MOTOR_BWD:
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
		case RC5_KEY_RIGHT_MOTOR_FWD:
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
		case RC5_KEY_RIGHT_MOTOR_BWD:
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
	
	if(movement_command) 
	{
		if(getDesSpeedLeft() < max_speed_left)
		{							
			moveAtSpeed(getDesSpeedLeft()+acl_left, getDesSpeedRight());
			if(getDesSpeedLeft() < 10)
				moveAtSpeed(10, getDesSpeedRight());
		}
		if(getDesSpeedRight() < max_speed_right)
		{
			moveAtSpeed(getDesSpeedLeft(), getDesSpeedRight()+acl_right);
			if(getDesSpeedRight() < 10)
				moveAtSpeed(getDesSpeedLeft(), 10);
		}
		setStopwatch1(0);
		startStopwatch1();
	}
#endif

}


void deccelerate(void)
{
	if(getStopwatch1() > 250)
	{
		if(getDesSpeedLeft() <= 10) 
			moveAtSpeed(0, getDesSpeedRight());
		else			
			moveAtSpeed(getDesSpeedLeft()-decl_left, getDesSpeedRight());
		if(getDesSpeedRight() <= 10)
			moveAtSpeed(getDesSpeedLeft(), 0); 
		else		
			moveAtSpeed(getDesSpeedLeft(), getDesSpeedRight()-decl_right);
			
		if (getDesSpeedRight() == 0 && getDesSpeedLeft() == 0)	
			stopStopwatch1(); 
		max_speed_left = getDesSpeedLeft();
		max_speed_right = getDesSpeedRight();
		setLEDs(0b000000);
		setStopwatch1(0);
	}
	
	if(getDesSpeedLeft() > max_speed_left) 
	{
		if(getDesSpeedLeft() <= 10)
			moveAtSpeed(0, getDesSpeedRight());
		else
			moveAtSpeed(getDesSpeedLeft()-decl_left, getDesSpeedRight()); 
	}
	if(getDesSpeedRight() > max_speed_right) 
	{
		if(getDesSpeedRight() <= 10)
				moveAtSpeed(getDesSpeedLeft(), 0);
		else
			moveAtSpeed(getDesSpeedLeft(), getDesSpeedRight()-decl_right); 
	}
}

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
	
	IRCOMM_setRC5DataReadyHandler(receiveRC5Data);

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
	
	while(true) 
	{
		deccelerate();
		task_RP6System();
	}
	return 0;
}
