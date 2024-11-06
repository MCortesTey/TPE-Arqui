#ifndef VIDEO_DRIVER_H
#define VIDEO_DRIVER_H

#define MARGIN 10
#define BLACK 0x000000
#define RED 0xFF0000
#define WHITE 0xFFFFFF

#include <stdint.h>

typedef struct vbe_mode_info_structure * VBEInfoPtr;

extern VBEInfoPtr VBE_mode_info;

void putPixel(uint32_t hexColor, uint64_t x, uint64_t y);
void vdPrintCharColor(char c, uint64_t fcolor, uint64_t bcolor);
void checkChar(char character, uint64_t fcolor, uint64_t bcolor);
void vdPrintColorLen(char * string, uint64_t fcolor, uint64_t bcolor, int len);
void vdPrintColor(char * string, uint64_t fColor, uint64_t bColor);
void vdPrint(char * string);
void vdPrintColorUpto(char * string, uint64_t fcolor, uint64_t bcolor, int len);
void vdPrintUpto(char * string, int len);
void vdPrintError(char * string);
void vdDelete();
void backspaceMove();
void vdNewline();
void vdTab();
void vdPrintRegister(char *regName, uint64_t regValue);
void clear();
void moveScreenUpIfFull();
void changeSize(int a);
void drawSquare(uint64_t x, uint64_t y, uint32_t size, uint64_t color);
void vdEndPrintRegisters();

#endif // VIDEO_DRIVER_H

