#include <limits.h>
#include <stdlib.h>
#include "game_engine.h"

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

int minimax_node(Game* game, unsigned int max_depth, GameMove* suggested_move, int alpha, int beta) {
	if (!game) {
		return 0;
	}
	if (max_depth == 0) {
		return score(game);
	}

	Color current_player_color = game->player_color[game->current_player];
	int game_over = is_game_over(game);
	int check_mate = (game_over && game->check);

	if (current_player_color == WHITE && check_mate) {
		return INT_MIN;
	}
	if (current_player_color == BLACK && check_mate) {
		return INT_MAX;
	}
	if (game_over) {
		return 0;
	}

	int son_score;
	SPArrayList *pieces = (current_player_color == WHITE) ? game->white_pieces : game->black_pieces;

	for (int i = 0; i < spArrayListSize(pieces); i++) {
		GamePiece *piece = (GamePiece *)spArrayListGetAt(pieces, i);
		SPArrayList *moves;
		if (get_possible_moves(&moves, game, piece) == SUCCESS) {
			for (int j = 0; j < spArrayListSize(moves); j++) {
				GameMove *move = (GameMove *)spArrayListGetAt(moves, j);
				Game *copy = copy_game(game);
				move_game_piece(copy, piece->pos_x, piece->pos_y, move->dst_x, move->dst_y);
				son_score = minimax_node(copy, max_depth-1, NULL, alpha, beta);
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
	}
	if (current_player_color == WHITE) {
		return alpha;
	}
	return beta;
}

GameMove minimax_suggest_move(Game* game, unsigned int max_depth) {
	GameMove suggested_move;
	minimax_node(game, max_depth, &suggested_move, INT_MIN, INT_MAX);
	return suggested_move;
}

