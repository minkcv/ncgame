#ifndef CHUNK_H_
#define CHUNK_H_

#include <ncurses.h>

#define CHUNK_SIZE 8

typedef struct Chunk {
    char tiles[CHUNK_SIZE][CHUNK_SIZE];
    short color_pair[CHUNK_SIZE][CHUNK_SIZE];
} Chunk ;

Chunk* create_chunk();
void draw_chunk(Chunk* chk, WINDOW* window, int y, int x);

#endif
