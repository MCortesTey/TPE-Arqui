#ifndef SNAKES_H
#define SNAKES_H

#include <stdint.h>

// Definiciones de constantes
#define PLAYER1 1
#define PLAYER2 2
#define EXIT 0

// Prototipos de funciones para el juego de Snakes
void animation();
int snakes();
int menu();
void gameLoop(int players);
int onePlayer();
int twoPlayer();
int collision();
int getMove();
void displayBackground();
void displayMenu();
void updateSnakes();
void pause();
int charToDir();
int startSnakes();

#endif // SNAKES_H
