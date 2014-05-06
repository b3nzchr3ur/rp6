/*
 * level_check.h
 *
 *  Created on: 22 nov. 2012
 *      Author: ben
 */

#ifndef LEVEL_CHECK_H_
#define LEVEL_CHECK_H_

#include <stdint.h>
#include <bool.h>

bool is_level_detected(uint16_t measured_level, uint16_t threshold_level);

#endif /* LEVEL_CHECK_H_ */
