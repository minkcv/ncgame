#include "chunk.h"
#include "colors.h"
#include "player.h"
#include "world.h"
#include "tiles.h"
#include "util.h"
#include "console.h"
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

    refresh();
    topw = newwin(TOP_WINDOW_HEIGHT, TOP_WINDOW_WIDTH, 0, 0);
    botw = newwin(botw_height, width, 20, 0);
    wclear(topw);
    acs_box(topw);
    wrefresh(topw);

    console_print(botw, 1, 1, " tab: change play/edit mode");

    init_color_pairs();

    Player* player = create_player();

    World* world = create_world(2, 2);

    world->chunks[0][0].tiles[5][5] = TILE_WALL;

    game_mode = MODE_PLAY;
    
    keypad(topw, TRUE);
    int c = 0;
    while( (char)c != 'q' ) {
        draw_chunk(&(world->chunks[player->chunk_y][player->chunk_x]), topw, 1, 1);
        draw_player(player, topw, player->y, player->x);
        c = wgetch(topw);

        if( c == '\t' ) {
            if( game_mode == MODE_PLAY ) {
                game_mode = MODE_EDIT;
            }
            else if( game_mode == MODE_EDIT ) {
                game_mode = MODE_PLAY;
            }
        }

        if( game_mode == MODE_EDIT ) {
            console_print(botw, 1, 1, " c: change color e: change character s: save world l: load world");
            if( c == 'e' ) {
                console_print(botw, 1, 1, " input a character");
                int newtile = wgetch(topw);
                if( isprint( newtile )) {
                    world->chunks[player->chunk_y][player->chunk_x].tiles[player->y][player->x] = newtile;
                    wrefresh(botw);
                }
                else {
                    console_print(botw, 1, 1, " not a printable character");
                }
            }
            else if( c == 'c' ) {
                console_print(botw, 1, 1, " input a number between 1 and the number of defined colors");
                int newcolor = wgetch(topw) - '0';
                if( newcolor > 0 && newcolor <= NUM_COLOR_PAIRS ) {
                    world->chunks[player->chunk_y][player->chunk_x].color_pair[player->y][player->x] = newcolor;
                    console_clear(botw);
                }
                else {
                    console_print(botw, 1, 1, " not a number between 1 and the number of defined colors");
                }
            }
            else if( c == 's') {
                console_print(botw, 1, 1, " input a name for the world file and press enter");
                char name[20];
                int pos = 0;
                c = wgetch(botw);
                while( c != KEY_ENTER  && c != 10) { // 10 is another enter signal (control m)
                    if((c == KEY_BACKSPACE || c == 263) && pos > 0) {
                        name[pos] = ' ';
                        pos--;
                    }
                    else if(isprint(c)) {
                        name[pos] = (char)c;
                        if(pos < 20) {
                            pos++;
                        }
                    }
                    console_print(botw, 2, 1, name);
                    c = wgetch(botw);
                }
                save_world(world, name);
                console_print(botw, 1, 1, " saved world");
            }
            else if( c == 'l') {
                console_print(botw, 1, 1, " input a name for the world file and press enter");
                char name[20];
                int pos = 0;
                c = wgetch(botw);
                while( c != KEY_ENTER  && c != 10) { // 10 is another enter signal (control m)
                    if((c == KEY_BACKSPACE || c == 263) && pos > 0) {
                        name[pos] = ' ';
                        pos--;
                    }
                    else if(isprint(c)) {
                        name[pos] = (char)c;
                        if(pos < 20) {
                            pos++;
                        }
                    }
                    console_print(botw, 2, 1, name);
                    c = wgetch(botw);
                }
                world = load_world(name);
                console_print(botw, 1, 1, " loaded world");
            }
        }
        else {
            console_print(botw, 1, 1, " tab: change play/edit mode");
        }

        if( c == KEY_LEFT  || c == KEY_RIGHT || c == KEY_UP || c == KEY_DOWN ) {
            move_player(player, world, &(world->chunks[player->chunk_y][player->chunk_x]), c);
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
