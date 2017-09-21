#ifndef BUTTON_H
#define BUTTON_H

#include <SDL.h>
#include <SDL_video.h>

#include "../game_engine.h"

typedef enum {
    NO_BUTTON_EVENT = 0,
    BUTTON_PUSHED
} ButtonEventType;

typedef struct {
    ButtonEventType type;
} ButtonEvent;

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
EngineMessage button_event_handler(SDL_Event *event, Button *button, ButtonEvent *button_event);

#endif
