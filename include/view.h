#ifndef VIEW_H
#define VIEW_H

#include "model.h"
typedef struct {
    void (*init)(void);   
    //clean(fer windows)
    void (*cleanup)(void);  
    //clear old screen             
    void (*clear_screen)(void);                 
    void (*draw)(const GameState *game); 
    void (*update_title)(int score, int lives, int level);
} GameView;
extern GameView view_ncurses;
extern GameView view_sdl;
#endif