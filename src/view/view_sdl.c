#include "view.h"
#include <SDL3/SDL.h>
#include <stdio.h>

//Window & Renderer pointers
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

void sdl_init(void){
    if(!SDL_Init(SDL_INIT_VIDEO)){
        printf("SDL Init Error: %s\n", SDL_GetError());
        return;
    }

    //Create 800x600 window
    window = SDL_CreateWindow("Space Invaders (SDL3 Mode)", GAME_WIDTH, GAME_HEIGHT, 0);
    if(!window){
        printf("Window Error: %s\n", SDL_GetError());
        return;
    }

    renderer = SDL_CreateRenderer(window, NULL);
    if(!renderer){
        printf("Renderer Error: %s\n", SDL_GetError());
    }
}

void sdl_cleanup(void){
    if(renderer) SDL_DestroyRenderer(renderer);
    if(window) SDL_DestroyWindow(window);
    SDL_Quit();
    printf("SDL UI closed.\n");
}

void sdl_clear(void){
    //Set black background
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void sdl_update_title(int score, int lives){
    char title[64];
    sprintf(title, "Space Invaders | Score: %d | Lives: %d", score, lives);
    SDL_SetWindowTitle(window, title);
}

void sdl_draw(const GameState *game){
    //Background color logic
    if(game->status == GAME_OVER){
        SDL_SetRenderDrawColor(renderer, 100, 0, 0, 255); //Dark red
    } else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); //Black
    }
    SDL_RenderClear(renderer);

    //Handle Game Over state
    if(game->status == GAME_OVER){
        SDL_RenderPresent(renderer);
        return; 
    }

    //Render Player (Green)
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_FRect player_rect = {
        game->player.base.x, 
        game->player.base.y, 
        (float)game->player.base.width, 
        (float)game->player.base.height
    };
    SDL_RenderFillRect(renderer, &player_rect);

    //Render Enemies
    for(int i = 0; i < TOTAL_ENEMIES; i++){
        if(game->enemies[i].alive){
            //Color by type
            if(game->enemies[i].type == ENTITY_ENEMY_SQUID) 
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); //Red
            else if(game->enemies[i].type == ENTITY_ENEMY_CRAB) 
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); //Yellow
            else 
                SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255); //Cyan

            SDL_FRect enemy_rect = {
                game->enemies[i].base.x,
                game->enemies[i].base.y,
                (float)game->enemies[i].base.width,
                (float)game->enemies[i].base.height
            };
            SDL_RenderFillRect(renderer, &enemy_rect);
        }
    }

    //Render Projectiles
    for(int i = 0; i < MAX_PROJECTILES; i++){
        if(game->projectiles[i].active){
            
            //Player vs Enemy colors
            if(game->projectiles[i].from_player){
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); //White
            } else {
                SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255); //Magenta
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

//Interface setup
GameView view_sdl = {
    .init = sdl_init,
    .cleanup = sdl_cleanup,
    .clear_screen = sdl_clear,
    .draw = sdl_draw,
    .update_title = sdl_update_title
};