#include "snakes.h"
#include "functions.h"
#include "syscalls_shell.h"

#define OFFSET_X 24
#define OFFSET_Y 80

#define B_COLOR1 0xF5D5A1
#define B_COLOR2 0xCDA360
#define BORDER_COLOR 0x8B8589

#define P1_COLOR 0x1F1FFF
#define P2_COLOR 0x50962D

#define MENU_FONT 3
#define COUNTDOWN_FONT 3
#define FINAL_FONT 4 

#define PLAYER_1 1
#define PLAYER_2 2
#define EMPTY 0

#define SPAWN_1_X 3
#define SPAWN_1_Y 4
#define SPAWN_2_X (COLUMNS-SPAWN_1_X)
#define SPAWN_2_Y SPAWN_1_Y

#define MAX_SNAKES 2 // Número máximo de serpientes

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

#define player1Up 'w'
#define player1Down 's'
#define player1Left 'a'
#define player1Right 'd'

#define player2Up 'i'
#define player2Down 'k'
#define player2Left 'j'
#define player2Right 'l'

//se guardan los settings
static int players = 1; 
static int speed = 1;
static int end = 0;
static int winner = 0;

// matriz/tablero de posiciones
uint64_t board[SCREEN_WIDTH][SCREEN_HEIGHT] = {EMPTY};
static Snake snake1 = {0}; // Instancia de serpiente 1, inicializada a 0
static Snake snake2 = {0}; // Instancia de serpiente 2, inicializada a 0

int snakes(){
    // Animación
    clearScreen();
    setSize(MENU_FONT);
    printf_s(START_MSG, players, speed);
    menu();
    clearScreen();
    displayBackground();
    displayLayout();
    //gameLoop();
    syscall_sleep(2000);
    finalScreen();
    // while(!end){
    //     int option = menu();
    //     if(option == EXIT){
    //         break;
    //     }
    //     gameLoop();
    //     finalScreen();
    // }
    // end = 0;
    syscall_sleep(5000);
    //clearScreen();
    resetSize();
    return 0;
}

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
            printf_s("Ingrese velocidad (1-5): ");
            char speed_char = getchar_s();
            int speed_option = speed_char - '0';
            
            if (speed >= 1 && speed <= 5) {
                speed = speed_option;
                printf_s("%d\n\n", speed);
                printf_s("Presione ENTER para jugar\n\n");
            } else {
                printf_s("Error: La velocidad debe estar entre 1 y 5\n\n");
            }
        } else if (key == 'e') {
            option = EXIT;
        }
    }
    return option;
}

void finalScreen(){
    clearScreen();
    setSize(FINAL_FONT);
    winner = 1 ;
    if(winner != 0){
        printf_s("\n\n\t   Winner winner\n\t  chicken dinner!\n");
        printf_s("\t\tPlayer%d wins", winner);
    }else{
        printf_s("\n\n\n\t\tGAME OVER");
    }
    syscall_sleep(3000);
    resetSize();
    winner = 0; //reseteo el winner para la proxima
}

void gameTick(){
    syscall_sleep(800/speed);
}

