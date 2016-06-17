#include "windowstyle.h"
#include "chunk.h"
#include "colors.h"
#include "player.h"
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>

void init_ncurses() {
    initscr();
    start_color();
    noecho();
    raw();
    curs_set(FALSE);
}

void quit_error(char* err) {
    endwin();
    printf("%s\n", err);
    exit(1);
}

int main(int argc, char* argv[]) {
    int x = 1;
    int y = 1;
    int width = 80;
    int height = 24;
    int term_width = 0;
    int term_height = 0;
    int topw_height = 20;
    int botw_height = 4;
    WINDOW* topw;
    WINDOW* botw;
    init_ncurses();
    getmaxyx(stdscr, term_height, term_width);

    // check terminal capabilities
    if(term_width < width || term_height < height) {
        quit_error("Your terminal size is below the required 80x24");
    }
    if(has_colors() == FALSE) {
        quit_error("Your terminal does not support colors");
    }
    if(COLOR_PAIRS < DESIRED_COLOR_PAIRS) {
        char err[30];
        sprintf(err, "Your terminal supports less than %d color pairs", DESIRED_COLOR_PAIRS);
        quit_error(err);
    }
    printf("%d\n", COLOR_PAIRS);

    refresh();
    topw = newwin(topw_height, width, 0, 0);
    botw = newwin(botw_height, width, 20, 0);
    wrefresh(topw);
    wrefresh(botw);
    wclear(topw);
    acs_box(topw);
    wclear(botw);
    acs_box(botw);

    init_color_pairs();

    Chunk* chunk1 = create_chunk();
    chunk1->tiles[0][0] = 'B';
    chunk1->color_pair[0][0] = C_WHITE_BLACK;

    Player* player = create_player();

    
    keypad(topw, TRUE);
    int c = wgetch(topw);   
    while( (char)c != 'q' ) {
        wclear(topw);
        acs_box(topw);
        draw_chunk(chunk1, topw, 1, 1);
        draw_player(player, topw, player->y, player->x);
        c = wgetch(topw);
        if( c == KEY_LEFT ) {
            player->x--;
        }
        else if( c == KEY_RIGHT ) {
            player->x++;
        }
        else if( c == KEY_UP ) {
            player->y--;
        }
        else if( c == KEY_DOWN ) {
            player->y++;
        }
        wrefresh(topw);
        wrefresh(botw);
        refresh();
    }
    
    delwin(topw);
    endwin();

    return 0;
}
