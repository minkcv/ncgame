#include "chunk.h"
#include <string.h>
#include <stdlib.h>

Chunk* create_chunk() {
    Chunk* chk = malloc(sizeof(Chunk));
    int i, j;
    for(i = 0; i < CHUNK_SIZE; i++) {
        for(j = 0; j < CHUNK_SIZE; j++) {
            chk->tiles[i][j] = '.';
            chk->color_pair[i][j] = 1;
        }
    }
    return chk;
}

void draw_chunk(Chunk* chk, WINDOW* window, int y, int x) {
    int i, j;
    for(i = 0; i < CHUNK_SIZE; i++) {
        for(j = 0; j < CHUNK_SIZE; j++) {
            wattron(window, COLOR_PAIR(chk->color_pair[i][j]));
            mvwaddch(window, y + i, x + j, chk->tiles[i][j]);
            wattroff(window, COLOR_PAIR(chk->color_pair[i][j]));
        }
    }
}
