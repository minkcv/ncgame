#include "util.h"
#include "ncurses.h"
#include <stdio.h>
#include <stdlib.h>

void quit_error(char* error) {
    endwin();
    printf("%s\n", error);
    exit(1);
}
