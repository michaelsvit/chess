#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL.h>
#include <SDL_video.h>
#include "../game_engine.h"

typedef struct {
    SDL_Texture* texture;
    SDL_Rect area;
} Texture;

EngineMessage create_inactive_texture(Texture **texture, SDL_Rect *area, SDL_Renderer *renderer, char *path);
EngineMessage draw_inactive_texture(SDL_Renderer *renderer, Texture *texture);
void destroy_inactive_texture(Texture *texture);

#endif
