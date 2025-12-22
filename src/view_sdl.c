#include "view.h"
#include <SDL3/SDL.h>
#include <stdio.h>

// Biến cục bộ để quản lý cửa sổ SDL
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

void sdl_init(void) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("Lỗi SDL Init: %s\n", SDL_GetError());
        return;
    }

    // Tạo cửa sổ kích thước đúng bằng GAME_WIDTH x GAME_HEIGHT (800x600)
    window = SDL_CreateWindow("Space Invaders (SDL3 Mode)", GAME_WIDTH, GAME_HEIGHT, 0);
    if (!window) {
        printf("Lỗi tạo Window: %s\n", SDL_GetError());
        return;
    }

    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        printf("Lỗi tạo Renderer: %s\n", SDL_GetError());
    }
}

void sdl_cleanup(void) {
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
    printf("Da dong giao dien SDL.\n");
}

void sdl_clear(void) {
    // 1. Chọn màu đen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    // 2. Xóa màn hình bằng màu đen
    SDL_RenderClear(renderer);
}

void sdl_draw(const GameState *game) {
    // --- VẼ NGƯỜI CHƠI (Màu Xanh Lá) ---
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_FRect player_rect = {
        game->player.base.x, 
        game->player.base.y, 
        (float)game->player.base.width, 
        (float)game->player.base.height
    };
    SDL_RenderFillRect(renderer, &player_rect);

    // --- VẼ KẺ THÙ (Màu Trắng, Đỏ, Tím tùy loại) ---
    for (int i = 0; i < TOTAL_ENEMIES; i++) {
        if (game->enemies[i].alive) {
            // Chọn màu dựa trên loại enemy
            if (game->enemies[i].type == ENTITY_ENEMY_SQUID) 
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Đỏ
            else if (game->enemies[i].type == ENTITY_ENEMY_CRAB) 
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Vàng
            else 
                SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255); // Cyan

            SDL_FRect enemy_rect = {
                game->enemies[i].base.x,
                game->enemies[i].base.y,
                (float)game->enemies[i].base.width,
                (float)game->enemies[i].base.height
            };
            SDL_RenderFillRect(renderer, &enemy_rect);
        }
    }

    // --- VẼ ĐẠN (Màu Trắng) ---
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (game->projectiles[i].active) {
            SDL_FRect bullet_rect = {
                game->projectiles[i].base.x,
                game->projectiles[i].base.y,
                (float)game->projectiles[i].base.width,
                (float)game->projectiles[i].base.height
            };
            SDL_RenderFillRect(renderer, &bullet_rect);
        }
    }

    // Đẩy hình ảnh lên màn hình
    SDL_RenderPresent(renderer);
}

// Định nghĩa Interface cho SDL
GameView view_sdl = {
    .init = sdl_init,
    .cleanup = sdl_cleanup,
    .clear_screen = sdl_clear,
    .draw = sdl_draw
};