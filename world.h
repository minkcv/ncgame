#ifndef WORLD_H_
#define WORLD_H_

#include "chunk.h"

#define WORLD_MAX_WIDTH 16
#define WORLD_MAX_HEIGHT 16

typedef struct World {
    int height;
    int width;
    Chunk chunks[WORLD_MAX_HEIGHT][WORLD_MAX_WIDTH];
} World;

World* create_world(int height, int width);

World* load_world(char* worldname);

void save_world(World* world, char* worldname);

#endif
