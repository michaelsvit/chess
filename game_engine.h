/* Game engine module containing state and interaction related functions */
#ifndef GAME_H
#define GAME_H

#define BOARD_SIZE 8
#define ARRAY_SIZE 16

typedef enum {PLAYER1, PLAYER2} Player;
typedef enum {PAWN, BISHOP, ROOK, KNIGHT, QUEEN, KING} PieceType;
typedef enum {BLACK, WHITE} Color;
typedef enum {1_PLAYER, 2_PLAYER} Mode;

typedef struct game_t{
	GamePiece *board[BOARD_SIZE][BOARD_SIZE]; /* current game board */
	SPArrayList white_pieces, black_pieces; /* lists representing white and black game pieces on the board */
	Player current_player;
	Color player_color[2];
	Mode mode;
	int difficulty;
} Game;

/* struct defining a single game piece on the board */
typedef struct game_piece{
	PieceType type;
	Color color;
	int pos_y;
	int pos_x;
} GamePiece;

Game *create_game(Mode mode, int difficulty, Color player1_color);

void destroy_game();
#endif
