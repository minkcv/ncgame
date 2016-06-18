#include "windowstyle.h"
#include "chunk.h"
#include "colors.h"
#include "player.h"
#include "world.h"
#include "tiles.h"
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>

#define TOP_WINDOW_HEIGHT 20
#define TOP_WINDOW_WIDTH 80

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
    topw = newwin(TOP_WINDOW_HEIGHT, TOP_WINDOW_WIDTH, 0, 0);
    botw = newwin(botw_height, width, 20, 0);
    wrefresh(topw);
    wrefresh(botw);
    wclear(topw);
    acs_box(topw);
    wclear(botw);
    acs_box(botw);

    init_color_pairs();

    Player* player = create_player();

    World* world = create_world(2, 2);

    world->chunks[0][0]->tiles[5][5] = TILE_WALL;

    
    keypad(topw, TRUE);
    int c = wgetch(topw);   
    while( (char)c != 'q' ) {
        wclear(topw);
        acs_box(topw);
        draw_chunk(world->chunks[0][0], topw, 1, 1);
        draw_player(player, topw, player->y, player->x);
        c = wgetch(topw);
        if( c == KEY_LEFT  || c == KEY_RIGHT || c == KEY_UP || c == KEY_DOWN ) {
            move_player(player, world, world->chunks[player->chunk_y][player->chunk_x], c);
        }
        wrefresh(topw);
        wrefresh(botw);
        refresh();
    }
    
    delwin(topw);
    endwin();

    return 0;
}
