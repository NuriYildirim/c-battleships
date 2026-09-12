#include "board.h"
#include "render.h"

enum Alignment{
    HORIZONTAL,
    VERTICAL
};

void player_place(Board *player_board, int (*board_place)(Board*, int, int, int, Coordinates));