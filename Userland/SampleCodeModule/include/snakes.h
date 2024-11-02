#ifndef SNAKES_H
#define SNAKES_H

#include <stdint.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 650

#define CELL_SIZE 65
#define ROWS (SCREEN_HEIGHT / CELL_SIZE)
#define COLUMNS (SCREEN_WIDTH / CELL_SIZE)

// Definiciones de constantes
#define EXIT 0
#define PLAY 1

#define MAX_LENGTH (COLUMNS * ROWS) // Tamaño máximo de la serpiente

#define START_MSG "SNAKES GAME\n PLAYERS: %d \n SPEED: %d \n\n Press: \n [ENTER] to begin game \n 's' for settings \n 'e' to exit \n"

// Prototipos de funciones para el juego de Snakes
void gameTick();
void animation();
int snakes();
int menu();
void finalScreen();
void gameLoop();
int onePlayer();
int twoPlayer();
void collision(int player);
int getMove();
void displayBackground();
void displayLayout();
void displayMenu();
void updateSnakes();
void pause();
int charToDir();
int startSnakes();
void spawnPlayers();
void countDown();
void handleInput(char key);
int getPlayerByKey(char key);
int inputToDir(char key, int player);
void fruitControl();


// Definición de la estructura Snake
typedef struct {
    int dir;
    int length;      // Longitud de la serpiente
    int x[MAX_LENGTH]; // Coordenadas X de los segmentos de la serpiente
    int y[MAX_LENGTH]; // Coordenadas Y de los segmentos de la serpiente
} Snake;

// Prototipos para la estructura de las serpientes
void initSnakes();
void grow(Snake* snake, int x, int y);
void iterateSnake(Snake* snake, int player);
void resetSnakes(Snake* snake);
void drawSnakePosition(Snake* snake, int player);
void moveSnake(Snake* snake, int player, int g);
#endif 
