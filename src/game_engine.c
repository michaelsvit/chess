#include <stdlib.h>
#include <ctype.h>
#include "game_engine.h"

/******************************* Interface functions *********************************/

Game *create_game(GameSettings *settings){
	if(!settings) return NULL;
	if(settings->difficulty < 1 || settings->difficulty > 4) return NULL;
	Game *game = malloc(sizeof(Game));
	if(!game) return NULL;

	game->white_pieces = spArrayListCreate(sizeof(GamePiece), ARRAY_SIZE);
	game->black_pieces = spArrayListCreate(sizeof(GamePiece), ARRAY_SIZE);
	game->move_history = spArrayListCreate(sizeof(GameMove), HISTORY_SIZE);
	game->removed_pieces = spArrayListCreate(sizeof(GamePiece), HISTORY_SIZE);
	if(!game->white_pieces ||
			!game->black_pieces ||
			!game->move_history ||
			!game->removed_pieces){
		destroy_game(game);
		return NULL;
	}

	/* Initialize board to be empty */
	for(int i = 0; i < BOARD_SIZE; i++){
		for(int j = 0; j < BOARD_SIZE; j++){
			game->board[i][j] = NULL;
		}
	}
	game->mode = settings->mode;
	game->difficulty = settings->difficulty;
	game->check = 0;
	game->player_color[PLAYER1] = settings->player1_color;
	game->player_color[PLAYER2] = !settings->player1_color;

	if(init_game(game) != SUCCESS){
		destroy_game(game);
		return NULL;
	} else {
		return game;
	}
}

