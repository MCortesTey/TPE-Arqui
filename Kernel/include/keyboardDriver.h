#ifndef _KEYBOARD_DRIVER_H_
#define _KEYBOARD_DRIVER_H_

void bufferAppend(char c);
void keyboard_handler();
unsigned char keyHandler(unsigned int key);
char isFKey(unsigned int key);
char isSpecialKey(unsigned int key);
int bufferLen();
void bufferClearAll();

#endif // _KEYBOARD_DRIVER_H_
