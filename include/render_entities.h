#ifndef RENDER_ENTITIES_H
#define RENDER_ENTITIES_H

#include <SDL3/SDL.h>

void draw_pixel_player(SDL_Renderer *renderer, float x, float y, float size);
void draw_pixel_crab(SDL_Renderer *renderer, float x, float y, float size);
void draw_pixel_squid(SDL_Renderer *renderer, float x, float y, float size);
void draw_pixel_octopus(SDL_Renderer *renderer, float x, float y, float size);

#endif