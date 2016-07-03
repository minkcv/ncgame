#include "chunk.h"
#include "colors.h"
#include <string.h>
#include <stdlib.h>

void init_chunk(Chunk* chk) {
    int i, j;
    for(i = 0; i < CHUNK_HEIGHT; i++) {
        for(j = 0; j < CHUNK_WIDTH; j++) {
            chk->tiles[i][j] = ' ';
            chk->color_pair[i][j] = C_WHITE_BLACK;
            chk->redraw_tile[i][j] = TRUE;
        }
    }
}

void reset_redraw(Chunk* chk) {
    int i, j;
    for(i = 0; i < CHUNK_HEIGHT; i++) {
        for(j = 0; j < CHUNK_WIDTH; j++) {
            chk->redraw_tile[i][j] = TRUE;
        }
    }
}


void draw_chunk(Chunk* chk, WINDOW* window, int y, int x) {
    int i, j;
    for(i = 0; i < CHUNK_HEIGHT; i++) {
        for(j = 0; j < CHUNK_WIDTH; j++) {
            if(chk->redraw_tile[i][j]) {
                wattron(window, COLOR_PAIR(chk->color_pair[i][j]));
                mvwaddch(window, y + i, x + j, chk->tiles[i][j]);
                wattroff(window, COLOR_PAIR(chk->color_pair[i][j]));
                chk->redraw_tile[i][j] = FALSE;
            }
        }
    }
}
