#include <stdio.h>
#include <unistd.h>
#include "game_slots.h"


const char* files[] = {
	"./chess_0.xml",
	"./chess_1.xml",
	"./chess_2.xml",
	"./chess_3.xml",
	"./chess_4.xml"
};

int num_saved_games() {
	int i;
	const char *file_name;
	for (i = 0; i < MAX_SAVED_GAME; i++) {
		file_name = files[i];
		if (access(file_name, F_OK) != 0) {
			return i;
		}
	}
	return MAX_SAVED_GAME;
}

Game* load_saved_game(int slot) {
	FILE *out = fopen(files[slot], "r");
	if(!out) {
		return NULL;
	}
	Game *game = load_game(out);
	fclose(out);
	return game;
}

EngineMessage new_saved_game(Game *game) {
	int i;
	int err;
	int num = num_saved_games();

	if (num == MAX_SAVED_GAME) {
		err = remove(files[num - 1]);
		if (err != 0) {
			return SAVE_ERROR;
		}
		num--;
	}

	for (i = num; i > 0; i--) {
		err = rename(files[i - 1], files[i]);
		if (err != 0) {
			return SAVE_ERROR;
		}
	}

	return save_game(game, files[0]);
}
