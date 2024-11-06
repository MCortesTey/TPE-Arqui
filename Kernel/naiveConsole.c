#include <naiveConsole.h>

static char buffer[64] = { '0' };
static uint8_t * const video = (uint8_t*)0xB8000;
static uint8_t * currentVideo = (uint8_t*)0xB8000;
static const uint32_t width = 80;
static const uint32_t height = 25 ;

void ncPrint(const char * string)  // imprime string
{
	int i;

	for (i = 0; string[i] != 0; i++)
		ncPrintChar(string[i]);
}

void ncPrintColour(const char * string, uint8_t textColour, uint8_t bgColour) // imprime string con color de letra y fondo
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

void ncPrintChar(char character)  // imprime char
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

void ncPrintDec(uint64_t value)   // imprime valor decimal
{
	ncPrintBase(value, 10);
}

void ncPrintHex(uint64_t value)  // imprime valor hexadecimal
{
	ncPrintBase(value, 16);
}

void ncPrintBin(uint64_t value)  // imprime binario
{
	ncPrintBase(value, 2);
}

void ncPrintBase(uint64_t value, uint32_t base)  // imprime valor segun su base
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

uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base) // convierte valor uint segun su base
{
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	// Calcula caracteres para cada digito
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= base);

	*p = 0;

	//Invierte la cadena en el buffer
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
