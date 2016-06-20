#include "util.h"
#include "ncurses.h"
#include <stdio.h>
#include <stdlib.h>

void quit_error(char* error) {
    endwin();
    printf("%s\n", error);
    exit(1);
}

void acs_box(WINDOW* win) {
    //wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
    wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
    wrefresh(win);
}
