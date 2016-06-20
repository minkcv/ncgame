#ifndef CONSOLE_H_
#define CONSOLE_H_

#include <ncurses.h>

void console_print(WINDOW* window, int y, int x, char* message);

void console_clear();

#endif
