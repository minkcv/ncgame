#include "world.h"
#include <stdlib.h>

World* create_world(int height, int width) {
    World* w = malloc(sizeof(World));
    w->height = height;
    w->width = width;
    w->chunks = malloc(sizeof(Chunk*) * height);
    int i, j;
    for(i = 0; i < height; i++) {
        w->chunks[i] = malloc(sizeof(Chunk) * width);
        for(j = 0; j < width; j++) {
            w->chunks[i][j] = create_chunk();
        }
    }
    return w;
}
