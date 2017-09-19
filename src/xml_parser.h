#ifndef XML_PARSER_H
#define XML_PARSER_H

#include <stdio.h>

#define OPEN_DELIM "\t<>\n\r"
#define CLOSE_DELIM "\t<\\>\n\r"
#define CONTENT_DELIM "><"

typedef enum {
	GAME_TAG,
	CUR_PLAYER_TAG,
	MODE_TAG,
	DIFFICULTY_TAG,
	USER_COLOR_TAG,
	BOARD_TAG
} Tag;

/*
 * Parse given xml file into a game instance.
 * @param in  	 read file
 * @param buf 	 buffer to hold file contents
 * @return    	 true iff load succeeded
 */
int init_parser(FILE *in, char **buf);

/*
 * Retrieve next opening tag from buffer holding XML file.
 * @precond 	 strtok has been initialized to use buffer
 * @return  	 tag that was read
 */
Tag next_open();

/*
 * Retrieve next closing tag from buffer holding XML file.
 * @precond 	 strtok has been initialized to use buffer
 * @return  	 tag that was read
 */
Tag next_close();

/*
 * Retrieve content of current XML tag.
 * @precond 	 strtok has been initialized to use buffer
 * @precond 	 strtok pointer is set to content part of current tag
 * @return  	 read content
 */
char *content();

/*
 * Convert given string to a tag.
 * @param str 	 string representation of tag
 * @return    	 enum value of tag
 */
Tag get_tag(char *str);

#endif /* ifndef XML_PARSER_H */