void displayLayout() {
    // Lógica para mostrar el fondo del juego
    // This function draws the margins of the game board by iterating through each pixel on the screen.
    // It checks if the current pixel is part of the margin (top, bottom, left, or right) and if so, sets its color to RED.
    for(int i = OFFSET_X; i < SCREEN_WIDTH - OFFSET_X; i++){
        for(int j = OFFSET_Y; j <= SCREEN_HEIGHT + OFFSET_Y ; j++){
            // The following conditions check if the current pixel is part of the margin.
            // The margin is defined as 20 pixels from the edges of the screen.
            if((i == OFFSET_X && j >= OFFSET_Y && j <= SCREEN_HEIGHT + OFFSET_Y) || // Left margin
               (i == SCREEN_WIDTH - OFFSET_X - 1 && j >= OFFSET_Y && j <= SCREEN_HEIGHT + OFFSET_Y) || // Right margin
               (j == OFFSET_Y && i >= OFFSET_X && i <= SCREEN_WIDTH - OFFSET_X - 1) || // Top margin
               (j == SCREEN_HEIGHT + OFFSET_Y && i >= OFFSET_X && i <= SCREEN_WIDTH - OFFSET_X - 1)){ // Bottom margin
                drawSquare(i, j, 5, BORDER_COLOR); // Draw a 5x5 square at the current pixel position with RED color
            }
        }
    }
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


void gameLoop() {
    spawnPlayers();
    countDown();
    int pos = syscall_getbufferpos();
    char key;
    int timeLimit = 3;
    //printf_s("%d", startTime);
    while(!end){
        int currentTime;
        //printf_s("%d", pos);
        int startTime;
        syscall_timerms(&startTime);
        printf_s("%d\n", startTime);
        while(1){
            syscall_timerms(&currentTime);
            printf_s("%d\n",currentTime);
            if (currentTime - startTime > timeLimit) {
                break; // Salir del bucle si se ha alcanzado el tiempo límite
            }
            //printf_s("ayuda");
            key = syscall_getcharat(pos-1);
            if(key != 0){
                pos++;
            }
            handleInput(key);
        }
        moveSnake(&snake1, PLAYER_1, 0);
        moveSnake(&snake2, PLAYER_2, 0);
        gameTick();
    }
    end = 0; //para que solo se salga por el menu
    return;
}

void collision(int player){
    end = 1;
    if(players == 2){
        switch (player){
        case PLAYER_1: //PERDIO EL P1
            winner = PLAYER_2;
            break;
        
        case PLAYER_2: //PERDIO EL P2
            winner = PLAYER_1;
            break;
        }
    }else{ // si solo era 1 jugador por defecto gana la casa
        winner = 0 ;
    }
    
}

void handleInput(char key) {
        int who = getPlayerByKey(key);
        if(who == PLAYER_1){
            snake1.dir = inputToDir(key, PLAYER_1);
            
        } else if(who == PLAYER_2){
            snake2.dir = inputToDir(key, PLAYER_2);
            
        }
}

int getPlayerByKey(char key) {
    // Completar los casos para determinar el jugador según la tecla
    if (key == player1Up || key == player1Down || key == player1Left || key == player1Right) {
        return PLAYER_1; // Retorna el jugador 1 si se presiona una tecla de jugador 1
    } else if (key == player2Up || key == player2Down || key == player2Left || key == player2Right) {
        return PLAYER_2; // Retorna el jugador 2 si se presiona una tecla de jugador 2
    }
    return EMPTY; // Retorna EMPTY si no se reconoce la tecla
}

int inputToDir(char key, int player){
    if(player == PLAYER_1){
        switch(key){
            case player1Up:
                return UP; // Retorna dirección arriba
            case player1Down:
                return DOWN; // Retorna dirección abajo
            case player1Left:
                return LEFT; // Retorna dirección izquierda
            case player1Right:
                return RIGHT; // Retorna dirección derecha
        }
    } else if(player == PLAYER_2) {
        switch(key){
            case player2Up:
                return UP; // Retorna dirección arriba
            case player2Down:
                return DOWN; // Retorna dirección abajo
            case player2Left:
                return LEFT; // Retorna dirección izquierda
            case player2Right:
                return RIGHT; // Retorna dirección derecha
        }
    }
    return -1; // Retorna -1 si no se reconoce la tecla
}

void spawnPlayers(){
    if(players == 2){
        //p2
        drawSquare((SPAWN_2_X*CELL_SIZE)+OFFSET_X, (SPAWN_2_Y*CELL_SIZE)+OFFSET_Y, CELL_SIZE, P2_COLOR);
        drawSquare(((SPAWN_2_X-1)*CELL_SIZE)+OFFSET_X, (SPAWN_2_Y*CELL_SIZE)+OFFSET_Y, CELL_SIZE, P2_COLOR);
    }
    //p1
    drawSquare((SPAWN_1_X*CELL_SIZE)+OFFSET_X, (SPAWN_1_Y*CELL_SIZE)+OFFSET_Y, CELL_SIZE, P1_COLOR);
    drawSquare(((SPAWN_1_X-1)*CELL_SIZE)+OFFSET_X, (SPAWN_1_Y*CELL_SIZE)+OFFSET_Y, CELL_SIZE, P1_COLOR);
    initSnakes();
    return;
}

void countDown() {
    setSize(COUNTDOWN_FONT);
    printf_s("\t\t\t\t");
    for (int i = 3; i > 0; i--) {
        printf_s("%d", i); // Imprimir el número de la cuenta regresiva
        syscall_sleep(1000); // Esperar 1 segundo (1000 ms)
        putchar_s('\b');
    }
    printf_s("Go!\n"); // Mensaje al finalizar la cuenta regresiva
    resetSize();
}

// Inicializar las serpientes
void initSnakes() { // Se pasa la serpiente como parámetro
    if(players == 2 ){
        snake2.x[snake2.length] = SPAWN_2_X;
        snake2.y[snake2.length++] = SPAWN_2_Y;
        snake2.x[snake2.length] = SPAWN_2_X+1;
        snake2.y[snake2.length++] = SPAWN_2_Y;
    }
    snake1.x[snake1.length] = SPAWN_1_X; // Usar el parámetro
    snake1.y[snake1.length++] = SPAWN_1_Y; // Usar el parámetro
    snake1.x[snake1.length] = SPAWN_1_X-1; // Usar el parámetro
    snake1.y[snake1.length++] = SPAWN_1_Y; // Usar el parámetro
}

// Añadir un nodo a la serpiente especificada
void grow(Snake* snake, int x, int y) { // Se pasa la serpiente como parámetro
    if (snake->length < MAX_LENGTH) { // Verificar longitud de la serpiente
        snake->x[snake->length] = x; // Establecer la coordenada X
        snake->y[snake->length] = y; // Establecer la coordenada Y
        snake->length++; // Incrementar la longitud de la serpiente
    }
}

// Iterar sobre los nodos de la serpiente especificada y aplicar una función a cada uno
void iterateSnake(Snake* snake, int player) { // Se pasa la serpiente como parámetro
    for (int i = 0; i < snake->length; i++) {
        drawSquare(snake->x[i] * CELL_SIZE + OFFSET_X, 
                   snake->y[i] * CELL_SIZE + OFFSET_Y, 
                   CELL_SIZE, P1_COLOR); // Aplicar la función a las coordenadas del nodo
    }
}

// Reiniciar las serpientes
void resetSnakes(Snake* snake) { // Se pasa la serpiente como parámetro
    snake->length = 0; // Reiniciar la longitud de la serpiente
}

// Mover la serpiente especificada en la dirección dada
void moveSnake(Snake* snake, int player, int g) {
    if (snake->length > 0 ) { // Verificar que la serpiente tenga segmentos
        if(!g){
        
            drawSquare(snake->x[snake->length-1] * CELL_SIZE + OFFSET_X, 
                   snake->y[snake->length-1] * CELL_SIZE + OFFSET_Y, 
                   CELL_SIZE, 
                   ((snake->x[snake->length-1]+ snake->y[snake->length-1])% 2 == 0) ? B_COLOR1 : B_COLOR2); // Color de fondo alternativo)
        }else{
            grow(snake, snake->x[snake->length], snake->y[snake->length]);
        }
        for (int i = snake->length; i > 0; i--) {// Mover los segmentos de la serpiente
            snake->x[i] = snake->x[i - 1]; // Mover la coordenada X
            snake->y[i] = snake->y[i - 1]; // Mover la coordenada Y
            board[snake->x[i]][snake->y[i]] = player; // Actualizo el tablero con quien ocupa ese espacio
        }

        // Actualizar la cabeza de la serpiente según la dirección
        switch (snake->dir) {
            case UP:
                if (snake->y[0] > 0) {
                    snake->y[0]--; // Mover hacia arriba si no sale del rango
                } else {
                    collision(player); // Llamar a collision() si intenta salir del rango
                }
                break;
            case DOWN:
                if (snake->y[0] < SCREEN_HEIGHT - 1) {
                    snake->y[0]++; // Mover hacia abajo si no sale del rango
                } else {
                    collision(player); // Llamar a collision() si intenta salir del rango
                }
                break;
            case LEFT:
                if (snake->x[0] > 0) {
                    snake->x[0]--; // Mover hacia la izquierda si no sale del rango
                } else {
                    collision(player); // Llamar a collision() si intenta salir del rango
                }
                break;
            case RIGHT:
                if (snake->x[0] < SCREEN_WIDTH - 1) {
                    snake->x[0]++; // Mover hacia la derecha si no sale del rango
                } else {
                    collision(player); // Llamar a collision() si intenta salir del rango
                }
                break;
        }
        // Dibuja la cabeza y la cola de la serpiente
        iterateSnake(snake, player);
        //drawSnakePosition(snake, player); // Llamada actualizada
    }
}

void drawSnakePosition(Snake* snake, int player) {
    // Dibuja la cola de la serpiente en el color de fondo
    if (snake->length+1 > 1) {
        drawSquare(snake->x[snake->length - 1] * CELL_SIZE + OFFSET_X, 
                   snake->y[snake->length - 1] * CELL_SIZE + OFFSET_Y, 
                   CELL_SIZE, 
                   ((snake->x[snake->length]+ snake->y[snake->length])% 2 == 0) ? B_COLOR2 : B_COLOR1); // Color de fondo alternativo
    }

    // Dibuja la cabeza de la serpiente
    printf_s("%d",player);
    switch(player){
        case PLAYER_1:
            drawSquare(snake->x[0] * CELL_SIZE + OFFSET_X, 
            snake->y[0] * CELL_SIZE + OFFSET_Y, 
            CELL_SIZE, 
            P1_COLOR); // Color de la cabeza
            break;
        case PLAYER_2:
            drawSquare(snake->x[0] * CELL_SIZE + OFFSET_X, 
            snake->y[0] * CELL_SIZE + OFFSET_Y, 
            CELL_SIZE, 
            P2_COLOR); // Color de la cabeza
            break;
    }
   
}

void fruitControl(){
    int aux = getRandom(); // Genera un número aleatorio
    // Aquí puedes usar 'aux' para determinar la posición de la fruta o cualquier otra lógica
}

