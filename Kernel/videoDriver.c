#include <videoDriver.h>
#include <stdint.h>
#include <font.h>
#include <naiveConsole.h>

struct vbe_mode_info_structure {
	uint16_t attributes;		// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;			// deprecated
	uint8_t window_b;			// deprecated
	uint16_t granularity;		// deprecated; used while calculating bank numbers
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr;		// deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;			// number of bytes per horizontal line
	uint16_t width;			// width in pixels
	uint16_t height;			// height in pixels
	uint8_t w_char;			// unused...
	uint8_t y_char;			// ...
	uint8_t planes;
	uint8_t bpp;			// bits per pixel in this mode
	uint8_t banks;			// deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size;		// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
	uint8_t image_pages;
	uint8_t reserved0;
 
	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;
 
	uint32_t framebuffer;		// physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
} __attribute__ ((packed));

typedef struct vbe_mode_info_structure * VBEInfoPtr;

VBEInfoPtr VBE_mode_info = (VBEInfoPtr) 0x0000000000005C00;
uint32_t posX = MARGIN;
uint32_t posY = MARGIN;
int cursor_pos = 0;
static int size = 1;


void putPixel(uint32_t hexColor, uint64_t x, uint64_t y) {
    uint8_t * framebuffer = (uint8_t *)(uintptr_t) VBE_mode_info->framebuffer;
    uint64_t offset = (x * ((VBE_mode_info->bpp)/8)) + (y * VBE_mode_info->pitch);
    framebuffer[offset]     =  (hexColor) & 0xFF;
    framebuffer[offset+1]   =  (hexColor >> 8) & 0xFF; 
    framebuffer[offset+2]   =  (hexColor >> 16) & 0xFF;
}


void vdPrintCharColor(char c, uint64_t fcolor, uint64_t bcolor) {
    moveScreenUpIfFull();

    // Si el carácter es un espacio, usa el color de fondo para el primer plano
    if (c == ' ') {
        fcolor = bcolor;
    }

    // Inicializo las variables para la posición del carácter y los contadores del bucle
    int cx, cy;
    int pos = c - 33; // Posición en el array de fuentes basándose en el carácter

    // Recorre cada fila del carácter
    for (cy = 0; cy < 16; cy++) {
        int maskCheck = 0x01; // Inicializa una máscara para comprobar cada bit en el array de fuentes
        for (cx = 0; cx < 10; cx++) {
            // Comprueba si el píxel actual del carácter está configurado (1) o no (0)
            if (((font[(pos*32) + (2*cy)] & (maskCheck << cx)) != 0) && cx < 8) {
                for (int z = 0; z < size; z++) {
                    for (int w = 0; w < size; w++) {
                        putPixel(fcolor, cx*size + posX + z, cy*size + posY + w);
                    }
                }
            }
            else {
                // Si el píxel no está configurado, dibújalo con el color de fondo
                for (int z = 0; z < size; z++) {
                    for (int w = 0; w < size; w++) {
                        putPixel(bcolor, cx*size + posX + z, cy*size + posY + w);
                    }
                }
            }
        }
    }
    // Compruebo si la posición actual está en el borde de la pantalla y pasa a la siguiente línea si es necesario
    if (posX >= VBE_mode_info->width-MARGIN-(16*size)) {
        posY += 16*size;
        posX = MARGIN;
        return;
    }

    // Pasa a la siguiente posición del carácter
    posX += 10*size;
}

void checkChar(char character, uint64_t fcolor, uint64_t bcolor){
    switch(character){
        case '\b':
             vdDelete();
             return;
        case '\n':
            vdNewline();
            return;
        case '\t':
            vdTab();
            return;
        default:
            vdPrintCharColor(character, fcolor, bcolor);
            return;

    }
}


void vdPrintColorLen(char * string, uint64_t fcolor, uint64_t bcolor, int len){
    for(int i = 0; i < len && string[i] != 0; i++){
        checkChar(string[i], fcolor, bcolor );
    }
}


void vdPrintColor(char * string, uint64_t fColor, uint64_t bColor){
    for(int i = 0; string[i] != 0; i++){
        checkChar(string[i], fColor, bColor);
    }
}


void vdPrint(char * string){
    vdPrintColor(string, WHITE, BLACK);
}

void vdPrintColorUpto(char * string, uint64_t fcolor, uint64_t bcolor, int len){
    for(int i = 0; i < len && string[i] != 0; i++){
        checkChar(string[i], fcolor, bcolor );
    }
}

void vdPrintUpto(char * string, int len){
    vdPrintColorUpto(string, WHITE, BLACK, len);
}


void vdPrintError(char *string){
    vdPrintColor(string, RED, BLACK);
}


