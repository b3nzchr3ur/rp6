#ifndef ACS_INTERNAL_H
#define ACS_INTERNAL_H

extern uint8_t obstacle_left;
extern uint8_t obstacle_right;
#define isObstacleLeft() (obstacle_left)
#define isObstacleRight() (obstacle_right)

#endif