#include "game_board.h"

/*
struct ChessBoard {
    SDL_Texture* board_texture;
    SDL_Texture* piece_textures[2][NUM_PIECES_TYPES];
    SDL_Rect board_area;
    int is_dragging;
    int dragging_piece_row;
    int gragging_piece_col;
    int start_mouse_x_pos;
    int start_mouse_y_pos;
    int current_mouse_x_pos;
    int current_mouse_y_pos;
};
* */

SDL_Texture* create_texture(SDL_Renderer *renderer, char *file_name) {
	SDL_Texture* res = NULL;
	SDL_Surface* loading_surface = SDL_LoadBMP(file_name);
	if (loading_surface == NULL) {
		return NULL;
	}
	SDL_SetColorKey(loading_surface, SDL_TRUE, SDL_MapRGB(loading_surface->format, 0, 187, 0));
	res = SDL_CreateTextureFromSurface(renderer, loading_surface);
	if (res == NULL ) {
		printf("Could not create a texture: %s\n", SDL_GetError());
		SDL_DestroyTexture(res);
		return NULL ;
	}
	SDL_FreeSurface(loading_surface);
	return res;
}

EngineMessage create_chess_board(ChessBoard **board, SDL_Rect *board_area, SDL_Renderer *renderer) {
	ChessBoard *new_board = (ChessBoard*)malloc(sizeof(ChessBoard*));
	if (!new_board) {
		return MALLOC_FAILURE;
	}
	new_board->piece_textures[0][PAWN] = create_texture(renderer, "./images/white_pawn.bmp");
	new_board->piece_textures[0][BISHOP] = create_texture(renderer, "./images/white_bishop.bmp");
	new_board->piece_textures[0][ROOK] = create_texture(renderer, "./images/white_rook.bmp");
	new_board->piece_textures[0][KNIGHT] = create_texture(renderer, "./images/white_knight.bmp");
	new_board->piece_textures[0][QUEEN] = create_texture(renderer, "./images/white_queen.bmp");
	new_board->piece_textures[0][KING] = create_texture(renderer, "./images/white_king.bmp");
	new_board->piece_textures[1][PAWN] = create_texture(renderer, "./images/black_pawn.bmp");
	new_board->piece_textures[1][BISHOP] = create_texture(renderer, "./images/black_bishop.bmp");
	new_board->piece_textures[1][ROOK] = create_texture(renderer, "./images/black_rook.bmp");
	new_board->piece_textures[1][KNIGHT] = create_texture(renderer, "./images/black_knight.bmp");
	new_board->piece_textures[1][QUEEN] = create_texture(renderer, "./images/black_queen.bmp");
	new_board->piece_textures[1][KING] = create_texture(renderer, "./images/black_king.bmp");
	
	new_board->board_texture = create_texture(renderer, "./images/chess_board.bmp");
	
	new_board->board_area = *board_area;
	new_board->is_dragging = 0;

	*board = new_board;
	
	return SUCCESS;
}

EngineMessage draw_chess_board(SDL_Renderer *renderer, ChessBoard *board, Game *game) {
	SDL_Rect current_rect;
	SDL_RenderCopy(renderer, board->board_texture, NULL, &(board->board_area)); //need &(board->board_area)???
	int i, j;
	int width = board->board_area.w/BOARD_SIZE;
	int height = board->board_area.h/BOARD_SIZE;
	
	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			if (game->board[i][j] != NULL) {
				SDL_Texture* current_texture = board->piece_textures[game->board[i][j]->color][game->board[i][j]->type];
				if (board->is_dragging == 1 && board->dragging_piece_row == i && board->dragging_piece_col == j) {
					current_rect.x = board->board_area.x + j * width + board->current_mouse_x_pos - board->start_mouse_x_pos;
					current_rect.y = board->board_area.y + i * height + board->current_mouse_y_pos - board->start_mouse_y_pos;
					current_rect.w = width;
					current_rect.h = height;
				} else {
					current_rect.x = j * width + board->board_area.x;
					current_rect.y = i * height + board->board_area.y;
					current_rect.w = width;
					current_rect.h = height;
				}
				SDL_RenderCopy(renderer, current_texture, NULL, &current_rect);
			}
		}
	}
	return SUCCESS;
}

GameEvent *chess_board_event_handler(SDL_Event *event, ChessBoard *board) {
	/*if (!event) {
		return ERROR;
	}*/
	GameEvent *game_event = (GameEvent*)malloc(sizeof(GameEvent*));
	game_event->prev_piece_row = 0;
	game_event->prev_piece_col = 0;
	game_event->new_piece_row = 0;
	game_event->new_piece_col = 0;
	
	switch (event->type) {
		case SDL_MOUSEBUTTONDOWN:
			board->is_dragging = 1;
			board->dragging_piece_row = (event->button.y - board->board_area.y) * BOARD_SIZE / board->board_area.h;
			board->dragging_piece_col = (event->button.x - board->board_area.x) * BOARD_SIZE / board->board_area.w;
			board->start_mouse_x_pos = event->button.x;
			board->current_mouse_x_pos = event->button.x;
			board->start_mouse_y_pos = event->button.y;
			board->current_mouse_y_pos = event->button.y;
			break;
		case SDL_MOUSEBUTTONUP:
			if (board->is_dragging) {
				board->is_dragging = 0;
				// return game event.
				
				game_event->prev_piece_row = board->dragging_piece_row;
				game_event->prev_piece_col = board->dragging_piece_col;
				
				game_event->new_piece_row = (board->current_mouse_y_pos - board->board_area.y) * BOARD_SIZE / board->board_area.h;
				game_event->new_piece_col = (board->current_mouse_x_pos - board->board_area.x) * BOARD_SIZE / board->board_area.w;
				
			}
			break;
		case SDL_MOUSEMOTION:
			if (board->is_dragging) {
				board->current_mouse_x_pos = event->button.x;
				board->current_mouse_y_pos = event->button.y;
			}
			break;
	}
	return game_event;

}























