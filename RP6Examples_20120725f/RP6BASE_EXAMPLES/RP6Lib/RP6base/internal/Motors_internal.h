#ifndef MOTORS_INTERNAL_H
#define MOTORS_INTERNAL_H

/*****************************************************************************/
// Motor control

// Motion Control status register:
union {
	uint8_t byte;
	struct {
		unsigned move_L:1;
		unsigned move_R:1;
		unsigned overcurrent:1;
		unsigned reserved:5;
	};
} motion_status;

extern uint8_t mleft_dir;
extern uint8_t mright_dir;
extern int16_t mleft_power;
extern int16_t mright_power;

#define MC_IDLE 0
#define MC_MOVEDIST_R 1
#define MC_MOVEDIST_L 2
#define MC_ROTATE 4

#define MC_CHANGE_DIR_R 32
#define MC_CHANGE_DIR_L 64

/**
 * These Macros return the current direction value. 
 * They return true if direction is Forwards, false otherwise.
 */
#define getLeftDir() (mleft_dir)
#define getRightDir() (mright_dir)

/**
 * You can use these two Macros to get the current speed value.
 * They are just synonyms for the two OCR1x Registers of the Timer Module.
 */
#define getMotorRight() (OCR1A)
#define getMotorLeft() (OCR1B)

//Encoders:
extern volatile uint16_t mleft_speed;
extern volatile uint16_t mright_speed;

#define getLeftSpeed() (mleft_speed)
#define getRightSpeed() (mright_speed)

// Distance
extern volatile uint16_t mleft_dist;
extern volatile uint16_t mright_dist;

#define getLeftDistance() (mleft_dist)
#define getRightDistance() (mright_dist)

// Desired Speed:
extern uint16_t mleft_des_speed;
extern uint16_t mright_des_speed;

#define getDesSpeedLeft() (mleft_des_speed)
#define getDesSpeedRight() (mright_des_speed)

//Direction:
#define FWD 0
#define BWD 1
#define LEFT 2
#define RIGHT 3

extern uint8_t drive_dir;
#define getDirection() (drive_dir)

#define NON_BLOCKING 0
#define BLOCKING 1

#ifdef DEBUG_MEASURE_DUTY_CYCLE
	extern volatile uint16_t cycle_h_l;
	extern volatile uint16_t cycle_l_l;
	extern volatile uint16_t cycle_h_r;
	extern volatile uint16_t cycle_l_r;
#endif

#endif

