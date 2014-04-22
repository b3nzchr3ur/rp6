#ifndef MOTORS_H
#define MOTORS_H

/*****************************************************************************/
#include "internal/Motors_internal.h"

void task_motionControl(void);

void setMotorDir(uint8_t left_dir, uint8_t right_dir);

void setMotorPower(uint8_t left_power, uint8_t right_power);

void changeDirection(uint8_t dir);

void move(uint8_t desired_speed, uint8_t dir, uint16_t distance, uint8_t blocking);
void rotate(uint8_t desired_speed, uint8_t dir, uint16_t angle, uint8_t blocking);

void moveAtSpeed(uint8_t desired_speed_left, uint8_t desired_speed_right);
uint8_t isMovementComplete(void);

void stop(void);

void MOTIONCONTROL_setStateChangedHandler(void (*motionControlHandler)(void));

#endif

