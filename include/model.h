#ifndef MODEL_H // eviter le repete de creation dans main etc..
#define MODEL_H

#include<stdbool.h>
#include<stdint.h>

#define GAME_WIDTH  800
#define GAME_HEIGHT 600

// Configuration du Gameplay
#define PLAYER_SPEED 5.0f
#define ENEMY_SPEED_X 2.0f
#define ENEMY_DROP_Y 20.0f    // De combien ils descendent quand ils touchent un bord
#define PROJECTILE_SPEED 7.0f

// Limites
#define MAX_PROJECTILES 100   // Nombre max de tirs simultanés (joueur + ennemis)
#define NUM_ENEMIES_COLS 11
#define NUM_ENEMIES_ROWS 5
#define TOTAL_ENEMIES (NUM_ENEMIES_COLS * NUM_ENEMIES_ROWS)

// --- ÉNUMÉRATIONS (États) ---

// Les différents états possibles du jeu
typedef enum {
    GAME_MENU,      // Menu principal
    GAME_PLAYING,   // En train de jouer
    GAME_PAUSED,    // Pause
    GAME_OVER,      // Perdu (Plus de vies ou invasion réussie)
    GAME_VICTORY    // Gagné (Tous les ennemis détruits)
} GameStatus;

// Type d'entité (utile pour l'affichage graphique plus tard)
typedef enum {
    ENTITY_PLAYER,
    ENTITY_ENEMY_SQUID,   // Ennemi type 1 (Haut)
    ENTITY_ENEMY_CRAB,    // Ennemi type 2 (Milieu)
    ENTITY_ENEMY_OCTOPUS, // Ennemi type 3 (Bas)
    ENTITY_PROJECTILE
} EntityType;

// Direction de déplacement
typedef enum {
    DIR_LEFT = -1,
    DIR_RIGHT = 1,
    DIR_NONE = 0
} Direction;

// --- STRUCTURES DE DONNÉES ---

// Structure de base pour tout objet ayant une position et une taille
typedef struct {
    float x, y;         // Position précise (float pour la fluidité)
    int width, height;  // Taille (Hitbox pour les collisions)
    float speed;        // Vitesse de déplacement
} Entity;

// Le Vaisseau du Joueur
typedef struct {
    Entity base;        // Hérite de x, y, w, h
    int lives;          // Vies restantes (ex: 3)
    int score;          // Score actuel
    int cooldown;       // Temps d'attente avant de pouvoir tirer à nouveau
} Player;

// Un Ennemi (Space Invader)
typedef struct {
    Entity base;        // Hérite de x, y, w, h
    EntityType type;    // Quel sprite afficher ?
    bool alive;         // true = vivant, false = détruit
} Enemy;

// Un Projectile (Tir)
typedef struct {
    Entity base;
    bool active;        // true = à l'écran, false = disponible/invisible
    bool from_player;   // true = tir du joueur, false = tir ennemi
    float dy;           // Direction verticale (-1 monte, +1 descend)
} Projectile;

// --- ÉTAT GLOBAL DU JEU (La structure principale) ---
// C'est cet objet qui sera passé à la Vue pour l'affichage
typedef struct {
    GameStatus status;          // État courant (Menu, Jeu...)
    
    Player player;              // Le joueur
    
    // Tableau statique d'ennemis (plus simple que l'allocation dynamique pour débuter)
    Enemy enemies[TOTAL_ENEMIES]; 
    int enemy_direction;        // 1 (Droite) ou -1 (Gauche) pour tout le groupe
    float enemy_move_timer;     // Pour gérer le rythme de déplacement des ennemis
    
    // "Pool" de projectiles (on réutilise les inactifs au lieu de malloc/free sans cesse)
    Projectile projectiles[MAX_PROJECTILES];
    
    int level;                  // Niveau actuel
    uint32_t last_update_time;  // Pour calculer le Delta Time (temps écoulé)
} GameState;

// --- PROTOTYPES DES FONCTIONS DU MODÈLE ---
// Ce sont les seules fonctions que le Contrôleur aura le droit d'appeler

// Initialise tout le jeu (positions, vies, etc.)
void init_game(GameState *game);

// Met à jour la logique (physique, déplacements, collisions)
// dt = Delta Time (temps écoulé depuis la dernière frame en secondes)
void update_game(GameState *game, float dt);

// Actions du joueur (commandes abstraites)
void player_move(GameState *game, Direction dir);
void player_shoot(GameState *game);

// Actions système
void reset_game(GameState *game);

#endif // MODEL_H