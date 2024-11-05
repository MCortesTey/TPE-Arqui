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
#define FRUIT_COLOR 0xFF0000

#define MENU_FONT 3
#define COUNTDOWN_FONT 3
#define FINAL_FONT 4 

#define PLAYER_1 1
#define PLAYER_2 2
#define EMPTY 0
#define FRUIT 3

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
static int fruit = 0;

// matriz/tablero de posiciones
uint64_t board[SCREEN_WIDTH][SCREEN_HEIGHT] = {EMPTY};
static Snake snake1 = {0}; 
static Snake snake2 = {0}; 

int snakes(){
    while(!end){
        //clearScreen();
        int option = menu();
        if(option == EXIT){
            break;
        }
        displayBackground();
        displayLayout();
        gameLoop();
        finalScreen();
    }
    end = 0;
    clearScreen();
    resetSize();
    return 0;
}

int menu() {
    setSize(MENU_FONT);
    printf_s(START_MSG, players, speed);
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
            
            if (speed_option >= 1 && speed_option <= 5) {
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
    clearScreen();
    resetSize();
    return option;
}

void finalScreen(){
    clearScreen();
    setSize(FINAL_FONT);
    if(winner != 0){
        printf_s("\n\n\t   Winner winner\n\t  chicken dinner!\n");
        printf_s("\t\tPlayer%d wins", winner);
    }else{
        printf_s("\n\n\n\t\tGAME OVER");
    }
    syscall_sleep(5000);
    resetSize();
    clearScreen();
    winner = 0; //reseteo el winner para la proxima
}

void gameTick(){
    syscall_sleep(800/speed);
}

void displayLayout() {
    
    for(int i = OFFSET_X; i < SCREEN_WIDTH - OFFSET_X; i++){
        for(int j = OFFSET_Y; j <= SCREEN_HEIGHT + OFFSET_Y ; j++){
            if((i == OFFSET_X && j >= OFFSET_Y && j <= SCREEN_HEIGHT + OFFSET_Y) || // Margen izquierdo
               (i == SCREEN_WIDTH - OFFSET_X - 1 && j >= OFFSET_Y && j <= SCREEN_HEIGHT + OFFSET_Y) || // Margen derecho
               (j == OFFSET_Y && i >= OFFSET_X && i <= SCREEN_WIDTH - OFFSET_X - 1) || // Margen superior
               (j == SCREEN_HEIGHT + OFFSET_Y && i >= OFFSET_X && i <= SCREEN_WIDTH - OFFSET_X - 1)){ // Margen inferior
                drawSquare(i, j,5, BORDER_COLOR); // Dibujar un cuadrado de 3x3 en la posición actual con el color del borde
            }
        }
    }
}

void displayBackground() {
    for(int i = 0; i < COLUMNS; i++){
        for(int j = 0; j < ROWS; j ++){
            if((i + j) % 2 == 0){
                drawSquare((i*CELL_SIZE)+OFFSET_X, (j*CELL_SIZE)+OFFSET_Y, CELL_SIZE, B_COLOR1); 
            } else {
                drawSquare((i*CELL_SIZE)+OFFSET_X, (j*CELL_SIZE)+OFFSET_Y, CELL_SIZE, B_COLOR2); 
            }
        }
    }
}


void gameLoop() {
    spawnPlayers();
    countDown();
    int pos = syscall_getbufferpos();
    char key;
    long timeLimit = 1000/speed;
    while(!end){
        if(fruit == 0){fruitControl();};
        long currentTime;
        long startTime;
        syscall_timerms(&startTime);
        while(1){
            syscall_timerms(&currentTime);
            if (currentTime - startTime > timeLimit) {
                break; // Salir del bucle si se ha alcanzado el tiempo límite
            }
            key = syscall_getcharat(pos-1);
            if(key != 0){
                pos++;
            }
            handleInput(key);
        }
        moveSnake(&snake1, PLAYER_1);
        if(players ==2){moveSnake(&snake2, PLAYER_2);}
        //printBoard();
        //gameTick();
    }
    resetGame();
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
        int newDir = inputToDir(key, PLAYER_1);
        if ((snake1.dir == UP && newDir != DOWN) || 
            (snake1.dir == DOWN && newDir != UP) || 
            (snake1.dir == LEFT && newDir != RIGHT) || 
            (snake1.dir == RIGHT && newDir != LEFT)) {
            snake1.dir = newDir;
        }
    } else if(who == PLAYER_2){
        int newDir = inputToDir(key, PLAYER_2);
        if ((snake2.dir == UP && newDir != DOWN) || 
            (snake2.dir == DOWN && newDir != UP) || 
            (snake2.dir == LEFT && newDir != RIGHT) || 
            (snake2.dir == RIGHT && newDir != LEFT)) {
            snake2.dir = newDir;
        }
    }
}

