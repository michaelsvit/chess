#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_video.h>

#include "main_aux.h"
#include "graphics/window.h"

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

EngineMessage gui_main() {
	EngineMessage err = SUCCESS;
	SDL_Init(SDL_INIT_VIDEO);

	Window *window;
	err = create_window(&window);
	if (err != SUCCESS){
		if (err == SDL_ERROR) {
			printf("SDL_ERROR: %s\n", SDL_GetError());
		}
		return err;
	}

	while (1) {
		SDL_Event event;
		WindowEvent window_event;

		err = draw_window(window);
		if (err != SUCCESS) {
			break;
		}
		SDL_WaitEvent(&event);
		err = window_event_handler(&event, window, &window_event);
		if (err != SUCCESS){
			break;
		}
		if (window_event.type == QUIT_WINDOW) {
			break;
		}
	}

	destroy_window(window);
	SDL_Quit();

	if (err == SDL_ERROR) {
		printf("SDL_ERROR: %s\n", SDL_GetError());
	}

	return err;
}

int main(int argc, char *argv[]) {
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
