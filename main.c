#include "windowstyle.h"
#include "chunk.h"
#include "colors.h"
#include "player.h"
#include "world.h"
#include "tiles.h"
#include <locale.h>
#include <stdio.h>
#include <ctype.h>
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
    bool redraw_botw = TRUE;
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

    refresh();
    topw = newwin(TOP_WINDOW_HEIGHT, TOP_WINDOW_WIDTH, 0, 0);
    botw = newwin(botw_height, width, 20, 0);
    wclear(topw);
    acs_box(topw);
    wrefresh(topw);

    wclear(botw);
    acs_box(botw);
    mvwprintw(botw, 1, 1, " tab: change play/edit mode");
    wrefresh(botw);

    init_color_pairs();

    Player* player = create_player();

    World* world = create_world(2, 2);

    world->chunks[0][0]->tiles[5][5] = TILE_WALL;

    game_mode = MODE_PLAY;
    
    keypad(topw, TRUE);
    int c = wgetch(topw);   
    while( (char)c != 'q' ) {
        draw_chunk(world->chunks[player->chunk_y][player->chunk_x], topw, 1, 1);
        draw_player(player, topw, player->y, player->x);
        c = wgetch(topw);
        if(redraw_botw) {
            wclear(botw);
            acs_box(botw);
            wrefresh(botw);
            redraw_botw = FALSE;
        }

        if( c == '\t' ) {
            if( game_mode == MODE_PLAY ) {
                game_mode = MODE_EDIT;
            }
            else if( game_mode == MODE_EDIT ) {
                game_mode = MODE_PLAY;
            }
        }

        if( game_mode == MODE_EDIT ) {
            wclear(botw);
            acs_box(botw);
            mvwprintw(botw, 1, 1, " c: change color e: change character");
            wrefresh(botw);
            redraw_botw = TRUE;
            if( c == 'e' ) {
                wclear(botw);
                acs_box(botw);
                mvwprintw(botw, 1, 1, " input a character");
                wrefresh(botw);
                int newtile = wgetch(topw);
                if( isprint( newtile )) {
                    world->chunks[player->chunk_y][player->chunk_x]->tiles[player->y][player->x] = newtile;
                    wclear(botw);
                    acs_box(botw);
                    wrefresh(botw);
                }
                else {
                    wclear(botw);
                    acs_box(botw);
                    mvwprintw(botw, 1, 1, " not a printable character");
                    wrefresh(botw);
                }
                redraw_botw = TRUE;
            }
            else if( c == 'c' ) {
                wclear(botw);
                acs_box(botw);
                mvwprintw(botw, 1, 1, " input a number between 1 and the number of defined colors");
                wrefresh(botw);
                int newcolor = wgetch(topw) - '0';
                if( newcolor > 0 && newcolor <= NUM_COLOR_PAIRS ) {
                    world->chunks[player->chunk_y][player->chunk_x]->color_pair[player->y][player->x] = newcolor;
                    wclear(botw);
                    acs_box(botw);
                    wrefresh(botw);
                }
                else {
                    wclear(botw);
                    acs_box(botw);
                    mvwprintw(botw, 1, 1, " not a number between 1 and the number of defined colors");
                    wrefresh(botw);
                }
                redraw_botw = TRUE;
            }
        }
        else {
            wclear(botw);
            acs_box(botw);
            mvwprintw(botw, 1, 1, " tab: change play/edit mode");
            wrefresh(botw);
        }

        if( c == KEY_LEFT  || c == KEY_RIGHT || c == KEY_UP || c == KEY_DOWN ) {
            move_player(player, world, world->chunks[player->chunk_y][player->chunk_x], c);
        }
        wrefresh(topw);
        wrefresh(botw);
        refresh();
    }
    
    delwin(topw);
    delwin(botw);
    endwin();

    return 0;
}
