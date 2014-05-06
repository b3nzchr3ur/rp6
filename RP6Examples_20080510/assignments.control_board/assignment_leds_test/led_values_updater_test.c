/*
 * led_value_updater_test.c
 *
 *  Created on: 22 nov. 2012
 *      Author: ben
 */

#include "led_values_updater_test.h"
#include "unity.h"
#include <led_values_updater.h>

/* Note: setUp and tearDown are called
 * by Unity frameWork before each testcases is executed */
void led_values_updater_test_setUp(void) {
	//log_message("Test-case start...");
}

void led_values_updater_test_tearDown(void) {
	//log_message("...end!");
}

void test_led_value_is_updated_when_initially_off(void)
{
	uint8_t led_value = 0b000;
	uint8_t expected_led_value = 0b001;

	led_values_update(led_value, &led_value);

	TEST_ASSERT_EQUAL(expected_led_value, led_value);
}

void test_led_value_is_updated_when_already_on(void)
{
	uint8_t led_value = 0b0100;
	uint8_t expected_led_value = 0b1000;

	led_values_update(led_value, &led_value);

	TEST_ASSERT_EQUAL(expected_led_value, led_value);
}

void test_led_value_is_set_to_1st_led_when_4th_led_is_on(void)
{
	uint8_t led_value = 0b1000;
	uint8_t expected_led_value = 0b0001;

	led_values_update(led_value, &led_value);

	TEST_ASSERT_EQUAL(expected_led_value, led_value);
}

void led_values_updater_run_testcases(void) {
	UnitySetSetUpTearDown(led_values_updater_test_setUp, led_values_updater_test_tearDown);

	RUN_TEST(test_led_value_is_updated_when_initially_off,1);
	RUN_TEST(test_led_value_is_updated_when_already_on,2);
	RUN_TEST(test_led_value_is_set_to_1st_led_when_4th_led_is_on,3);

	UnityResetSetUpTearDownFunc();
}
