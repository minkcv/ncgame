#include "world.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

World* create_world(int height, int width) {
    World* w = malloc(sizeof(World));
    w->height = height;
    w->width = width;
    int i, j;
    for(i = 0; i < height; i++) {
        for(j = 0; j < width; j++) {
            init_chunk(&(w->chunks[i][j]));
        }
    }
    return w;
}

World* load_world(char* worldname) {
    char fullpath[20];
    strcpy(fullpath, "worlds/");
    strcat(fullpath, worldname);
    FILE* worldfile = fopen(fullpath, "r+");
    if(worldfile == NULL) {
        return NULL;
    }
    World* world = malloc(sizeof(World));
    size_t  success_count = fread(world, sizeof(World), 1, worldfile);
    if(success_count != 1) {
        quit_error("failed to read world\n");
    }
    fclose(worldfile);
    int i, j;
    for(i = 0; i < world->height; i++) {
        for(j = 0; j < world->width; j++) {
            reset_redraw(&(world->chunks[i][j]));
        }
    }
    return world;
}

void save_world(World* world, char* worldname) {
    char fullpath[20];
    strcpy(fullpath, "worlds/");
    strcat(fullpath, worldname);
    FILE* worldfile = fopen(fullpath, "w");
    int success_count = fwrite(world, sizeof(World), 1, worldfile);
    if(success_count != 1) {
        quit_error("failed to save world");
    }
    int success_flush = fflush(worldfile);
    if(success_flush == -1) {
        quit_error("failed to flush world");
    }
    fclose(worldfile);
}
