#ifndef _KEYBOARD_DRIVER_H_
#define _KEYBOARD_DRIVER_H_

void bufferAppend(char c);
void keyboard_handler();
unsigned char keyHandler(unsigned int key);
char isFKey(unsigned int key);
char isSpecialKey(unsigned int key);
int bufferLen();
void bufferClearAll();
char getBuffAtCurrent();
void buffNext();

static unsigned char asccCode[58][2] = {
 	{0, 0},
 	{27, 27},
 	{'1', '!'},
 	{'2', '@'},
 	{'3', '#'},
 	{'4', '$'},
 	{'5', '%'},
 	{'6', '^'},
 	{'7', '&'},
 	{'8', '*'},
 	{'9', '('},
 	{'0', ')'},
 	{'-', '_'},
 	{'=', '+'},
 	{8, 8},
 	{9, 9},
 	{'q', 'Q'},
 	{'w', 'W'},
 	{'e', 'E'},
 	{'r', 'R'},
 	{'t', 'T'},
 	{'y', 'Y'},
 	{'u', 'U'},
 	{'i', 'I'},
 	{'o', 'O'},
 	{'p', 'P'},
 	{'[', '{'},
 	{']', '}'},
 	{13, 13},
 	{0, 0},
 	{'a', 'A'},
 	{'s', 'S'},
 	{'d', 'D'},
 	{'f', 'F'},
 	{'g', 'G'},
 	{'h', 'H'},
 	{'j', 'J'},
 	{'k', 'K'},
 	{'l', 'L'},
 	{';', ':'},
 	{39, 34},
 	{'`', '~'},
 	{0, 0},
 	{'\\', '|'},
 	{'z', 'Z'},
 	{'x', 'X'},
 	{'c', 'C'},
 	{'v', 'V'},
 	{'b', 'B'},
 	{'n', 'N'},
 	{'m', 'M'},
 	{',', '<'},
 	{'.', '>'},
 	{'/', '?'},
 	{0, 0},
 	{0, 0},
 	{0, 0},
 	{' ', ' '},
};

#endif // _KEYBOARD_DRIVER_H_
