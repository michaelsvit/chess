#include <limits.h>
#include <stdlib.h>

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

int game_over_score(Game *game){
	if(game->check){
		return (game->player_color[game->current_player] == WHITE) ? INT_MIN : INT_MAX;
	} else {
		return 0;
	}
}

EngineMessage handle_edge_case(Game *game, int game_over, int *node_score){
	if (game_over == -1) return MALLOC_FAILURE;
	if(node_score) *node_score = game_over ? game_over_score(game) : score(game);
	return SUCCESS;
}

EngineMessage quit_error(Game *game, SPArrayList *moves){
	destroy_game(game);
	spArrayListDestroy(moves);
	return MALLOC_FAILURE;
}

EngineMessage minimax_node(Game* game, int max_depth, int alpha, int beta, GameMove *suggested_move, int *node_score) {
	EngineMessage msg = SUCCESS;
	Color current_player_color = game->player_color[game->current_player];

	int game_over = is_game_over(game);
	if (game_over || max_depth == 0){
		return handle_edge_case(game, game_over, node_score);
	}

	int son_score;
	SPArrayList *pieces = (current_player_color == WHITE) ? game->white_pieces : game->black_pieces;

	for (int i = 0; i < spArrayListSize(pieces); i++) {
		GamePiece *piece = (GamePiece *)spArrayListGetAt(pieces, i);
		SPArrayList *moves;
		if ((msg = get_possible_moves(&moves, game, piece)) != SUCCESS) return msg;

		for (int j = 0; j < spArrayListSize(moves); j++) {
			GameMove *move = (GameMove *)spArrayListGetAt(moves, j);
			/* Make sure suggested_move is initialized */
			if (max_depth == game->difficulty && suggested_move && suggested_move->src_x == -1)
				*suggested_move = *move;

			Game *copy = copy_game(game);
			if (!copy) return quit_error(NULL, moves);

			msg = move_game_piece(copy, piece->pos_x, piece->pos_y, move->dst_x, move->dst_y);
			if (msg != SUCCESS && msg != GAME_OVER) return quit_error(copy, moves);

			if ((msg = minimax_node(copy, max_depth-1, alpha, beta, NULL, &son_score)) != SUCCESS)
				return quit_error(copy, moves);

			if (current_player_color == WHITE && son_score > alpha) {
				alpha = son_score;
				if (max_depth == game->difficulty) {
					*suggested_move = *move;
				}
			}
			if (current_player_color == BLACK && son_score < beta) {
				beta = son_score;
				if (max_depth == game->difficulty) {
					*suggested_move = *move;
				}
			}

			destroy_game(copy);
			if (alpha >= beta) break;
		}
		spArrayListDestroy(moves);
		if (alpha >= beta) break;
	}

	if (node_score) *node_score = (current_player_color == WHITE) ? alpha : beta;
	return SUCCESS;
}

EngineMessage minimax_suggest_move(Game* game, unsigned int max_depth, GameMove *suggested_move) {
	return minimax_node(game, max_depth, INT_MIN, INT_MAX, suggested_move, NULL);
}
