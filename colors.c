#include "colors.h"
#include "console.h"
#include <ncurses.h>

void init_color_pairs() {
    init_pair(C_BLACK_WHITE, COLOR_BLACK, COLOR_WHITE);
    init_pair(C_WHITE_BLACK, COLOR_WHITE, COLOR_BLACK);
    init_pair(C_RED_BLACK, COLOR_RED, COLOR_BLACK);
    init_pair(C_BLUE_BLACK, COLOR_BLUE, COLOR_BLACK);
    init_pair(C_BLACK_YELLOW, COLOR_BLACK, COLOR_YELLOW);
    init_pair(C_WHITE_BLUE, COLOR_WHITE, COLOR_BLUE);
    init_pair(C_GREEN_BLACK, COLOR_GREEN, COLOR_BLACK);
}

void draw_available_colors(WINDOW* window, int y, int x) {
    for (short i = 1; i < NUM_COLOR_PAIRS; i++) {
        wattron(window,  COLOR_PAIR(i));
        mvwaddch(window, y, x + i, (char)(i + 48));
        wattroff(window, COLOR_PAIR(i));
    }
    wrefresh(window);
}
