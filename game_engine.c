#include <stdlib.h>
#include "game_engine.h"

/******************************* Interface functions *********************************/

Game *create_game(Mode mode, int difficulty, Color player1_color){
	if(difficulty < 1 || difficulty > 4) return NULL;
	Game *game = malloc(sizeof(Game));
	if(!game) return NULL;

	game->white_pieces = spArrayListCreate(sizeof(GamePiece), ARRAY_SIZE);
	game->black_pieces = spArrayListCreate(sizeof(GamePiece), ARRAY_SIZE);
	game->history = spArrayListCreate(sizeof(GameMove), HISTORY_SIZE);
	if(!game->white_pieces || !game->black_pieces || !game->history){
		destroy_game(game);
		return NULL;
	}

	game->current_player = PLAYER1;
	game->mode = mode;
	game->difficulty = difficulty;
	game->check = 0;
	game->player_color[PLAYER1] = player1_color;
	game->player_color[PLAYER2] = !player1_color;
	return game;
}

Game *copy_game(Game *game){
	if(!game) return NULL;

	Game *copy = malloc(sizeof(Game));
	if(!copy) return NULL;

	copy->white_pieces = spArrayListCopy(game->white_pieces);
	copy->black_pieces = spArrayListCopy(game->black_pieces);
	copy->history = spArrayListCopy(game->history);
	if(!copy->white_pieces || !copy->black_pieces || !copy->history){
		destroy_game(copy);
		return NULL;
	}

	/* Place copied pieces on new board */
	for(int i = 0; i < spArrayListSize(copy->white_pieces); i++){
		GamePiece *piece = spArrayListGetAt(copy->white_pieces, i);
		copy->board[piece->pos_y][piece->pos_x] = piece;
	}
	for(int i = 0; i < spArrayListSize(copy->black_pieces); i++){
		GamePiece *piece = spArrayListGetAt(copy->black_pieces, i);
		copy->board[piece->pos_y][piece->pos_x] = piece;
	}

	for(int i = 0; i < PLAYER_COUNT; i++){
		copy->player_color[i] = game->player_color[i];
	}
	copy->mode = game->mode;
	copy->difficulty = game->difficulty;
	copy->check = game->check;
	return copy;
}

void destroy_game(Game *game){
	if(!game) return;

	/* Free all allocated memory */
	if(game->white_pieces){
		for(int i = 0; i < spArrayListSize(game->white_pieces); i++){
			GamePiece *piece = spArrayListGetAt(game->white_pieces, i);
			free(piece);
		}
	}
	if (game->black_pieces) {
		for(int i = 0; i < spArrayListSize(game->black_pieces); i++){
			GamePiece *piece = (GamePiece *)spArrayListGetAt(game->black_pieces, i);
			free(piece);
		}
	}
	if (game->history) {
		for(int i = 0; i < spArrayListSize(game->history); i++){
			GameMove *move = (GameMove *)spArrayListGetAt(game->history, i);
			free(move);
		}
	}
	spArrayListDestroy(game->white_pieces);
	spArrayListDestroy(game->black_pieces);
	free(game);
}

EngineMessage move_game_piece(Game *game, int src_x, int src_y, int dst_x, int dst_y){
	if(!game) return INVALID_ARGUMENT;
	GamePiece *piece = game->board[src_y][src_x];
	if(!piece || dst_x < 0 || dst_x > BOARD_SIZE-1 || dst_y < 0 || dst_y > BOARD_SIZE-1){
		return INVALID_ARGUMENT;
	}
	int result = is_legal_move(game, piece, dst_x, dst_y);
	if(!result){
		return ILLEGAL_MOVE;
	} else if (result == -1) {
		return MALLOC_FAILURE;
	}

	move_piece_to_position(game, piece, dst_x, dst_y);
	if(add_move_to_history(game, src_x, src_y, dst_x, dst_y) != SUCCESS) return MALLOC_FAILURE;
	/* Determine if moving the piece ended with check state for enemy king */
	game->check = is_check_state_created_enemy(game, piece);

	return SUCCESS;
}

