#include <ncurses.h>
#include "nodeman/tui.h"

void draw_ndm_box(int width, int height) {
    mvaddch(0, 0, ACS_ULCORNER);
    for(int i = 1; i < width - 1; i++) addch(ACS_HLINE);
    addch(ACS_URCORNER);
    for(int i = 1; i < height - 1; i++) {
        mvaddch(i, 0, ACS_VLINE);
        mvaddch(i, width - 1, ACS_VLINE);
    }
    mvaddch(height - 1, 0, ACS_LLCORNER);
    for(int i = 1; i < width - 1; i++) addch(ACS_HLINE);
    addch(ACS_LRCORNER);
}

void draw_layout(TuiState *state) {
    int h, w;
    getmaxyx(stdscr, h, w);
    int max_display = h - 7; // Ruang list: Total layar minus Header & Footer

    draw_ndm_box(w, h);
    mvprintw(0, (w - 14) / 2, "[ NDM TUI v2.5.0 ]");

    // Render Tab Header
    mvprintw(2, 4, state->current_view == VIEW_LOCAL ? "-> (1) LOCAL " : "   (1) LOCAL ");
    printw(state->current_view == VIEW_SYSTEM ? " (2) SYSTEM " : " (2) SYSTEM ");
    printw(state->current_view == VIEW_REMOTE ? " (3) REMOTE <-" : " (3) REMOTE   ");

    // Render List dengan Scrolling Logic
    for (int i = 0; i < max_display; i++) {
        int item_idx = state->scroll_offset + i;
        if (item_idx >= state->list_count) break;

        int row = 4 + i;
        if (item_idx == state->cursor) {
            attron(A_REVERSE);
            mvprintw(row, 4, " > %-20s ", state->current_list[item_idx]);
            attroff(A_REVERSE);
        } else {
            mvprintw(row, 4, "   %-20s ", state->current_list[item_idx]);
        }
    }

    // Render Footer
    mvprintw(h - 2, 4, "q:Quit | 1-3:Tabs | j/k:Nav | ENTER:%s", 
             state->current_view == VIEW_REMOTE ? "Install" : "Use");
}