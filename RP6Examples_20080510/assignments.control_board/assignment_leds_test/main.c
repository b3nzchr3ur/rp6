#include "unity.h"
#include <stdio.h>
#include "led_values_updater_test.h"
#include "level_check_test.h"

/*****************************************************************************/

/* Meer informatie over unit testen via Unity:
 *	https://github.com/ThrowTheSwitch/Unity
*/

#include <stdio.h>
/*------------------------------------------------------------------------------*/
/* Needed for unity testing framework.. */
/* Check out define in makfile:
 *	CDEFS = -DUNITY_INT_WIDTH=16 -DUNITY_OUTPUT_CHAR=rp6_output_char_for_unity
 */
int rp6_output_char_for_unity(int character)
{
	printf("%c",character);
	return 0;
}
/*------------------------------------------------------------------------------*/


int main(void)
{
	UnityBegin();
	led_values_updater_run_testcases();
	level_check_run_testcases();
	UnityEnd();
}