SPArrayList *get_possible_moves(Game *game, GamePiece *piece){
	if(!game || !piece) return NULL;

	SPArrayList *moves = NULL;

	switch(piece->type){
		case PAWN:
			moves = get_pawn_moves(game, piece);
			break;
		case ROOK:
			moves = get_rook_moves(game, piece);
			break;
		case KNIGHT:
			moves = get_knight_moves(game, piece);
			break;
		case BISHOP:
			moves = get_bishop_moves(game, piece);
			break;
		case QUEEN:
			moves = get_queen_moves(game, piece);
			break;
		case KING:
			moves = get_king_moves(game, piece);
			break;
	}

	return moves;
}

/******************************** Auxiliary functions ******************************/

GamePiece *create_game_piece(PieceType type, Color color, int pos_x, int pos_y){
	GamePiece *piece = (GamePiece *)malloc(sizeof(GamePiece));
	if(!piece) return NULL;
	piece->type = type;
	piece->color = color;
	piece->pos_x = pos_x;
	piece->pos_y = pos_y;
	return piece;
}

EngineMessage init_game(Game *game){
	/* Create all pieces and place them on the board */
	EngineMessage msg;
	msg = add_game_pieces_set(game, WHITE);
	if(msg != SUCCESS) return msg;
	msg = add_game_pieces_set(game, BLACK);
	return msg;
}

EngineMessage add_game_piece(Game *game, PieceType type, Color color, int pos_x, int pos_y){
	GamePiece *pawn = create_game_piece(type, color, pos_x, pos_y);
	if(!pawn) return MALLOC_FAILURE;
	spArrayListAddLast(game->white_pieces, pawn); /* cannot fail on a new board */
	game->board[pos_x][pos_y] = pawn;
	return SUCCESS;
}

void remove_game_piece(Game *game, GamePiece *piece){
	if(!piece) return;

	/* Remove piece from list of game pieces */
	if(piece->color == WHITE){
		spArrayListRemoveItem(game->white_pieces, piece);
	} else {
		spArrayListRemoveItem(game->black_pieces, piece);
	}

	/* Remove piece from game board */
	game->board[piece->pos_y][piece->pos_x] = NULL;
	free(piece);
}

EngineMessage add_game_pieces_set(Game *game, Color color){
	EngineMessage msg;
	/* Add pawns */
	int row = color == WHITE ? 1 : BOARD_SIZE-2;
	for(int i = 0; i < BOARD_SIZE; i++){
		msg = add_game_piece(game, PAWN, color, i, row);
		if(msg != SUCCESS) return msg;
	}

	row = color == WHITE ? 0 : BOARD_SIZE-1;
	/* Add rooks */
	if((msg = add_game_piece(game, ROOK, color, 0, row)) != SUCCESS) return msg;
	if((msg = add_game_piece(game, ROOK, color, BOARD_SIZE-1, row)) != SUCCESS) return msg;

	/* Add knights */
	if((msg = add_game_piece(game, KNIGHT, color, 1, row)) != SUCCESS) return msg;
	if((msg = add_game_piece(game, KNIGHT, color, BOARD_SIZE-2, row)) != SUCCESS) return msg;

	/* Add bishops */
	if((msg = add_game_piece(game, BISHOP, color, 2, row)) != SUCCESS) return msg;
	if((msg = add_game_piece(game, BISHOP, color, BOARD_SIZE-3, row)) != SUCCESS) return msg;

	/* Add queen */
	if((msg = add_game_piece(game, QUEEN, color, 4, row)) != SUCCESS) return msg;

	/* Add king */
	if((msg = add_game_piece(game, KING, color, 3, row)) != SUCCESS) return msg;
	return SUCCESS;
}

int is_occupied_position(Game *game, int pos_x, int pos_y){
	return game->board[pos_y][pos_x] != NULL;
}

