#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_video.h>

#include "main_aux.h"
#include "graphics/event.h"
#include "graphics/chess_board.h"

int cli_main() {
	ProgramState *prog_state;
	if(!(prog_state = create_program_state())){
		print_generic_message(MALLOC_FAILURE);
		return 0;
	}
	do {
		if(prog_state->indicators->run_state == GAME){
			if(!(fetch_and_exe_game(prog_state))){
				print_generic_message(MALLOC_FAILURE);
				break;
			}
		} else {
			if(!fetch_and_exe_settings(prog_state)){
				print_generic_message(MALLOC_FAILURE);
				break;
			}
		}
	} while (!prog_state->indicators->quit);

	destroy_program_state(prog_state);
	return 0;
}

int gui_main() {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("Chess",	// window title
			SDL_WINDOWPOS_CENTERED,			// initial x position
			SDL_WINDOWPOS_CENTERED,			// initial y position
			800,							// width, in pixels
			800,							// height, in pixels
			SDL_WINDOW_OPENGL				// flags - see below
	);

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	GameSettings *settings = create_settings();
	set_default_settings(settings);

	Game *game = create_game(settings);

	ChessBoard *board;
	GameEvent game_event;
	SDL_Rect board_area = {.x = 200, .y = 200, .w = 400, .h = 400};
	create_chess_board(&board, &board_area, renderer);

	SDL_RenderClear(renderer);
	draw_chess_board(renderer, board, game);
	SDL_RenderPresent(renderer);

	while (1) {
		SDL_Event event;
		SDL_WaitEvent(&event);
		if (event.type == SDL_QUIT) {
			break;
		}

		chess_board_event_handler(&event, board, &game_event);
		if (game_event.type == PIECE_MOVED) {
			move_game_piece(game, game_event.data.move.prev_piece_col, game_event.data.move.prev_piece_row, game_event.data.move.new_piece_col, game_event.data.move.new_piece_row);
		}

		SDL_RenderClear(renderer);
		draw_chess_board(renderer, board, game);
		SDL_RenderPresent(renderer);
	}

	destroy_chess_board(board);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

int main(int argc, char *argv[]){
	if (argc == 1) {
		return cli_main();
	};

	if (argc == 2) {
		if (!strcmp(argv[1], "-c")) {
			return cli_main();
		}
		if (!strcmp(argv[1], "-g")) {
			return gui_main();
		}
	}

	printf("USAGE: %s <-c/-g>\n", argv[0]);
	return 1;
}
