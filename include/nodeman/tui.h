#ifndef NDM_TUI_H
#define NDM_TUI_H

#include <stdbool.h>

/**
 * TUI View states to toggle between different lists.
 */
typedef enum { 
    VIEW_LOCAL, 
    VIEW_SYSTEM, 
    VIEW_REMOTE 
} TuiView;

/**
 * Global state for the Terminal User Interface.
 * Manages navigation, selection, and rendering data.
 */
typedef struct {
    TuiView current_view;
    int cursor;
    int scroll_offset;
    bool running;
    char **current_list;
    int list_count;
} TuiState;

/* --- UI Rendering Functions --- */

// Draws the main layout, header, and the list of versions
void draw_layout(TuiState *state);

/* --- Input & Logic Handling --- */

// Processes keyboard events for navigation and actions (Install/Use/Remove)
void handle_input(TuiState *state);

// Entry point for launching the TUI mode
int main_menu();

#endif // NDM_TUI_H