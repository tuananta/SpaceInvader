// #include <stdio.h>
// #include <stdlib.h>
// #include <time.h>
// #include <unistd.h>
// #include "model.h"
// #include "view.h"
// #include "controller.h"
// #include <string.h>

// bool is_sdl_mode = false;

// int main(int argc, char *argv[]) {
//     srand(time(NULL));

//     // 1. Menu chọn giao diện
//     GameView *view = NULL;
//     if (argc > 1 && strcmp(argv[1], "-sdl") == 0) {
//         view = &view_sdl;
//         is_sdl_mode = true;
//     } else if (argc > 1 && strcmp(argv[1], "-text") == 0) {
//         view = &view_ncurses;
//         is_sdl_mode = false;
//     } else {
//         printf("1. Che do Ncurses\n2. Che do SDL3\nLua chon: ");
//         int choice;
//         if (scanf("%d", &choice) != 1) choice = 1;
//         while (getchar() != '\n'); // Xóa bộ nhớ đệm phím Enter

//         if (choice == 2) {
//             view = &view_sdl;
//             is_sdl_mode = true;
//         } else {
//             view = &view_ncurses;
//         }
//     }

//     // 2. Khởi tạo
//     GameState game;
//     init_game(&game);
//     view->init();

//     // 3. Game Loop
//     int running = 1;
//     while (running) {
//         // A. XỬ LÝ INPUT (GỌI CONTROLLER)
//         running = handle_input(&game, is_sdl_mode);

//         // B. UPDATE MODEL
//         update_game(&game, 0.016f);

//         // C. DRAW VIEW
//         view->clear_screen();
//         view->draw(&game);
//         if (view->update_title) 
//             view->update_title(game.player.score, game.player.lives);

//         // D. FRAMERATE
//         usleep(16000); 
//     }

//     // 4. Giải phóng
//     view->cleanup();
//     return 0;
// }

//ben tren oke

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "model.h"
#include "view.h"
#include "controller.h"
#include <string.h>

bool is_sdl_mode = false;

int main(int argc, char *argv[]){
    srand(time(NULL));

    //Select UI mode
    GameView *view = NULL;
    if(argc > 1 && strcmp(argv[1], "-sdl") == 0){
        view = &view_sdl;
        is_sdl_mode = true;
    } else if(argc > 1 && strcmp(argv[1], "-text") == 0){
        view = &view_ncurses;
        is_sdl_mode = false;
    } else {
        printf("1. Text mode\n2. Sdl3 mode\nSelect: ");
        int choice;
        if(scanf("%d", &choice) != 1) choice = 1;
        while(getchar() != '\n'); //Flush buffer

        if(choice == 2){
            view = &view_sdl;
            is_sdl_mode = true;
        } else {
            view = &view_ncurses;
        }
    }

    //Setup session
    GameState game;
    init_game(&game);
    view->init();

    //Main loop
    int running = 1;
    while(running){
        //A.Input processing
        running = handle_input(&game, is_sdl_mode);

        //B.Logic update
        update_game(&game, 0.016f);

        //C.Rendering
        view->clear_screen();
        view->draw(&game);
        if(view->update_title) 
            view->update_title(game.player.score, game.player.lives);

        //D.Lock framerate (~60fps)
        usleep(16000); 
    }

    //Cleanup resources
    view->cleanup();
    return 0;
}