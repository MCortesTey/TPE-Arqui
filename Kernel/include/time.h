#ifndef _TIME_H_
#define _TIME_H_

#include <lib.h>

void timer_handler();
int ticks_elapsed();
int seconds_elapsed();
uint64_t ms_elapsed();
void timer_ms(int * time );
uint8_t getSeconds();
uint8_t getMinutes();
uint8_t getHours();
void convertToGMTMinus3(int *hours);
char * TimeClock(char * buffer);

#endif