int is_legal_move(Game *game, GamePiece *piece, int pos_x, int pos_y){
	/* Make sure piece is the same color as current player */
	if(game->player_color[game->current_player] != piece->color) return 0;

	/* If target position is occupied, make sure target piece is of different color */
	GamePiece *target_piece = game->board[pos_y][pos_x];
	if(target_piece && target_piece->color == piece->color) return 0;

	/* Handle each piece type according to game rules */
	switch(piece->type){
		case PAWN:
			if(!is_legal_pawn_move(game, piece, pos_x, pos_y)) return 0;
			break;
		case ROOK:
			if(!is_legal_rook_move(game, piece, pos_x, pos_y)) return 0;
			break;
		case KNIGHT:
			if(!is_legal_knight_move(game, piece, pos_x, pos_y)) return 0;
			break;
		case BISHOP:
			if(!is_legal_bishop_move(game, piece, pos_x, pos_y)) return 0;
			break;
		case QUEEN:
			if(!is_legal_queen_move(game, piece, pos_x, pos_y)) return 0;
			break;
		case KING:
			if(!is_legal_king_move(game, piece, pos_x, pos_y)) return 0;
			break;
	}

	/* Make sure moving the piece doesn't end with a check state for allied king */
	int result = is_check_state_created_allied(game, piece, pos_x, pos_y);
	if(result){
		return 0;
	} else if (result == -1) {
		return -1;
	}

	return 1;
}

int is_legal_pawn_move(Game *game, GamePiece *piece, int pos_x, int pos_y){
	/* Determine which direction pawn should move */
	int direction = (piece->color == WHITE) ? 1 : -1;

	/* Make sure pawn is moving exactly 1 or 2 rows in the correct direction */
	if(pos_y != piece->pos_y + direction*1 && pos_y != piece->pos_y + direction*2) return 0;

	if(pos_y == piece->pos_y + direction*1){
		/* Make sure pawn is moving no more than 1 column away from current position */
		if(piece->pos_x == pos_x){
			/* Pawn stays in same column, target position must be unoccupied */
			if (is_occupied_position(game, pos_x, pos_y)) return 0;
		} else if (abs(pos_x - piece->pos_x) > 1) {
			/* Pawn is trying to move more than 1 column away from current position */
			return 0;
		} else {
			/* Pawn is moving diagonally */
			/* Position must be occupied by a piece with the other color */
			GamePiece *target_piece = game->board[pos_y][pos_x];
			if(!target_piece || piece->color == target_piece->color) return 0;
		}
	} else {
		/* Pawn is moving 2 rows, position must be initial position */
		int init_position = (piece->color == WHITE) ? 1 : BOARD_SIZE - 2;
		if(pos_y != init_position) return 0;
		/* Pawn stays in same column, target position must be unoccupied */
		if (is_occupied_position(game, pos_x, pos_y)) return 0;
	}
	return 1;
}

int is_legal_rook_move(Game *game, GamePiece *piece, int pos_x, int pos_y){
	/* Make sure rook is moving in a straight line */
	if(pos_x != piece->pos_x && pos_y != piece->pos_y) return 0;

	/* Make sure no other pieces are inbetween */
	if (pos_x == piece->pos_x) {
		if (piece->pos_y < pos_y) {
			for(int i = piece->pos_y; i < pos_y; i++){
				if(is_occupied_position(game, pos_x, i)) return 0;
			}
		} else {
			for(int i = pos_y; i > piece->pos_y; i--){
				if(is_occupied_position(game, pos_x, i)) return 0;
			}
		}
	} else {
		if (piece->pos_x < pos_x) {
			for(int i = piece->pos_x; i < pos_x; i++){
				if(is_occupied_position(game, i, pos_y)) return 0;
			}
		} else {
			for(int i = pos_x; i > piece->pos_x; i--){
				if(is_occupied_position(game, i, pos_y)) return 0;
			}
		}
	}
	return 1;
}

int is_legal_knight_move(Game *game, GamePiece *piece, int pos_x, int pos_y){
	/* Make sure knight is moving at least 1 step in either direction */
	if(pos_x == piece->pos_x || pos_y == piece->pos_y) return 0;

	/* Make sure sum of steps in both directions is 3 */
	/* (so that it moves 1 step in one direction and 2 steps in the other direction) */
	if(abs(pos_x - piece->pos_x) + abs(pos_y - piece->pos_y) != 3) return 0;

	return 1;
}

