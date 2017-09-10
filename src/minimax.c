#include <limits.h>
#include <stdlib.h>
#include "minimax.h"

GameMove *minimax_suggest_move(Game* game, unsigned int max_depth) {
	GameMove suggested_move;
	if(minimax_node(game, max_depth, &suggested_move, NULL, INT_MIN, INT_MAX) == SUCCESS){
		GameMove *move = create_move(
				suggested_move.src_x,
				suggested_move.src_y,
				suggested_move.dst_x,
				suggested_move.dst_y);
		return move;
	} else {
		return NULL;
	}
}

int color_score(Game* game, Color color) {
	int colorScore = 0;
	SPArrayList *pieces = (color == WHITE) ? game->white_pieces : game->black_pieces;
	for(int i = 0; i < spArrayListSize(pieces); i++) {
		GamePiece* piece = (GamePiece*)spArrayListGetAt(pieces, i);
		if (piece->type == PAWN) {
			colorScore += 1;
		} else if (piece->type == KNIGHT || piece->type == BISHOP) {
			colorScore += 3;
		} else if (piece->type == ROOK) {
			colorScore += 5;
		} else if (piece->type == QUEEN) {
			colorScore += 9;
		} else if (piece->type == KING) {
			colorScore += 100;
		}
	}
	return colorScore;
}

int score(Game* game) {
	int white = color_score(game, WHITE);
	int black = color_score(game, BLACK);
	return white - black;
}

int get_game_over_value(Color current_player_color, int check_mate){
	if (check_mate) {
		return (current_player_color == WHITE) ? INT_MIN : INT_MAX;
	} else {
		/* Game ended with a tie */
		return 0;
	}
}

EngineMessage check_minimax_subtree(
		Game *game,
		Color cur_player_color,
		GameMove *suggested_move,
		int max_depth,
		int *alpha,
		int *beta){
	SPArrayList *pieces = (cur_player_color == WHITE) ?
		game->white_pieces : game->black_pieces;
	int child_score;
	for (int i = 0; i < spArrayListSize(pieces); i++) {
		GamePiece *piece = (GamePiece *)spArrayListGetAt(pieces, i);
		SPArrayList *moves;
		if (get_possible_moves(&moves, game, piece) == SUCCESS) {
			for (int j = 0; j < spArrayListSize(moves); j++) {
				GameMove *move = (GameMove *)spArrayListGetAt(moves, j);
				Game *copy = copy_game(game);
				if (!copy) {
					spArrayListDestroy(moves);
					return MALLOC_FAILURE;
				}
				EngineMessage msg = move_game_piece(copy, piece->pos_x, piece->pos_y, move->dst_x, move->dst_y);
				if (msg == MALLOC_FAILURE) {
					spArrayListDestroy(moves);
					destroy_game(copy);
					return MALLOC_FAILURE;
				}
				if(minimax_node(copy, max_depth-1, NULL, &child_score, *alpha, *beta) != SUCCESS){
					spArrayListDestroy(moves);
					destroy_game(copy);
					return MALLOC_FAILURE;
				}
				if (cur_player_color == WHITE && child_score > *alpha) {
					*alpha = child_score;
					if (suggested_move != NULL) {
						*suggested_move = *move;
					}
				}
				if (cur_player_color == BLACK && child_score < *beta) {
					*beta = child_score;
					if (suggested_move != NULL) {
						*suggested_move = *move;
					}
				}
				destroy_game(copy);
				if (*alpha >= *beta){
					break;
				}
			}
			spArrayListDestroy(moves);
			if (*alpha >= *beta){
				break;
			}
		} else {
			/* Malloc failure occurred */
			return MALLOC_FAILURE;
		}
	}
	return SUCCESS;
}

EngineMessage minimax_node(
		Game* game,
		unsigned int max_depth,
		GameMove* suggested_move,
		int *son_score,
		int alpha,
		int beta) {
	Color current_player_color = game->player_color[game->current_player];
	int game_over = is_game_over(game);
	int check_mate = (game_over && game->check);
	if (game_over){
		*son_score = get_game_over_value(current_player_color, check_mate);
		return SUCCESS;
	}

	if (max_depth == 0) {
		*son_score = score(game);
		return SUCCESS;
	}

	EngineMessage msg = check_minimax_subtree(
			game,
			current_player_color,
			suggested_move,
			max_depth,
			&alpha,
			&beta);
	if(son_score) *son_score = (current_player_color == WHITE) ? alpha : beta;
	return msg;
}
