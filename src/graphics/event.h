#ifndef EVENT_H
#define EVENT_H

#include <SDL.h>
#include <SDL_video.h>

typedef enum {NO_EVENT = 0, PIECE_MOVED, BUTTON_CLICKED} EventType;


typedef struct {
    EventType type;
    union EventData {
        struct PieceMove {
            int prev_piece_row;
            int prev_piece_col;
            int new_piece_row;
            int new_piece_col;
        } move;

        struct ButtonClick {
            int button_id;
        } button;
    } data;
} GameEvent;

#endif