void vdDelete(){
    // Si la posición actual está en el borde superior izquierdo de la pantalla, no hay nada que borrar
    if (posX == MARGIN && posY == MARGIN) {
        return;
    }
    // Apaga el cursor
    //cursorOff();
    // Si la posición actual está en el borde izquierdo de la pantalla, mueve el cursor al final de la línea anterior
    if (posX <= MARGIN) {
        posX = VBE_mode_info->width-MARGIN-4;
        posY -= 16*size;
    }
    int i, j;
    // borrar el carácter actual
    for (i = 0; i < 16*size; i++) {
        for (j = 0; j < 10*size; j++) {
            // Dibuja un píxel negro en la posición del carácter actual
            putPixel(0, posX - j - 1, posY + i);
        }
    }

    // Si hay un cursor visible, mueve el contenido a la izquierda para borrar el carácter actual
    if (cursor_pos != 0) {
        backspaceMove();
    }
    // Ajusta la posición del cursor después de borrar el carácter
    posX -= 10*size;
}


void backspaceMove() {
    // Puntero al framebuffer para acceder a los píxeles
    uint8_t * framebuffer = (uint8_t *)(uintptr_t) VBE_mode_info->framebuffer;

    // Bucle para mover el contenido a la izquierda
    for (int i = posY; i < VBE_mode_info->height && i < posY + 3*16*size; i++) {
        for (int j = MARGIN; j < VBE_mode_info->width-MARGIN; j++) {

            // Salta los píxeles que están en la posición del carácter actual
            if (i <= posY+16*size && j<posX) {
                continue;
            }

            // Calcula el offset del píxel actual en el framebuffer
            uint64_t offset = (j * ((VBE_mode_info->bpp)/8)) + (i * VBE_mode_info->pitch);

            // Lee los valores de los píxeles
            uint8_t blue = framebuffer[offset];
            uint8_t green = framebuffer[offset + 1];
            uint8_t red = framebuffer[offset + 2];

            // Convierte los valores RGB a un color hexadecimal
            uint64_t hexColor = (red << 16) | (green << 8) | blue;

            // Si el píxel está en el borde izquierdo de la pantalla, lo mueve al final de la línea anterior
            if (j <= MARGIN + 10*size - 1) {
                putPixel(hexColor, VBE_mode_info->width-MARGIN-10*size + (j - MARGIN)-4, i-16*size);
            } else{
                // De lo contrario, lo mueve a la izquierda
                putPixel(hexColor, j - (10*size), i);
            }

        }
    }
}
void vdNewline() {
    //cursorOff();
    cursor_pos = 0;
    // if (posY >= VBE_mode_info->height-(32*size)-MARGIN) {
    //     moveScreen();
    // }
    posX = MARGIN;
    posY += 16*size;
    moveScreenUpIfFull();
}

void vdTab(){
    vdPrint("    ");
    return;
}


void vdPrintRegister(char *regName, uint64_t regValue) { // le paso el nombre y el estado del registro en uint64
    char buffer[256] = {0}; 
    vdPrint(regName);  
    vdPrint(": 0x"); 
    uintToBase(regValue, buffer, 16);  // pasa el estado a hexa 
    vdPrint(buffer);
    vdNewline();
}


void clear() {
    int i, j;
    for (i = 0; i < VBE_mode_info->height; i++) {
        for (j = 0; j < VBE_mode_info->width; j++) {
            putPixel(0,j, i);
        }
    }
    posX = MARGIN;
    posY = MARGIN;
}

void moveScreenUpIfFull() {
    if (posY >= VBE_mode_info->height - (32*size) - MARGIN) {
        // Mover todo el contenido una línea hacia arriba
        uint8_t * framebuffer = (uint8_t *)(uintptr_t) VBE_mode_info->framebuffer;
        for (int y = MARGIN + 16*size; y < VBE_mode_info->height; y++) {
            for (int x = MARGIN; x < VBE_mode_info->width-MARGIN; x++) {
                uint64_t destOffset = (x * (VBE_mode_info->bpp/8)) + ((y - 16*size) * VBE_mode_info->pitch);
                uint64_t srcOffset = (x * (VBE_mode_info->bpp/8)) + (y * VBE_mode_info->pitch);
                
                framebuffer[destOffset] = framebuffer[srcOffset];
                framebuffer[destOffset + 1] = framebuffer[srcOffset + 1];
                framebuffer[destOffset + 2] = framebuffer[srcOffset + 2];
            }
        }
        // Limpiar la última línea
        for (int y = VBE_mode_info->height - 16*size; y < VBE_mode_info->height; y++) {
            for (int x = MARGIN; x < VBE_mode_info->width-MARGIN; x++) {
                putPixel(0, x, y);
            }
        }
        posY -= 16*size;
    }
}


void changeSize(int a){
    if(a == 0){ // Reducir el tamaño a 1 si se pasa 0
        size = 1;
    }else if(a > 0){ // Si se pasa un valor positivo, establecer el tamaño directamente o incrementar en 1 si a es 1
        size = a == 1 ? size + 1 : a;
    }else if(size + a > 0){ // Si se pasa un valor negativo, restar del tamaño actual
        size += a;
    }
}

void drawSquare(uint64_t x, uint64_t y, uint32_t size, uint64_t color) {
    for (uint32_t i = 0; i < size; i++) {
        for (uint32_t j = 0; j < size; j++) {
            putPixel(color, x + j, y + i);
        }
    }
}

void vdEndPrintRegisters() {
    vdNewline();  // Agregar un salto de línea explícito
    posX = MARGIN;  // Resetear la posición X
    moveScreenUpIfFull(); 
}