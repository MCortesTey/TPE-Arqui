#include "snakes.h"
#include "functions.h"
#include "syscalls_shell.h"

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

#define P1_COLOR 0x1F1FFF
#define P2_COLOR 0x50962D

#define MENU_FONT 3
#define COUNTDOWN_FONT 0

#define PLAYER_1 1
#define PLAYER_2 2
#define EMPTY 0

#define SPAWN_1_X 3
#define SPAWN_1_Y 4
#define SPAWN_2_X (COLUMNS-SPAWN_1_X)
#define SPAWN_2_Y SPAWN_1_Y


//se guardan los settings
static int players = 1; 
static int speed = 1;
static int exit = 0;

// matriz/tablero de posiciones
uint64_t board[SCREEN_WIDTH][SCREEN_HEIGHT] = {EMPTY};

int snakes(){
    // Animación
    clearScreen();
    setSize(MENU_FONT);
    printf_s(START_MSG, players, speed);
    menu();
    clearScreen();
    displayBackground();
    displayLayout();
    spawnPlayers();
    countDown();
    // while(!exit){
    //     int option = menu();
    //     if(option == EXIT){
    //         break;
    //     }
    //     gameLoop(option);
    // }
    // exit = 0;
    syscall_sleep(5000);
    //clearScreen();
    resetSize();
    return 0;
}
/*
int menu(){
    //displayMenu();
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
*/
int menu() {
    int option = -1;
    
    while(option == -1) {
        char key = getchar_s();
        
        if (key == '\n') {
            option = PLAY;
            //gameLoop(option);
        } else if (key == 's') {
            // Preguntar por cantidad de jugadores
            printf_s("Ingrese cantidad de jugadores (1/2): ");
            char players_response = getchar_s();
            players = (players_response == '2') ? 2 : 1;
            printf_s("%d\n\n", players);
            
            // Preguntar por velocidad
            printf_s("Ingrese velocidad (1-3): ");
            char speed_char = getchar_s();
            int speed_option = speed_char - '0';
            
            if (speed >= 1 && speed <= 3) {
                speed = speed_option;
                printf_s("%d\n\n", speed);
                printf_s("Presione ENTER para jugar\n\n");
            } else {
                printf_s("Error: La velocidad debe estar entre 1 y 3\n\n");
            }
        } else if (key == 'e') {
            option = EXIT;
        }
    }
    return option;
}

/*
void displayMenu() {
   // changeSize();
    printf_s("Seleccione una opción:\n");
    printf_s("0: Jugar\n");
    printf_s("e: Salir\n");
}
*/

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
    int end = 0;
    spawnPlayers();
    countDown();
    while(!end){

    }
    // Aquí iría la lógica del juego
}

void spawnPlayers(){
    if(players == 2){
        //p2
        drawSquare((SPAWN_2_X*CELL_SIZE)+OFFSET_X, (SPAWN_2_Y*CELL_SIZE)+OFFSET_Y, CELL_SIZE, P2_COLOR);
        drawSquare(((SPAWN_2_X-1)*CELL_SIZE)+OFFSET_X, (SPAWN_2_Y*CELL_SIZE)+OFFSET_Y, CELL_SIZE, P2_COLOR);
        board[SPAWN_2_Y][SPAWN_2_X] = PLAYER_2;
        board[SPAWN_2_Y][SPAWN_2_X-1] = PLAYER_2;
    }
    //p1
    drawSquare((SPAWN_1_X*CELL_SIZE)+OFFSET_X, (SPAWN_1_Y*CELL_SIZE)+OFFSET_Y, CELL_SIZE, P1_COLOR);
    drawSquare(((SPAWN_1_X-1)*CELL_SIZE)+OFFSET_X, (SPAWN_1_Y*CELL_SIZE)+OFFSET_Y, CELL_SIZE, P1_COLOR);
    board[SPAWN_1_Y][SPAWN_1_X] = PLAYER_1;
    board[SPAWN_1_Y][SPAWN_1_X-1] = PLAYER_1;
    return;
}

void countDown() {
    setSize(COUNTDOWN_FONT);
    printf_s("\t\t\t\t\t\t");
    for (int i = 3; i > 0; i--) {
        printf_s("%d", i); // Imprimir el número de la cuenta regresiva
        syscall_sleep(1000); // Esperar 1 segundo (1000 ms)
        putchar_s('\b');
    }
    printf_s("Go!\n"); // Mensaje al finalizar la cuenta regresiva
    resetSize();
}




