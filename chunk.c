#include "chunk.h"
#include "tile.h"
#include "colors.h"
#include <string.h>
#include <stdlib.h>

void init_chunk(Chunk* chk) {
    int i, j;
    for(i = 0; i < CHUNK_HEIGHT; i++) {
        for(j = 0; j < CHUNK_WIDTH; j++) {
            chk->tiles[i][j].ch = ' ';
            chk->tiles[i][j].color = C_WHITE_BLACK;
            chk->tiles[i][j].redraw = TRUE;
            chk->tiles[i][j].visible = FALSE;
        }
    }
}

void reset_redraw(Chunk* chk) {
    int i, j;
    for(i = 0; i < CHUNK_HEIGHT; i++) {
        for(j = 0; j < CHUNK_WIDTH; j++) {
            chk->tiles[i][j].redraw = TRUE;
        }
    }
}


void draw_chunk(Chunk* chk, WINDOW* window, int y, int x) {
    int i, j;
    for(i = 0; i < CHUNK_HEIGHT; i++) {
        for(j = 0; j < CHUNK_WIDTH; j++) {
            if(chk->tiles[i][j].redraw) {
                wattron(window, COLOR_PAIR(chk->tiles[i][j].color));
                mvwaddch(window, y + i, x + j, chk->tiles[i][j].ch);
                wattroff(window, COLOR_PAIR(chk->tiles[i][j].color));
                chk->tiles[i][j].redraw = FALSE;
            }
        }
    }
}
