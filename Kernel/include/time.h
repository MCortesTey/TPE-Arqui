#ifndef _TIME_H_
#define _TIME_H_

#include <lib.h>

void timer_handler();
int ticks_elapsed();
int seconds_elapsed();
uint8_t getSeconds();
uint8_t getMinutes();
uint8_t getHours();
char * TimeClock(char * buffer);

#endif
