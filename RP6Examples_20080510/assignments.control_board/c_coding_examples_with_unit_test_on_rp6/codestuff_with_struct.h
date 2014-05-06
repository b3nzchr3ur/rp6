#ifndef CODESTUFF_WITH_STRUCT_H
#define CODESTUFF_WITH_STRUCT_H


#include <stdint.h>

typedef struct {
	int16_t a; 
	int16_t b;
} TweeGetallen;

uint16_t tel_twee_getallen_op_met_struct(TweeGetallen ab);
int16_t optellen_en_kwadraat_met_struct(TweeGetallen ab);

#endif
