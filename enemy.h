#include "board.h"
#include <stdbool.h>

#define MAX_STACK_SIZE 4

typedef enum {
    DIR_NONE,
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
} Direction;

enum SHOT_RESULTS {
    MISS,
    HIT,
    ALREADY_SHOT,
    SUNK
};

typedef struct {
    Coordinates pos;
    Direction dir;
} Shot;

typedef struct {
    Shot arr[MAX_STACK_SIZE];
    int top;
} Stack;

void initializeEnemy(Stack *shot_stack);

bool isEmpty(Stack *stack);

bool isFull(Stack *stack);

bool hasIdleNeighbour(Board *board, Coordinates pos);

Shot pop(Stack *stack);

void clear(Stack *stack);

void push_neighbors(Stack *stack, Coordinates p);

void enemy_place(Board *board, int (*board_place)(Board*, int, int, int, Coordinates));

int enemy_shoot(Board *board, Stack *shot_stack, int (*board_shoot)(Board*, Coordinates));