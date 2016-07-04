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
    bool redraw_edit_help = TRUE;
    char load_save_name[20];
    // for copy paste
    int clipboard_tile = ' ';
    int clipboard_color = 1;
    bool leave_trail = FALSE;
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

    topw = newwin(TOP_WINDOW_HEIGHT, TOP_WINDOW_WIDTH, 0, 0);
    botw = newwin(botw_height, width, 20, 0);
    wclear(topw);
    acs_box(topw);

    console_clear(botw);
    console_print(botw, 1, 1, " tab: change play/edit mode q: quit");

    init_color_pairs();

    Player* player = create_player();

    World* world = create_world(2, 2);

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
            redraw_edit_help = TRUE;
        }

        if( game_mode == MODE_EDIT ) {
            if( redraw_edit_help ) {
                console_clear(botw);
                console_print(botw, 1, 1, " c: change color | e: change character | s: save world | l: load world");
                console_print(botw, 2, 1, " space: paste | y: copy | t: trail ");
                redraw_edit_help = FALSE;
            }
            // pasting while moving
            if( leave_trail) {
                world->chunks[player->chunk_y][player->chunk_x].tiles[player->y][player->x] = clipboard_tile;
                world->chunks[player->chunk_y][player->chunk_x].color_pair[player->y][player->x] = clipboard_color;
            }
            if( c == 'e' ) {
                console_clear(botw);
                console_print(botw, 1, 1, " input a character");
                int newtile = wgetch(topw);
                if( isprint( newtile )) {
                    world->chunks[player->chunk_y][player->chunk_x].tiles[player->y][player->x] = newtile;
                }
                else {
                    console_print(botw, 1, 1, " not a printable character");
                }
                redraw_edit_help = TRUE;
            }
            else if( c == 'c' ) {
                console_clear(botw);
                console_print(botw, 1, 1, " input a number between 1 and the number of defined colors");
                int newcolor = wgetch(topw) - '0';
                if( newcolor > 0 && newcolor <= NUM_COLOR_PAIRS ) {
                    world->chunks[player->chunk_y][player->chunk_x].color_pair[player->y][player->x] = newcolor;
                    console_clear(botw);
                }
                else {
                    console_clear(botw);
                    console_print(botw, 1, 1, " not a number between 1 and the number of defined colors");
                }
                redraw_edit_help = TRUE;
            }
            else if( c == ' ' ) {
                // paste
                world->chunks[player->chunk_y][player->chunk_x].tiles[player->y][player->x] = clipboard_tile;
                world->chunks[player->chunk_y][player->chunk_x].color_pair[player->y][player->x] = clipboard_color;
            }
            else if( c == 'y' ) {
                // copy tile and color
                clipboard_tile = world->chunks[player->chunk_y][player->chunk_x].tiles[player->y][player->x];
                clipboard_color = world->chunks[player->chunk_y][player->chunk_x].color_pair[player->y][player->x];
            }
            else if( c == 't') {
                leave_trail = ! leave_trail;
                if(leave_trail) {
                    console_clear(botw);
                    console_print(botw, 1, 1, " leaving a trail of the clipboard");
                }
                else {
                    redraw_edit_help = TRUE;
                }
            }
            else if( c == 's') {
                console_clear(botw);
                console_print(botw, 1, 1, " input a name for the world file and press enter");
                memset(load_save_name, 0, strlen(load_save_name));
                int pos = 0;
                c = wgetch(botw);
                while( c != KEY_ENTER  && c != 10) { // 10 is another enter signal (control m)
                    if((c == KEY_BACKSPACE || c == 263) && pos > 0) {
                        load_save_name[pos] = ' ';
                        pos--;
                    }
                    else if(isprint(c)) {
                        load_save_name[pos] = (char)c;
                        if(pos < 20) {
                            pos++;
                        }
                    }
                    console_print(botw, 2, 1, load_save_name);
                    c = wgetch(botw);
                }
                save_world(world, load_save_name);
                console_clear(botw);
                console_print(botw, 1, 1, " saved world");
                redraw_edit_help = TRUE;
            }
            else if( c == 'l') {
                console_clear(botw);
                console_print(botw, 1, 1, " input a name for the world file and press enter");
                memset(load_save_name, 0, strlen(load_save_name));
                int pos = 0;
                c = wgetch(botw);
                while( c != KEY_ENTER  && c != 10) { // 10 is another enter signal (control m)
                    if((c == KEY_BACKSPACE || c == 263) && pos > 0) {
                        load_save_name[pos] = ' ';
                        pos--;
                    }
                    else if(isprint(c)) {
                        load_save_name[pos] = (char)c;
                        if(pos < 20) {
                            pos++;
                        }
                    }
                    console_print(botw, 2, 1, load_save_name);
                    c = wgetch(botw);
                }
                world = load_world(load_save_name);
                console_clear(botw);
                console_print(botw, 1, 1, " loaded world");
                redraw_edit_help = TRUE;
            }
        }
        else {
            if( redraw_edit_help ) {
                console_print(botw, 1, 1, " tab: change play/edit mode");
                redraw_edit_help = FALSE;
            }
        }

        if( c == KEY_LEFT  || c == KEY_RIGHT || c == KEY_UP || c == KEY_DOWN ) {
            move_player(player, world, &(world->chunks[player->chunk_y][player->chunk_x]), c);
        }
    }
    
    delwin(topw);
    delwin(botw);
    endwin();

    return 0;
}
