#include <stdbool.h>

#ifndef BOARD_H
#define BOARD_H

#define BOARD_SIZE 10

#define CHAR_HIDDEN '?'
#define CHAR_IDLE   '#'
#define CHAR_SHIP   'S'
#define CHAR_HIT    'X'
#define CHAR_MISS   'O'
#define CHAR_SUNK   '*'

#define MAX_SHIPS 5
#define MAX_SHIP_LEN 5

typedef struct {
    int row;
    int col;
} Coordinates;

typedef struct {
    int length;
    Coordinates positions[MAX_SHIP_LEN];
    bool isAlive; 
} Ship;

typedef struct {
    char cells[BOARD_SIZE+1][BOARD_SIZE+1];
    int type; //0 Player 1 Enemy
    Ship ships[MAX_SHIPS];
    int ship_count;
} Board;

void board_init(Board *board, int board_type);

//board shipIndex length align pos
//returns 0, if placing fails, else returns 1.
int board_place(Board *board, int shipIndex, int length, int align, Coordinates pos);

int board_shoot(Board *board, Coordinates pos);

void board_destroy(Board *board);

#endif
