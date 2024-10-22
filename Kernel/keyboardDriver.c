#include <keyboardDriver.h>
#include <naiveConsole.h>
#include <lib.h>

char getPressed(char shift){

	char asccode[58][2] = 
	{
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

	// Obtener la tecla presionada
	int pressedKey = getKeyPressed();

	// Manejo de teclas especiales
	if(pressedKey == 0xAA || pressedKey == 0x2A)
		return pressedKey; // Retornar tecla de shift o caps lock
	if(pressedKey > 0x90)
		return 0x90; // Manejo de teclas fuera de rango

	if(shift)// Determinar si se debe retornar el carácter mayúscula o minúscula
		return asccode[pressedKey][1]; // Retornar carácter mayúscula si shift está activado
	return asccode[pressedKey][0]; // sino retornar carácter minúscula
}

void pressKey(){
    ncPrint("[Text]: ");
	
	char pressed;
	char shiftPressed = 0;
	char write = 1;
	while(1){
		pressed = getPressed(shiftPressed);

		if(pressed == 27)
			break;

		switch(pressed){
			case 8:
				ncDelete();
				break;
			case 13:
				ncNewline();
				break;
			case (char)0xAA:
				shiftPressed = 0;
				write = 0;
				break;
			case 0x2A:
				shiftPressed = 1;
				write = 0;
				break;
			case (char)0x90:
				write = 0;
				break;
			default:
				ncPrintChar(pressed);
		}
		if(write)
			pressed = getPressed(shiftPressed);
		write = 1;
	}
}