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
	new_board->posibble_moves = create_texture(renderer, "./images/posible_moves.bmp");
	new_board->threat_move = create_texture(renderer, "./images/threat_move.bmp");
	new_board->eat_opponent_piece = create_texture(renderer, "./images/eat_opponent.bmp");

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
	int err;
	SDL_Rect current_rect;
	SDL_Texture* current_texture;
	err = SDL_RenderCopy(renderer, board->board_texture, NULL, &board->board_area);

	if (err != 0) {
		return SDL_ERROR;
	}

	int i, j;
	int cur_i = 0;
	int cur_j = 0;
	int width = board->board_area.w/BOARD_SIZE;
	int height = board->board_area.h/BOARD_SIZE;

	if (!game) {
		return SUCCESS;
	}
	if (board->right_click == 1 && board->current_possible_moves != NULL) {
		for (i = 0; i < spArrayListSize(board->current_possible_moves); i++) {
			GameMove *temp = (GameMove *)spArrayListGetAt(board->current_possible_moves, i);
			int threat = is_piece_threatened_after_move(game, game->board[temp->src_y][temp->src_x], temp);
			if (threat == 1) {
				current_texture = board->threat_move;
			} else if (game->board[temp->dst_y][temp->dst_x] != NULL && game->board[temp->src_y][temp->src_x]->color != game->board[temp->dst_y][temp->dst_x]->color) {
				current_texture = board->eat_opponent_piece;
			} else{
				current_texture = board->posibble_moves;
			}
			current_rect.x = board->board_area.x + temp->dst_x * width;
			current_rect.y = (BOARD_SIZE - 1 - temp->dst_y) * height + board->board_area.y;
			current_rect.w = width;
			current_rect.h = height;
			err = SDL_RenderCopy(renderer, current_texture, NULL, &current_rect);
			if (err != 0) {
				return err;
			}
		}
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
					err = SDL_RenderCopy(renderer, current_texture, NULL, &current_rect);
					if (err != 0) {
						return SDL_ERROR;
					}
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
		err = SDL_RenderCopy(renderer, current_texture, NULL, &current_rect);
		if (err != 0) {
			return SDL_ERROR;
		}
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

EngineMessage chess_board_event_handler(SDL_Event *event, ChessBoard *board , ChessBoardEvent *chess_board_event, Game *game) {
	chess_board_event->type = NO_CHESS_BOARD_EVENT;

	switch (event->type) {
		case SDL_MOUSEBUTTONDOWN:
			if (event->button.x >= board->board_area.x && event->button.x <= board->board_area.x + board->board_area.w
				&& event->button.y >= board->board_area.y && event->button.y <= board->board_area.y + board->board_area.h) {
				int row = BOARD_SIZE - 1 - ((event->button.y - board->board_area.y) * BOARD_SIZE / board->board_area.h);
				int col = (event->button.x - board->board_area.x) * BOARD_SIZE / board->board_area.w;
				if (game->board[row][col] == NULL || game->board[row][col]->color != game->player_color[game->current_player]) {
					break;
				}

				if (event->button.button == SDL_BUTTON_LEFT) {
					board->is_dragging = 1;
					board->dragging_piece_row = row;
					board->dragging_piece_col = col;
					board->start_mouse_x_pos = event->button.x;
					board->current_mouse_x_pos = event->button.x;
					board->start_mouse_y_pos = event->button.y;
					board->current_mouse_y_pos = event->button.y;
				} else if (event->button.button == SDL_BUTTON_RIGHT && !(game->mode == ONE_PLAYER && game->difficulty > 2)) {
					board->right_click = 1;
					EngineMessage msg = get_possible_moves(&board->current_possible_moves, game, game->board[row][col]);
					if (msg != SUCCESS) {
						return msg;
					}
				}
				break;
			}
		case SDL_MOUSEBUTTONUP:
			if (board->right_click == 1) {
				board->right_click = 0;
				spArrayListDestroy(board->current_possible_moves);
			}
			if (board->is_dragging) {
				board->is_dragging = 0;

				// return game event.
				chess_board_event->type = PIECE_MOVED;
				chess_board_event->data.move.prev_piece_row = board->dragging_piece_row;
				chess_board_event->data.move.prev_piece_col = board->dragging_piece_col;

				chess_board_event->data.move.new_piece_row = (board->current_mouse_y_pos - board->board_area.y) * BOARD_SIZE / board->board_area.h;
				chess_board_event->data.move.new_piece_row = BOARD_SIZE - 1 - chess_board_event->data.move.new_piece_row;
				chess_board_event->data.move.new_piece_col = (board->current_mouse_x_pos - board->board_area.x) * BOARD_SIZE / board->board_area.w;
			}
			break;
		case SDL_MOUSEMOTION:
			if (board->is_dragging) {
				board->current_mouse_x_pos = event->button.x;
				board->current_mouse_y_pos = event->button.y;
			}
			break;
	}

	return SUCCESS;
}

