

#ifndef BOARD_H_
#define BOARD_H_


#include "list.h"


/* Variables for tracking all live cells. */

struct list *cells;

/* Parameters of the board. */
int WIDTH;
int HEIGHT;

/* Functions for interacting the board. */

void printBoard(char *board, int x, int y);
void turn(char *board, int x, int y);
int setGlider(char *board, int x, int y, int posX, int posY);
void setShattle(char *board, int x, int y, int posX, int posY);
int setGun(char *board, int x, int y, int posX, int posY);
int setPoint(char *board, int x, int y, int setX, int setY);


#endif

