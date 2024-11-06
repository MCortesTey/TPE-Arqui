#ifndef _TIME_H_
#define _TIME_H_

#include <lib.h>

void timer_handler();
int ticks_elapsed();
int seconds_elapsed();
uint64_t ms_elapsed();
void timer_ms(long * time );
char * TimeClock(char * buffer);
void convertToGMTMinus3(int *hours);

#endif
