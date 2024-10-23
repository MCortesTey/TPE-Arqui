#include <registers.h>
#define ZERO_EXCEPTION_ID 0

static void zero_division();
#include "exceptions.h"
#include "videoDriver.h"

void exceptionDispatcher(int exception) {
	if (exception == ZERO_EXCEPTION_ID)
		zero_division();
	else{
		invalidOperation();
	}
	//hago print de registros
	vdPrint("Presione una tecla para continuar");
	//leer ese caracter pero no escribirlo en pantalla, solo leerlo para poder 
	// rectivar el cursor
}

static void zero_division() {
	// Activar isChecked si no se hizo antes
	vdPrintError("Error: División por cero no es posible");
	//new Line

}

void invalidOperation(){
	// Activar isChecked si no se hizo antes
	vdPrintError("Error: Operación Inválida");
	//newLine	
}