int is_legal_bishop_move(Game *game, GamePiece *piece, int pos_x, int pos_y){
	/* Make sure bishop is moving diagonally */
	if(abs(pos_x - piece->pos_x) != abs(pos_y - piece->pos_y)) return 0;

	/* Make sure there are no other pieces inbetween */
	int direction_x = (piece->pos_x < pos_x) ? 1 : -1;
	int direction_y = (piece->pos_y < pos_y) ? 1 : -1;
	int dist = abs(piece->pos_x - pos_x);
	for(int i = 1; i <= dist; i++){
		int cur_pos_x = piece->pos_x + direction_x * i;
		int cur_pos_y = piece->pos_y + direction_y * i;
		if(is_occupied_position(game, cur_pos_x, cur_pos_y)) return 0;
	}
	return 1;
}

int is_legal_queen_move(Game *game, GamePiece *piece, int pos_x, int pos_y){
	/* Queen can move in the same way as either a rook or a bishop */
	return is_legal_rook_move(game, piece, pos_x, pos_y) ||
		is_legal_bishop_move(game, piece, pos_x, pos_y);
}

int is_legal_king_move(Game *game, GamePiece *piece, int pos_x, int pos_y){
	/* Make sure king moves exactly one step in any direction */
	int dist_x = abs(piece->pos_x - pos_x);
	int dist_y = abs(piece->pos_y - pos_y);
	if(dist_x > 1 || dist_y > 1 || dist_x + dist_y == 0) return 0;

	return 1;
}

int is_check_state_created_allied(Game *game, GamePiece *piece, int pos_x, int pos_y){
	Game *copy = copy_game(game);
	if(!copy) return -1;
	SPArrayList *same_color_pieces;
	SPArrayList *enemy_pieces;
	if(piece->color == WHITE){
		same_color_pieces = copy->white_pieces;
		enemy_pieces = copy->black_pieces;
	} else {
		same_color_pieces = copy->black_pieces;
		enemy_pieces = copy->white_pieces;
	}
	/* Get copied game piece */
	GamePiece *piece_copy = copy->board[piece->pos_y][piece->pos_x];
	/* Move copied piece to new position */
	move_piece_to_position(copy, piece_copy, pos_x, pos_y);
	copy->current_player = !copy->current_player;
	/* Iterate through all enemy pieces and check if they threaten allied king */
	GamePiece *king_copy = find_king_piece(same_color_pieces);
	for(int i = 0; i < spArrayListSize(enemy_pieces); i++){
		GamePiece *temp = (GamePiece *)spArrayListGetAt(enemy_pieces, i);
		if(is_legal_move(copy, temp, king_copy->pos_x, king_copy->pos_y)){
			destroy_game(copy);
			return 0;
		}
	}
	destroy_game(copy);
	return 1;
}

int is_check_state_created_enemy(Game *game, GamePiece *piece){
	SPArrayList *enemy_pieces = (piece->color == WHITE) ? game->white_pieces : game->black_pieces;
	GamePiece *enemy_king = find_king_piece(enemy_pieces);
	return is_legal_move(game, piece, enemy_king->pos_x, enemy_king->pos_y);
}

void move_piece_to_position(Game *game, GamePiece *piece, int pos_x, int pos_y){
	if(is_occupied_position(game, pos_x, pos_x)){
		remove_game_piece(game, game->board[pos_y][pos_x]);
	}

	/* Move piece to the given position */
	piece->pos_y = pos_y;
	piece->pos_x = pos_x;
	game->board[pos_y][pos_x] = piece;
}

GamePiece *find_king_piece(SPArrayList *set){
	for(int i = 0; i < spArrayListSize(set); i++){
		GamePiece *temp = (GamePiece *)spArrayListGetAt(set, i);
		if(temp->type == KING) return temp;
	}
	return NULL; /* should not be reached */
}

EngineMessage add_move_to_history(Game *game, int src_x, int src_y, int dst_x, int dst_y){
	if(spArrayListIsFull(game->history)) spArrayListRemoveLast(game->history);
	GameMove *move = create_move(src_x, src_y, dst_x, dst_y);
	if(!move) return MALLOC_FAILURE;
	spArrayListAddFirst(game->history, move);
	return SUCCESS;
}

