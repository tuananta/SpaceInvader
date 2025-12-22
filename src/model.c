#include "model.h"
#include <stdio.h>  // Pour NULL
#include <string.h> // Pour memset

// --- FONCTIONS D'AIDE (INTERNES) ---

// Retourne vrai si au moins un ennemi touche le bord de l'écran
// dir : direction actuelle (1 = droite, -1 = gauche)
bool check_enemy_edge(GameState *game, int dir) {
    for (int i = 0; i < TOTAL_ENEMIES; i++) {
        if (game->enemies[i].alive) {
            float x = game->enemies[i].base.x;
            int w = game->enemies[i].base.width;

            // Si on va à DROITE et qu'on touche le bord DROIT
            if (dir == 1 && (x + w) >= GAME_WIDTH) return true;
            
            // Si on va à GAUCHE et qu'on touche le bord GAUCHE
            if (dir == -1 && x <= 0) return true;
        }
    }
    return false;
}

// --- IMPLÉMENTATION DES FONCTIONS DU MODÈLE ---

void init_game(GameState *game) {
    // 1. Initialiser l'état global
    game->status = GAME_PLAYING;
    game->level = 1;
    game->last_update_time = 0; // Sera mis à jour par le main

    // 2. Initialiser le Joueur (Au centre en bas)
    game->player.base.width = 50;  // Taille arbitraire pour la logique
    game->player.base.height = 30;
    game->player.base.x = (GAME_WIDTH - game->player.base.width) / 2;
    game->player.base.y = GAME_HEIGHT - 60; // Un peu au-dessus du fond
    game->player.base.speed = PLAYER_SPEED;
    game->player.lives = 3;
    game->player.score = 0;
    game->player.cooldown = 0;

    // 3. Initialiser les Ennemis (Le "Régiment")
    game->enemy_direction = 1; // Commence vers la droite
    game->enemy_move_timer = 0;

    int start_x = 50;
    int start_y = 50;
    int spacing_x = 60; // Espace entre les ennemis
    int spacing_y = 50;

    for (int row = 0; row < NUM_ENEMIES_ROWS; row++) {
        EntityType type;
        // Définir le type selon la ligne (Haut=SQUID, Milieu=CRAB, Bas=OCTOPUS)
        if (row == 0) type = ENTITY_ENEMY_SQUID;
        else if (row < 3) type = ENTITY_ENEMY_CRAB;
        else type = ENTITY_ENEMY_OCTOPUS;

        for (int col = 0; col < NUM_ENEMIES_COLS; col++) {
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

    // 4. Initialiser les projectiles (tous inactifs au début)
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        game->projectiles[i].active = false;
    }
}

void update_game(GameState *game, float dt) {
    (void)dt;
    if (game->status != GAME_PLAYING) return;

    // --- A. Gestion du Joueur ---
    if (game->player.cooldown > 0) {
        game->player.cooldown--;
    }

    // --- B. Gestion des Ennemis (Mouvement de Groupe) ---
    // On bouge les ennemis tout le temps (fluide)
    // Mais on vérifie les bords pour descendre
    
    bool touch_edge = check_enemy_edge(game, game->enemy_direction);

    if (touch_edge) {
        // 1. Inverser la direction
        game->enemy_direction *= -1;
        
        // 2. Faire descendre TOUT LE MONDE
        for (int i = 0; i < TOTAL_ENEMIES; i++) {
            game->enemies[i].base.y += ENEMY_DROP_Y;
            
            // Vérifier Game Over (Invasion)
            if (game->enemies[i].alive && game->enemies[i].base.y + game->enemies[i].base.height >= game->player.base.y) {
                game->status = GAME_OVER;
            }
        }
    } else {
        // 3. Sinon, avancer latéralement
        float move_amount = ENEMY_SPEED_X; // Vitesse de base
        // (Optionnel : on pourrait augmenter la vitesse ici selon le nombre de morts)
        
        for (int i = 0; i < TOTAL_ENEMIES; i++) {
            if (game->enemies[i].alive) {
                game->enemies[i].base.x += (move_amount * game->enemy_direction);
            }
        }
    }

    // --- C. Gestion des Projectiles ---
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (game->projectiles[i].active) {
            // Déplacer le tir
            game->projectiles[i].base.y += (game->projectiles[i].dy * PROJECTILE_SPEED);

            // Désactiver si sort de l'écran
            if (game->projectiles[i].base.y < 0 || game->projectiles[i].base.y > GAME_HEIGHT) {
                game->projectiles[i].active = false;
            }
            
            // TODO: Ajouter la collision ici plus tard
        }
    }
}

void player_move(GameState *game, Direction dir) {
    if (game->status != GAME_PLAYING) return;

    float new_x = game->player.base.x + (dir * game->player.base.speed);

    // Empêcher de sortir de l'écran (Clamping)
    if (new_x < 0) new_x = 0;
    if (new_x + game->player.base.width > GAME_WIDTH) {
        new_x = GAME_WIDTH - game->player.base.width;
    }

    game->player.base.x = new_x;
}

void player_shoot(GameState *game) {
    if (game->player.cooldown > 0) return; // Recharger !

    // Trouver un projectile inactif dans le tableau (Object Pooling)
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (!game->projectiles[i].active) {
            game->projectiles[i].active = true;
            game->projectiles[i].from_player = true;
            game->projectiles[i].base.width = 5;
            game->projectiles[i].base.height = 10;
            // Le tir part du centre du vaisseau
            game->projectiles[i].base.x = game->player.base.x + (game->player.base.width / 2) - 2;
            game->projectiles[i].base.y = game->player.base.y - 10;
            game->projectiles[i].dy = -1.0f; // Vers le haut

            game->player.cooldown = 20; // Délai avant prochain tir
            break; // On a tiré une balle, on arrête la boucle
        }
    }
}