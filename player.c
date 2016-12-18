#include "player.h"
#include "colors.h"
#include "tiles.h"
#include <stdlib.h>

Player* create_player() {
    Player* p = malloc(sizeof(Player));
    p->x = 0;
    p->y = 0;
    p->chunk_x = 0;
    p->chunk_y = 0;
    p->character = '@';
    p->color_pair = C_WHITE_BLACK;
    return p;
}

bool can_move(char tile) {
    if(game_mode == MODE_MENU)
        return FALSE;
    else if(game_mode == MODE_EDIT)
        return TRUE;

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
    if(key == KEY_LEFT || key == 'h') {
        if(player->x > 0) {
            if(can_move(chk->tiles[player->y][player->x - 1].ch)) {
                chk->tiles[player->y][player->x].redraw = TRUE;
                player->x--;
            }
        }
        else if(player->chunk_x > 0) {
            reset_redraw(chk);
            player->chunk_x--;
            player->x = CHUNK_WIDTH - 1;
        }
    }
    else if (key == KEY_RIGHT || key == 'l') {
        if(player->x < CHUNK_WIDTH - 1) {
            if(can_move(chk->tiles[player->y][player->x + 1].ch)) {
                chk->tiles[player->y][player->x].redraw = TRUE;
                player->x++;
            }
        }
        else if(player->chunk_x < world->width - 1) {
            reset_redraw(chk);
            player->chunk_x++;
            player->x = 0;
        }
    }
    else if (key == KEY_UP || key == 'k') {
        if(player->y > 0) {
            if(can_move(chk->tiles[player->y - 1][player->x].ch)) {
                chk->tiles[player->y][player->x].redraw = TRUE;
                player->y--;
            }
        }
        else if(player->chunk_y > 0) {
            reset_redraw(chk);
            player->chunk_y--;
            player->y = CHUNK_HEIGHT - 1;
        }
    }
    else if (key == KEY_DOWN || key == 'j') {
        if(player->y < CHUNK_HEIGHT - 1) {
            if(can_move(chk->tiles[player->y + 1][player->x].ch)) {
                chk->tiles[player->y][player->x].redraw = TRUE;
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
