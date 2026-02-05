#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>
#include "nodeman/tui.h"
#include "nodeman/core.h"
#include "nodeman/utils.h"

void handle_input(TuiState *state) {
    int h, w;
    getmaxyx(stdscr, h, w);
    int max_display = h - 7;

    int ch = getch();
    switch (ch) {
        case 'q': 
            state->running = false; 
            break;
        
        case '1': 
            state->current_view = VIEW_LOCAL;  
            state->cursor = 0; 
            state->scroll_offset = 0; 
            break;

        case '2': 
            state->current_view = VIEW_SYSTEM; 
            state->cursor = 0; 
            state->scroll_offset = 0; 
            break;

        case '3': 
            state->current_view = VIEW_REMOTE; 
            state->cursor = 0; 
            state->scroll_offset = 0; 
            break;

        case 'k': case KEY_UP:
            if (state->cursor > 0) {
                state->cursor--;
                if (state->cursor < state->scroll_offset) {
                    state->scroll_offset = state->cursor;
                }
            }
            break;

        case 'j': case KEY_DOWN:
            if (state->cursor < state->list_count - 1) {
                state->cursor++;
                if (state->cursor >= state->scroll_offset + max_display) {
                    state->scroll_offset = state->cursor - max_display + 1;
                }
            }
            break;

        case '\n': case KEY_ENTER:
            if (state->list_count > 0) {
                char *selected = state->current_list[state->cursor];
                bool v = true;
                char *args[] = { selected, NULL };

                // Temporarily exit TUI for CLI action
                def_prog_mode();
                endwin();
                
                // 1. Clear terminal using your engine
                command(&v, "clear");

                // 2. Execute based on context
                if (state->current_view == VIEW_REMOTE) {
                    install(&v, args);
                } else {
                    use(&v, 1, args);
                }

                // 3. User interaction to return
                printf("\n\r[Press ENTER to return to NDM]");
                fflush(stdout);
                
                // Cleaner buffer flush
                int temp_ch;
                while ((temp_ch = getchar()) != '\n' && temp_ch != EOF);

                // 4. Back to TUI
                reset_prog_mode();
                refresh();
            }
            break;
    }
}