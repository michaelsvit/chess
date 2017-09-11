#ifndef XML_SERIALIZER_H
#define XML_SERIALIZER_H

#include <stdio.h>
#include "game_engine.h"
#include "print_utils.h"

/*
 * Serialize given game instance into an XML tree and write it to given file.
 * @param game 	 game instance
 * @param out  	 write target file
 */
void serialize_game(Game *game, FILE *out);

/*
 * Write XML opening tag to file.
 * @param out 	 write target file
 */
void write_xml_tag(FILE *out);

/*
 * Write current player to target file.
 * @param game   	 game instance
 * @param out    	 write target file
 * @param indent 	 indent level
 */
void write_current_player(Game *game, FILE *out, int indent);

/*
 * Write game mode to target file.
 * @param game   	 game instance
 * @param out    	 write target file
 * @param indent 	 indent level
 */
void write_mode(Game *game, FILE *out, int indent);

/*
 * Write game difficulty to target file (only if in one-player mode).
 * @param game   	 game instance
 * @param out    	 write target file
 * @param indent 	 indent level
 */
void write_difficulty(Game *game, FILE *out, int indent);

/*
 * Write user's piece color to target file (only if in one-player mode).
 * @param game   	 game instance
 * @param out    	 write target file
 * @param indent 	 indent level
 */
void write_color(Game *game, FILE *out, int indent);

/*
 * Write game board to target file.
 * @param game   	 game instance
 * @param out    	 write target file
 * @param indent 	 indent level
 */
void write_board(Game *game, FILE *out, int indent);

/*
 * Write a single row of the game board to target file.
 * @param game   	 game instance
 * @param out    	 write target file
 * @param indent 	 indent level
 * @param row    	 row index
 */
void write_board_row(Game *game, FILE *out, int indent, int row);

/*
 * Write opening tag to target file.
 * @param tag   	 tag name
 * @param out    	 write target file
 * @param indent 	 indent level
 */
void open_tag(char *tag, FILE *out, int indent);

/*
 * Write closing tag to target file.
 * @param tag   	 tag name
 * @param out    	 write target file
 * @param indent 	 indent level
 */
void close_tag(char *tag, FILE *out, int indent);

#endif /* ifndef XML_SERIALIZER_H */
