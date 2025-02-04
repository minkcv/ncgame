#ifndef PLAYER_H_
#define PLAYER_H_

#include "chunk.h"
#include "world.h"
#include <ncurses.h>

#define MODE_MENU 0
#define MODE_PLAY 1
#define MODE_EDIT 2

typedef struct Player {
    int x;
    int y;
    int chunk_x;
    int chunk_y;
    char character;
    short color_pair;
} Player;

Player* create_player();

bool can_move(short game_mode, char tile);

void draw_player(Player* player, WINDOW* window, int y, int x);

void move_player(short game_mode, Player* player, World* world, Chunk* chk, int key);

#endif
