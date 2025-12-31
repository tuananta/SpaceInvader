
// #include <stdio.h>
// #include <stdlib.h>
// #include <time.h>
// #include <unistd.h>
// #include "model.h"
// #include "view.h"
// #include "controller.h"
// #include <string.h>

// bool is_sdl_mode = false;

// int main(int argc, char *argv[]){
//     srand(time(NULL));

//     // 1. Select UI mode
//     GameView *view = NULL;
//     if(argc > 1 && strcmp(argv[1], "-sdl") == 0){
//         view = &view_sdl;
//         is_sdl_mode = true;
//     } else if(argc > 1 && strcmp(argv[1], "-text") == 0){
//         view = &view_ncurses;
//         is_sdl_mode = false;
//     } else {
//         printf("1. Text mode (Ncurses)\n2. SDL3 mode\nSelect: ");
//         int choice;
//         if(scanf("%d", &choice) != 1) choice = 1;
//         while(getchar() != '\n'); // Flush buffer

//         if(choice == 2){
//             view = &view_sdl;
//             is_sdl_mode = true;
//         } else {
//             view = &view_ncurses;
//             is_sdl_mode = false;
//         }
//     }

//     // 2. Setup session
//     GameState game;
//     init_game(&game);
//     view->init();

//     // 3. Main loop
//     int running = 1;
    
//     while(running){
//     running = handle_input(&game, is_sdl_mode);

//     // Chỉ cập nhật logic nếu game đang chơi
//     if (game.status == GAME_PLAYING) {
//         update_game(&game, 0.016f);
//     }

//     view->clear_screen();
//     view->draw(&game);
//     usleep(16000); 
// }

//     // 4. Cleanup resources
//     view->cleanup();
//     return 0;
// }


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "model.h"
#include "view.h"
#include "controller.h"
#include <string.h>

/* Indicateur du mode d'affichage actuel (SDL ou Ncurses) */
bool is_sdl_mode = false;

int main(int argc, char *argv[]){
    /* Initialisation du générateur de nombres aléatoires */
    srand(time(NULL));

    /* 1. Sélection du mode de l'interface utilisateur (UI) */
    GameView *view = NULL;

    if(argc > 1 && strcmp(argv[1], "-sdl") == 0){
        view = &view_sdl;
        is_sdl_mode = true;
    } else if(argc > 1 && strcmp(argv[1], "-text") == 0){
        view = &view_ncurses;
        is_sdl_mode = false;
    } else {
        /* Menu de sélection si aucun argument n'est fourni */
        printf("1. Mode Texte (Ncurses)\n2. Mode SDL3\nSélection : ");
        int choice;
        if(scanf("%d", &choice) != 1) choice = 1;
        while(getchar() != '\n'); // Vider le tampon d'entrée

        if(choice == 2){
            view = &view_sdl;
            is_sdl_mode = true;
        } else {
            view = &view_ncurses;
            is_sdl_mode = false;
        }
    }

    /* 2. Configuration de la session de jeu */
    GameState game;
    init_game(&game);
    view->init();

    /* 3. Boucle principale du jeu */
    int running = 1;
    
    while(running){
        /* Gestion des entrées utilisateur */
        running = handle_input(&game, is_sdl_mode);

        /* Mise à jour de la logique uniquement si le jeu est en cours */
        if (game.status == GAME_PLAYING) {
            update_game(&game, 0.016f);
        }

        /* Rendu graphique de la scène */
        view->clear_screen();
        view->draw(&game);
        
        /* Pause pour stabiliser le taux de rafraîchissement (~60 FPS) */
        usleep(16000); 
    }

    /* 4. Nettoyage des ressources et fermeture */
    view->cleanup();
    return 0;
}