#include <time.h>
#include <naiveConsole.h>
#include <videoDriver.h>
#include <interrupts.h>

static unsigned long ticks = 0;

void timer_handler() {
	ticks++;
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}

uint64_t ms_elapsed() {
    return ticks * 55;
}

void timer_ms(long * time ){
    *time = ms_elapsed();
}

// Arma un string con la hora actual en formato HH:MM:SS
char * TimeClock(char * buffer) {
    int hours = (int)getHours();
    int minutes = (int)getMinutes();
    int seconds = (int)getSeconds();

    // Los valores vienen en BCD, hay que pasarlos a decimal
    hours = ((hours & 0xF0) >> 4) * 10 + (hours & 0x0F);
    minutes = ((minutes & 0xF0) >> 4) * 10 + (minutes & 0x0F);
    seconds = ((seconds & 0xF0) >> 4) * 10 + (seconds & 0x0F);

    convertToGMTMinus3(&hours);
    minutes = minutes % 60;
    seconds = seconds % 60;

    int digits = 0;
    
    // si hora mins o segs son menors a 10 le agregamos un 0 adelante
    if (hours < 10) {
        buffer[digits++] = '0';
        digits += uintToBase(hours, buffer + digits, 10);
    } else {
        digits += uintToBase(hours, buffer + digits, 10);
    }
    
    buffer[digits++] = ':';
    
    if (minutes < 10) {
        buffer[digits++] = '0';
        digits += uintToBase(minutes, buffer + digits, 10);
    } else {
        digits += uintToBase(minutes, buffer + digits, 10);
    }
    
    buffer[digits++] = ':';
    
    if (seconds < 10) {
        buffer[digits++] = '0';
        digits += uintToBase(seconds, buffer + digits, 10);
    } else {
        digits += uintToBase(seconds, buffer + digits, 10);
    }
    
    buffer[digits] = 0; 
    return buffer;
}

void convertToGMTMinus3(int *hours) {
    *hours -= 3;
    if (*hours < 0) {
        *hours += 24; 
    }
}