int getPlayerByKey(char key) {
    if (key == player1Up || key == player1Down || key == player1Left || key == player1Right) {
        return PLAYER_1;
    } else if (key == player2Up || key == player2Down || key == player2Left || key == player2Right) {
        return PLAYER_2;
    }
    return EMPTY;
}

int inputToDir(char key, int player){
    if(player == PLAYER_1){
        switch(key){
            case player1Up:
                return UP;
            case player1Down:
                return DOWN;
            case player1Left:
                return LEFT;
            case player1Right:
                return RIGHT;
        }
    } else if(player == PLAYER_2) {
        switch(key){
            case player2Up:
                return UP;
            case player2Down:
                return DOWN;
            case player2Left:
                return LEFT;
            case player2Right:
                return RIGHT;
        }
    }
    return -1;
}

void spawnPlayers(){
    if(players == 2){
        //p2
        drawSquare(((SPAWN_2_X-1)*CELL_SIZE)+OFFSET_X, (SPAWN_2_Y*CELL_SIZE)+OFFSET_Y, CELL_SIZE, P2_COLOR);
        drawSquare((SPAWN_2_X*CELL_SIZE)+OFFSET_X, (SPAWN_2_Y*CELL_SIZE)+OFFSET_Y, CELL_SIZE, P2_COLOR);
    }
    //p1
    drawSquare((SPAWN_1_X*CELL_SIZE)+OFFSET_X, (SPAWN_1_Y*CELL_SIZE)+OFFSET_Y, CELL_SIZE, P1_COLOR);
    drawSquare(((SPAWN_1_X-1)*CELL_SIZE)+OFFSET_X, (SPAWN_1_Y*CELL_SIZE)+OFFSET_Y, CELL_SIZE, P1_COLOR);
    initSnakes();
    return;
}

void countDown() { //cuenta regresiva
    setSize(COUNTDOWN_FONT);
    printf_s("\t\t\t\t");
    for (int i = 3; i > 0; i--) {
        printf_s("%d", i);
        syscall_sleep(1000);
        putchar_s('\b');
    }
    printf_s("Go!\n"); // Mensaje al finalizar la cuenta regresiva
    resetSize();
}

// Inicializar las serpientes
void initSnakes() { 
    if(players == 2 ){
        snake2.x[snake2.length] = SPAWN_2_X-1;
        snake2.y[snake2.length++] = SPAWN_2_Y;
        snake2.x[snake2.length] = SPAWN_2_X;
        snake2.y[snake2.length++] = SPAWN_2_Y;
    }
    snake1.x[snake1.length] = SPAWN_1_X; 
    snake1.y[snake1.length++] = SPAWN_1_Y; 
    snake1.x[snake1.length] = SPAWN_1_X-1; 
    snake1.y[snake1.length++] = SPAWN_1_Y; 
}

// Añadir un nodo a la serpiente especificada
void grow(Snake* snake, int x, int y) { 
    if (snake->length < MAX_LENGTH) { 
        snake->x[snake->length] = x; 
        snake->y[snake->length] = y; 
        snake->length++; 
    }
}

