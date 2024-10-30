#include "snakes.h"
#include "functions.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 760

#define CELL_SIZE 80
#define ROWS (SCREEN_HEIGHT / CELL_SIZE)
#define COLUMNS (SCREEN_WIDTH / CELL_SIZE)

#define OFFSET_X 32
#define OFFSET_Y 40

#define B_COLOR1 0xF5D5A1
#define B_COLOR2 0xCDA360
#define BORDER_COLOR 0x8B8589

#define MENU_FONT 4

static int exit = 0;
// matriz/tablero de posiciones
uint64_t board[SCREEN_WIDTH][SCREEN_HEIGHT];

int snakes(){
    // Animación
    displayBackground();
    displayLayout();
    // while(!exit){
    //     int option = menu();
    //     if(option == EXIT){
    //         break;
    //     }
    //     gameLoop(option);
    // }
    // exit = 0;
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
   // changeSize();
    printf_s("Seleccione una opción:\n");
    printf_s("0: Jugar\n");
    printf_s("e: Salir\n");
}

void displayLayout() {
    // Lógica para mostrar el fondo del juego
    // This function draws the margins of the game board by iterating through each pixel on the screen.
    // It checks if the current pixel is part of the margin (top, bottom, left, or right) and if so, sets its color to RED.
    for(int i = OFFSET_X; i < SCREEN_WIDTH - OFFSET_X; i++){
        for(int j = OFFSET_Y; j < SCREEN_HEIGHT; j++){
            // The following conditions check if the current pixel is part of the margin.
            // The margin is defined as 20 pixels from the edges of the screen.
            if((i == OFFSET_X && j >= OFFSET_Y && j <= SCREEN_HEIGHT) || // Left margin
               (i == SCREEN_WIDTH - OFFSET_X - 1 && j >= OFFSET_Y && j <= SCREEN_HEIGHT) || // Right margin
               (j == OFFSET_Y && i >= OFFSET_X && i <= SCREEN_WIDTH - OFFSET_X - 1) || // Top margin
               (j == SCREEN_HEIGHT - 1 && i >= OFFSET_X && i <= SCREEN_WIDTH - OFFSET_X - 1)){ // Bottom margin
                drawSquare(i, j, 5, BORDER_COLOR); // Draw a 5x5 square at the current pixel position with RED color
            }
        }
    }
    //printf_s("Fondo del juego mostrado.\n");
}
void displayBackground() {
    for(int i = 0; i < COLUMNS; i++){
        for(int j = 0; j < ROWS; j ++){
            if((i + j) % 2 == 0){
                drawSquare((i*CELL_SIZE)+OFFSET_X, (j*CELL_SIZE)+OFFSET_Y, CELL_SIZE, B_COLOR1); // Draw a CELL_SIZE x CELL_SIZE square at the current pixel position with WHITE color
            } else {
                drawSquare((i*CELL_SIZE)+OFFSET_X, (j*CELL_SIZE)+OFFSET_Y, CELL_SIZE, B_COLOR2); // Draw a CELL_SIZE x CELL_SIZE square at the current pixel position with BLACK color
            }
        }
    }
}


void gameLoop(int option) {
    // Lógica del juego basada en la opción seleccionada
    printf_s("Iniciando el juego con la opción: %d\n", option);
    // Aquí iría la lógica del juego
}


