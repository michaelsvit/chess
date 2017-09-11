#include "xml_serializer.h"

void serialize_game(Game *game, FILE *out){
	write_xml_tag(out);
	int indent = 0;
	open_tag("game", out, indent);
	putc('\n', out);
	indent++;
	write_current_player(game, out, indent);
	write_mode(game, out ,indent);
	write_difficulty(game, out, indent);
	write_color(game, out, indent);
	write_board(game, out, indent);
	indent--;
	close_tag("game", out, indent);
}

void write_xml_tag(FILE *out){
	open_tag("?xml version=\"1.0\" encoding=\"UTF-8\"?", out, 0);
}

void write_current_player(Game *game, FILE *out, int indent){
	open_tag("current_turn", out, indent);
	putc((game->player_color[game->current_player] == WHITE) ? '1' : '0', out);
	close_tag("current_turn", out, 0);
	putc('\n', out);
}

void write_mode(Game *game, FILE *out, int indent){
	open_tag("game_mode", out, indent);
	putc((game->mode == ONE_PLAYER) ? '1' : '2', out);
	close_tag("game_mode", out, 0);
	putc('\n', out);
}

void write_difficulty(Game *game, FILE *out, int indent){
	if(game->mode == ONE_PLAYER){
		open_tag("difficulty", out, indent);
		putc(game->difficulty + '0', out);
		close_tag("difficulty", out, 0);
		putc('\n', out);
	}
}

void write_color(Game *game, FILE *out, int indent){
	if(game->mode == ONE_PLAYER){
		open_tag("user_color", out, indent);
		putc((game->player_color[PLAYER1] == WHITE) ? '1' : '0', out);
		close_tag("user_color", out, 0);
		putc('\n', out);
	}
}

void write_board(Game *game, FILE *out, int indent){
	open_tag("board", out, indent);
	putc('\n', out);
	indent++;
	for (int i = 0; i < BOARD_SIZE; ++i) {
		write_board_row(game, out, indent, BOARD_SIZE-1-i);
	}
	indent--;
	close_tag("board", out, 0);
	putc('\n', out);
}

void write_board_row(Game *game, FILE *out, int indent, int row){
	for (int i = 0; i < indent; ++i) {
		putc('\t', out);
	}
	fprintf(out, "<row_%d>", row+1);
	for (int i = 0; i < BOARD_SIZE; ++i) {
		GamePiece *piece = game->board[row][i];
		putc(get_piece_repr(piece), out);
	}
	fprintf(out, "</row_%d\n>", row+1);
}

void open_tag(char *tag, FILE *out, int indent){
	for (int i = 0; i < indent; ++i) {
		putc('\t', out);
	}
	fprintf(out, "<%s>", tag);
}

void close_tag(char *tag, FILE *out, int indent){
	for (int i = 0; i < indent; ++i) {
		putc('\t', out);
	}
	fprintf(out, "</%s>", tag);
}
