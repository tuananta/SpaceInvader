#define _DEFAULT_SOURCE
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
#include <SDL3/SDL.h>
#include "model.h"
#include "view.h"

// Biến kiểm tra xem ta đang dùng chế độ nào
bool is_sdl_mode = false;

int main(int argc, char *argv[]) {
    // 1. Xử lý tham số dòng lệnh để chọn giao diện
    GameView *view = &view_ncurses; // Mặc định là Ncurses
    
    if (argc > 1 && strcmp(argv[1], "-sdl") == 0) {
        view = &view_sdl;
        is_sdl_mode = true;
        printf("Dang khoi dong che do DO HOA (SDL3)...\n");
    } else {
        printf("Dang khoi dong che do TEXT (Ncurses)...\n");
        printf("Chay './space_invaders_app -sdl' de mo do hoa.\n");
        sleep(2); // Dừng 2s để người dùng kịp đọc
    }

    // 2. Khởi tạo
    GameState game;
    init_game(&game);
    view->init();

    // 3. Game Loop
    int running = 1;
    while (running) {
        
        // --- A. XỬ LÝ INPUT (Khác nhau giữa 2 chế độ) ---
        if (is_sdl_mode) {
            // == Chế độ SDL ==
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_EVENT_QUIT) running = 0;
                else if (event.type == SDL_EVENT_KEY_DOWN) {
                    if (event.key.key == SDLK_ESCAPE) running = 0;
                    if (event.key.key == SDLK_LEFT) player_move(&game, DIR_LEFT);
                    if (event.key.key == SDLK_RIGHT) player_move(&game, DIR_RIGHT);
                    if (event.key.key == SDLK_SPACE) player_shoot(&game);
                }
            }
        } else {
            // == Chế độ Ncurses ==
            int ch = getch();
            if (ch == 'q') running = 0;
            else if (ch == KEY_LEFT) player_move(&game, DIR_LEFT);
            else if (ch == KEY_RIGHT) player_move(&game, DIR_RIGHT);
            else if (ch == ' ') player_shoot(&game);
        }

        // --- B. UPDATE & DRAW ---
        update_game(&game, 0.016f);

        view->clear_screen();
        view->draw(&game);

        usleep(16000); // ~60 FPS
    }

    view->cleanup();
    return 0;
}