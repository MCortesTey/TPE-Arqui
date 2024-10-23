#include <registers.h>
#define ZERO_EXCEPTION_ID 0

static void zero_division();

void exceptionDispatcher(int exception) {
	if (exception == ZERO_EXCEPTION_ID)
		zero_division();
	else{
		invalidOperation();
	}
	//hago print de registros
	//printf de "Presione una tecla para continuar"
	//leer ese caracter pero no escribirlo en pantalla, solo leerlo para poder 
	// rectivar el cursor
}

static void zero_division() {
	// Activar isChecked si no se hizo antes
	//escribir "ERROR: No se puede dividir por 0"
	//newLine

}

void invalidOperation(){
	// Activar isChecked si no se hizo antes
	//escribir "ERROR: Operación Invalida"
	//newLine	
}
