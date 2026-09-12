#include "board.h"
#include "enemy.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "render.h"


void initializeEnemy(Stack *shot_stack) {
    shot_stack->top = -1;  
}

// Function to check if the stack is empty
bool isEmpty(Stack *stack) {
    return stack->top == -1;  
}

// Function to check if the stack is full
bool isFull(Stack *stack) {
    return stack->top == MAX_STACK_SIZE - 1;  
}

void push(Stack *stack, Shot value) {
    if (isFull(stack)) return;
    stack->arr[++stack->top] = value;
}

Shot pop(Stack *stack) {
    if (isEmpty(stack))
        return (Shot){{-1,-1}, DIR_NONE};

    return stack->arr[stack->top--];
}

void clear(Stack *stack) {
    if (!stack) return;
    stack->top = -1;
}

bool hasIdleNeighbour(Board *board, Coordinates pos) {

    if (board->cells[pos.row][pos.col] == CHAR_SUNK)
        return false;

    if (pos.row > 0 &&
        board->cells[pos.row - 1][pos.col] == CHAR_IDLE)
        return true;

    if (pos.row < BOARD_SIZE - 1 &&
        board->cells[pos.row + 1][pos.col] == CHAR_IDLE)
        return true;

    if (pos.col > 0 &&
        board->cells[pos.row][pos.col - 1] == CHAR_IDLE)
        return true;

    if (pos.col < BOARD_SIZE - 1 &&
        board->cells[pos.row][pos.col + 1] == CHAR_IDLE)
        return true;
    return false;
}


void enemy_place(Board *board, int (*board_place)(Board*, int, int, int, Coordinates)) {
    int place_state = 0;
    srand(1);
    while (place_state < 5)
    {
        int rand_row = rand() % BOARD_SIZE;
        int rand_col = rand() % BOARD_SIZE;
        int align = rand() % 2;
        switch (place_state)
            {
            case 0:
                place_state += board_place(board, place_state, 5, align, (Coordinates){rand_row, rand_col});
                break;
            case 1:
                place_state += board_place(board, place_state, 4, align, (Coordinates){rand_row, rand_col});
                break;
            case 2:
                place_state += board_place(board, place_state, 3, align, (Coordinates){rand_row, rand_col});
                break;
            case 3:
                place_state += board_place(board, place_state, 3, align, (Coordinates){rand_row, rand_col});
                break;
            case 4:
                place_state += board_place(board, place_state, 2, align, (Coordinates){rand_row, rand_col});
                break;
            }
    }
}

void push_neighbors(Stack *stack, Coordinates p) {
    if (p.row > 0)
        push(stack, (Shot){{p.row - 1, p.col}, DIR_UP});
    if (p.row < BOARD_SIZE - 1)
        push(stack, (Shot){{p.row + 1, p.col}, DIR_DOWN});
    if (p.col > 0)
        push(stack, (Shot){{p.row, p.col - 1}, DIR_LEFT});
    if (p.col < BOARD_SIZE - 1)
        push(stack, (Shot){{p.row, p.col + 1}, DIR_RIGHT});
}

Shot next_in_direction(Shot s) {
    Coordinates p = s.pos;

    switch (s.dir) {
        case DIR_UP:
            p.row--;
            break;
        case DIR_DOWN:
            p.row++;
            break;
        case DIR_LEFT:
            p.col--;
            break;
        case DIR_RIGHT:
            p.col++; 
            break;
        default: 
            break;
    }

    return (Shot){p, s.dir};
}

int enemy_shoot(Board *board, Stack *shot_stack,int (*board_shoot)(Board*, Coordinates)) {

    int shot_state = -1;

    do {
        Shot shot;

        if (!isEmpty(shot_stack)) {
            shot = pop(shot_stack);
        } else {
            do {
                shot.pos.row = rand() % BOARD_SIZE;
                shot.pos.col = rand() % BOARD_SIZE;
                shot.dir = DIR_NONE;
            } while(!hasIdleNeighbour(board, shot.pos));
        }

        // FIXME: board_shoot unterscheidet nicht zwischen einem lebendem und einem totem Schiff 
        shot_state = board_shoot(board, shot.pos);

        if(shot_state == MISS) {
            printf("COM: Verfehlt.\n");
            break;
        }

        if (shot_state == ALREADY_SHOT) {
            continue;
        }

        if(shot_state == SUNK) {
            printf("COM: TREFFER bei %c%d\n", ASCII_CHAR_A + shot.pos.col, shot.pos.row);
            printf("COM: ** SCHIFF VERSUNKEN **\n");
            clear(shot_stack);
            continue;
        }

        if (shot_state == HIT) {
            printf("COM: TREFFER bei %c%d\n", ASCII_CHAR_A + shot.pos.col, shot.pos.row);

            if (shot.dir == DIR_NONE) {
                push_neighbors(shot_stack, shot.pos);
            } else {
                Shot next = next_in_direction(shot);

                if (next.pos.row >= 0 && next.pos.row < BOARD_SIZE &&
                    next.pos.col >= 0 && next.pos.col < BOARD_SIZE) {
                    push(shot_stack, next);
                }
            }
        }

    } while (shot_state == 1 || shot_state == 2);

    return shot_state;
}