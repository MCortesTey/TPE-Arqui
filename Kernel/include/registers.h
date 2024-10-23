#ifndef REGISTERS_H_
#define REGISTERS_H_

#include <stdint.h>
#include <stdio.h>

uint64_t *getRegisters();
void saveRegisters ( uint64_t * reg , char ok);
char isChecked();


#endif