Game *copy_game(Game *game){
	if(!game) return NULL;

	Game *copy = malloc(sizeof(Game));
	if(!copy) return NULL;

	copy->white_pieces = spArrayListCopy(game->white_pieces);
	copy->black_pieces = spArrayListCopy(game->black_pieces);
	copy->move_history = spArrayListCopy(game->move_history);
	copy->removed_pieces = spArrayListCopy(game->removed_pieces);
	if(!copy->white_pieces ||
			!copy->black_pieces ||
			!copy->move_history ||
			!copy->removed_pieces){
		destroy_game(copy);
		return NULL;
	}

	/* Initialize board to be empty */
	for(int i = 0; i < BOARD_SIZE; i++){
		for(int j = 0; j < BOARD_SIZE; j++){
			copy->board[i][j] = NULL;
		}
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
	copy->current_player = game->current_player;
	copy->difficulty = game->difficulty;
	copy->check = game->check;
	return copy;
}

void destroy_game(Game *game){
	if(!game) return;

	/* Free all allocated memory */
	spArrayListDestroy(game->white_pieces);
	spArrayListDestroy(game->black_pieces);
	spArrayListDestroy(game->move_history);
	spArrayListDestroy(game->removed_pieces);
	free(game);
}

EngineMessage save_game(Game *game, const char *file){
	FILE *out = fopen(file, "w");
	if(!out) return INVALID_ARGUMENT;
	if(!serialize_game(game, out)){
		fclose(out);
		return MALLOC_FAILURE;
	}
	fclose(out);
	return SUCCESS_NO_PRINT;
}

Game *load_game(FILE *in){
	/* Initialize game object */
	Game *game = malloc(sizeof(Game));
	if(!game) return NULL;

	game->white_pieces = spArrayListCreate(sizeof(GamePiece), ARRAY_SIZE);
	game->black_pieces = spArrayListCreate(sizeof(GamePiece), ARRAY_SIZE);
	game->move_history = spArrayListCreate(sizeof(GameMove), HISTORY_SIZE);
	game->removed_pieces = spArrayListCreate(sizeof(GamePiece), HISTORY_SIZE);
	if(!game->white_pieces ||
			!game->black_pieces ||
			!game->move_history ||
			!game->removed_pieces){
		destroy_game(game);
		return NULL;
	}

	/* Initialize board to be empty */
	for(int i = 0; i < BOARD_SIZE; i++){
		for(int j = 0; j < BOARD_SIZE; j++){
			game->board[i][j] = NULL;
		}
	}
	game->mode = 1;
	game->difficulty = 2;
	game->check = 0;
	game->player_color[PLAYER1] = WHITE;
	game->player_color[PLAYER2] = BLACK;
	game->current_player = (game->player_color[PLAYER1] == WHITE) ? PLAYER1 : PLAYER2;

	/* Load file into game object */
	char *buf;
	init_parser(in, &buf);
	next_open(); /* consume opening GAME_TAG */
	Tag tag;
	while((tag = next_open()) != GAME_TAG){
		if(!read_content(tag, game)){
			free(buf);
			destroy_game(game);
			return NULL;
		}
		next_close(); /* consume closing tag */
	}
	free(buf);
	game->check = is_in_check_state(game);
	spArrayListShuffle(game->white_pieces);
	spArrayListShuffle(game->black_pieces);
	return game;
}

GameMove *copy_move(GameMove *move){
	GameMove *copy = (GameMove *)malloc(sizeof(GameMove));
	if(!copy) return NULL;
	copy->src_x = move->src_x;
	copy->src_y = move->src_y;
	copy->dst_x = move->dst_x;
	copy->dst_y = move->dst_y;
	return copy;
}

EngineMessage move_game_piece(Game *game, int src_x, int src_y, int dst_x, int dst_y){
	if(!game) return INVALID_ARGUMENT;
	GamePiece *piece = game->board[src_y][src_x];
	if(!piece) return ILLEGAL_MOVE;
	if(dst_x < 0 || dst_x > BOARD_SIZE-1 || dst_y < 0 || dst_y > BOARD_SIZE-1){
		return INVALID_ARGUMENT;
	}
	/* Make sure piece is the same color as current player */
	if(game->player_color[game->current_player] != piece->color) return ILLEGAL_MOVE;

	int result = is_legal_move(game, piece, dst_x, dst_y);
	if(!result){
		return ILLEGAL_MOVE;
	} else if (result == -1) {
		return MALLOC_FAILURE;
	}

	if(add_move_to_history(game, src_x, src_y, dst_x, dst_y) != SUCCESS) return MALLOC_FAILURE;
	move_piece_to_position(game, piece, dst_x, dst_y);
	/* Determine if moving the piece ended with check state for enemy king */
	game->current_player = !game->current_player;
	game->check = is_in_check_state(game);
	int game_over = is_game_over(game);
	if(game_over == -1){
		return MALLOC_FAILURE;
	} else if (game_over == 1) {
		return GAME_OVER;
	}

	return SUCCESS;
}

EngineMessage undo_move(Game *game, GameMove **removed_move){
	if(!game) return INVALID_ARGUMENT;

	if(spArrayListIsEmpty(game->removed_pieces)) return EMPTY_HISTORY;

	GameMove *move = spArrayListGetFirst(game->move_history);
	spArrayListRemoveFirst(game->move_history);
	GamePiece *removed_piece = spArrayListGetFirst(game->removed_pieces);
	spArrayListRemoveFirst(game->removed_pieces);

	/* Restore previous state */
	if(removed_piece){
		SPArrayList *pieces_set = (removed_piece->color == WHITE) ?
			game->white_pieces : game->black_pieces;
		spArrayListAddLast(pieces_set, removed_piece);
	}
	move_piece_to_position(game,
			game->board[move->dst_y][move->dst_x],
			move->src_x,
			move->src_y);
	game->board[move->dst_y][move->dst_x] = removed_piece;
	game->check = is_in_check_state(game);
	game->current_player = !game->current_player;

	*removed_move = move;
	return SUCCESS;
}

EngineMessage get_possible_moves(SPArrayList **moves, Game *game, GamePiece *piece){
	*moves = NULL;
	if(!game) return INVALID_ARGUMENT;
	if(!piece || piece->color != game->player_color[game->current_player])
		return ILLEGAL_MOVE;

	switch(piece->type){
		case PAWN:
			*moves = get_pawn_moves(game, piece);
			break;
		case ROOK:
			*moves = get_rook_moves(game, piece);
			break;
		case KNIGHT:
			*moves = get_knight_moves(game, piece);
			break;
		case BISHOP:
			*moves = get_bishop_moves(game, piece);
			break;
		case QUEEN:
			*moves = get_queen_moves(game, piece);
			break;
		case KING:
			*moves = get_king_moves(game, piece);
			break;
	}

	return (*moves) ? SUCCESS : MALLOC_FAILURE;
}

int is_piece_threatened_after_move(Game *game, GamePiece *piece, GameMove *move){
	Game *copy = copy_game(game);
	if(!copy) return -1;
	SPArrayList *enemy_pieces = (piece->color == WHITE) ?
		copy->black_pieces : copy->white_pieces;

	/* Get copy of piece at given position */
	GamePiece *piece_copy = copy->board[piece->pos_y][piece->pos_x];
	/* Get copy of piece to be moved */
	GamePiece *src_piece_copy = copy->board[move->src_y][move->src_x];

	/* Move copied source piece to move destination */
	if(is_occupied_position(copy, move->dst_x, move->dst_y)){
		GamePiece *dst_piece = copy->board[move->dst_y][move->dst_x];
		remove_game_piece(copy, dst_piece);
		free(dst_piece);
	}
	move_piece_to_position(copy, src_piece_copy, move->dst_x, move->dst_y);
	copy->current_player = !copy->current_player;

	/* Iterate through all enemy pieces and check if they threaten given piece */
	for(int i = 0; i < spArrayListSize(enemy_pieces); i++){
		GamePiece *temp = (GamePiece *)spArrayListGetAt(enemy_pieces, i);
		if(is_legal_move(copy, temp, piece_copy->pos_x, piece_copy->pos_y)){
			destroy_game(copy);
			return 1;
		}
	}
	destroy_game(copy);
	return 0;
}

int is_game_over(Game *game){
	/* Check if current player has any possible move */
	SPArrayList *pieces =
		(game->player_color[game->current_player] == WHITE) ?
		game->white_pieces : game->black_pieces;
	for (int i = 0; i < spArrayListSize(pieces); ++i) {
		SPArrayList *moves;
		GamePiece *piece = (GamePiece *)spArrayListGetAt(pieces, i);
		EngineMessage msg = get_possible_moves(&moves, game, piece);
		/* Invalid argument message is not possible here */
		if(msg == MALLOC_FAILURE) return -1;
		if(spArrayListSize(moves) > 0){
			spArrayListDestroy(moves);
			return 0;
		}
		spArrayListDestroy(moves);
	}
	return 1;
}

EngineMessage restart_game(Game *game) {
	for(int i = 0; i < BOARD_SIZE; i++){
		for(int j = 0; j < BOARD_SIZE; j++){
			game->board[i][j] = NULL;
		}
	}
	spArrayListClear(game->black_pieces);
	spArrayListClear(game->white_pieces);
	spArrayListClear(game->move_history);
	spArrayListClear(game->removed_pieces);

	return init_game(game);
}
/******************************** Auxiliary functions ******************************/

int serialize_game(Game *game, FILE *out){
	init_serializer(out);
	int indent = 0;
	open_tag("game", out, indent);
	putc('\n', out);
	indent++;
	write_tag(
			"current_turn",
			(game->player_color[game->current_player] == WHITE) ? "1" : "0",
			out,
			indent);
	write_tag(
			"game_mode",
			(game->mode == ONE_PLAYER) ? "1" : "2",
			out,
			indent);
	if(game->mode == ONE_PLAYER){
		char *difficulty = (char *)malloc(2);
		if (!difficulty) return 0;
		sprintf(difficulty, "%d", game->difficulty);
		write_tag(
				"difficulty",
				difficulty,
				out,
				indent);
		free(difficulty);
		write_tag(
				"user_color",
				(game->player_color[PLAYER1] == WHITE) ? "1" : "0",
				out,
				indent);
	}
	if(!write_board(game, out, indent)) return 0;
	indent--;
	close_tag("game", out, indent);
	putc('\n', out);
	return 1;
}

char get_piece_repr(GamePiece *piece); /* definition is in print_utils.c */

int write_board(Game *game, FILE *out, int indent){
	open_tag("board", out, indent);
	putc('\n', out);
	indent++;
	for (int i = 0; i < BOARD_SIZE; ++i) {
		char *row_str = (char *)malloc(BOARD_SIZE+1);
		if(!row_str) return 0;
		for (int j = 0; j < BOARD_SIZE; j++) {
			GamePiece *piece = game->board[BOARD_SIZE-1-i][j];
			row_str[j] = get_piece_repr(piece);
		}
		row_str[BOARD_SIZE] = '\0';
		char *row_tag = (char *)malloc(6);
		if(!row_tag){
			free(row_str);
			return 0;
		}
		sprintf(row_tag, "row_%d", BOARD_SIZE-i);
		write_tag(row_tag, row_str, out, indent);
		free(row_str);
		free(row_tag);
	}
	indent--;
	close_tag("board", out, indent);
	putc('\n', out);
	return 1;
}

int read_content(Tag tag, Game *game){
	switch (tag){
		case CUR_PLAYER_TAG:
			/* Assume for now that PLAYER1 is white */
			game->current_player = !atoi(content());
			break;
		case MODE_TAG:
			game->mode = atoi(content());
			break;
		case DIFFICULTY_TAG:
			if(game->mode == ONE_PLAYER)
				game->difficulty = atoi(content());
			break;
		case USER_COLOR_TAG:
			if(game->mode == ONE_PLAYER){
				Color user_color = atoi(content());
				if(user_color == BLACK){
					/* Need to flip current player */
					game->current_player = !game->current_player;
					game->player_color[PLAYER1] = BLACK;
					game->player_color[PLAYER2] = WHITE;
				}
			}
			break;
		case BOARD_TAG:
			for (int i = 0; i < BOARD_SIZE; ++i) {
				next_open();
				if(!read_board_row(game, BOARD_SIZE-1-i)) return 0;
				next_close();
			}
			break;
		default:
			break;
	}
	return 1;
}

int read_board_row(Game *game, int row){
	char *row_str = content();
	for (int i = 0; i < BOARD_SIZE; ++i) {
		if (row_str[i] == '_') continue;
		PieceType piece_type = get_piece_type(row_str[i]);
		Color piece_color = get_piece_color(row_str[i]);
		EngineMessage msg = add_game_piece(game, piece_type, piece_color, i, row);
		if(msg == MALLOC_FAILURE) return 0;
	}
	return 1;
}

PieceType get_piece_type(char repr){
	repr = tolower(repr);
	switch (repr){
		case 'm':
			return PAWN;
		case 'r':
			return ROOK;
		case 'n':
			return KNIGHT;
		case 'b':
			return BISHOP;
		case 'q':
			return QUEEN;
		case 'k':
			return KING;
	}
	return 0; /* unreachable */
}

Color get_piece_color(char repr){
	if (repr >= 'a' && repr <= 'z') {
		return WHITE;
	} else {
		return BLACK;
	}
}

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
	game->current_player = (game->player_color[PLAYER1] == WHITE) ? PLAYER1 : PLAYER2;
	msg = add_game_pieces_set(game, WHITE);
	spArrayListShuffle(game->white_pieces);
	if(msg != SUCCESS) return msg;
	msg = add_game_pieces_set(game, BLACK);
	spArrayListShuffle(game->black_pieces);
	return msg;
}

