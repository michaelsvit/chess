/* Game engine module containing state and interaction related functions */
#ifndef GAME_H
#define GAME_H

#define BOARD_SIZE 8
#define ARRAY_SIZE 16

typedef enum {PLAYER1, PLAYER2} Player;
typedef enum {PAWN, BISHOP, ROOK, KNIGHT, QUEEN, KING} PieceType;
typedef enum {BLACK, WHITE} Color;

typedef struct game_t{
	GamePiece *board[BOARD_SIZE][BOARD_SIZE]; /* current game board */
	SPArrayList whitePieces, blackPieces; /* lists representing white and black game pieces on the board */
	Player currentPlayer;
} Game;

/* struct defining a single game piece on the board */
typedef struct game_piece{
	PieceType type;
	Color color;
	int posY;
	int posX;
} GamePiece;

#endif
