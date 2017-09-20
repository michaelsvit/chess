#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <SDL.h>
#include <SDL_video.h>

#include "../game_engine.h"

#define NUM_PIECES_TYPES 6

typedef enum {
    NO_CHESS_BOARD_EVENT = 0,
    PIECE_MOVED
} ChessBoardEventType;

typedef struct {
    ChessBoardEventType type;
    union ChessBoardEventData {
        struct PieceMove {
            int prev_piece_row;
            int prev_piece_col;
            int new_piece_row;
            int new_piece_col;
        } move;
    } data;
} ChessBoardEvent;

typedef struct {
    SDL_Texture* board_texture;
    SDL_Texture* piece_textures[2][NUM_PIECES_TYPES];
    SDL_Rect board_area;
    int is_dragging;
    int dragging_piece_row;
    int dragging_piece_col;
    int start_mouse_x_pos;
    int start_mouse_y_pos;
    int current_mouse_x_pos;
    int current_mouse_y_pos;
    SDL_Texture* posibble_moves;
    int right_click;
    SDL_Texture* threat_move;
    SDL_Texture* eat_opponent_piece;
    SPArrayList *current_possible_moves;
} ChessBoard;

typedef struct {
    int prev_piece_row;
    int prev_piece_col;
    int new_piece_row;
    int new_piece_col;
} PieceMove;

EngineMessage create_chess_board(ChessBoard **board, SDL_Rect *board_area, SDL_Renderer *renderer);
EngineMessage draw_chess_board(SDL_Renderer *renderer, ChessBoard *board, Game *game);
void destroy_chess_board(ChessBoard *board);
EngineMessage chess_board_event_handler(SDL_Event *event, ChessBoard *board , ChessBoardEvent *chess_board_event, Game *game);

#endif
