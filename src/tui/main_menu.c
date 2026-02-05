#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "nodeman/tui.h"
#include "nodeman/core.h"
#include "nodeman/utils.h"

int main_menu() {
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);

    TuiState state = { 
        .current_view = VIEW_LOCAL, 
        .cursor = 0, 
        .running = true, 
        .list_count = 0, 
        .current_list = NULL, 
        .scroll_offset = 0 
    };

    char **remote_cache = NULL;
    int remote_cache_count = 0;
    TuiView last_view = -1;

    while (state.running) {
        bool needs_data = (state.current_view != last_view);

        if (needs_data) {
            // 1. Cleanup old local/system list pointers
            if (state.current_list && state.current_list != remote_cache) {
                free_versions_array(state.current_list, state.list_count);
                state.current_list = NULL;
            }

            // 2. Fetch Data based on View
            if (state.current_view == VIEW_LOCAL) {
                char path[512];
                const char *home = getenv("HOME");
                if (home) {
                    snprintf(path, sizeof(path), "%s/.ndm", home);
                    state.current_list = get_local_versions_array(path, &state.list_count);
                }
            } 
            else if (state.current_view == VIEW_SYSTEM) {
                state.current_list = get_local_versions_array(NODE_INSTALL_DIR, &state.list_count);
            } 
            else if (state.current_view == VIEW_REMOTE) {
                if (!remote_cache) {
                    clear();
                    mvprintw(LINES/2, (COLS-18)/2, "Fetching Remote...");
                    refresh();
                    bool v = false;
                    remote_cache = get_remote_versions_array(&v, NULL, 0, &remote_cache_count);
                }
                state.current_list = remote_cache;
                state.list_count = remote_cache_count;
            }
            last_view = state.current_view;
        }

        // 3. Render Cycle
        clear();
        draw_layout(&state);
        refresh();

        // 4. Interaction
        TuiView view_before = state.current_view;
        handle_input(&state);

        // 5. Smart Refresh Logic
        // Force refresh data if view changed OR if user just came back from an action (Enter)
        // Note: Enter action logic is handled by setting last_view to -1 inside handle_input if needed, 
        // or simply detecting if state changes.
        if (state.current_view != view_before) {
            last_view = -1; 
        }
    }

    // Final Memory Cleanup
    if (remote_cache) {
        free_versions_array(remote_cache, remote_cache_count);
    }
    
    endwin();
    return 0;
}