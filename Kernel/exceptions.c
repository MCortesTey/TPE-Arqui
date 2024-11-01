#include <registers.h>
#define ZERO_EXCEPTION_ID 0

static void zero_division();
#include "exceptions.h"
#include "videoDriver.h"
#include "lib.h"

void exceptionDispatcher(int exception) {
	if (exception == ZERO_EXCEPTION_ID)
		zero_division();
	else{
		invalidOperation();
	}
	printRegStatus(1); //&regBackup
	//vdNewline();
	//vdPrint("Press keyboard to continue");
	//getKeyPressed(); 
	//leer ese caracter pero no escribirlo en pantalla, solo leerlo para poder 
	// rectivar el cursor
}

void zero_division() {
	//regsChecked = 1;
	vdPrintError("Error: Division by cero is not permitted");
	vdNewline();

}

void invalidOperation(){
	//regsChecked = 1;
	vdPrintError("Error: Invalid Operation");
	vdNewline();
}