EngineMessage add_game_piece(Game *game, PieceType type, Color color, int pos_x, int pos_y){
	GamePiece *piece = create_game_piece(type, color, pos_x, pos_y);
	if(!piece) return MALLOC_FAILURE;
	SPArrayList *piece_set = (color == WHITE) ? game->white_pieces : game->black_pieces;
	spArrayListAddLast(piece_set, piece); /* cannot fail on a new board */
	game->board[pos_y][pos_x] = piece;
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
	if((msg = add_game_piece(game, QUEEN, color, 3, row)) != SUCCESS) return msg;

	/* Add king */
	if((msg = add_game_piece(game, KING, color, 4, row)) != SUCCESS) return msg;
	return SUCCESS;
}

int is_occupied_position(Game *game, int pos_x, int pos_y){
	return game->board[pos_y][pos_x] != NULL;
}

int is_valid_position(int pos_x, int pos_y){
	return pos_x >= 0 && pos_x < BOARD_SIZE && pos_y >= 0 && pos_y < BOARD_SIZE;
}

int is_legal_move(Game *game, GamePiece *piece, int pos_x, int pos_y){
	if(!is_valid_position(pos_x, pos_y)) return 0;
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
			if(!is_legal_knight_move(piece, pos_x, pos_y)) return 0;
			break;
		case BISHOP:
			if(!is_legal_bishop_move(game, piece, pos_x, pos_y)) return 0;
			break;
		case QUEEN:
			if(!is_legal_queen_move(game, piece, pos_x, pos_y)) return 0;
			break;
		case KING:
			if(!is_legal_king_move(piece, pos_x, pos_y)) return 0;
			break;
	}

	/* Make sure moving the piece doesn't end with a check state for allied king */
	int result;
	if(target_piece && target_piece->type == KING){
		/* Prevent loop, allied check is impossible in this state */
		result = 0;
	} else {
		result = is_check_state_created_allied(game, piece, pos_x, pos_y);
	}
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
		if(piece->pos_y != init_position) return 0;
		/* Pawn cannot jump above another piece */
		if(is_occupied_position(game, piece->pos_x, piece->pos_y + direction)) return 0;
		/* Pawn stays in same column, target position must be unoccupied */
		if (piece->pos_x != pos_x) return 0;
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
			for(int i = piece->pos_y + 1; i < pos_y; i++){
				if(is_occupied_position(game, pos_x, i)) return 0;
			}
		} else {
			for(int i = pos_y + 1; i < piece->pos_y; i++){
				if(is_occupied_position(game, pos_x, i)) return 0;
			}
		}
	} else {
		if (piece->pos_x < pos_x) {
			for(int i = piece->pos_x + 1; i < pos_x; i++){
				if(is_occupied_position(game, i, pos_y)) return 0;
			}
		} else {
			for(int i = pos_x + 1; i < piece->pos_x; i++){
				if(is_occupied_position(game, i, pos_y)) return 0;
			}
		}
	}
	return 1;
}

