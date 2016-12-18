#include "main.h"
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
    
    init_color_pairs();

    Player* player = create_player();

    World* world = create_world(2, 2);

    game_mode = MODE_PLAY;

    keypad(topw, TRUE);
    int c = 0;
    while( c != 'q' ) {
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
                console_print(botw, 1, 1, " c: Change color | e: Change character | s: Save world | o: Load world");
                console_print(botw, 2, 1, " Space: paste | y: copy | t: Trail | q: Quit");
                redraw_edit_help = FALSE;
            }
            // pasting while moving
            if( leave_trail) {
                world->chunks[player->chunk_y][player->chunk_x].tiles[player->y][player->x].ch = clipboard_tile;
                world->chunks[player->chunk_y][player->chunk_x].tiles[player->y][player->x].color = clipboard_color;
            }
            if( c == 'e' ) {
                console_clear(botw);
                console_print(botw, 1, 1, " input a character");
                int newtile = wgetch(topw);
                if( isprint( newtile )) {
                    world->chunks[player->chunk_y][player->chunk_x].tiles[player->y][player->x].ch = newtile;
                }
                else {
                    console_print(botw, 1, 1, " Not a printable character");
                }
                redraw_edit_help = TRUE;
            }
            else if( c == 'c' ) {
                console_clear(botw);
                console_print(botw, 1, 1, " Select one of the following colors:");
                draw_available_colors(botw, 2, 1);
                int newcolor = wgetch(topw) - '0';
                if( newcolor > 0 && newcolor <= NUM_COLOR_PAIRS ) {
                    world->chunks[player->chunk_y][player->chunk_x].tiles[player->y][player->x].color = newcolor;
                    console_clear(botw);
                }
                else {
                    console_clear(botw);
                    console_print(botw, 1, 1, " Not a valid color");
                }
                redraw_edit_help = TRUE;
            }
            else if( c == ' ' ) {
                // paste
                world->chunks[player->chunk_y][player->chunk_x].tiles[player->y][player->x].ch = clipboard_tile;
                world->chunks[player->chunk_y][player->chunk_x].tiles[player->y][player->x].color = clipboard_color;
            }
            else if( c == 'y' ) {
                // copy tile and color
                clipboard_tile = world->chunks[player->chunk_y][player->chunk_x].tiles[player->y][player->x].ch;
                clipboard_color = world->chunks[player->chunk_y][player->chunk_x].tiles[player->y][player->x].color;
            }
            else if( c == 't') {
                leave_trail = ! leave_trail;
                if(leave_trail) {
                    console_clear(botw);
                    console_print(botw, 1, 1, " Leaving a trail of the clipboard. Press t to stop");
                }
                else {
                    redraw_edit_help = TRUE;
                }
            }
            else if( c == 's') {
                console_clear(botw);
                console_print(botw, 1, 1, " Input a name for the world file and press enter");
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
                console_print(botw, 1, 1, " Saved world");
                redraw_edit_help = TRUE;
            }
            else if( c == 'o') {
                console_clear(botw);
                console_print(botw, 1, 1, " Input a name for the world file and press enter");
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
                World* tmp = load_world(load_save_name);
                if(tmp == NULL) {
                    console_clear(botw);
                    console_print(botw, 1, 1, " Failed to load world");
                }
                else {
                    world = tmp;
                    console_clear(botw);
                    console_print(botw, 1, 1, " Loaded world");
                }
                redraw_edit_help = TRUE;
            }
        }
        else if( game_mode == MODE_PLAY ) {
            if( redraw_edit_help ) {
                console_clear(botw);
                console_print(botw, 1, 1, " tab: Change play/edit mode | q: Quit");
                redraw_edit_help = FALSE;
            }
        }
        move_player(player, world, &(world->chunks[player->chunk_y][player->chunk_x]), c);

        draw_chunk(&(world->chunks[player->chunk_y][player->chunk_x]), topw, 1, 1);
        draw_player(player, topw, player->y, player->x);

        c = wgetch(topw);
    }
    
    delwin(topw);
    delwin(botw);
    endwin();

    return 0;
}