void drawSnake(Snake* snake, int player) {
    for (int i = 0; i < snake->length; i++) {
        drawSquare(snake->x[i] * CELL_SIZE + OFFSET_X, 
                   snake->y[i] * CELL_SIZE + OFFSET_Y, 
                   CELL_SIZE, player == PLAYER_1? P1_COLOR:P2_COLOR);
    }
}

// Reiniciar las serpientes
void resetSnake(Snake* snake) { // Se pasa la serpiente como parámetro
    snake->dir =0;
    for(int i = 0 ; i < snake->length ; i++){
        snake->x[i] = 0;
        snake->y[i] = 0;
    }
    snake->length = 0; // Reiniciar la longitud de la serpiente
}
void resetGame(){
    resetSnake(&snake1);
    resetSnake(&snake2);
    for(int i = 0 ; i<ROWS ; i++){
        for (int j = 0; j < COLUMNS; j++){
            board[i][j] = EMPTY;
        }
    }
    fruit = 0;
}

// Mover la serpiente especificada en la dirección dada
void moveSnake(Snake* snake, int player) {
    // Próxima posición según la dirección de la serpiente
    int nextX = (snake->dir == LEFT) ? snake->x[0] - 1 : (snake->dir == RIGHT) ? snake->x[0] + 1 : snake->x[0];
    int nextY = (snake->dir == UP) ? snake->y[0] - 1 : (snake->dir == DOWN) ? snake->y[0] + 1 : snake->y[0];

    // Chequear colisiones
    if (nextY < 0 || nextY >= ROWS || nextX < 0 || nextX >= COLUMNS || board[nextY][nextX] == PLAYER_1 || board[nextY][nextX] == PLAYER_2  ) {
        collision(player);
        return; 
    }

    int g = 0; //veo si crece
    if(board[nextY][nextX] == FRUIT) {
        fruit = 0;
        board[nextY][nextX] = EMPTY; 
        g = 1; 
    }
    
    if (snake->length > 0) { // Verificar que la serpiente tenga segmentos
        if (!g) {
            drawSquare(snake->x[snake->length - 1] * CELL_SIZE + OFFSET_X, 
                       snake->y[snake->length - 1] * CELL_SIZE + OFFSET_Y, 
                       CELL_SIZE, 
                       ((snake->x[snake->length - 1] + snake->y[snake->length - 1]) % 2 == 0) ? B_COLOR1 : B_COLOR2); // Color de fondo alternativo
        } else {
            grow(snake, snake->x[snake->length], snake->y[snake->length]);
        }
        for (int i = snake->length-1; i > 0; i--) { // Muevo los segmentos de la serpiente
            if(i == snake->length-1){
                board[snake->y[i]][snake->x[i]] = EMPTY;
            }
            snake->x[i] = snake->x[i - 1]; 
            snake->y[i] = snake->y[i - 1];
            board[snake->y[i]][snake->x[i]] = player; 
        }

        // Actualizar la cabeza de la serpiente según la dirección
        snake->x[0] = nextX; 
        snake->y[0] = nextY; 
        board[snake->y[0]][snake->x[0]] = player;

        //redibujo la serpiente
        drawSnake(snake, player);
    }
}
void printBoard(){//version text-based
    for(int i = 0 ; i<ROWS ; i++){
        for (int j = 0; j < COLUMNS; j++)
        {
            printf_s("%d", board[i][j]);
        }
        printf_s("\n");
    }
    printf_s("\n");
}

void fruitControl(){
    int auxX; 
    int auxY; 
    do{
        auxX = getRandomInRange(0, COLUMNS-1);
        auxY = getRandomInRange(0, ROWS-1);
    }while(board[auxY][auxX] != 0);
    board[auxY][auxX] = FRUIT;
    drawSquare(auxX * CELL_SIZE + OFFSET_X, auxY* CELL_SIZE + OFFSET_Y, CELL_SIZE, FRUIT_COLOR);
    fruit = 1;
}

