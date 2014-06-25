/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - RP6 CONTROL M32 Examples
 * ****************************************************************************
 * Example: I2C Master 10 - Lib 2
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 * Header file for new library.
 *
 * ****************************************************************************
 */

#ifndef RP6CONTROL_I2CMASTERLIB_H
#define RP6CONTROL_I2CMASTERLIB_H


/*****************************************************************************/
// Includes:

#include "RP6ControlLib.h"
#include "RP6I2CmasterTWI.h"

#include "internal/RP6Control_I2CMasterLib_internal.h"

/*****************************************************************************/
// 

#define RP6_MASTERLIB_I2C_ADDRESS (100)

void initI2C_RP6Lib(void);
void task_checkINT0(void);


/*****************************************************************************/

#define NON_BLOCKING 0
#define BLOCKING 1

void rotate(uint8_t desired_speed, uint8_t dir, uint16_t angle, uint8_t blocking);
void move(uint8_t desired_speed, uint8_t dir, uint16_t distance, uint8_t blocking);
void moveAtSpeed(uint8_t desired_speed_left, uint8_t desired_speed_right);
void changeDirection(uint8_t dir);
void stop(void);

void getAllSensors(void);

#define getMotorRight() (OCR1A)
#define getMotorLeft() (OCR1B)

#define getLeftSpeed() (mleft_speed)
#define getRightSpeed() (mright_speed)
#define getLeftDistance() (mleft_dist)
#define getRightDistance() (mright_dist)

#define getDesSpeedLeft() (mleft_des_speed)
#define getDesSpeedRight() (mright_des_speed)

//Direction:

#define FWD 0
#define BWD 1
#define LEFT 2
#define RIGHT 3

#define getDirection() (drive_status.direction)

#define isMovementComplete() (drive_status.movementComplete)
#define isBumperLeft() (bumper_left)
#define isBumberRight() (bumper_right)

#define isObstacleLeft() (obstacle_left)
#define isObstacleRight() (obstacle_right)

/*****************************************************************************/

typedef union {
	uint16_t data;
	struct {
		uint8_t key_code:6;
		uint8_t device:5;
		uint8_t toggle_bit:1;
		uint8_t reserved:3;
	};
} RC5data_t;

void IRCOMM_sendRC5(uint8_t adr, uint8_t data);
void IRCOMM_setRC5DataReadyHandler(void (*rc5Handler)(RC5data_t));

/*****************************************************************************/
void ACS_setStateChangedHandler(void (*acsHandler)(void));
void MOTIONCONTROL_setStateChangedHandler(void (*motionControlHandler)(void));
void BUMPERS_setStateChangedHandler(void (*bumperHandler)(void));
void BATTERY_setLowVoltageHandler(void (*lowVoltageHandler)(uint8_t));
void WDT_setRequestHandler(void (*requestHandler)(void));
void I2C_setTransmissionErrorHandler(void (*transmissionErrorHandler)(uint8_t));

#endif

