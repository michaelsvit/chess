#include <stdlib.h>
#include "game_engine.h"

/******************************* Interface functions *********************************/

Game *create_game(Mode mode, int difficulty, Color player1_color){
	if(difficulty < 1 || difficulty > 4) return NULL;
	Game *game = malloc(sizeof(Game));
	if(!game) return NULL;
	game->white_pieces = spArrayListCreate(sizeof(GamePiece), ARRAY_SIZE);
	if(!game->white_pieces){
		free(game);
		return NULL;
	}
	game->black_pieces = spArrayListCreate(sizeof(GamePiece), ARRAY_SIZE);
	if(!game->black_pieces){
		spArrayListDestroy(game->white_pieces);
		free(game);
		return NULL;
	}
	game->current_player = PLAYER1;
	game->mode = mode;
	game->difficulty = difficulty;
	game->player_color[PLAYER1] = player1_color;
	game->player_color[PLAYER2] = !player1_color;
	return game;
}

void destroy_game(Game *game){
	if(!game) return;

	/* Free all allocated memory */
	for(int i = 0; i < spArrayListSize(game->white_pieces); i++){
		GamePiece *piece = spArrayListGetAt(game->white_pieces, i);
		free(piece);
	}
	for(int i = 0; i < spArrayListSize(game->black_pieces); i++){
		GamePiece *piece = (GamePiece *)spArrayListGetAt(game->black_pieces, i);
		free(piece);
	}
	spArrayListDestroy(game->white_pieces);
	spArrayListDestroy(game->black_pieces);
	free(game);
}

EngineMessage move_game_piece(Game *game, GamePiece *piece, int pos_x, int pos_y){
	if(
		!game || !piece ||
		pos_x < 0 || pos_x > BOARD_SIZE-1 ||
		pos_y < 0 || pos_y > BOARD_SIZE-1
	) return INVALID_ARGUMENT;
	if(!is_legal_move(game, piece, pos_x, pos_y)) return ILLEGAL_MOVE;

	if(is_occupied_position(game, pos_x, pos_x)){
		remove_game_piece(game, game->board[pos_y][pos_x]);
	}

	/* Move piece to the given position */
	piece->pos_y = pos_y;
	piece->pos_x = pos_x;
	game->board[pos_y][pos_x] = piece;
	return SUCCESS;
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
			break;
		case QUEEN:
			break;
		case KING:
			break;
	}


	/* If last turn ended with a check then current player must resolve it */
	return 1;
}

int is_legal_pawn_move(Game *game, GamePiece *piece, int pos_x, int pos_y){
	/* Determine which direction pawn should move */
	int direction = (piece->color == WHITE) ? 1 : -1;

	/* Make sure pawn is moving exactly 1 row in the correct direction */
	if(pos_y != piece->pos_y + direction * 1) return 0;

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
	return 1;
}

int is_legal_rook_move(Game *game, GamePiece *piece, int pos_x, int pos_y){
	/* Make sure rook is moving in a straight line */
	if(pos_x != piece->pos_x && pos_y != piece->pos_y) return 0;

	/* If target position is occupied, make sure target piece is of different color */
	GamePiece *target_piece = game->board[pos_y][pos_x];
	if(target_piece && target_piece->color == piece->color) return 0;

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

	/* If target position is occupied, make sure target piece is of different color */
	GamePiece *target_piece = game->board[pos_y][pos_x];
	if(target_piece && target_piece->color == piece->color) return 0;

	return 1;
}
