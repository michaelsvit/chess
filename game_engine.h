#ifndef GAME_H
#define GAME_H
/*
 * Game engine module containing state and interaction related functions
 */
#include "SPArrayList.h"

#define BOARD_SIZE 8
#define ARRAY_SIZE 16

typedef enum {PLAYER1, PLAYER2} Player;
typedef enum {PAWN, BISHOP, ROOK, KNIGHT, QUEEN, KING} PieceType;
typedef enum {BLACK, WHITE} Color;
typedef enum {ONE_PLAYER, TWO_PLAYER} Mode;

/* Struct defining a single game piece on the board */
typedef struct game_piece{
	PieceType type;
	Color color;
	int pos_y;
	int pos_x;
} GamePiece;

/* Struct defining a board game with relevant properties */
typedef struct game_t{
	GamePiece *board[BOARD_SIZE][BOARD_SIZE]; /* current game board */
	SPArrayList white_pieces, black_pieces; /* lists representing white and black game pieces on the board */
	Player current_player;
	Color player_color[2];
	Mode mode;
	int difficulty;
} Game;

/*
 * Creates a new game board.
 * @param mode          	 one player or two player mode
 * @param difficulty    	 chosen game difficulty, between 1 and 4
 * @param player1_color 	 first player's game piece color, second player is the inverse color
 * @return              	 pointer to the newly created game on success, NULL otherwise
 */
Game *create_game(Mode mode, int difficulty, Color player1_color);

/*
 * Destroy an existing game board.
 */
void destroy_game();
#endif
