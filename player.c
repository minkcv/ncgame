#include "player.h"
#include <stdlib.h>

Player* create_player() {
    Player* p = malloc(sizeof(Player));
    p->x = 1;
    p->y = 1;
    p->character = '@';
    p->color_pair = 2;
    return p;
}

void draw_player(Player* player, WINDOW* window, int y, int x) {
    wattron(window, COLOR_PAIR(player->color_pair));
    mvwaddch(window, y, x, player->character);
    wattroff(window, COLOR_PAIR(player->color_pair));
}
