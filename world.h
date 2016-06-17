#ifndef WORLD_H_
#define WORLD_H_

#include "chunk.h"

typedef struct World {
    int height;
    int width;
    Chunk*** chunks;
} World;

World* create_world(int height, int width);

#endif
