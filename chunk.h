#ifndef CHUNK_H_
#define CHUNK_H_

#include <ncurses.h>
#include "tile.h"

#define CHUNK_WIDTH 78
#define CHUNK_HEIGHT 18

typedef struct Chunk {
    Tile tiles[CHUNK_HEIGHT][CHUNK_WIDTH];
} Chunk ;

void init_chunk(Chunk* chk);
void reset_redraw(Chunk* chk);
void draw_chunk(Chunk* chk, WINDOW* window, int y, int x);

#endif
