#include "windowstyle.h"

void acs_box(WINDOW* win) {
    //wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
    wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
    wrefresh(win);
}
