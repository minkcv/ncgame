#include "colors.h"
#include <ncurses.h>

void init_color_pairs() {
    init_pair(C_WHITE_BLACK, COLOR_WHITE, COLOR_BLACK);
    init_pair(C_RED_BLACK, COLOR_RED, COLOR_BLACK);
    init_pair(C_BLUE_BLACK, COLOR_BLUE, COLOR_BLACK);
    init_pair(C_YELLOW_BLACK, COLOR_YELLOW, COLOR_BLACK);
}
