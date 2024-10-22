#ifndef NAIVE_CONSOLE_H
#define NAIVE_CONSOLE_H

#include <stdint.h>

void ncPrint(const char * string);
void ncPrintChar(char character);
void ncPrintStyle(const char * string, char style);
void ncPrintCharStyle(char character, char style);
void ncPrintColour(const char * string, uint8_t textColour, uint8_t bgColour);
void ncNewline();
void ncPrintDec(uint64_t value);
void ncPrintHex(uint64_t value);
void ncPrintBin(uint64_t value);
void ncPrintBase(uint64_t value, uint32_t base);
void ncClear();
void ncDelete();

#endif