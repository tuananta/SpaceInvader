#include "model.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <time.h>

//---1.HELPERS---
//Enemy logic
void enemy_try_shoot(GameState *game){
    //Fire rate: ~1-2% per frame
    if(rand()%100 < 2){ 

        //Find inactive slot
        for(int i = 0; i < MAX_PROJECTILES; i++){
            if(!game->projectiles[i].active){

                //Pick a random living enemy
                int attempt = 0;
                int enemy_idx = -1;
                while(attempt < 100){
                    int r = rand()%TOTAL_ENEMIES;
                    if(game->enemies[r].alive){
                        enemy_idx = r;
                        break;
                    }
                    attempt++;
                }

                //If shooter found
                if(enemy_idx != -1){
                    Projectile *p = &game->projectiles[i];
                    Enemy *e = &game->enemies[enemy_idx];

                    p->active = true;
                    p->from_player = false; //Enemy fire
                    p->base.width = 5;
                    p->base.height = 10;
                    p->base.x = e->base.x + (e->base.width / 2);
                    p->base.y = e->base.y + e->base.height; //Spawn at bottom
                    p->dy = 1.0f; //Move DOWN
                }
                break; //One shot per call
            }
        }
    }
}

//Collision check
bool check_collision(BaseEntity a, BaseEntity b){
    return (a.x < b.x + b.width && 
            a.x + a.width > b.x && 
            a.y < b.y + b.height && 
            a.y + a.height > b.y);
}

//Screen boundary check
bool check_enemy_edge(GameState *game, int dir){
    for(int i = 0; i < TOTAL_ENEMIES; i++){
        if(game->enemies[i].alive){
            float x = game->enemies[i].base.x;
            int w = game->enemies[i].base.width;

            if(dir == 1 && (x + w) >= GAME_WIDTH) return true;
            if(dir == -1 && x <= 0) return true;
        }
    }
    return false;
}

//---2.COLLISIONS---

//Process all hits
void handle_collisions(GameState *game){
    for(int i = 0; i < MAX_PROJECTILES; i++){
        //Player bullets
        if(game->projectiles[i].active && game->projectiles[i].from_player){
            
            for(int j = 0; j < TOTAL_ENEMIES; j++){
                if(game->enemies[j].alive){
                    
                    if(check_collision(game->projectiles[i].base, game->enemies[j].base)){
                        
                        //Target hit
                        game->enemies[j].alive = false;
                        game->projectiles[i].active = false;
                        
                        //Add points
                        if(game->enemies[j].type == ENTITY_ENEMY_SQUID) 
                            game->player.score += 30;
                        else if(game->enemies[j].type == ENTITY_ENEMY_CRAB) 
                            game->player.score += 20;
                        else 
                            game->player.score += 10;

                        break; 
                    }
                }
            }
        }
    }
    //Enemy bullets vs Player
    for(int i = 0; i < MAX_PROJECTILES; i++){
        if(game->projectiles[i].active && !game->projectiles[i].from_player){

            if(check_collision(game->projectiles[i].base, game->player.base)){

                //Player hit
                game->player.lives--;
                game->projectiles[i].active = false;

                //Respawn position
                game->player.base.x = (GAME_WIDTH - game->player.base.width) / 2;

                if(game->player.lives <= 0){
                    game->status = GAME_OVER;
                }
            }
        }
    }
}

//---3.MAIN LOGIC---

void init_game(GameState *game){
    //Globals
    srand(time(NULL));
    game->status = GAME_PLAYING;
    game->level = 1;
    game->last_update_time = 0;

    //Setup Player
    game->player.base.width = 50;
    game->player.base.height = 30;
    game->player.base.x = (GAME_WIDTH - game->player.base.width) / 2;
    game->player.base.y = GAME_HEIGHT - 60;
    game->player.base.speed = PLAYER_SPEED;
    game->player.lives = 3;
    game->player.score = 0;
    game->player.cooldown = 0;

    //Setup Grid
    game->enemy_direction = 1;
    game->enemy_move_timer = 0;

    int start_x = 50;
    int start_y = 50;
    int spacing_x = 60;
    int spacing_y = 50;

    for(int row = 0; row < NUM_ENEMIES_ROWS; row++){
        EntityType type;
        if(row == 0) type = ENTITY_ENEMY_SQUID;
        else if(row < 3) type = ENTITY_ENEMY_CRAB;
        else type = ENTITY_ENEMY_OCTOPUS;

        for(int col = 0; col < NUM_ENEMIES_COLS; col++){
            int index = row * NUM_ENEMIES_COLS + col;
            Enemy *e = &game->enemies[index];

            e->base.x = start_x + (col * spacing_x);
            e->base.y = start_y + (row * spacing_y);
            e->base.width = 40;
            e->base.height = 30;
            e->alive = true;
            e->type = type;
        }
    }

    //Clear bullets
    for(int i = 0; i < MAX_PROJECTILES; i++){
        game->projectiles[i].active = false;
    }
}

void update_game(GameState *game, float dt){
    (void)dt; 
    
    if(game->status != GAME_PLAYING) return;

    //Cooldown
    if(game->player.cooldown > 0){
        game->player.cooldown--;
    }

    //Bullets
    for(int i = 0; i < MAX_PROJECTILES; i++){
        if(game->projectiles[i].active){
            game->projectiles[i].base.y += (game->projectiles[i].dy * PROJECTILE_SPEED);

            if(game->projectiles[i].base.y < 0 || game->projectiles[i].base.y > GAME_HEIGHT){
                game->projectiles[i].active = false;
            }
        }
    }

    enemy_try_shoot(game);
    handle_collisions(game);

    //Movement logic
    bool touch_edge = check_enemy_edge(game, game->enemy_direction);

    if(touch_edge){
        game->enemy_direction *= -1;
        
        for(int i = 0; i < TOTAL_ENEMIES; i++){
            game->enemies[i].base.y += ENEMY_DROP_Y;
            
            //Lose condition
            if(game->enemies[i].alive && 
               (game->enemies[i].base.y + game->enemies[i].base.height) >= game->player.base.y){
                game->status = GAME_OVER;
            }
        }
    } else {
        for(int i = 0; i < TOTAL_ENEMIES; i++){
            if(game->enemies[i].alive){
                game->enemies[i].base.x += (ENEMY_SPEED_X * game->enemy_direction);
            }
        }
    }
}

void player_move(GameState *game, Direction dir){
    if(game->status != GAME_PLAYING) return;

    float new_x = game->player.base.x + (dir * game->player.base.speed);

    //Stay on screen
    if(new_x < 0) new_x = 0;
    if(new_x + game->player.base.width > GAME_WIDTH){
        new_x = GAME_WIDTH - game->player.base.width;
    }

    game->player.base.x = new_x;
}

void player_shoot(GameState *game){
    if(game->player.cooldown > 0) return;

    for(int i = 0; i < MAX_PROJECTILES; i++){
        if(!game->projectiles[i].active){
            game->projectiles[i].active = true;
            game->projectiles[i].from_player = true;
            game->projectiles[i].base.width = 5;
            game->projectiles[i].base.height = 10;
            
            game->projectiles[i].base.x = game->player.base.x + (game->player.base.width / 2) - 2;
            game->projectiles[i].base.y = game->player.base.y - 10;
            game->projectiles[i].dy = -1.0f; //Up

            game->player.cooldown = 20; 
            break;
        }
    }
}