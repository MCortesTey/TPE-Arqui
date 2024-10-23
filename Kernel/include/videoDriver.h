#ifndef VIDEO_DRIVER_H
#define VIDEO_DRIVER_H

#include <stdint.h>

typedef struct vbe_mode_info_structure * VBEInfoPtr;

extern VBEInfoPtr VBE_mode_info;

void putPixel(uint32_t hexColor, uint64_t x, uint64_t y);
void vdPrintCharColor(char c, uint64_t fcolor, uint64_t bcolor);
void vdPrintColor(char * string, uint64_t fColor, uint64_t bColor);
void vdPrintColorLen(char * string, uint64_t fcolor, uint64_t bcolor, int len);
void vdPrintRegister(const char *regName, uint64_t regValue);

#endif // VIDEO_DRIVER_H

