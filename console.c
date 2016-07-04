#include "console.h"
#include "util.h"

void console_print(WINDOW* window, int y, int x, char* message) {
    mvwprintw(window, y, x, message);
    wrefresh(window);
}

void console_clear(WINDOW* window) {
    wclear(window);
    acs_box(window);
    wrefresh(window);
}
