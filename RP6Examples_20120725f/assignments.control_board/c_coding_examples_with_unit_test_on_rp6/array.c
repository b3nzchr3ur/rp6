#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#define SIZE_OF_ARRAY (5)
#define SIZE_OF_LUCKY_NUMBER_ARRAY (3)
#define SIZE_OF_UNLUCKY_NUMBER_ARRAY (2)

typedef struct 
{
	unsigned int lucky_number_array[SIZE_OF_LUCKY_NUMBER_ARRAY];
	unsigned int unlucky_number_array[SIZE_OF_UNLUCKY_NUMBER_ARRAY];
} special_number_struct;

uint16_t count_my_lucky_numbers(special_number_struct numbers);

void array_examples(void)
{
	uint8_t numbers_array[SIZE_OF_ARRAY];
	numbers_array[SIZE_OF_ARRAY-1] = 10;

	special_number_struct my_numbers;
	my_numbers.lucky_number_array[0] = 7;
	my_numbers.lucky_number_array[1] = 28;	
	my_numbers.lucky_number_array[2] = 3;
	assert( 38 == count_my_lucky_numbers(my_numbers));
}

uint16_t count_my_lucky_numbers(special_number_struct numbers)
{
	uint16_t sum = 0;
	for(uint8_t index = 0; index < SIZE_OF_LUCKY_NUMBER_ARRAY; index++)
	{
		sum += numbers.lucky_number_array[index];
	}
	return sum;
}

