#include <stdlib.h>
#include "game_engine.h"

/* Auxiliary function declarations */
GamePiece *create_game_piece(PieceType type, Color color, int pos_x, int pos_y);
void remove_game_piece(Game *game, GamePiece *piece);
EngineMessage add_game_piece(Game *game, PieceType type, Color color, int pos_x, int pos_y);
EngineMessage add_game_pieces_set(Game *game, Color color);
int is_occupied_position(Game *game, int pos_x, int pos_y);

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

EngineMessage init_game(Game *game){
	/* Create all pieces and place them on the board */
	EngineMessage msg;
	msg = add_game_pieces_set(game, WHITE);
	if(msg != SUCCESS) return msg;
	msg = add_game_pieces_set(game, BLACK);
	return msg;
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

/*
 * Create a single game piece of the given type and color and initialize its position
 * to the given coordinates.
 * @param type  	 type of the piece to be created
 * @param color 	 color of the piece to be created
 * @param pos_x 	 column position of the piece on the board
 * @param pos_y 	 row position of the piece on the board
 * @return      	 pointer to the newly created piece on success, NULL otherwise
 */
GamePiece *create_game_piece(PieceType type, Color color, int pos_x, int pos_y){
	GamePiece *piece = (GamePiece *)malloc(sizeof(GamePiece));
	if(!piece) return NULL;
	piece->type = type;
	piece->color = color;
	piece->pos_x = pos_x;
	piece->pos_y = pos_y;
	return piece;
}

/*
 * Add a single game piece of the given type and color to the given position on the board,
 * and add it to the list of pieces of the given color.
 * @precond     	 game != NULL
 * @param game  	 existing game instance
 * @param type  	 type of the piece to be added
 * @param color 	 color of the piece to be added
 * @param pos_x 	 column position of the piece on the board
 * @param pos_y 	 row position of the piece on the board
 * @return      	 message indicating success or failure
 */
EngineMessage add_game_piece(Game *game, PieceType type, Color color, int pos_x, int pos_y){
	GamePiece *pawn = create_game_piece(type, color, pos_x, pos_y);
	if(!pawn) return MALLOC_FAILURE;
	spArrayListAddLast(game->white_pieces, pawn); /* cannot fail on a new board */
	game->board[pos_x][pos_y] = pawn;
	return SUCCESS;
}

/*
 * Remove a game piece from the given game instance.
 * @precond     	 game != NULL
 * @param game  	 existing game instance
 * @param piece 	 game piece to be removed from the board
 */
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

/*
 * Add all game pieces of a set with the given color.
 * @precond     	 game != NULL
 * @param game  	 existing game instance
 * @param color 	 color of the set to be added
 * @return      	 message indicating success or failure
 */
EngineMessage add_game_pieces_set(Game *game, Color color){
	EngineMessage msg;
	/* Add pawns */
	int row = color == WHITE ? 1 : BOARD_SIZE-2;
	for(int i = 0; i < BOARD_SIZE; i++){
		msg = add_game_piece(game, PAWN, color, i, row);
		if(msg != SUCCESS) return msg;
	}

	row = color == WHITE ? 0 : BOARD_SIZE-1;
	/* Add bishops */
	if((msg = add_game_piece(game, BISHOP, color, 0, row)) != SUCCESS) return msg;
	if((msg = add_game_piece(game, BISHOP, color, BOARD_SIZE-1, row)) != SUCCESS) return msg;

	/* Add knights */
	if((msg = add_game_piece(game, KNIGHT, color, 1, row)) != SUCCESS) return msg;
	if((msg = add_game_piece(game, KNIGHT, color, BOARD_SIZE-2, row)) != SUCCESS) return msg;

	/* Add rooks */
	if((msg = add_game_piece(game, ROOK, color, 2, row)) != SUCCESS) return msg;
	if((msg = add_game_piece(game, ROOK, color, BOARD_SIZE-3, row)) != SUCCESS) return msg;

	/* Add queen */
	if((msg = add_game_piece(game, QUEEN, color, 4, row)) != SUCCESS) return msg;

	/* Add king */
	if((msg = add_game_piece(game, KING, color, 3, row)) != SUCCESS) return msg;
	return SUCCESS;
}

/*
 * Check whether a given position on the board is occupied.
 * @param game  	 game instance
 * @param pos_x 	 column of the position
 * @param pos_y 	 row of the position
 * @return      	 true iff position is occupied
 */
int is_occupied_position(Game *game, int pos_x, int pos_y){
	return game->board[pos_y][pos_x] != NULL;
}
