#ifndef _KEYBOARD_DRIVER_H_
#define _KEYBOARD_DRIVER_H_

#include <stdint.h>

void bufferAppend(char c);
void keyboard_handler();
unsigned char keyHandler(unsigned int key);
char isFKey(unsigned int key);
char isSpecialKey(unsigned int key);
int bufferLen();
void bufferClearAll();
char getBuffAtCurrent();
void buffNext();
int getPos();
char getBuffCharAt(int pos);

#endif // _KEYBOARD_DRIVER_H_
