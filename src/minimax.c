#include <limits.h>
#include <stdlib.h>

#include "game_engine.h"
#include "minimax.h"

int color_score(Game* game, Color color) {
	int value = 0;
	SPArrayList *pieces = (color == WHITE) ? game->white_pieces : game->black_pieces;
	for(int i = 0; i < spArrayListSize(pieces); i++) {
		GamePiece* piece = (GamePiece*)spArrayListGetAt(pieces, i);
		if (piece->type == PAWN) {
			value += 1;
		} else if (piece->type == KNIGHT || piece->type == BISHOP) {
			value += 3;
		} else if (piece->type == ROOK) {
			value += 5;
		} else if (piece->type == QUEEN) {
			value += 9;
		} else if (piece->type == KING) {
			value += 100;
		}
	}
	return value;
}

int score(Game* game) {
	int white = color_score(game, WHITE);
	int black = color_score(game, BLACK);
	return white - black;
}

EngineMessage minimax_node(Game* game, unsigned int max_depth, int alpha, int beta, GameMove *suggested_move, int *node_score) {
	if (!game) {
		return INVALID_ARGUMENT;
	}

	EngineMessage msg = SUCCESS;
	Color current_player_color = game->player_color[game->current_player];

	int game_over = is_game_over(game);
	if (game_over == -1) {
		return MALLOC_FAILURE;
	}

	int check_mate = (game_over && game->check);

	if (current_player_color == WHITE && check_mate) {
		if (node_score) {
			*node_score = INT_MIN;
		}
		return msg;
	}
	if (current_player_color == BLACK && check_mate) {
		if (node_score) {
			*node_score =  INT_MAX;
		}
		return msg;
	}
	if (game_over) {
		// Tie
		if (node_score) {
			*node_score = 0;
		}
		return msg;
	}
	if (max_depth == 0) {
		if (node_score) {
			*node_score = score(game);
		}
		return msg;
	}

	int son_score;
	SPArrayList *pieces = (current_player_color == WHITE) ? game->white_pieces : game->black_pieces;

	for (int i = 0; i < spArrayListSize(pieces); i++) {
		GamePiece *piece = (GamePiece *)spArrayListGetAt(pieces, i);
		SPArrayList *moves;

		if ((msg = get_possible_moves(&moves, game, piece)) != SUCCESS) {
			return msg;
		}
		for (int j = 0; j < spArrayListSize(moves); j++) {
			GameMove *move = (GameMove *)spArrayListGetAt(moves, j);
			Game *copy = copy_game(game);
			if (!copy) {
				spArrayListDestroy(moves);
				return MALLOC_FAILURE;
			}

			msg = move_game_piece(copy, piece->pos_x, piece->pos_y, move->dst_x, move->dst_y);
			if (msg != SUCCESS && msg != GAME_OVER) {
				// GAME_OVER is returned by move_game_piece not when move_game_piece fails, but when game is over AFTER the move was done.
				spArrayListDestroy(moves);
				destroy_game(copy);
				return msg;
			}

			if ((msg = minimax_node(copy, max_depth-1, alpha, beta, NULL, &son_score)) != SUCCESS) {
				spArrayListDestroy(moves);
				destroy_game(copy);
				return msg;
			}

			if (current_player_color == WHITE && son_score > alpha) {
				alpha = son_score;
				if (suggested_move != NULL) {
					*suggested_move = *move;
				}
			}
			if (current_player_color == BLACK && son_score < beta) {
				beta = son_score;
				if (suggested_move != NULL) {
					*suggested_move = *move;
				}
			}

			destroy_game(copy);
			if (alpha >= beta){
				break;
			}
		}
		spArrayListDestroy(moves);
		if (alpha >= beta){
			break;
		}
	}

	if (current_player_color == WHITE && node_score) {
		*node_score = alpha;
	} else if (current_player_color == BLACK && node_score) {
		*node_score = beta;
	}
	return SUCCESS;
}

EngineMessage minimax_suggest_move(Game* game, unsigned int max_depth, GameMove *suggested_move) {
	if (!game) {
		return INVALID_ARGUMENT;
	}
	int game_over = is_game_over(game);
	if (game_over == -1) {
		return MALLOC_FAILURE;
	}
	if (game_over) {
		return ILLEGAL_MOVE;
	}
	return minimax_node(game, max_depth, INT_MIN, INT_MAX, suggested_move, NULL);
}
