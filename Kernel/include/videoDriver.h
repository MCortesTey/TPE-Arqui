#ifndef VIDEO_DRIVER_H
#define VIDEO_DRIVER_H

#include <stdint.h>

typedef struct vbe_mode_info_structure * VBEInfoPtr;

extern VBEInfoPtr VBE_mode_info;

void putPixel(uint32_t hexColor, uint64_t x, uint64_t y);

#endif // VIDEO_DRIVER_H

