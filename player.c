#include "player.h"
#include "tiles.h"
#include <stdlib.h>

Player* create_player() {
    Player* p = malloc(sizeof(Player));
    p->x = 0;
    p->y = 0;
    p->chunk_x = 0;
    p->chunk_y = 0;
    p->character = '@';
    p->color_pair = 2;
    return p;
}

bool can_move(char tile) {
   switch(tile) {
       case TILE_WATER :
       case TILE_WALL :
           return FALSE;
           break;
       default:
           return TRUE;
   }
}

void draw_player(Player* player, WINDOW* window, int y, int x) {
    wattron(window, COLOR_PAIR(player->color_pair));
    mvwaddch(window, y + 1, x + 1, player->character);
    wattroff(window, COLOR_PAIR(player->color_pair));
}

void move_player(Player* player, World* world, Chunk* chk, int key) {
    if(key == KEY_LEFT) {
        if(player->x > 0) {
            if(can_move(chk->tiles[player->y][player->x - 1])) {
                chk->redraw_tile[player->y][player->x] = TRUE;
                player->x--;
            }
        }
        else if(player->chunk_x > 0) {
            reset_redraw(chk);
            player->chunk_x--;
            player->x = CHUNK_WIDTH - 1;
        }
    }
    else if (key == KEY_RIGHT) {
        if(player->x < CHUNK_WIDTH - 1) {
            if(can_move(chk->tiles[player->y][player->x + 1])) {
                chk->redraw_tile[player->y][player->x] = TRUE;
                player->x++;
            }
        }
        else if(player->chunk_x < world->width - 1) {
            reset_redraw(chk);
            player->chunk_x++;
            player->x = 0;
        }
    }
    else if (key == KEY_UP) {
        if(player->y > 0) {
            if(can_move(chk->tiles[player->y - 1][player->x])) {
                chk->redraw_tile[player->y][player->x] = TRUE;
                player->y--;
            }
        }
        else if(player->chunk_y > 0) {
            reset_redraw(chk);
            player->chunk_y--;
            player->y = CHUNK_HEIGHT - 1;
        }
    }
    else if (key == KEY_DOWN) {
        if(player->y < CHUNK_HEIGHT - 1) {
            if(can_move(chk->tiles[player->y + 1][player->x])) {
                chk->redraw_tile[player->y][player->x] = TRUE;
                player->y++;
            }
        }
        else if(player->chunk_y < world->height - 1) {
            reset_redraw(chk);
            player->chunk_y++;
            player->y = 0;
        }
    }
}
