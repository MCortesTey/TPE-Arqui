#ifndef SOUND_DRIVER_H
#define SOUND_DRIVER_H

#include <stdint.h>

static void play_sound(uint32_t nFrequence);
static void nosound();
void beep(int time, int frequency);


#endif
