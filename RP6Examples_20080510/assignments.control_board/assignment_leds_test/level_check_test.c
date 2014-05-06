/*
 * led_value_updater_test.c
 *
 *  Created on: 22 nov. 2012
 *      Author: ben
 */

#include "led_values_updater_test.h"
#include "unity.h"
#include <level_check.h>

void test_level_is_too_low(void)
{
	bool actual = true; // TODO: is_level_detected(?, ?);
	bool expected = false;
	TEST_ASSERT( expected == actual );
}

void test_level_is_just_high_enough(void)
{
	bool actual = true; // TODO: is_level_detected(?, ?);
	bool expected = false;
	TEST_ASSERT( expected == actual );
}

void level_check_run_testcases(void) {
	RUN_TEST(test_level_is_too_low,100);
	RUN_TEST(test_level_is_just_high_enough,101);
}