GameMove *create_move(int src_x, int src_y, int dst_x, int dst_y){
	GameMove *move = malloc(sizeof(GameMove));
	if(!move) return NULL;
	move->src_x = src_x;
	move->src_y = src_y;
	move->dst_x = dst_x;
	move->dst_y = dst_y;
	return move;
}

int is_game_over(Game *game){
	/* Check if current player has any possible move */
	/* Seperate handling if in check state to improve performance */
	if(game->check){
		/* Need to be able to resolve check */
		/* TODO */
	}
}

SPArrayList *get_pawn_moves(Game *game, GamePiece *piece){
	SPArrayList *moves = spArrayListCreate(sizeof(GameMove), MAX_PAWN_MOVES);
	if(!moves) return NULL;

	for(int i = -1; i <= 1; i++){
		if(is_legal_move(game, piece, piece->pos_x+i, piece->pos_y+1)){
			GameMove *move = create_move(
					piece->pos_x,
					piece->pos_y,
					piece->pos_x+i,
					piece->pos_y+1);
			if(!move){
				spArrayListDestroy(moves);
				return NULL;
			}
			spArrayListAddLast(moves, move);
		}
	}
	if(is_legal_move(game, piece, piece->pos_x, piece->pos_y+2)){
		GameMove *move = create_move(
				piece->pos_x,
				piece->pos_y,
				piece->pos_x,
				piece->pos_y+2);
		if(!move){
			spArrayListDestroy(moves);
			return NULL;
		}
		spArrayListAddLast(moves, move);
	}
	return moves;
}

SPArrayList *get_rook_moves(Game *game, GamePiece *piece){
	SPArrayList *moves = spArrayListCreate(sizeof(GameMove), MAX_ROOK_MOVES);
	if(!moves) return NULL;

	/* Add moves along column */
	int start_index = find_range_end(game, piece->pos_x, piece->pos_y, 0, -1);
	int end_index = find_range_end(game, piece->pos_x, piece->pos_y, 0, 1);
	for(int i = start_index; i <= end_index; i++){
		if(i == 0) continue;
		if(is_legal_move(game, piece, piece->pos_x, i)){
			GameMove *move = create_move(
					piece->pos_x,
					piece->pos_y,
					piece->pos_x,
					piece->pos_y+i);
			if(!move){
				spArrayListDestroy(moves);
				return NULL;
			}
			spArrayListAddLast(moves, move);
		}
	}

	/* Add moves along row */
	start_index = find_range_end(game, piece->pos_x, piece->pos_y, -1, 0);
	end_index = find_range_end(game, piece->pos_x, piece->pos_y, 1, 0);
	for(int i = start_index; i <= end_index; i++){
		if(i == 0) continue;
		if(is_legal_move(game, piece, i, piece->pos_y)){
			GameMove *move = create_move(
					piece->pos_x,
					piece->pos_y,
					piece->pos_x+i,
					piece->pos_y);
			if(!move){
				spArrayListDestroy(moves);
				return NULL;
			}
			spArrayListAddLast(moves, move);
		}
	}
	return moves;
}

SPArrayList *get_knight_moves(Game *game, GamePiece *piece){
	SPArrayList *moves = spArrayListCreate(sizeof(GameMove), MAX_KNIGHT_MOVES);
	if(!moves) return NULL;

	for(int i = -2; i <= 2; i++){
		if(i == 0) continue;
		int j = (abs(i) == 1) ? 2 : 1;
		if(is_legal_move(game, piece, piece->pos_x - j, piece->pos_y + i)){
			GameMove *move = create_move(
					piece->pos_x,
					piece->pos_y,
					piece->pos_x - j,
					piece->pos_y + i);
			if(!move){
				spArrayListDestroy(moves);
				return NULL;
			}
			spArrayListAddLast(moves, move);
		}
		if(is_legal_move(game, piece, piece->pos_x + j, piece->pos_y + i)){
			GameMove *move = create_move(
					piece->pos_x,
					piece->pos_y,
					piece->pos_x + j,
					piece->pos_y + i);
			if(!move){
				spArrayListDestroy(moves);
				return NULL;
			}
			spArrayListAddLast(moves, move);

		}
	}
	return moves;
}

