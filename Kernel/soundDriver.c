//https://wiki.osdev.org/PC_Speaker
#include "include/soundDriver.h"
#include "include/sysCalls.h"
#include "include/lib.h"

#define PIT_FREQUENCY 1193180
#define TIMER2 0x42
#define TIMER2_CONTROL 0x43
#define SPEAKER_PORT 0x61

static void play_sound(uint32_t frequency) {
    uint32_t div = PIT_FREQUENCY / frequency;
    
    // Configura el timer 2
    _out(TIMER2_CONTROL, 0xB6);
    _out(TIMER2, (uint8_t)(div & 0xFF));
    _out(TIMER2, (uint8_t)(div >> 8));
    
    // Obtiene el valor actual del puerto del speaker
    uint8_t tmp = _in(SPEAKER_PORT);
    
    // Activa el bit 1 (permite el timer) y el bit 0 (activa el speaker)
    if (tmp != (tmp | 3)) {
        _out(SPEAKER_PORT, tmp | 3);
    }
}
 //make it shut up
static void nosound() {
    uint8_t tmp = _in(SPEAKER_PORT) & 0xFC;
    _out(SPEAKER_PORT, tmp);
}

 
 //Make a beep
 void beep(int time, int frequency) {
 	 play_sound(frequency);
 	 sys_sleep(time);
 	 nosound();
          //set_PIT_2(old_frequency);
 }