#ifndef MODEL_H
#define MODEL_H

#include <stdbool.h>

/* --- GAME SETTINGS --- */
#define GAME_WIDTH 800
#define GAME_HEIGHT 600

/* Baseline difficulty constants */
#define PLAYER_SPEED 5.0f
#define ENEMY_SPEED_X 2.0f    
#define ENEMY_DROP_Y 10.0f
#define PROJECTILE_SPEED 7.0f

#define TOTAL_ENEMIES 55      
#define NUM_ENEMIES_ROWS 5
#define NUM_ENEMIES_COLS 11
#define MAX_PROJECTILES 100 


typedef enum {
    ENTITY_PLAYER,
    ENTITY_ENEMY_SQUID,
    ENTITY_ENEMY_CRAB,
    ENTITY_ENEMY_OCTOPUS,
    ENTITY_PROJECTILE
} EntityType;

typedef enum {
    DIR_LEFT = -1,
    DIR_RIGHT = 1
} Direction;

typedef enum {
    GAME_PLAYING,
    GAME_OVER,
    GAME_WIN
} GameStatus;

/* --- STRUCTURES --- */

typedef struct {
    float x, y;
    int width, height;
    float speed;
} BaseEntity;

typedef struct {
    BaseEntity base;
    int lives;
    int score;
    int cooldown;
} Player;

typedef struct {
    BaseEntity base;
    bool alive;
    EntityType type;
} Enemy;

typedef struct {
    BaseEntity base;
    bool active;
    bool from_player;
    float dy;
} Projectile;

/* * Forward declaration of GameState to be used in function prototypes 
 * before the full struct definition.
 */
typedef struct GameState GameState;

struct GameState {
    GameStatus status;
    int level;
    float last_update_time;
    
    Player player;
    Enemy enemies[TOTAL_ENEMIES];
    Projectile projectiles[MAX_PROJECTILES];

    int enemy_direction; 
    float enemy_move_timer;
};

/* --- CORE LOGIC FUNCTION PROTOTYPES --- */

/**
 * Initializes the global game state (score, lives, level 1).
 */
void init_game(GameState *game);

/**
 * Sets up a specific level, spawning enemies and resetting projectiles.
 */
void init_level(GameState *game, int level_number);

/**
 * Populates the enemy grid.
 */
void spawn_enemies(GameState *game);

/**
 * Main logic update called every frame.
 * @param dt Delta time (time passed since last frame).
 */
void update_game(GameState *game, float dt);

/**
 * Moves the player horizontally.
 */
void player_move(GameState *game, Direction dir);

/**
 * Triggers player shooting logic.
 */
void player_shoot(GameState *game);

/**
 * Checks if enemies reached the screen edge.
 */
bool check_enemy_edge(GameState *game, int dir);

/**
 * Internal helper: Handles enemy projectile logic.
 */
void enemy_try_shoot(GameState *game);

/**
 * Internal helper: Processes all AABB collisions.
 */
void handle_collisions(GameState *game);

#endif /* MODEL_H */