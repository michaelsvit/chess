#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H
/*
 * Game engine module containing state and interaction related functions
 *
 * Summary of interface:
 * create_game        	 create new game instance
 * copy_game          	 copy an existing game instance
 * destroy_game       	 free all allocated memory and destroy game instance
 * move_game_piece    	 move a game piece from current place on board to another
 * undo_move          	 undo most recent move
 * get_possible_moves 	 get all possible moves for a given game piece
 * is_game_over       	 checks if the given game instance reached a final state
 */
#include <stdio.h>
#include "array_list.h"
#include "xml_serializer.h"

#define BOARD_SIZE 8
#define ARRAY_SIZE 16
#define PLAYER_COUNT 2
#define HISTORY_SIZE 6

#define MAX_PAWN_MOVES 4
#define MAX_ROOK_MOVES 14
#define MAX_KNIGHT_MOVES 8
#define MAX_BISHOP_MOVES 13
#define MAX_QUEEN_MOVES 27
#define MAX_KING_MOVES 8

typedef enum {
	MALLOC_FAILURE,
	INVALID_COMMAND,
	INVALID_ARGUMENT,
	ILLEGAL_MOVE,
	EMPTY_HISTORY,
	GAME_OVER,
	GAME_LOAD,
	START_GAME,
	RESTART,
	QUIT,
	SUCCESS,
	SUCCESS_NO_PRINT
} EngineMessage;
typedef enum {PLAYER1=0, PLAYER2} Player;
typedef enum {PAWN, BISHOP, ROOK, KNIGHT, QUEEN, KING} PieceType;
typedef enum {BLACK, WHITE} Color;
typedef enum {ONE_PLAYER=1, TWO_PLAYER} Mode;

typedef struct {
	Mode mode;
	int difficulty;
	Color player1_color;
} GameSettings;

/* Struct defining a single game piece on the board */
typedef struct {
	PieceType type;
	Color color;
	int pos_y;
	int pos_x;
} GamePiece;

/* Struct defining a game board with relevant properties */
typedef struct {
	GamePiece *board[BOARD_SIZE][BOARD_SIZE]; /* current game board */
	SPArrayList *white_pieces, *black_pieces; /* lists representing white and black game pieces on the board */
	SPArrayList *move_history; /* list to remember last 3 moves of each player */
	SPArrayList *removed_pieces; /* list to remember last 3 removed pieces of each player */
	Player current_player;
	Color player_color[PLAYER_COUNT];
	Mode mode;
	int difficulty;
	int check; /* determines whether last turn ended with a check */
} Game;

/* Struct defining a single game move from one position to another */
typedef struct {
	int src_x;
	int src_y;
	int dst_x;
	int dst_y;
} GameMove;

/*
 * Creates a new game instance.
 * @param settings 	 struct containing settings set by the user for the created game
 * @return         	 pointer to the newly created game on success, NULL otherwise
 */
Game *create_game(GameSettings *settings);

/*
 * Copies an existing game instance.
 * @param game 	 game to be copied
 * @return     	 pointer to the generated copy on success, NULL otherwise
 */
Game *copy_game(Game *game);

/*
 * Save game instance into given XML file.
 * @param game 	 game instance
 * @param file 	 file path (relative or absolute)
 * @return
 * INVALID_ARGUMENT 	 file can't be opened for writing
 * SUCCESS          	 otherwise
 */
EngineMessage save_game(Game *game, char *file);


/*
 * Load a saved game instance from given XML file.
 * @param in 	 XML file to be loaded
 * @return   	 pointer to loaded game on success, NULL otherwise
 */
Game *load_game(FILE *in);

/*
 * Destroy an existing game instance and free all allocated memory.
 */
void destroy_game();

/*
 * Copy given game move.
 * @param move 	 move to copy
 * @return     	 pointer to generated copy on success, NULL otherwise
 */
GameMove *copy_move(GameMove *move);

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
 * MALLOC_FAILURE   	 failed to allocate memory when checking if king move is valid
 * GAME_OVER        	 move succeeded and the game reached a final state
 * SUCCESS          	 otherwise
 */
EngineMessage move_game_piece(Game *game, int src_x, int src_y, int dst_x, int dst_y);

/*
 * Undo most recent move in the game.
 * @param game         	 game instance
 * @param removed_move 	 variable to store removed move for printing later
 * @return
 * INVALID_ARGUMENT 	 game == NULL
 * EMPTY_HISTORY    	 move history is empty
 * SUCCESS          	 otherwise
 */
EngineMessage undo_move(Game *game, GameMove **removed_move);

/*
 * Get a list of all possible moves of a given game piece.
 * @param moves 	 array to be filled with possible moves, will be set to NULL on failure
 * @param game  	 game instance
 * @param piece 	 game piece
 * @return
 * SUCCESS          	 given array was filled with possible moves
 * MALLOC_FAILURE   	 memory error occurred
 * ILLEGAL_MOVE     	 piece is NULL or current player doesn't own it
 * INVALID_ARGUMENT 	 game is NULL
 */
