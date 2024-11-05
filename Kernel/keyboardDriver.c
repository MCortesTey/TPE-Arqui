#include <keyboardDriver.h>
#include <naiveConsole.h>
#include <lib.h>
#include <videoDriver.h>
#include <sysCalls.h>

// Definiciones de constantes
#define TOTAL_KEYS 58 // Número total de teclas definidas
#define MAX_KEY_PRESS 0x70 // Los valores superiores son los release de las teclas
#define TAB_KEY_COUNT 4 // Número de teclas de tabulación
#define KEYBOARD_BUFFER_SIZE 256 // Tamaño del búfer de teclado

// Definiciones de teclas especiales:
#define ESCAPE_KEY 0x01 // Tecla de escape
#define ENTER_KEY 0x1C // Tecla de enter
#define BACKSPACE_KEY 0x0E // Tecla de retroceso
#define TAB_KEY 0x0F // Tecla de tabulación
#define LEFT_SHIFT_PRESS 0x2A // Tecla de shift izquierdo presionada
#define LEFT_SHIFT_RELEASE 0xAA // Tecla de shift izquierdo liberada
#define RIGHT_SHIFT_PRESS 0x36 // Tecla de shift derecho presionada
#define RIGHT_SHIFT_RELEASE 0xB6 // Tecla de shift derecho liberada
#define CAPS_LOCK_PRESS 0x3A // Tecla de bloqueo de mayúsculas presionada
#define ALT_PRESS 0x3B // Tecla de alt presionada
#define ALT_RELEASE 0xB8 // Tecla de alt liberada

#define CTRL_PRESS 0x1D // Tecla de control presionada
#define CTRL_RELEASE 0x9D // Tecla de control liberada

static uint8_t regs_ok = 0 ;


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

struct kbuff {
    int pos;
    int len;
    uint16_t buffer[KEYBOARD_BUFFER_SIZE];
};
typedef struct kbuff * buffer_ptr;


static struct kbuff buff = {0, 0, {'\0'}}; // Inicializa el buffer de teclado
static buffer_ptr ptr = &buff; // Puntero al buffer de teclado
int buffer_pos = 0; // Posición actual en el buffer


void bufferAppend(char c) {
    if (ptr->len < KEYBOARD_BUFFER_SIZE) {
        int writePos = (ptr->pos + ptr->len) % KEYBOARD_BUFFER_SIZE;
        ptr->buffer[writePos] = c;
        ptr->len++;
    }
}

 // Búfer de teclado
 //static char buffer[KEYBOARD_BUFFER_SIZE] = {0};
 static int currentKey = 0; // Índice actual de la tecla en el búfer

 int shift = 0; // Estado de la tecla shift
 int capsLock = 0; // Estado del bloqueo de mayúsculas
 int registerPressed = 0; // Estado de la tecla de registro

 void keyboard_handler(){	
 	uint8_t key = keyHandler(getKeyPressed()); // Filtrar la tecla y obtener el caracter

 	if (key){ // Si no es una tecla especial
 		bufferAppend(key); // Escribir en el búfer
		//vdPrintCharColor(key, 0xFFFFFF, 0x000000);
 	}

 	currentKey %= KEYBOARD_BUFFER_SIZE; // Asegurarse de no sobrepasar el tamaño del búfer
 	//sys_write( STDOUT,ptr->buffer, bufferLen());
 	
 }

unsigned char keyHandler(unsigned int key){
 	switch (key)
 	{
	case ESCAPE_KEY:
 		regs_ok = 1; // Activar registro
 		return 0; // No se inserta nada en el búfer
 	case RIGHT_SHIFT_PRESS:
 	case LEFT_SHIFT_PRESS:
 		shift = 1; // Activar shift
 		return 0; // No se inserta nada en el búfer
 	case RIGHT_SHIFT_RELEASE:
 	case LEFT_SHIFT_RELEASE:
 		shift = 0; // Desactivar shift
 		return 0; // No se inserta nada en el búfer
 	case CAPS_LOCK_PRESS:
 		capsLock = 1 - capsLock; // Cambiar estado del bloqueo de mayúsculas
 		return 0; // No se inserta nada en el búfer
	case ENTER_KEY:
		return '\n';
	case BACKSPACE_KEY:
		return '\b';
 	}

 	// Si no es una tecla especial, retornar la tecla correspondiente
 	if (key <= MAX_KEY_PRESS && !isSpecialKey(key)){
 		int index;
 		if (asccCode[key][0] >= 'a' && asccCode[key][0] <= 'z')
 		{
 			index = capsLock ? !shift : shift; // Determinar el índice según el estado de shift y capsLock
 		}
 		else
 		{
 			index = shift; // Determinar el índice según el estado de shift
 		}
 		return asccCode[key][index]; // Retornar la tecla a insertar en el búfer
 	}
 	switch (key)
 	{
 	case TAB_KEY:
 		vdPrint("    "); // Retornar el carácter de tabulación
 	case BACKSPACE_KEY:
 		//bsBuffer(); // Retornar el carácter de retroceso
 	}

 	return 0; // Retornar 0 si no se debe insertar nada
 }


 // Función para determinar si una tecla es una tecla F
 char isFKey(unsigned int key)
 {
 	return (key >= 0x3B && key <= 0x44) || key == 0x57 || key == 0x58;
 }

 // Función para determinar si una tecla es una tecla especial
 char isSpecialKey(unsigned int key)
 {
 	return key == LEFT_SHIFT_PRESS || key == RIGHT_SHIFT_PRESS ||
 		   key == CAPS_LOCK_PRESS || key == ALT_PRESS || isFKey(key) || key == ESCAPE_KEY || key == TAB_KEY;
 }

 char getBuffAtCurrent() { // Retorna el carácter en la posición actual del buffer
    if (ptr->len > 0) {
        char c = ptr->buffer[ptr->pos];
        return c;
    }
    return 0;
}

int getPos() { // Retorna la posición actual en el buffer
    return ptr->pos; // Retorna la posición actual
}

char getBuffCharAt(int pos){ // Retorna el carácter en una posición específica del buffer
    return ptr->buffer[pos]; 
}

void buffNext() {
    if (ptr->len > 0) {
        ptr->pos = (ptr->pos + 1) % KEYBOARD_BUFFER_SIZE;
        ptr->len--;
    }
}

 int bufferLen(){ // Retorna la longitud del buffer
     return ptr->len; // Retorna la longitud actual del buffer
 }

 void bufferClearAll(){ // Limpia completamente el buffer
     ptr->pos = 0; // Resetea la posición del buffer
     ptr->len = 0; // Resetea la longitud del buffer
     for (int i = 0; i < KEYBOARD_BUFFER_SIZE; i++) { // Itera sobre todo el buffer
         ptr->buffer[i] = 0; // Limpia cada posición del buffer
     }
 }

 uint8_t regsReady(){
	return regs_ok;
 }

 void uncheckRegs(){
	regs_ok = 0;
 }

 