#ifndef TIMER_INTERNAL_H
#define TIMER_INTERNAL_H

//*****************************************************************************/
// Universal timer:

// This timer variable:
volatile uint16_t timer;  
// is incremented each 100µs. It is suited for performing time
// measurements where the resolution of the stopwatches (1ms) is not enough. 

#define delay_us(us)  _delay_loop_2(( ( ( 1*(F_CPU/4000) )*us )/1000 ))

#endif
