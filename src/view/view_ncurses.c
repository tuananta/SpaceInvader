#include "view.h"
#include <ncurses.h>
#include <stdio.h>

//Scale factors
#define SCALE_X 10
#define SCALE_Y 20

//---1.INITIALIZATION---
void ncurses_init(void){
    initscr();              //Start session
    cbreak();               //Disable line buffering
    noecho();               //Hide keystrokes
    curs_set(0);            //Hide cursor
    
    //Crucial for real-time input
    nodelay(stdscr, TRUE);  //Non-blocking getch
    keypad(stdscr, TRUE);   //Enable arrows
}

void ncurses_cleanup(void){
    erase();
    refresh();

    endwin();
}

void ncurses_clear(void){
    erase(); //Wipe screen
}

//---2.RENDERING---
void ncurses_draw(const GameState *game){
    //Frame border
    box(stdscr, 0, 0);

    //HUD
    mvprintw(0, 2, "[ SPACE INVADERS - NCURSES ]");
    mvprintw(1, 2, "Level: %d | Score: %d | Lives: %d", 
             game->level, game->player.score, game->player.lives);

    //State check
    if(game->status == GAME_OVER){
        mvprintw(10, 20, "!!! GAME OVER !!!");
        mvprintw(11, 15, "Press 'R' to Restart");
        refresh();
        return;
    }

    //Render Player
    int px = (int)(game->player.base.x / SCALE_X);
    int py = (int)(game->player.base.y / SCALE_Y);
    mvprintw(py, px, "A");

    //Render Swarm
    for(int i = 0; i < TOTAL_ENEMIES; i++){
        if(game->enemies[i].alive){
            int ex = (int)(game->enemies[i].base.x / SCALE_X);
            int ey = (int)(game->enemies[i].base.y / SCALE_Y);
            
            char sprite = 'M';
            if(game->enemies[i].type == ENTITY_ENEMY_SQUID) sprite = 'W';
            else if(game->enemies[i].type == ENTITY_ENEMY_CRAB) sprite = 'H';
            
            mvprintw(ey, ex, "%c", sprite);
        }
    }

    //Render Projectiles
    for(int i = 0; i < MAX_PROJECTILES; i++){
        if(game->projectiles[i].active){
            int bx = (int)(game->projectiles[i].base.x / SCALE_X);
            int by = (int)(game->projectiles[i].base.y / SCALE_Y);
            
            if(by > 0 && by < 30){
                if(game->projectiles[i].from_player)
                    mvprintw(by, bx, "|"); //Ally
                else
                    mvprintw(by, bx, "v"); //Enemy
            }
        }
    }

    refresh(); //Push to screen
}

//Update stats
void ncurses_update_title(int score, int lives){
    (void)score; (void)lives; //Unused
}

//---3.INTERFACE BINDING---
GameView view_ncurses = {
    .init = ncurses_init,
    .cleanup = ncurses_cleanup,
    .clear_screen = ncurses_clear,
    .draw = ncurses_draw,
    .update_title = ncurses_update_title
};