#include "player.h"

#include <stdio.h>
#include <ctype.h>

void player_place(Board *player_board, int (*board_place)(Board*, int, int, int, Coordinates)){
    
    int row, col;
    char letter, align;
    int ship_lengths[MAX_SHIPS]  = {5,4,3,3,2};
    char * ship_names[MAX_SHIPS] = {"Schlachtschiff", "Kreuzer", "Fregatte", "U-Boot", "Zerstörer"};
    
    for (int i = 0; i < MAX_SHIPS; i++){
        do{
            draw_edit_mode(player_board);
            printf("Platziere %s (Länge %d) <Spalte(A-Z)<Zeile(0-9)><Ausrichtung(v,h)>:\n", ship_names[i], ship_lengths[i]);
            scanf(" %c %d %c", &letter, &row, &align);
            letter = toupper(letter);
            align = toupper(align);

        } while ((row < 0)      || (row > 9) ||  // TODO: Kleinbuchstaben handeln?
                 (letter < 'A') || (letter > 'J') ||
                 ((align != 'V') && (align != 'H')));
        col = (letter - ASCII_CHAR_A); // Wandelt buchstabe in coloum number um
        Coordinates pos = {row,col};
        // ausrichtung in int umwandeln
        int align_int;
        switch (align) {
        case 'V': align_int = VERTICAL; break;
        case 'H': align_int = HORIZONTAL; break;
        default: return;
        }
        int result = board_place(player_board, i, ship_lengths[i], align_int, pos);
        if (result == 0){
            printf("Fehler: Schiff überlappt/grenzt anderem\n");
            i--; 
        }
        else{
            printf("Koordinaten gespeichert...\n");
        }
    }
}