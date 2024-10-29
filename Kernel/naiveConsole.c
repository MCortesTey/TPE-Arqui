#include <naiveConsole.h>
#include <stddef.h>

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);

static char buffer[64] = { '0' };
static uint8_t * const video = (uint8_t*)0xB8000;
static uint8_t * currentVideo = (uint8_t*)0xB8000;
static const uint32_t width = 80;
static const uint32_t height = 25 ;

void ncPrint(const char * string)
{
	int i;

	for (i = 0; string[i] != 0; i++)
		ncPrintChar(string[i]);
}

void ncPrintColour(const char * string, uint8_t textColour, uint8_t bgColour)
{
    int i;
    uint8_t attribute = (bgColour << 4) | (textColour & 0x0F);

    for (i = 0; string[i] != 0; i++)
    {
        *currentVideo = string[i];
        *(currentVideo + 1) = attribute;
        currentVideo += 2;
    }
}

void ncPrintChar(char character)
{
	*currentVideo = character;
	currentVideo += 2;
}

void ncPrintCharStyle(char character, char style){
	*currentVideo++ = character;
	*currentVideo++ = style;
}

void ncPrintStyle(const char * string, char style){
	int i;
	for (i = 0; string[i] != 0; i++)
		ncPrintCharStyle(string[i], style);
}

void ncNewline()
{
	do
	{
		ncPrintChar(' ');
	}
	while((uint64_t)(currentVideo - video) % (width * 2) != 0);
}

void ncPrintDec(uint64_t value)
{
	ncPrintBase(value, 10);
}

void ncPrintHex(uint64_t value)
{
	ncPrintBase(value, 16);
}

void ncPrintBin(uint64_t value)
{
	ncPrintBase(value, 2);
}

void ncPrintBase(uint64_t value, uint32_t base)
{
    uintToBase(value, buffer, base);
    ncPrint(buffer);
}

void ncClear()
{
	int i;

	for (i = 0; i < height * width; i++)
		video[i * 2] = ' ';
	currentVideo = video;
}

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base)
{
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}

void ncDelete()
{
	currentVideo -= 2;
	ncPrintChar(' ');
	currentVideo -= 2;
}

int getHours();
int getMinutes();
int getSeconds();

// Arma un string con la hora actual en formato HH:MM:SS
char * TimeClock(char * buffer) {
    int hours = getHours();
    int minutes = getMinutes();
    int seconds = getSeconds();

    // Los valores vienen en BCD, hay que pasarlos a decimal
    hours = ((hours & 0xF0) >> 4) * 10 + (hours & 0x0F);
    minutes = ((minutes & 0xF0) >> 4) * 10 + (minutes & 0x0F);
    seconds = ((seconds & 0xF0) >> 4) * 10 + (seconds & 0x0F);

    convertToGMTMinus3(&hours);
    minutes = minutes % 60;
    seconds = seconds % 60;

    int digits = 0;
    
    // si hora mins o segs son menors a 10 le agregamos un 0 adelante
    if (hours < 10) {
        buffer[digits++] = '0';
        digits += uintToBase(hours, buffer + digits, 10);
    } else {
        digits += uintToBase(hours, buffer + digits, 10);
    }
    
    buffer[digits++] = ':';
    
    if (minutes < 10) {
        buffer[digits++] = '0';
        digits += uintToBase(minutes, buffer + digits, 10);
    } else {
        digits += uintToBase(minutes, buffer + digits, 10);
    }
    
    buffer[digits++] = ':';
    
    if (seconds < 10) {
        buffer[digits++] = '0';
        digits += uintToBase(seconds, buffer + digits, 10);
    } else {
        digits += uintToBase(seconds, buffer + digits, 10);
    }
    
    buffer[digits] = 0; 
    return buffer;
}


void convertToGMTMinus3(int *hours) {
    *hours -= 3;
    if (*hours < 0) {
        *hours += 24; 
    }
}
