#include "codestuff_with_struct.h" 
#include "codestuff.h"  

TweeGetallen geef_wat_getallen(void) 
{
	TweeGetallen wat = { 0xDEAD, 0xBEAF };
	
	TweeGetallen anders;
	anders.a = 0xFAAD;
	anders.b = 0xDAAF;
	
	return wat;
}

uint16_t tel_twee_getallen_op_met_struct(TweeGetallen ab)
{
	return tel_twee_getallen_op(ab.a,ab.b);
}

int16_t optellen_en_kwadraat_met_struct(TweeGetallen ab)
{
	return optellen_en_kwadraat(ab.a,ab.b);
}

