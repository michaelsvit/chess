#ifndef EVENT_H
#define EVENT_H

#include <SDL.h>
#include <SDL_video.h>
#include "../game_engine.h"

typedef enum {
    NO_EVENT = 0,
    PIECE_MOVED,
    NEW_GAME,
    RESTART_GAME,
    SAVE_GAME,
    UNDO_MOVE,
    MOVE_TO_MAIN_MENU,
    MOVE_TO_SETTINGS_WINDOW,
    MOVE_TO_LOAD_SCREEN,
    EXIT_SETTINGS_SCREEN,
    QUIT_GAME,
} EventType;


typedef struct {
    EventType type;
    union EventData {
        struct PieceMove {
            int prev_piece_row;
            int prev_piece_col;
            int new_piece_row;
            int new_piece_col;
        } move;
        GameSettings new_game_settings;
    } data;
} GameEvent;

#endif
