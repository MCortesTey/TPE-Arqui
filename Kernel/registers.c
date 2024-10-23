#include <registers.h>
#define SIZE 18
uint64_t regArray [SIZE];

char regChecked;

uint64_t *getRegisters(){
    if ( isChecked() ){  
        return regArray;
    }
    return NULL;
}

void saveRegisters ( uint64_t * reg , char ok){
    for ( int i = 0 ; i<SIZE ; i++ ){
        regArray[i] = reg[i];
    }
    regChecked = ok;
    return;
}

char isChecked ( ){
    return ( regChecked == 1 );
}