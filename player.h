#ifndef PLAYER_H_
#define PLAYER_H_

#include <ncurses.h>

typedef struct Player {
    int x;
    int y;
    char character;
    short color_pair;
} Player;

Player* create_player();

void draw_player(Player* player, WINDOW* window, int y, int x);

#endif
