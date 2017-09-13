#include <SDL.h>
#include <SDL_video.h>

#include "game_board.h"
#include "../game_engine.h"
#include "../main_aux.h"

int main() {
	SDL_Init(SDL_INIT_VIDEO);
	
	GameSettings *settings = create_settings();
	set_default_settings(settings);
	Game *game = create_game(settings);
	
	SDL_Window* window = SDL_CreateWindow("Chess",	// window title
			SDL_WINDOWPOS_CENTERED,			// initial x position
			SDL_WINDOWPOS_CENTERED,			// initial y position
			800,							// width, in pixels
			800,							// height, in pixels
			SDL_WINDOW_OPENGL				// flags - see below
	);
	
	int quit = 0;
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	ChessBoard *board;
	SDL_Rect board_area = {.x = 200, .y = 200, .w = 400, .h = 400};
	create_chess_board(&board, &board_area, renderer);
	GameEvent *game_event;
	EngineMessage msg;
	
	draw_chess_board(renderer, board, game);
	while (!quit) {
		SDL_Event event;
		SDL_WaitEvent(&event);
		if (event.type == SDL_QUIT) {
			quit = 1;
			break;
		}
		SDL_RenderClear(renderer);
		draw_chess_board(renderer, board, game);

		SDL_RenderPresent(renderer);
		
		game_event = chess_board_event_handler(&event, board);
		msg = move_game_piece(game, game_event->prev_piece_col, game_event->prev_piece_row, game_event->new_piece_col, game_event->new_piece_row);
		
		SDL_RenderClear(renderer);
		draw_chess_board(renderer, board, game);

		SDL_RenderPresent(renderer);
	}
	SDL_Quit();

}
