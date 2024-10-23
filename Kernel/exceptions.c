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


/*static const char* registerNames[18] = {
    "RAX", "RBX", "RCX", "RDX", "RSI", "RDI", "RBP", "RSP", "R8 ", "R9 ", "R10", "R11", "R12", "R13", "R14", "R15", "RIP", "RFLAGS"
};*/
 
// llamar a esta funcion despues de las excepciones para que imprima el estado de los registros
// las funciones de printear les puse cualq nombre, cuando las definamos bien lo cambio
void printRegisters () {
	/*
	funcion que printee cadena
	print("Register Status: ");
	uint64_t *array = getRegisters();
	uint64_t reg;
	for (int i = 0; i < REGISTERS; i++)
	{
		status = array[i];
		print(registerNames[i]);
		print(' ');
		print hexa (status);
	}
	*/
	return;
}