int is_legal_knight_move(GamePiece *piece, int pos_x, int pos_y){
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
	for(int i = 1; i < dist; i++){
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

int is_legal_king_move(GamePiece *piece, int pos_x, int pos_y){
	/* Make sure king moves exactly one step in any direction */
	int dist_x = abs(piece->pos_x - pos_x);
	int dist_y = abs(piece->pos_y - pos_y);
	if(dist_x > 1 || dist_y > 1 || dist_x + dist_y == 0) return 0;

	return 1;
}

int is_in_check_state(Game *game){
	SPArrayList *same_color_pieces;
	SPArrayList *enemy_pieces;
	if(game->player_color[game->current_player] == WHITE){
		same_color_pieces = game->white_pieces;
		enemy_pieces = game->black_pieces;
	} else {
		same_color_pieces = game->black_pieces;
		enemy_pieces = game->white_pieces;
	}

	GamePiece *allied_king = find_king_piece(same_color_pieces);
	for (int i = 0; i < spArrayListSize(enemy_pieces); ++i) {
		GamePiece *temp = (GamePiece *)spArrayListGetAt(enemy_pieces, i);
		if(is_legal_move(game, temp, allied_king->pos_x, allied_king->pos_y)) return 1;
	}
	return 0;
}

int is_check_state_created_allied(Game *game, GamePiece *piece, int pos_x, int pos_y){
	SPArrayList *same_color_pieces = (piece->color == WHITE) ?
		game->white_pieces : game->black_pieces;

	GamePiece *king = find_king_piece(same_color_pieces);
	GameMove *move = create_move(piece->pos_x, piece->pos_y, pos_x, pos_y);
	if(!move) return -1;
	int result = is_piece_threatened_after_move(game, king, move);
	free(move);
	return result;
}

void move_piece_to_position(Game *game, GamePiece *piece, int pos_x, int pos_y){
	if(is_occupied_position(game, pos_x, pos_y)){
		remove_game_piece(game, game->board[pos_y][pos_x]);
	}

	/* Remove piece from previous location */
	game->board[piece->pos_y][piece->pos_x] = NULL;
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
	if(spArrayListIsFull(game->move_history)){
		free(spArrayListGetLast(game->move_history));
		spArrayListRemoveLast(game->move_history);
		free(spArrayListGetLast(game->removed_pieces));
		spArrayListRemoveLast(game->removed_pieces);
	}
	GameMove *move = create_move(src_x, src_y, dst_x, dst_y);
	if(!move) return MALLOC_FAILURE;
	spArrayListAddFirst(game->move_history, move);
	GamePiece *piece = game->board[dst_y][dst_x];
	spArrayListAddFirst(game->removed_pieces, piece);
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

SPArrayList *get_pawn_moves(Game *game, GamePiece *piece){
	SPArrayList *moves = spArrayListCreate(sizeof(GameMove), MAX_PAWN_MOVES);
	if(!moves) return NULL;

	int direction = (piece->color == WHITE) ? 1 : -1;
	for(int i = -1; i <= 1; i++){
		if(is_legal_move(game, piece, piece->pos_x+i, piece->pos_y + direction*1)){
			GameMove *move = create_move(
					piece->pos_x,
					piece->pos_y,
					piece->pos_x+i,
					piece->pos_y + direction*1);
			if(!move){
				spArrayListDestroy(moves);
				return NULL;
			}
			spArrayListAddLast(moves, move);
		}
	}
	if(is_legal_move(game, piece, piece->pos_x, piece->pos_y + direction*2)){
		GameMove *move = create_move(
				piece->pos_x,
				piece->pos_y,
				piece->pos_x,
				piece->pos_y + direction*2);
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
	int start_index = find_range_end(game, ROOK, piece->pos_x, piece->pos_y, 0, -1);
	int end_index = find_range_end(game, ROOK, piece->pos_x, piece->pos_y, 0, 1);
	/* TODO: Optimize for un-checked state if needed */
	for(int i = start_index; i <= end_index; i++){
		if(i == 0) continue;
		if(is_legal_move(game, piece, piece->pos_x, piece->pos_y+i)){
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
	start_index = find_range_end(game, ROOK, piece->pos_x, piece->pos_y, -1, 0);
	end_index = find_range_end(game, ROOK, piece->pos_x, piece->pos_y, 1, 0);
	for(int i = start_index; i <= end_index; i++){
		if(i == 0) continue;
		if(is_legal_move(game, piece, piece->pos_x+i, piece->pos_y)){
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
	int start_index = find_range_end(game, BISHOP, piece->pos_x, piece->pos_y, -1, -1);
	int end_index = find_range_end(game, BISHOP, piece->pos_x, piece->pos_y, 1, 1);
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
	start_index = find_range_end(game, BISHOP, piece->pos_x, piece->pos_y, -1, 1);
	end_index = find_range_end(game, BISHOP, piece->pos_x, piece->pos_y, 1, -1);
	for(int i = start_index; i <= end_index; i++){
		if(i == 0) continue;
		if(is_legal_move(game, piece, piece->pos_x + i, piece->pos_y - i)){
			GameMove *move = create_move(
					piece->pos_x,
					piece->pos_y,
					piece->pos_x+i,
					piece->pos_y-i);
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
		spArrayListAddLast(moves, copy_move(spArrayListGetAt(rook_moves, i)));
	}
	for(int i = 0; i < spArrayListSize(bishop_moves); i++){
		spArrayListAddLast(moves, copy_move(spArrayListGetAt(bishop_moves, i)));
	}
	spArrayListDestroy(rook_moves);
	spArrayListDestroy(bishop_moves);
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

int find_range_end(Game *game, PieceType type, int src_x, int src_y, int inc_x, int inc_y){
	int i = inc_x, j = inc_y;
	if(is_valid_position(src_x+i, src_y+j)){
		/* Increment counters until board edge has been reached or we found a game piece,
		whichever comes first */
		while(is_valid_position(src_x+i, src_y+j)
				&& !is_occupied_position(game, src_x+i, src_y+j)){
			i += inc_x;
			j += inc_y;
		}
	} else {
		/* There are no possible moves in this direction */
		i = -i;
		j = -j;
	}
	/* Return index relevant to game piece type */
	if(type == ROOK){
		return (inc_x == 0) ? j : i;
	} else {
		return i;
	}
}