EngineMessage get_possible_moves(SPArrayList **moves, Game *game, GamePiece *piece);

/*
 * Check if given game piece is threatened by enemy after performing given move.
 * @param game  	 game instance
 * @param piece 	 game piece
 * @param move  	 game move (possibly of a game piece different than given piece)
 * @return      	 -1 on memory failure, true if piece is threatened after move, false otherwise
 */
int is_piece_threatened_after_move(Game *game, GamePiece *piece, GameMove *move);

/*
 * Determine whether the game has reached a final state.
 * @param game 	 game instance
 * @return     	 true iff game is over, -1 on memory failure
 */
int is_game_over(Game *game);

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
 * Check if given position is a valid board position.
 * @return 	 true iff position is valid
 */
int is_valid_position(int pos_x, int pos_y);

/*
 * Check whether a given move is a legal chess move.
 * @precond     	 assumes arguments are valid
 * @param game  	 game instance
 * @param piece 	 game piece to be moved
 * @param pos_x 	 column to move the piece onto
 * @param pos_y 	 row to move the piece onto
 * @return      	 -1 on malloc failure, true iff move is legal otherwise
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
int is_legal_knight_move(GamePiece *piece, int pos_x, int pos_y);

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
int is_legal_king_move(GamePiece *piece, int pos_x, int pos_y);

/*
 * Check if given move creates a check state for the allied king of the given piece.
 * @precond     	 assumes arguments are valid
 * @param game  	 game instance
 * @param piece 	 piece to be moved
 * @param pos_x 	 column to move the piece onto
 * @param pos_y 	 row to move the piece onto
 * @return      	 -1 on malloc failure, true iff move is legal otherwise
 */
int is_check_state_created_allied(Game *game, GamePiece *piece, int pos_x, int pos_y);

/*
 * Determine if game is in check state for current player.
 * @param game 	 game instance
 * @return     	 true iff game is in check state
 */
int is_in_check_state(Game *game);

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

/*
 * Add given move to history.
 * @precond     	 assumes arguments are valid
 * @param src_x 	 column of source position
 * @param src_y 	 row of source position
 * @param dst_x 	 column of source position
 * @param dst_y 	 row of source position
 * @return
 * MALLOC_FAILURE 	 if failed to allocate memory for move struct
 * SUCCESS        	 otherwise
 */
EngineMessage add_move_to_history(Game *game, int src_x, int src_y, int dst_x, int dst_y);

/*
 * Create a new move struct with the given positions.
 * @param src_x 	 column of source position
 * @param src_y 	 row of source position
 * @param dst_x 	 column of source position
 * @param dst_y 	 row of source position
 * @return      	 pointer to new move struct on success, NULL otherwise
 */
GameMove *create_move(int src_x, int src_y, int dst_x, int dst_y);

/*
 * Get possible moves of given pawn.
 * @param game  	 game instance
 * @param piece 	 pawn instance
 * @return      	 pointer to list of possible moves on success, NULL otherwise
 */
SPArrayList *get_pawn_moves(Game *game, GamePiece *piece);

/*
 * Get possible moves of given rook.
 * @param game  	 game instance
 * @param piece 	 rook instance
 * @return      	 pointer to list of possible moves on success, NULL otherwise
 */
SPArrayList *get_rook_moves(Game *game, GamePiece *piece);

/*
 * Get possible moves of given knight.
 * @param game  	 game instance
 * @param piece 	 knight instance
 * @return      	 pointer to list of possible moves on success, NULL otherwise
 */
SPArrayList *get_knight_moves(Game *game, GamePiece *piece);

/*
 * Get possible moves of given bishop.
 * @param game  	 game instance
 * @param piece 	 bishop instance
 * @return      	 pointer to list of possible moves on success, NULL otherwise
 */
SPArrayList *get_bishop_moves(Game *game, GamePiece *piece);

/*
 * Get possible moves of given queen.
 * @param game  	 game instance
 * @param piece 	 queen instance
 * @return      	 pointer to list of possible moves on success, NULL otherwise
 */
SPArrayList *get_queen_moves(Game *game, GamePiece *piece);

/*
 * Get possible moves of given king.
 * @param game  	 game instance
 * @param piece 	 king instance
 * @return      	 pointer to list of possible moves on success, NULL otherwise
 */
SPArrayList *get_king_moves(Game *game, GamePiece *piece);

/*
 * Find an edge of the range of possible moves for a rook/bishop/queen.
 * @param game  	 game instance
 * @param src_x 	 column of source position
 * @param src_y 	 row of source position
 * @param inc_X 	 column direction of search
 * @param inc_y 	 row direction of search
 * @return      	 offset from given position
 */
int find_range_end(Game *game, PieceType type, int src_x, int src_y, int inc_x, int inc_y);
#endif
