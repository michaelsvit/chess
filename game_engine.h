#ifndef GAME_H
#define GAME_H
/*
 * Game engine module containing state and interaction related functions
 *
 * Summary of interface:
 * create_game       	 create new game instance
 * destroy_game      	 free all allocated memory and destroy game instance
 * init_game         	 initialize a new game instance with pieces in default slots
 * remove_game_piece 	 remove a game piece from the board
 * move_game_piece   	 move a game piece from current place on board to another
 * is_legal_move     	 checks whether a given move is valid for a given game piece
 * is_game_over      	 checks if the given game instance reached a final state
 */
#include "array_list.h"

#define BOARD_SIZE 8
#define ARRAY_SIZE 16

typedef enum {
	MALLOC_FAILURE,
	INVALID_ARGUMENT,
	ILLEGAL_MOVE,
	SUCCESS
} EngineMessage;
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

/* Struct defining a game board with relevant properties */
typedef struct game_t{
	GamePiece *board[BOARD_SIZE][BOARD_SIZE]; /* current game board */
	SPArrayList *white_pieces, *black_pieces; /* lists representing white and black game pieces on the board */
	Player current_player;
	Color player_color[2];
	Mode mode;
	int difficulty;
} Game;

/*
 * Creates a new game instance.
 * @param mode          	 one player or two player mode
 * @param difficulty    	 chosen game difficulty, between 1 and 4
 * @param player1_color 	 first player's game piece color, second player is the inverse color
 * @return              	 pointer to the newly created game on success, NULL otherwise
 */
Game *create_game(Mode mode, int difficulty, Color player1_color);

/*
 * Destroy an existing game instance and free all allocated memory.
 */
void destroy_game();

/*
 * Initialize game board with all of the pieces in their default slots.
 * @precond    	 game != NULL
 * @param game 	 existing game board
 * @return     	 message indicating success or failure
 */
EngineMessage init_game(Game *game);

/*
 * Move a game piece from its current place to the given coordinates.
 * @param game  	 game instance
 * @param piece 	 game piece to be moved
 * @param pos_x 	 column to move the piece onto
 * @param pos_y 	 row to move the piece onto
 * @return
 * INVALID_ARGUMENT 	 game == NULL or piece == NULL or coordinates out of bounds
 * ILLEGAL_MOVE     	 move is not a legal move as defined by chess rules
 * SUCCESS          	 otherwise
 */
EngineMessage move_game_piece(Game *game, GamePiece *piece, int pos_x, int pos_y);

/*
 * Check whether a given move is a legal chess move.
 * @precond     	 assumes arguments are valid
 * @param game  	 game instance
 * @param piece 	 game piece to be moved
 * @param pos_x 	 column to move the piece onto
 * @param pos_y 	 row to move the piece onto
 * @return      	 true iff move is legal
 */
int is_legal_move(Game *game, GamePiece *piece, int pos_x, int pos_y);
#endif
