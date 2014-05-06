#include "unity.h"
#include "codestuff.h"
#include "RP6ControlLib.h"

/* Needed for unity testing framework.. */
/* Check out define in makfile: 
 *	CDEFS = -DUNITY_INT_WIDTH=16 -DUNITY_OUTPUT_CHAR=rp6_output_char_for_unity	
 */
int rp6_output_char_for_unity(int character)
{
	writeChar(character);
	return 0;
}


void setUp(void) 
{
	writeString_P("\nSetup test...");
}

void tearDown(void)
{
	writeString_P("...cleaned up after test.\n");
}

void test_tel_twee_getallen_op(void)
{
	TEST_ASSERT_EQUAL(6, tel_twee_getallen_op(4,2));
}

void test_optellen_en_kwadraat(void)
{
	TEST_ASSERT_EQUAL( 49 , optellen_en_kwadraat(3,4));
}

void run_testCases(void) 
{
	writeString_P("Start running test cases:\n");
	UnityBegin();
	RUN_TEST(test_tel_twee_getallen_op,1);
	RUN_TEST(test_optellen_en_kwadraat,2);
	UnityEnd();
	writeString_P("...done.\n");
}
