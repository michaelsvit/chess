#ifndef BUTTON_H
#define BUTTON_H

#include "../game_engine.h"
#include "event.h"
#include <SDL.h>
#include <SDL_video.h>

typedef struct {
    SDL_Texture* regular_texture;
    SDL_Texture* pushed_texture;
    SDL_Rect area;
    int is_pushed;
} Button;

EngineMessage create_button(Button **button, SDL_Rect *area, SDL_Renderer *renderer, char *reg_path, char *pushed_path);
EngineMessage draw_button(SDL_Renderer *renderer, Button *button);
void destroy_button(Button *button);

// return 1 if clicked, 0 otherwise.
int button_event_handler(SDL_Event *event, Button *button);

#endif
