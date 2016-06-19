#ifndef WORLD_H_
#define WORLD_H_

#include "chunk.h"

typedef struct World {
    int height;
    int width;
    Chunk*** chunks;
} World;

World* create_world(int height, int width);

World* load_world(char* worldname);

void save_world(World* world, char* worldname);

#endif
