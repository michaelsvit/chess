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
#define PLAYER_COUNT 2

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
	Color player_color[PLAYER_COUNT];
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
 * Copies an existing game instance.
 * @param game 	 game to be copied
 * @return     	 pointer to the generated copy on success, NULL otherwise
 */
Game *copy_game(Game *game);

/*
 * Destroy an existing game instance and free all allocated memory.
 */
void destroy_game();

/*
 * Move a game piece from its current place to the given coordinates.
 * @param game  	 game instance
 * @param src_x 	 column of source position
 * @param src_y 	 row of source position
 * @param dst_x 	 column of target position
 * @param dst_y 	 row of target position
 * @return
 * INVALID_ARGUMENT 	 game == NULL or piece == NULL or coordinates out of bounds
 * ILLEGAL_MOVE     	 move is not a legal move as defined by chess rules
 * SUCCESS          	 otherwise
 */
EngineMessage move_game_piece(Game *game, int src_x, int src_y, int dst_x, int dst_y);

/*******************************************************************************************/
/**************** Auxiliary functions - should not be called outside this module ***********/
/*******************************************************************************************/

/*
 * Create a single game piece of the given type and color and initialize its position
 * to the given coordinates.
 * @param type  	 type of the piece to be created
 * @param color 	 color of the piece to be created
 * @param pos_x 	 column position of the piece on the board
 * @param pos_y 	 row position of the piece on the board
 * @return      	 pointer to the newly created piece on success, NULL otherwise
 */
GamePiece *create_game_piece(PieceType type, Color color, int pos_x, int pos_y);

/*
 * Initialize game board with all of the pieces in their default slots.
 * @precond    	 game != NULL
 * @param game 	 existing game board
 * @return
 * MALLOC_FAILURE 	 if allocating memory failed
 * SUCCESS        	 otherwise
 */
EngineMessage init_game(Game *game);

/*
 * Add a single game piece of the given type and color to the given position on the board,
 * and add it to the list of pieces of the given color.
 * @precond     	 game != NULL
 * @param game  	 existing game instance
 * @param type  	 type of the piece to be added
 * @param color 	 color of the piece to be added
 * @param pos_x 	 column position of the piece on the board
 * @param pos_y 	 row position of the piece on the board
 * @return
 * MALLOC_FAILURE 	 if allocating memory failed
 * SUCCESS        	 otherwise
 */
EngineMessage add_game_piece(Game *game, PieceType type, Color color, int pos_x, int pos_y);

/*
 * Remove a game piece from the given game instance.
 * @precond     	 game != NULL
 * @param game  	 existing game instance
 * @param piece 	 game piece to be removed from the board
 */
void remove_game_piece(Game *game, GamePiece *piece);

/*
 * Add all game pieces of a set with the given color.
 * @precond     	 game != NULL
 * @param game  	 existing game instance
 * @param color 	 color of the set to be added
 * @return
 * MALLOC_FAILURE 	 if allocating memory failed
 * SUCCESS        	 otherwise
 */
EngineMessage add_game_pieces_set(Game *game, Color color);

/*
 * Check whether a given position on the board is occupied.
 * @param game  	 game instance
 * @param pos_x 	 column of the position
 * @param pos_y 	 row of the position
 * @return      	 true iff position is occupied
 */
int is_occupied_position(Game *game, int pos_x, int pos_y);

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

/*
 * Check if a given pawn move is legal.
 * @precond     	 assumes arguments are valid
 * @param game  	 game instance
 * @param piece 	 pawn to be moved
 * @param pos_x 	 column to move the pawn onto
 * @param pos_y 	 row to move the pawn onto
 * @return      	 true iff move is legal
 */
int is_legal_pawn_move(Game *game, GamePiece *piece, int pos_x, int pos_y);

/*
 * Check if a given rook move is legal.
 * @precond     	 assumes arguments are valid
 * @param game  	 game instance
 * @param piece 	 rook to be moved
 * @param pos_x 	 column to move the rook onto
 * @param pos_y 	 row to move the rook onto
 * @return      	 true iff move is legal
 */
int is_legal_rook_move(Game *game, GamePiece *piece, int pos_x, int pos_y);

/*
 * Check if a given knight move is legal.
 * @precond     	 assumes arguments are valid
 * @param game  	 game instance
 * @param piece 	 knight to be moved
 * @param pos_x 	 column to move the knight onto
 * @param pos_y 	 row to move the knight onto
 * @return      	 true iff move is legal
 */
int is_legal_knight_move(Game *game, GamePiece *piece, int pos_x, int pos_y);

/*
 * Check if a given bishop move is legal.
 * @precond     	 assumes arguments are valid
 * @param game  	 game instance
 * @param piece 	 bishop to be moved
 * @param pos_x 	 column to move the bishop onto
 * @param pos_y 	 row to move the bishop onto
 * @return      	 true iff move is legal
 */
int is_legal_bishop_move(Game *game, GamePiece *piece, int pos_x, int pos_y);

/*
 * Check if a given queen move is legal.
 * @precond     	 assumes arguments are valid
 * @param game  	 game instance
 * @param piece 	 queen to be moved
 * @param pos_x 	 column to move the queen onto
 * @param pos_y 	 row to move the queen onto
 * @return      	 true iff move is legal
 */
int is_legal_queen_move(Game *game, GamePiece *piece, int pos_x, int pos_y);

/*
 * Check if a given king move is legal.
 * @precond     	 assumes arguments are valid
 * @param game  	 game instance
 * @param piece 	 king to be moved
 * @param pos_x 	 column to move the king onto
 * @param pos_y 	 row to move the king onto
 * @return      	 true iff move is legal
 */
int is_legal_king_move(Game *game, GamePiece *piece, int pos_x, int pos_y);

/*
 * Perform actual moving of game piece to new position.
 * @precond     	 assumes arguments are valid
 * @param game  	 game instance
 * @param piece 	 game piece to be moved
 * @param pos_x 	 column to move the piece onto
 * @param pos_y  	 row to move the piece onto
 */
void move_piece_to_position(Game *game, GamePiece *piece, int pos_x, int pos_y);

/*
 * Find king piece in given set of pieces.
 * @precond   	 assumes arguments are valid
 * @param set 	 set of game pieces to search in
 * @return    	 pointer to king piece in given set
 */
GamePiece *find_king_piece(SPArrayList *set);
#endif
