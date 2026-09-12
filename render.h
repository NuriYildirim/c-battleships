#include "board.h"

#ifndef RENDER_H

// ANSI COLORS
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define ASCII_CHAR_A 65

void draw_boards(Board *player, Board *enemy);

void draw_edit_mode(Board *player);

void draw(Board *player, Board *enemy);

#endif