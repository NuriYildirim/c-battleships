#include <stdio.h>
#include <string.h>
#include "board.h"
#include "enemy.h"



void board_init(Board *board, int board_type) {

    board->type = board_type;

    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            // Zellendarstellung für P1: '#', COM: '?'
            board->cells[row][col] = board_type == 0 ? CHAR_IDLE:CHAR_HIDDEN;
        }
    }
}

int board_place(Board *board, int shipIndex, int length, int align, Coordinates pos) {

    // Boundary check
    if (align) { // vertikal
        if (pos.row + length > BOARD_SIZE) 
        return 0;
    } else {     // horizontal
        if (pos.col + length > BOARD_SIZE) 
        return 0;
    }

    Ship *s = &board->ships[shipIndex];
    s->length = length;
    s->isAlive = true;

    for (int i = 0; i < length; i++) {
        Coordinates nextPos = { pos.row + i * align, pos.col + i * !align};

        // Overlap check
        for (int k = 0; k < shipIndex; k++) {
            Ship *other = &board->ships[k];

            for (int j = 0; j < other->length; j++) {
                if ((nextPos.row == other->positions[j].row && nextPos.col == other->positions[j].col)      ||      
                    (nextPos.row == other->positions[j].row + 1 && nextPos.col == other->positions[j].col)  ||  // unten 
                    (nextPos.row == other->positions[j].row  && nextPos.col == other->positions[j].col + 1) || // rechts
                    (nextPos.row == other->positions[j].row - 1 && nextPos.col == other->positions[j].col)  ||  // oben
                    (nextPos.row == other->positions[j].row && nextPos.col == other->positions[j].col - 1)) {  // links
                    return 0; // Überlappt
                }
            }
        }
        s->positions[i] = nextPos;
    }

    // Board setzen
    for (int i = 0; i < s->length; i++) {
        board->cells[s->positions[i].row][s->positions[i].col] = CHAR_SHIP;
    }

    board->ship_count = shipIndex + 1;

    // printf("%d", board->ship_count);

    return 1;
    // 1 == geklappt, 0 == nicht geklappt
}

int board_shoot(Board *board, Coordinates coords) {

    char *cell = &board->cells[coords.row][coords.col];

    // Schon beschossen
    if (*cell == CHAR_HIT || *cell == CHAR_MISS)
        return ALREADY_SHOT; 
    
    // Alle Schiffe durchgehen
    for (int i = 0; i < board->ship_count; i++) {
        Ship *s = &board->ships[i];

        // Prüfen, ob dieser Schuss dieses Schiff trifft
        for (int j = 0; j < s->length; j++) {
            if (s->positions[j].row == coords.row &&
                s->positions[j].col == coords.col) {
                
                if (!s->isAlive) return ALREADY_SHOT;
                
                // Treffer markieren
                *cell = CHAR_HIT;

                // Prüfen, ob Schiff jetzt komplett getroffen ist
                int hits = 0;
                for (int k = 0; k < s->length; k++) {
                    if (board->cells[s->positions[k].row][s->positions[k].col] == CHAR_HIT) {
                        hits++;
                    }
                }
                if (hits == s->length) {
                    s->isAlive = false;

                    for (int i = 0; i < s->length; i++) {
                        int r = s->positions[i].row;
                        int c = s->positions[i].col;

                        if (r + 1 < BOARD_SIZE)
                            board->cells[r + 1][c] = CHAR_SUNK;
                        if (r - 1 >= 0)
                            board->cells[r - 1][c] = CHAR_SUNK;
                        if (c + 1 < BOARD_SIZE)
                            board->cells[r][c + 1] = CHAR_SUNK;
                        if (c - 1 >= 0)
                            board->cells[r][c - 1] = CHAR_SUNK;
                    }

                    return SUNK;
                }
                return HIT;
            }
        }
    }

    // Kein Schiff getroffen
    *cell = CHAR_MISS;
    return MISS;
}

void board_destroy(Board *board){
    
    for (int i = 0; i < MAX_SHIPS; i++){
        Ship *s = &board->ships[i];
        // printf("%d isAlive(%d)\n", i, (int)s->isAlive);
        for (int j = 0; j < s->length; j++){
            int r = s->positions[j].row;
            int c = s->positions[j].col;
            // printf("ship %d at %d %d\n", i, r, c);
            Coordinates pos = {r,c};
            board_shoot(board, pos);
        }   
    }
}

