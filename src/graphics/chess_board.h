#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include "../game_engine.h"
#include "event.h"
#include <SDL.h>
#include <SDL_video.h>

#define NUM_PIECES_TYPES 6

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
void chess_board_event_handler(SDL_Event *event, ChessBoard *board, GameEvent *new_game_event);

#endif
