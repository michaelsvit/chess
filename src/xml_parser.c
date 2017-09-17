#include <stdlib.h>
#include <string.h>
#include "xml_parser.h"

int init_parser(FILE *in, char **buf){
	fseek(in, 0, SEEK_END);
	unsigned int file_size = ftell(in);
	*buf = (char *)malloc(file_size+1);
	if(!(*buf)) return 0;
	rewind(in);
	fread(*buf, sizeof(char), file_size, in);
	if(ferror(in)){
		free(*buf);
		return 0;
	}
	(*buf)[file_size] = '\0';
	strtok(*buf, OPEN_DELIM); /* skip XML version tag and initialize strtok */
	return 1;
}

Tag next_open(){
	char *tag_str = strtok(NULL, OPEN_DELIM);
	return get_tag(tag_str);
}

Tag next_close(){
	char *tag_str = strtok(NULL, CLOSE_DELIM);
	return get_tag(tag_str);
}

char *content(){
	return strtok(NULL, CONTENT_DELIM);
}

Tag get_tag(char *str){
	if (strcmp(str, "game") == 0) {
		return GAME_TAG;
	} else if (strcmp(str, "current_turn") == 0) {
		return CUR_PLAYER_TAG;
	} else if (strcmp(str, "game_mode") == 0) {
		return MODE_TAG;
	} else if (strcmp(str, "difficulty") == 0) {
		return DIFFICULTY_TAG;
	} else if (strcmp(str, "user_color") == 0) {
		return USER_COLOR_TAG;
	} else if (strcmp(str, "board") == 0) {
		return BOARD_TAG;
	} else {
		/* Row tag */
		return 0;
	}
}
