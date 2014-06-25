#include <stdlib.h>
#include <assert.h>

#define __ASSERT_USE_STDERR

/* Examples of bitfields use in structs */

union both {
	struct {
		unsigned int mode:2;     /* 2 bits */
		unsigned int accuracy:2; /* 2 bits */
		unsigned int other:4;    /* 4 bits */
	} bits;
	unsigned int byte;
};

void voorbeeld_met_bitfield_struct(void)
{
	union both configuration;
	configuration.byte = 0xFA;
		
	assert(configuration.bits.mode == 0x11);
	assert(configuration.bits.accuracy == 0x11);
	assert(configuration.bits.other == 0xA);

	configuration.bits.mode = 0b10;
	assert(configuration.bits.mode == 0b10);
}
