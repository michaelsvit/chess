#include <limits.h>
#include <stdlib.h>
#include "minimax.h"

GameMove *minimax_suggest_move(Game* game) {
	int max_depth = game->difficulty;
	int error = 0;
	GameMove *move = NULL;
	minimax_node(game, max_depth, &move, INT_MIN, INT_MAX, &error);
	return move;
}

int minimax_node(Game *game, int max_depth, GameMove **suggested_move, int alpha, int beta, int *error){
	if(max_depth == 0) return board_value(game, 0);
	Color cur_player_color = game->player_color[game->current_player];
	SPArrayList *pieces = (cur_player_color == WHITE) ?
		game->white_pieces : game->black_pieces;
	int node_value = (game->current_player == PLAYER1) ? INT_MAX : INT_MIN;
	GameMove *best_move = NULL;
	for (int i = 0; i < spArrayListSize(pieces); ++i) {
		/* For each piece of current player */
		GamePiece *piece = (GamePiece *)spArrayListGetAt(pieces, i);
		SPArrayList *moves = NULL;
		get_possible_moves(&moves, game, piece);
		if (!moves) return quit_error(NULL, NULL, error);
		for (int j = 0; j < spArrayListSize(moves); j++){
			/* For each possible move of piece */
			GameMove *move = (GameMove *)spArrayListGetAt(moves, j);
			/* Init best_move so it doesn't return NULL */
			if(max_depth == game->difficulty && !best_move) best_move = copy_move(move);
			Game *copy = copy_game(game);
			if (!copy) return quit_error(NULL, moves, error);
			EngineMessage msg = move_game_piece(copy, move->src_x, move->src_y, move->dst_x, move->dst_y);
			if (msg == MALLOC_FAILURE) return quit_error(copy, moves, error);
			int child_value;
			if (msg == GAME_OVER)
				child_value = board_value(copy, 1);
			else {
				child_value = minimax_node(copy, max_depth-1, suggested_move, alpha, beta, error);
				if (*error) return quit_error(copy, moves, error);
			}
			destroy_game(copy);
			if (game->current_player == PLAYER2) {
				/* Max node */
				if(child_value > node_value){
					node_value = child_value;
					free(best_move);
					best_move = copy_move(move);
				}
				alpha = (node_value > alpha) ? node_value : alpha;
			} else {
				/* Min node */
				if(child_value < node_value){
					node_value = child_value;
					free(best_move);
					best_move = copy_move(move);
				}
				beta = (node_value < beta) ? node_value : beta;
			}
			if (beta <= alpha) break;
		}
		spArrayListDestroy(moves);
		if (beta <= alpha) break;
	}

	if (max_depth == game->difficulty) {
		*suggested_move = best_move;
	} else {
		free(best_move);
	}
	return node_value;
}

int quit_error(Game *game, SPArrayList *moves, int *error){
	destroy_game(game);
	spArrayListDestroy(moves);
	*error = 1;
	return 0;
}

int board_value(Game *game, int game_over){
	if(game_over){
		if (game->check) {
			return (game->current_player == PLAYER2) ? INT_MIN : INT_MAX;
		} else {
			/* Game ended with a tie */
			return 0;
		}
	} else {
		int ai_value = player_pieces_value(game, PLAYER2);
		int user_value = player_pieces_value(game, PLAYER1);
		return ai_value - user_value;
	}
}

int player_pieces_value(Game *game, Player player){
	int value = 0;
	SPArrayList *pieces = (game->player_color[player] == WHITE) ?
		game->white_pieces : game->black_pieces;
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
