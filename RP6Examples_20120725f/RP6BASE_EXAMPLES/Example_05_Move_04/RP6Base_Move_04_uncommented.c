// Uncommented Version of RP6Base_Move_04.c
// written by Dominik S. Herwald
// ------------------------------------------------------------------------------------------

#include "RP6RobotBaseLib.h" 	

#define IDLE  0

typedef struct {
	uint8_t  speed_left; 
	uint8_t  speed_right;
	unsigned dir:2;
	unsigned move:1;
	unsigned rotate:1;
	uint16_t move_value;
	uint8_t  state;
} behaviour_command_t;

#define CRUISE_SPEED_FWD    80 

#define MOVE_FORWARDS 1
behaviour_command_t cruise = {CRUISE_SPEED_FWD, CRUISE_SPEED_FWD, FWD, 
								false, false, 0, MOVE_FORWARDS};

void behaviour_cruise(void)
{
}

#define ESCAPE_SPEED_BWD    60
#define ESCAPE_SPEED_ROTATE 50

#define ESCAPE_FRONT_START 	1
#define ESCAPE_FRONT_WAIT 	2
#define ESCAPE_LEFT_START  	3
#define ESCAPE_LEFT_WAIT	4
#define ESCAPE_RIGHT_START  5
#define ESCAPE_RIGHT_WAIT 	6
#define ESCAPE_WAIT_END		7
behaviour_command_t escape = {0, 0, FWD, false, false, 0, IDLE}; 

void behaviour_escape(void)
{
	static uint8_t bump_count = 0;
	switch(escape.state)
	{
		case IDLE: 
		break;
		case ESCAPE_FRONT_START:
			escape.speed_left = ESCAPE_SPEED_BWD;
			escape.dir = BWD;
			escape.move = true;
			if(bump_count > 3)
				escape.move_value = 180;
			else
				escape.move_value = 120;
			escape.state = ESCAPE_FRONT_WAIT;
			bump_count+=2;
		break;
		case ESCAPE_FRONT_WAIT:			
			if(!escape.move) 
			{
				escape.speed_left = ESCAPE_SPEED_ROTATE;
				if(bump_count > 3)
				{
					escape.move_value = 90;
					escape.dir = RIGHT;
					bump_count = 0;
				}
				else 
				{
					escape.dir = LEFT;
					escape.move_value = 50;
				}
				escape.rotate = true;
				escape.state = ESCAPE_WAIT_END;
			}
		break;
		case ESCAPE_LEFT_START:
			escape.speed_left = ESCAPE_SPEED_BWD;
			escape.dir 	= BWD;
			escape.move = true;
			if(bump_count > 3)
				escape.move_value = 160;
			else
				escape.move_value = 100;
			escape.state = ESCAPE_LEFT_WAIT;
			bump_count++;
		break;
		case ESCAPE_LEFT_WAIT:
			if(!escape.move)
			{
				escape.speed_left = ESCAPE_SPEED_ROTATE;
				escape.dir = RIGHT;
				escape.rotate = true;
				if(bump_count > 3)
				{
					escape.move_value = 80;
					bump_count = 0;
				}
				else
					escape.move_value = 45;
				escape.state = ESCAPE_WAIT_END;
			}
		break;
		case ESCAPE_RIGHT_START:
			escape.speed_left = ESCAPE_SPEED_BWD;
			escape.dir 	= BWD;
			escape.move = true;
			if(bump_count > 3)
				escape.move_value = 160;
			else
				escape.move_value = 100;
			escape.state = ESCAPE_RIGHT_WAIT;
			bump_count++;
		break;
		case ESCAPE_RIGHT_WAIT:
			if(!escape.move)
			{
				escape.speed_left = ESCAPE_SPEED_ROTATE;		
				escape.dir = LEFT;
				escape.rotate = true;
				if(bump_count > 3)
				{
					escape.move_value = 80;
					bump_count = 0;
				}
				else
					escape.move_value = 45;
				escape.state = ESCAPE_WAIT_END;
			}
		break;
		case ESCAPE_WAIT_END:
			if(!(escape.move || escape.rotate))
				escape.state = IDLE;
		break;
	}
}

void bumpersStateChanged(void)
{
	if(bumper_left && bumper_right)
	{
		escape.state = ESCAPE_FRONT_START;
	}
	else if(bumper_left)
	{
		if(escape.state != ESCAPE_FRONT_WAIT) 
			escape.state = ESCAPE_LEFT_START;
	}
	else if(bumper_right)
	{
		if(escape.state != ESCAPE_FRONT_WAIT)
			escape.state = ESCAPE_RIGHT_START;
	}
}

void moveCommand(behaviour_command_t * cmd)
{
	if(cmd->move_value > 0)
	{
		if(cmd->rotate) 
			rotate(cmd->speed_left, cmd->dir, cmd->move_value, false); 
		else if(cmd->move) 
			move(cmd->speed_left, cmd->dir, DIST_MM(cmd->move_value), false); 
		cmd->move_value = 0; 
	}
	else if(!(cmd->move || cmd->rotate)) 
	{
		changeDirection(cmd->dir);
		moveAtSpeed(cmd->speed_left,cmd->speed_right);
	}
	else if(isMovementComplete())
	{
		cmd->rotate = false;
		cmd->move = false;
	}
}

behaviour_command_t STOP = {0, 0, FWD, false, false, 0, IDLE};

void behaviourController(void)
{
	behaviour_cruise();
	behaviour_escape();

	if(escape.state != IDLE)
		moveCommand(&escape);
	else if(cruise.state != IDLE)
		moveCommand(&cruise); 
	else                   
		moveCommand(&STOP); 
}

int main(void)
{
	initRobotBase(); 
	setLEDs(0b111111);
	mSleep(2500);
	setLEDs(0b100100); 

	BUMPERS_setStateChangedHandler(bumpersStateChanged);
	
	powerON(); 
	
	while(true) 
	{		
		behaviourController();
		task_RP6System();
	}
	return 0;
}
