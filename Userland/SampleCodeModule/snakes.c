#include "snakes.h"
#include "functions.h"

static int exit = 0;

int snakes(){
    // Animación
    displayBackground();
    while(!exit){
        int option = menu();
        if(option == EXIT){
            break;
        }
        gameLoop(option);
    }
    exit = 0;
    return 0;
}

int menu(){
    displayMenu();
    int option = -1;
    while(option == -1){
        char dir = getchar_s();
        if (dir >= '0' && dir <= '9') {
            option = dir - '0'; // Convertir el carácter a un entero
        } else if (dir == 'e') { // Suponiendo que 'e' es para salir
            option = EXIT;
        } else {
            printf_s("Opción no válida. Intente de nuevo.\n");
        }
    }
    return option;
}

void displayMenu() {
    printf_s("Seleccione una opción:\n");
    printf_s("0: Jugar\n");
    printf_s("e: Salir\n");
}

void displayBackground() {
    // Lógica para mostrar el fondo del juego
    printf_s("Fondo del juego mostrado.\n");
}

void gameLoop(int option) {
    // Lógica del juego basada en la opción seleccionada
    printf_s("Iniciando el juego con la opción: %d\n", option);
    // Aquí iría la lógica del juego
}


