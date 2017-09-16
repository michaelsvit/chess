#ifndef MULTIPLE_CHOICE_H
#define MULTIPLE_CHOICE_H

#include <SDL.h>
#include <SDL_video.h>

#include "../game_engine.h"

#define MAX_NUM_CHOICES 5

typedef struct {
	char* regular_path;
	char* pushed_path;
	SDL_Rect area;
} ChoiceButtonArguments;

typedef struct {
    SDL_Texture* regular_texture;
    SDL_Texture* pushed_texture;
    SDL_Rect area;
} ChoiceButton;

typedef struct {
	ChoiceButton choices[MAX_NUM_CHOICES];
	int choice;
	int num_choices;
} MultipleChoice;


EngineMessage create_multiple_choice(MultipleChoice **multiple_choice, SDL_Renderer *renderer, ChoiceButtonArguments choices[], int num_choices);
EngineMessage draw_multiple_choice(SDL_Renderer *renderer, MultipleChoice *multiple_choice);
void destroy_multiple_choice(MultipleChoice *multiple_choice);
EngineMessage multiple_choice_event_handler(SDL_Event *event, MultipleChoice *multiple_choice);

#endif
