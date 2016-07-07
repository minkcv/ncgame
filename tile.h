#ifndef TILE_H_
#define TILE_H_

typedef struct Tile {
    char ch;
    short color;
    bool redraw;
    bool seen;
    bool visible;
} Tile;

#endif
