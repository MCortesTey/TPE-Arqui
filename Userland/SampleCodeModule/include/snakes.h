#ifndef SNAKES_H
#define SNAKES_H

#include <stdint.h>

// Definiciones de constantes
#define PLAYER1 1
#define PLAYER2 2
#define EXIT 0
#define PLAY 1

#define START_MSG "SNAKES GAME\n PLAYERS: %d \n SPEED: %d \n\n Press: \n [ENTER] to begin game \n 's' for settings \n 'e' to exit \n"

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
void displayLayout();
void displayMenu();
void updateSnakes();
void pause();
int charToDir();
int startSnakes();
void spawnPlayers();
void countDown();

#endif // SNAKES_H
