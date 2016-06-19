#include "world.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

World* load_world(char* worldname) {
    char fullpath[20];
    strcpy(fullpath, "world/");
    strcat(fullpath, worldname);
    FILE* worldfile = fopen(fullpath, "r+");
    World* world;
    int success_count = fread(world, sizeof(World), 1, worldfile);
    if(success_count != 1) {
        quit_error("failed to read world\n");
    }
    fclose(worldfile);
    return world;
}

void save_world(World* world, char* worldname) {
    char fullpath[20];
    strcpy(fullpath, "world/");
    strcat(fullpath, worldname);
    FILE* worldfile = fopen(fullpath, "w");
    int success_count = fwrite(world, sizeof(world), 1, worldfile);
    if(success_count != 1) {
        quit_error("failed to save world\n");
    }
    int success_flush = fflush(worldfile);
    if(success_flush == -1) {
        quit_error("filed to flush world");
    }
    fclose(worldfile);
}
