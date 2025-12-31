#include "model.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <time.h>

/* --- 1. HELPERS --- */

/**
 * Re-initializes enemy grid positions and states for a new level.
 * Ensures the model is modular and reusable.
 */
void spawn_enemies(GameState *game) {
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

    /* Clear all active projectiles to prevent ghost hits during transition */
    for(int i = 0; i < MAX_PROJECTILES; i++){
        game->projectiles[i].active = false;
    }
}

/**
 * Handles enemy shooting logic based on a random chance.
 * Difficulty scales slightly with level.
 */
void enemy_try_shoot(GameState *game){
    /* Shooting probability increases slightly with levels */
    int fire_chance = 2 + (game->level / 2); 
    if(rand() % 100 < fire_chance){ 

        for(int i = 0; i < MAX_PROJECTILES; i++){
            if(!game->projectiles[i].active){
                int attempt = 0;
                int enemy_idx = -1;
                
                /* Randomly select a living enemy to shoot */
                while(attempt < 100){
                    int r = rand() % TOTAL_ENEMIES;
                    if(game->enemies[r].alive){
                        enemy_idx = r;
                        break;
                    }
                    attempt++;
                }

                if(enemy_idx != -1){
                    Projectile *p = &game->projectiles[i];
                    Enemy *e = &game->enemies[enemy_idx];

                    p->active = true;
                    p->from_player = false; 
                    p->base.width = 5;
                    p->base.height = 10;
                    p->base.x = e->base.x + (e->base.width / 2);
                    p->base.y = e->base.y + e->base.height;
                    p->dy = 1.0f; 
                }
                break; 
            }
        }
    }
}




/**
 * Checks if any living enemy has touched the screen boundaries.
 */
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

/* --- 2. COLLISIONS --- */
bool check_collision(BaseEntity a, BaseEntity b){
    return (a.x < b.x + b.width && 
            a.x + a.width > b.x && 
            a.y < b.y + b.height && 
            a.y + a.height > b.y);
}

void handle_collisions(GameState *game){
    for(int i = 0; i < MAX_PROJECTILES; i++){
        if(!game->projectiles[i].active) continue;

        /* Player projectiles hitting enemies */
        if(game->projectiles[i].from_player){
            for(int j = 0; j < TOTAL_ENEMIES; j++){
                if(game->enemies[j].alive && check_collision(game->projectiles[i].base, game->enemies[j].base)){
                    game->enemies[j].alive = false;
                    game->projectiles[i].active = false;
                    
                    /* Scoring based on enemy type */
                    if(game->enemies[j].type == ENTITY_ENEMY_SQUID) game->player.score += 30;
                    else if(game->enemies[j].type == ENTITY_ENEMY_CRAB) game->player.score += 20;
                    else game->player.score += 10;
                    break; 
                }
            }
        } 
        /* Enemy projectiles hitting player */
        else {
            if(check_collision(game->projectiles[i].base, game->player.base)){
                game->player.lives--;
                game->projectiles[i].active = false;
                /* Reset player position after hit */
                game->player.base.x = (GAME_WIDTH - game->player.base.width) / 2;

                if(game->player.lives <= 0) game->status = GAME_OVER;
            }
        }
    }
}

/* --- 3. MAIN LOGIC & LEVEL MANAGEMENT --- */

/**
 * Initializes a specific level and adjusts difficulty parameters.
 */
void init_level(GameState *game, int level_number) {
    game->level = level_number;
    spawn_enemies(game);
    /* Future: Adjust global speed variables here if stored in GameState */
}

/**
 * Sets up the global game state and starts Level 1.
 */
void init_game(GameState *game){
    srand(time(NULL));
    game->status = GAME_PLAYING;
    game->last_update_time = 0;

    /* Player Setup */
    game->player.base.width = 50;
    game->player.base.height = 30;
    game->player.base.x = (GAME_WIDTH - game->player.base.width) / 2;
    game->player.base.y = GAME_HEIGHT - 60;
    game->player.base.speed = PLAYER_SPEED;
    game->player.lives = 10;
    game->player.score = 0;
    game->player.cooldown = 0;

    /* Enemy Movement Setup */
    game->enemy_direction = 1;
    game->enemy_move_timer = 0;

    /* Start the first level */
    init_level(game, 1);
}



void update_game(GameState *game, float dt){
    (void)dt; 
    if(game->status != GAME_PLAYING) return;

    /* 1. Count living enemies and check for level completion */
    int alive_count = 0;
    for(int i = 0; i < TOTAL_ENEMIES; i++) {
        if(game->enemies[i].alive) {
            alive_count++;
        }
    }

    if(alive_count == 0) {
        init_level(game, game->level + 1);
        return;
    }

    /* 2. Calculate dynamic acceleration */
    /* killed_ratio will be 0.0 at start and close to 1.0 when nearly all are dead */
    float killed_ratio = (float)(TOTAL_ENEMIES - alive_count) / TOTAL_ENEMIES;
    
    /* Dynamic speed formula: 
       Base_Speed + Level_Bonus + (Acceleration_Factor * killed_ratio) */
    float acceleration_factor = 3.0f; // How much faster it gets when almost all are dead
    float current_enemy_speed = (ENEMY_SPEED_X + (game->level * 0.2f)) + (acceleration_factor * killed_ratio);

    /* --- Rest of your logic remains the same --- */

    /* Player cooldown logic */
    if(game->player.cooldown > 0) game->player.cooldown--;

    /* Projectile movement */
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

    /* Enemy horizontal and vertical movement */
    bool touch_edge = check_enemy_edge(game, game->enemy_direction);

    if(touch_edge){
        game->enemy_direction *= -1;
        for(int i = 0; i < TOTAL_ENEMIES; i++){
            if(!game->enemies[i].alive) continue; // Optimization: skip dead enemies
            
            game->enemies[i].base.y += ENEMY_DROP_Y;
            /* Check if enemies reached the player's level */
            if((game->enemies[i].base.y + game->enemies[i].base.height) >= game->player.base.y){
                game->status = GAME_OVER;
            }
        }
    } else {
        for(int i = 0; i < TOTAL_ENEMIES; i++){
            if(game->enemies[i].alive){
                // Use the new dynamic speed here
                game->enemies[i].base.x += (current_enemy_speed * game->enemy_direction);
            }
        }
    }
}

void player_move(GameState *game, Direction dir){
    if(game->status != GAME_PLAYING) return;

    float new_x = game->player.base.x + (dir * game->player.base.speed);
    if(new_x < 0) new_x = 0;
    if(new_x + game->player.base.width > GAME_WIDTH) new_x = GAME_WIDTH - game->player.base.width;

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
            game->projectiles[i].dy = -1.0f;
            game->player.cooldown = 20; 
            break;
        }
    }
}