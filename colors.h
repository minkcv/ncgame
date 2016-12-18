#include <ncurses.h>
#ifndef COLORS_H_
#define COLORS_H_

#define DESIRED_COLOR_PAIRS 10
#define NUM_COLOR_PAIRS 8
#define C_WHITE_BLACK 1
#define C_RED_BLACK 2
#define C_BLUE_BLACK 3
#define C_BLACK_YELLOW 4
#define C_WHITE_BLUE 5
#define C_GREEN_BLACK 6
#define C_BLACK_WHITE 7

void init_color_pairs();

void draw_available_colors(WINDOW* window, int y, int x);

#endif
