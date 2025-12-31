#include "view.h"
#include <ncurses.h>
#include <stdio.h>

/* Scale factors to map 800x600 coordinates to terminal rows/cols */
#define SCALE_X 10
#define SCALE_Y 25 

/* --- 1. INITIALIZATION --- */
void ncurses_init(void){
    initscr();              /* Start ncurses mode */
    cbreak();               /* Line buffering disabled */
    noecho();               /* Don't echo() while we do getch */
    curs_set(0);            /* Hide terminal cursor */
    
    /* Crucial for real-time game loop */
    nodelay(stdscr, TRUE);  /* Non-blocking input */
    keypad(stdscr, TRUE);   /* Enable arrow keys */

    /* Optional: Start color functionality if terminal supports it */
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_GREEN, COLOR_BLACK); /* Player */
        init_pair(2, COLOR_RED, COLOR_BLACK);   /* Enemies */
        init_pair(3, COLOR_YELLOW, COLOR_BLACK);/* Bullets */
    }
}

void ncurses_cleanup(void){
    erase();
    refresh();
    endwin();
}

void ncurses_clear(void){
    erase(); 
}

/* --- 2. RENDERING --- */
void ncurses_draw(const GameState *game){
    if(game->status == GAME_OVER){
        clear(); 
        
       
        int py = LINES / 2;
        int px = COLS / 2;

        attron(A_BOLD);
        mvprintw(py - 1, px - 3, "End!"); 
        mvprintw(py + 1, px - 10, "Final Score: %d", game->player.score);
        attroff(A_BOLD);

        mvprintw(py + 3, px - 12, "Press 'R' to Play Again");
        
        refresh();
        return;
    }
    /* Draw border around the playable area */
    box(stdscr, 0, 0);

    /* HUD: Displaying Level, Score, and Lives as requested */
    attron(A_BOLD);
    mvprintw(0, 2, "[ SPACE INVADERS ]");
    attroff(A_BOLD);
    
    mvprintw(1, 2, "LEVEL: %-2d | SCORE: %-5d | LIVES: %-2d", 
             game->level, game->player.score, game->player.lives);

    /* Game Over State */
    if(game->status == GAME_OVER){
        attron(A_REVERSE);
        mvprintw(12, 30, "  !!! GAME OVER !!!  ");
        mvprintw(13, 28, " Press 'R' to Restart ");
        attroff(A_REVERSE);
        refresh();
        return;
    }

    /* Render Player */
    int px = (int)(game->player.base.x / SCALE_X);
    int py = (int)(game->player.base.y / SCALE_Y);
    attron(COLOR_PAIR(1));
    mvprintw(py, px, "A"); 
    attroff(COLOR_PAIR(1));

    /* Render Swarm with different sprites for each type */
    attron(COLOR_PAIR(2));
    for(int i = 0; i < TOTAL_ENEMIES; i++){
        if(game->enemies[i].alive){
            int ex = (int)(game->enemies[i].base.x / SCALE_X);
            int ey = (int)(game->enemies[i].base.y / SCALE_Y);
            
            char sprite;
            if(game->enemies[i].type == ENTITY_ENEMY_SQUID)      sprite = 'W';
            else if(game->enemies[i].type == ENTITY_ENEMY_CRAB) sprite = 'H';
            else if(game->enemies[i].type == ENTITY_ENEMY_OCTOPUS) sprite = 'O';
            else sprite = 'M';
            
            mvaddch(ey, ex, sprite);
        }
    }
    attroff(COLOR_PAIR(2));

    /* Render Projectiles */
    for(int i = 0; i < MAX_PROJECTILES; i++){
        if(game->projectiles[i].active){
            int bx = (int)(game->projectiles[i].base.x / SCALE_X);
            int by = (int)(game->projectiles[i].base.y / SCALE_Y);
            
            /* Basic boundary check for terminal height */
            if(by > 1 && by < LINES - 1){
                if(game->projectiles[i].from_player) {
                    attron(COLOR_PAIR(3));
                    mvaddch(by, bx, '|'); 
                    attroff(COLOR_PAIR(3));
                } else {
                    mvaddch(by, bx, '*'); 
                }
            }
        }
    }

    refresh(); 
}

/* Update title (mostly used for SDL, but kept for interface consistency) */
// void ncurses_update_title(int score, int lives, int level){
//     (void)score; (void)lives; (void)level;
// }
void ncurses_update_title(int score, int lives, int level) { // Thêm int level vào đây
    (void)score; (void)lives; (void)level; 
}


/* --- 3. INTERFACE BINDING --- */
GameView view_ncurses = {
    .init = ncurses_init,
    .cleanup = ncurses_cleanup,
    .clear_screen = ncurses_clear,
    .draw = ncurses_draw,
    .update_title = ncurses_update_title
};