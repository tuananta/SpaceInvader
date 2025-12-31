#ifndef MODEL_H
#define MODEL_H

#include <stdbool.h>

//Game settings
#define GAME_WIDTH 800
#define GAME_HEIGHT 600

#define PLAYER_SPEED 5.0f
#define ENEMY_SPEED_X 2.0f
#define ENEMY_DROP_Y 10.0f
#define PROJECTILE_SPEED 7.0f

#define TOTAL_ENEMIES 55    //5 rows x 11 cols
#define NUM_ENEMIES_ROWS 5
#define NUM_ENEMIES_COLS 11
#define MAX_PROJECTILES 100 


//Unit types
typedef enum {
    ENTITY_PLAYER,
    ENTITY_ENEMY_SQUID,   //Lv1
    ENTITY_ENEMY_CRAB,    //Lv2
    ENTITY_ENEMY_OCTOPUS, //Lv3
    ENTITY_PROJECTILE
} EntityType;

//Directions
typedef enum {
    DIR_LEFT = -1,
    DIR_RIGHT = 1
} Direction;

//Components
typedef struct {
    float x, y;
    int width, height;
    float speed;
} BaseEntity;

typedef struct {
    BaseEntity base; //Inheritance
    int lives;
    int score;
    int cooldown; //Shot delay
} Player;


typedef struct {
    BaseEntity base;
    bool alive;      //Active flag
    EntityType type; //For scoring
} Enemy;


typedef struct {
    BaseEntity base;
    bool active;      //Visibility flag
    bool from_player; //Owner check
    float dy;         //Y-axis vector
} Projectile;


typedef enum {
    GAME_PLAYING,
    GAME_OVER,
    GAME_WIN
} GameStatus;

typedef struct {
    GameStatus status;
    int level;
    float last_update_time;
    
    Player player;
    Enemy enemies[TOTAL_ENEMIES];
    Projectile projectiles[MAX_PROJECTILES];

    //Swarm logic
    int enemy_direction; //Side toggle
    float enemy_move_timer;
} GameState;

//---CORE LOGIC---

//Reset session
void init_game(GameState *game);

//Frame update
//dt:delta time
void update_game(GameState *game, float dt);

//User input
void player_move(GameState *game, Direction dir);
void player_shoot(GameState *game);

//Boundary check
bool check_enemy_edge(GameState *game, int dir);

#endif //MODEL_H