#include "view.h"
#include <SDL3/SDL.h>
#include <stdio.h>
#include "../../include/render_entities.h"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

void sdl_init(void){
    if(!SDL_Init(SDL_INIT_VIDEO)){
        SDL_Log("SDL Init Error: %s", SDL_GetError());
        return;
    }

    window = SDL_CreateWindow("Space Invaders", GAME_WIDTH, GAME_HEIGHT, 0);
    if(!window){
        SDL_Log("Window Error: %s", SDL_GetError());
        return;
    }

    renderer = SDL_CreateRenderer(window, NULL);
    if(!renderer){
        SDL_Log("Renderer Error: %s", SDL_GetError());
    }
}

void sdl_cleanup(void){
    if(renderer) SDL_DestroyRenderer(renderer);
    if(window) SDL_DestroyWindow(window);
    SDL_Quit();
}

void sdl_clear(void){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

/* Updated to include level in the title */
void sdl_update_title(int score, int lives, int level){
    char title[128];
    sprintf(title, "Space Invaders   Level: %d   Score: %d   Lives: %d", level, score, lives);
    SDL_SetWindowTitle(window, title);
}

void sdl_draw(const GameState *game){
    if(game->status == GAME_OVER){
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Màn hình đen xì
        SDL_RenderClear(renderer);
        
        // print score
        static bool score_printed = false;
        if (!score_printed) {
            printf("\n--- END! ---\n");
            printf("Final Score: %d\n", game->player.score);
            printf("Press 'R' to restart or 'ESC' to quit.\n");
            score_printed = true;
        }

        // aff end
        char title[64];
        sprintf(title, "End!    Final Score: %d", game->player.score);
        SDL_SetWindowTitle(window, title);

        SDL_RenderPresent(renderer);
        return; 
    }
    /* 1. Background & Title Update */
    sdl_update_title(game->player.score, game->player.lives, game->level);

    if(game->status == GAME_OVER){
        SDL_SetRenderDrawColor(renderer, 100, 0, 0, 255); /* Reddish for Game Over */
    } else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
    }
    SDL_RenderClear(renderer);

    if(game->status == GAME_OVER) {
        SDL_RenderPresent(renderer);
        return; 
    }

    /* 2. Draw Player */
    draw_pixel_player(renderer, game->player.base.x, game->player.base.y, (float)game->player.base.width);

    /* 3. Draw Enemies */
    for(int i = 0; i < TOTAL_ENEMIES; i++){
        if(game->enemies[i].alive){
            float x = game->enemies[i].base.x;
            float y = game->enemies[i].base.y;
            float w = (float)game->enemies[i].base.width;

            if(game->enemies[i].type == ENTITY_ENEMY_SQUID) draw_pixel_squid(renderer, x, y, w);
            else if(game->enemies[i].type == ENTITY_ENEMY_CRAB) draw_pixel_crab(renderer, x, y, w);
            else if(game->enemies[i].type == ENTITY_ENEMY_OCTOPUS) draw_pixel_octopus(renderer, x, y, w);
        }
    }

    /* 4. Draw Projectiles */
    for(int i = 0; i < MAX_PROJECTILES; i++){
        if(game->projectiles[i].active){
            if(game->projectiles[i].from_player){
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); /* White for player */
            } else {
                SDL_SetRenderDrawColor(renderer, 255, 100, 255, 255); /* Pinkish for enemies */
            }

            SDL_FRect bullet_rect = {
                game->projectiles[i].base.x,
                game->projectiles[i].base.y,
                (float)game->projectiles[i].base.width,
                (float)game->projectiles[i].base.height
            };
            SDL_RenderFillRect(renderer, &bullet_rect);
        }
    }

    SDL_RenderPresent(renderer);
}

GameView view_sdl = {
    .init = sdl_init,
    .cleanup = sdl_cleanup,
    .clear_screen = sdl_clear,
    .draw = sdl_draw,
    .update_title = sdl_update_title 
};

