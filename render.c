#include "render.h"
#include <stdio.h>

//Zeigt das Board
static void draw_board_row(Board *board, int row) {
    for (int col = 0; col <= BOARD_SIZE; col++) {
        //Board Beschriftungen
        if (row == 0) {
            if (col == 0)
                printf("   ");
            else
                printf("%c  ", ASCII_CHAR_A + col - 1);
        }
        else if (col == 0) {
            printf("%d  ", row - 1);
        }
        // -------
        else {
            char cell = board->cells[row-1][col-1];
            switch (cell)
            {
            case CHAR_SHIP:
                board->type == 0 ? printf(ANSI_COLOR_RESET "%c  ", cell):printf(ANSI_COLOR_BLUE "%c  " ANSI_COLOR_RESET, CHAR_HIDDEN);
                break;
            case CHAR_IDLE:
                printf(ANSI_COLOR_BLUE "%c  " ANSI_COLOR_RESET, cell);
                break;
            case CHAR_HIDDEN:
                printf(ANSI_COLOR_BLUE "%c  " ANSI_COLOR_RESET, cell);
                break;
            case CHAR_HIT:
                printf(ANSI_COLOR_RED "%c  " ANSI_COLOR_RESET, cell);
                break;
            case CHAR_SUNK:
                printf(ANSI_COLOR_RED "%c  " ANSI_COLOR_RESET, cell);
                break;
            default:
                printf(ANSI_COLOR_RESET"%c  ", cell);
                break;
            }
        }
    }
}

void draw_edit_mode(Board *player) {
    printf("\n   --- SCHIFFE PLATZIEREN ---\n");
    for (int row = 0; row <= BOARD_SIZE; row++) {
        draw_board_row(player, row);
        printf("\n");
    }
    printf("\n");
}

void draw(Board *player, Board *enemy) {
    printf("\n        --- PLAYER ---            |         --- ENEMY ---\n");
    for(int i = 0; i < MAX_SHIPS; i++) {
        for(int j = 0; j < player->ships[i].length; j++) {
            printf("%c ", player->ships[i].isAlive ? CHAR_SHIP:CHAR_SUNK);
            if(j == player->ships[i].length-1) {
                for(int k = j; k < MAX_SHIPS; k++) {
                    printf("  ");
                }
            }
        }
        for(int j = 0; j < enemy->ships[i].length; j++) {
            if(j == 0) {
                printf("                      | %c ", enemy->ships[i].isAlive ? CHAR_SHIP:CHAR_SUNK);
            }
            else {
                printf("%c ", enemy->ships[i].isAlive ? CHAR_SHIP:CHAR_SUNK);
            }
        }
        printf("\n");
    }
    printf("\n");
    
    for (int row = 0; row <= BOARD_SIZE; row++) {
        draw_board_row(player, row);
        printf(" | ");   // Trennzeichen zwischen Boards
        draw_board_row(enemy, row);
        printf("\n");
    }
    printf("\n");
}
