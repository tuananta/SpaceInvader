#include "controller.h"
#include <SDL3/SDL.h>
#include <ncurses.h>
int handle_input(GameState *game, bool is_sdl_mode) {
    if(is_sdl_mode) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) return 0;
            if (event.type == SDL_EVENT_KEY_DOWN) {
                if(event.key.key == SDLK_ESCAPE) return 0;
                
                if(game->status == GAME_OVER) {
                    if (event.key.key == SDLK_R) init_game(game);
                }else {
                    if (event.key.key == SDLK_LEFT) player_move(game, DIR_LEFT);
                    if (event.key.key == SDLK_RIGHT) player_move(game, DIR_RIGHT);
                    if (event.key.key == SDLK_SPACE) player_shoot(game);
                }
            }
        }
    }
    else{
        int ch = getch();
        if(ch != ERR) {
            if(ch == 'q') return 0;
            if(game->status == GAME_OVER) {
                if (ch == 'r' || ch == 'R') init_game(game);
            } 
            else {
                if (ch == KEY_LEFT) player_move(game, DIR_LEFT);
                else if (ch == KEY_RIGHT) player_move(game, DIR_RIGHT);
                else if (ch == ' ') player_shoot(game);
            }
        }
    }
    return 1; 
}