#include "RP6ControlLib.h"

void some_examples(uint16_t magic_number)
{
	for(uint8_t teller = 0; teller < 10; teller++) 
	{
		sound(180 + teller, 80, 25);
	}

	uint8_t teller = 0;
	while(teller < 10) 
	{
		sound(180 + teller,80,25);
		teller++;
	}
	
	switch(magic_number)
	{
		case 747:
			writeString_P("It's an airplane");
			break;
		case 4711:
			writeString_P("My grandma likes it...");
			break;
		default:
			writeString_P("No clue what this number means...");
			break;
	}

	if(magic_number == 112) 
	{
		writeString_P("Emergency!! Don't panic.");
	} 
	else if (magic_number == 1972)
	{
		writeString_P("A lost final...");
	}
	else
	{
		writeString_P("No clue what this number means...");
	}
}
