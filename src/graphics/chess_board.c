#include "chess_board.h"
#include "utils.h"

EngineMessage create_chess_board(ChessBoard **board, SDL_Rect *board_area, SDL_Renderer *renderer) {
	int i;
	ChessBoard *new_board = (ChessBoard*)calloc(1, sizeof(ChessBoard));
	if (!new_board) {
		return MALLOC_FAILURE;
	}

	new_board->piece_textures[WHITE][PAWN] = create_texture(renderer, "./images/white_pawn.bmp");
	new_board->piece_textures[WHITE][BISHOP] = create_texture(renderer, "./images/white_bishop.bmp");
	new_board->piece_textures[WHITE][ROOK] = create_texture(renderer, "./images/white_rook.bmp");
	new_board->piece_textures[WHITE][KNIGHT] = create_texture(renderer, "./images/white_knight.bmp");
	new_board->piece_textures[WHITE][QUEEN] = create_texture(renderer, "./images/white_queen.bmp");
	new_board->piece_textures[WHITE][KING] = create_texture(renderer, "./images/white_king.bmp");
	new_board->piece_textures[BLACK][PAWN] = create_texture(renderer, "./images/black_pawn.bmp");
	new_board->piece_textures[BLACK][BISHOP] = create_texture(renderer, "./images/black_bishop.bmp");
	new_board->piece_textures[BLACK][ROOK] = create_texture(renderer, "./images/black_rook.bmp");
	new_board->piece_textures[BLACK][KNIGHT] = create_texture(renderer, "./images/black_knight.bmp");
	new_board->piece_textures[BLACK][QUEEN] = create_texture(renderer, "./images/black_queen.bmp");
	new_board->piece_textures[BLACK][KING] = create_texture(renderer, "./images/black_king.bmp");

	for (i = 0; i < NUM_PIECES_TYPES; i++) {
		if ((!new_board->piece_textures[0][i]) || (!new_board->piece_textures[1][i])) {
			destroy_chess_board(new_board);
			return SDL_ERROR;
		}
	}

	new_board->board_texture = create_texture(renderer, "./images/chess_board.bmp");
	if (!new_board->board_texture) {
		destroy_chess_board(new_board);
		return SDL_ERROR;
	}

	new_board->board_area = *board_area;
	new_board->is_dragging = 0;

	*board = new_board;

	return SUCCESS;
}

EngineMessage draw_chess_board(SDL_Renderer *renderer, ChessBoard *board, Game *game) {
	SDL_Rect current_rect;
	SDL_RenderCopy(renderer, board->board_texture, NULL, &board->board_area);
	int i, j;
	int cur_i = 0;
	int cur_j = 0;
	int width = board->board_area.w/BOARD_SIZE;
	int height = board->board_area.h/BOARD_SIZE;

	if (!game) {
		return SUCCESS;
	}

	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			if (game->board[i][j] != NULL) {
				if (board->is_dragging == 1 && board->dragging_piece_row == i && board->dragging_piece_col == j) {
					cur_i = i;
					cur_j = j;
				} else {
					SDL_Texture* current_texture = board->piece_textures[game->board[i][j]->color][game->board[i][j]->type];
					current_rect.x = j * width + board->board_area.x;
					current_rect.y = (BOARD_SIZE - 1 - i) * height + board->board_area.y;
					current_rect.w = width;
					current_rect.h = height;
					SDL_RenderCopy(renderer, current_texture, NULL, &current_rect);
				}
			}
		}
	}
	if (board->is_dragging == 1 && board->dragging_piece_row == cur_i && board->dragging_piece_col == cur_j){
		SDL_Texture* current_texture = board->piece_textures[game->board[cur_i][cur_j]->color][game->board[cur_i][cur_j]->type];
		current_rect.x = board->board_area.x + cur_j * width + board->current_mouse_x_pos - board->start_mouse_x_pos;
		current_rect.y = board->board_area.y + (BOARD_SIZE - 1 - cur_i) * height + board->current_mouse_y_pos - board->start_mouse_y_pos;
		current_rect.w = width;
		current_rect.h = height;
		SDL_RenderCopy(renderer, current_texture, NULL, &current_rect);
	}
	return SUCCESS;
}


void destroy_chess_board(ChessBoard *board) {
	for (int i = 0; i < NUM_PIECES_TYPES; i++) {
		if (board->piece_textures[0][i]) {
			SDL_DestroyTexture(board->piece_textures[0][i]);
		}
		if (board->piece_textures[1][i]) {
			SDL_DestroyTexture(board->piece_textures[1][i]);
		}
	}

	if (board->board_texture) {
		SDL_DestroyTexture(board->board_texture);
	}

	free(board);
}

void chess_board_event_handler(SDL_Event *event, ChessBoard *board, GameEvent *game_event) {
	game_event->type = NO_EVENT;

	switch (event->type) {
		case SDL_MOUSEBUTTONDOWN:
			if (event->button.x >= board->board_area.x && event->button.x <= board->board_area.x + board->board_area.w
				&& event->button.y >= board->board_area.y && event->button.y <= board->board_area.y + board->board_area.h) {

				board->is_dragging = 1;
				board->dragging_piece_row = (event->button.y - board->board_area.y) * BOARD_SIZE / board->board_area.h;
				board->dragging_piece_row = BOARD_SIZE - 1 - board->dragging_piece_row;
				board->dragging_piece_col = (event->button.x - board->board_area.x) * BOARD_SIZE / board->board_area.w;
				board->start_mouse_x_pos = event->button.x;
				board->current_mouse_x_pos = event->button.x;
				board->start_mouse_y_pos = event->button.y;
				board->current_mouse_y_pos = event->button.y;
				break;
			}
		case SDL_MOUSEBUTTONUP:
			if (board->is_dragging) {
				board->is_dragging = 0;

				// return game event.
				game_event->type = PIECE_MOVED;
				game_event->data.move.prev_piece_row = board->dragging_piece_row;
				game_event->data.move.prev_piece_col = board->dragging_piece_col;

				game_event->data.move.new_piece_row = (board->current_mouse_y_pos - board->board_area.y) * BOARD_SIZE / board->board_area.h;
				game_event->data.move.new_piece_row = BOARD_SIZE - 1 - game_event->data.move.new_piece_row;
				game_event->data.move.new_piece_col = (board->current_mouse_x_pos - board->board_area.x) * BOARD_SIZE / board->board_area.w;
			}
			break;
		case SDL_MOUSEMOTION:
			if (board->is_dragging) {
				board->current_mouse_x_pos = event->button.x;
				board->current_mouse_y_pos = event->button.y;
			}
			break;
	}
}