SPArrayList *get_bishop_moves(Game *game, GamePiece *piece){
	SPArrayList *moves = spArrayListCreate(sizeof(GameMove), MAX_BISHOP_MOVES);
	if(!moves) return NULL;

	/* Add moves along upward diagonal */
	int start_index = find_range_end(game, piece->pos_x, piece->pos_y, -1, -1);
	int end_index = find_range_end(game, piece->pos_x, piece->pos_y, 1, 1);
	for(int i = start_index; i <= end_index; i++){
		if(i == 0) continue;
		if(is_legal_move(game, piece, piece->pos_x + i, piece->pos_y + i)){
			GameMove *move = create_move(
					piece->pos_x,
					piece->pos_y,
					piece->pos_x+i,
					piece->pos_y+i);
			if(!move){
				spArrayListDestroy(moves);
				return NULL;
			}
			spArrayListAddLast(moves, move);
		}
	}

	/* Add moves along downward diagonal */
	start_index = find_range_end(game, piece->pos_x, piece->pos_y, -1, 1);
	end_index = find_range_end(game, piece->pos_x, piece->pos_y, 1, -1);
	for(int i = start_index; i <= end_index; i++){
		if(i == 0) continue;
		if(is_legal_move(game, piece, piece->pos_x + i, piece->pos_y + i)){
			GameMove *move = create_move(
					piece->pos_x,
					piece->pos_y,
					piece->pos_x+i,
					piece->pos_y+i);
			if(!move){
				spArrayListDestroy(moves);
				return NULL;
			}
			spArrayListAddLast(moves, move);
		}
	}
	return moves;
}

SPArrayList *get_queen_moves(Game *game, GamePiece *piece){
	SPArrayList *moves = spArrayListCreate(sizeof(GameMove), MAX_QUEEN_MOVES);
	if(!moves) return NULL;

	SPArrayList *rook_moves = get_rook_moves(game, piece);
	if(!rook_moves){
		spArrayListDestroy(moves);
		return NULL;
	}
	SPArrayList *bishop_moves = get_bishop_moves(game, piece);
	if(!bishop_moves){
		spArrayListDestroy(rook_moves);
		spArrayListDestroy(moves);
		return NULL;
	}

	for(int i = 0; i < spArrayListSize(rook_moves); i++){
		spArrayListAddLast(moves, spArrayListGetAt(rook_moves, i));
	}
	for(int i = 0; i < spArrayListSize(bishop_moves); i++){
		spArrayListAddLast(moves, spArrayListGetAt(bishop_moves, i));
	}
	return moves;
}

SPArrayList *get_king_moves(Game *game, GamePiece *piece){
	SPArrayList *moves = spArrayListCreate(sizeof(GameMove), MAX_KING_MOVES);
	if(!moves) return NULL;

	for(int i = -1; i <= 1; i++){
		for(int j = -1; j <= 1; j++){
			if(i == 0 && j == 0) continue;
			if(is_legal_move(game, piece, piece->pos_x+i, piece->pos_y+j)){
				GameMove *move = create_move(
						piece->pos_x,
						piece->pos_y,
						piece->pos_x+i,
						piece->pos_y+j);
				if(!move){
					spArrayListDestroy(moves);
					return NULL;
				}
				spArrayListAddLast(moves, move);
			}
		}
	}
	return moves;
}

int find_range_end(Game *game, int src_x, int src_y, int inc_x, int inc_y){
	int i = 0, j = 0;
	/* Increment counters as long as board edge has not been reached or we found a game piece,
	whichever comes first */
	while(src_y+j >= 0 && src_y+j < BOARD_SIZE && src_x+i >= 0 && src_x+i < BOARD_SIZE
			&& game->board[src_y+i][src_x+j] == NULL){
		i += inc_x;
		j += inc_y;
	}
	if (inc_x == 0 || inc_y < 0) {
		return j;
	} else {
		return i;
	}
}
