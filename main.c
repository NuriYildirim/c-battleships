#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "render.h"
#include "enemy.h"
#include "player.h"
#include "board.h"
#include <ctype.h>
/*

https://docs.google.com/document/d/12JWeaG514pYKKcofsavNg5KBA0dc-jX6XZknvVqBXPc/edit?usp=sharing

Spielvorbereitung
Schiffe platzieren (vorerst default Spielfelder in nem Array hard-codieren)

Spiel:
- Tracken, wer dran ist (P1 beginnt)
- Koordinate eingeben
- Gültige Eingabe?, wenn nicht, neu prompten
- Cell State an der Koordinate checken ("ist da ein Schiff oder nicht?")
- Nachricht im Terminal ausgeben (Hit, Miss, Sunk)
- Spielbrett aktualisieren

- Tracken, wer dran ist (Jetzt COM)
- COM: Zufälliges Feld wählen
- Gültige Eingabe?, wenn nicht, neu prompten
- Cell State an der Koordinate checken ("ist da ein Schiff oder nicht?")
- Nachricht im Terminal ausgeben (Miss 0, Hit 1, Sunk 2)
- Spielbrett aktualisieren

- etc ...
*/

enum GAME_STATES {
    SETUP,
    PLAYER_TURN,
    COM_TURN,
    PLAYER_WON,
    ENEMY_WON
};

int main (void){

    srand(time(NULL));

    Board player, enemy;
    board_init(&player, 0);
    board_init(&enemy, 1);

    Stack shot_stack;
    initializeEnemy(&shot_stack);  

    int game_state = SETUP;

    int player_ships_left = MAX_SHIPS;
    int enemy_ships_left = MAX_SHIPS;

    while(1) { // Game clock, verschieden States je nach spiel situation. Break -> nächsten gamestate. continue -> bleibt.
        
        switch (game_state)
        {
        case SETUP:
            //draw(&player, &enemy);
            // board_place(&player, 0, 5, 0, (Coordinates){1,1});
            // board_place(&player, 1, 4, 1, (Coordinates){5,1});
            // board_place(&player, 2, 3, 0, (Coordinates){4,4});
            // board_place(&player, 3, 3, 0, (Coordinates){6,4});
            // board_place(&player, 4, 2, 1, (Coordinates){8,8});
            player_place(&player, board_place);
            enemy_place(&enemy, board_place);

            draw(&player, &enemy);
            game_state = PLAYER_TURN;
            break;
        case PLAYER_TURN:
            int r, c;
            char l;

            //Erwartet col und row z.b. A 3
            do{
                scanf(" %c %d", &l, &r);
                l = toupper(l);
                // CHEAT CODES
                if ((l == 'X') && (r == 13)){
                    
                    board_destroy(&enemy);
                    enemy_ships_left = 0;
                    printf("destroying enemy...\n");
                    break;
                }
                else if ((l == 'Y') && (r == 670)){
                    board_destroy(&player);
                    player_ships_left = 0;
                    printf("destroying player...\n");
                    game_state = COM_TURN;
                    l = 'A';
                    r = '0';
                    break;
                }
            } while ((l < 'A' || l > 'J') || (r < 0 || r > 9));
            c = (l - ASCII_CHAR_A); // Wandelt buchstabe in coloum number um
            Coordinates pos = {r,c};
            int shot_state = board_shoot(&enemy, pos);
            printf("%d \n", shot_state);
            if (enemy_ships_left <= 0){
                draw(&player, &enemy);
                printf("\nDu hast gewonnen!!\n");
                game_state = PLAYER_WON;
                return 0;
            }
            else if(shot_state == ALREADY_SHOT) { // Falls feld bereits getroffen
                printf("DU: Feld wurde bereits beschossen. Versuche erneut!\n");
                
                game_state = PLAYER_TURN;
                continue;
            }
            else if(shot_state == HIT) { // Nochmal schießen
                draw(&player, &enemy);
                game_state = PLAYER_TURN;
                printf("DU: TREFFER! Schieß nochmal.\n");
                continue;
            } // wenn 1 dann hit, wenn 0 dann miss
            
            else if(shot_state == MISS){ // Jetzt ist COM dran
                draw(&player, &enemy);
                game_state = COM_TURN;
                printf("DU: Verfehlt.\n");
                break;
            }
            else if(shot_state == SUNK){ // Spieler schießt weiter
                draw(&player, &enemy);
                game_state = PLAYER_TURN;
                printf("DU: ** SCHIFF VERSUNKEN ** (Schieß nochmal)\n");
                enemy_ships_left--;
                
                
            
                continue;
            }
            
            break;
        case COM_TURN:
        //  wenn 1 dann hit, wenn 2 dann neu versuchen, wenn 0 dann miss
            
            int result = enemy_shoot(&player, &shot_stack, board_shoot);
            if (result == SUNK) player_ships_left--;
            draw(&player, &enemy);
            if (player_ships_left <= 0){
                printf("\nCOM hat gewonnen :(\n");
                game_state = ENEMY_WON;
                return 0;
            }
            game_state = PLAYER_TURN;
        }
    }
    return 0;
}


