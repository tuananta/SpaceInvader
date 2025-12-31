

#include <SDL3/SDL.h>
#include "../../include/render_entities.h"

/**
 * Dessine le joueur (Vaisseau spatial) en style Pixel
 */
void draw_pixel_player(SDL_Renderer *renderer, float x, float y, float size) {
    float p = size / 8; 
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Couleur verte caractéristique

    SDL_FRect rects[] = {
        {x + 3*p, y,       2*p, p},   // Sommet du canon
        {x + 2*p, y + p,   4*p, p},   // Partie supérieure du corps
        {x + p,   y + 2*p, 6*p, 2*p}, // Corps principal
        {x,       y + 4*p, 8*p, p}    // Base du vaisseau
    };
    SDL_RenderFillRects(renderer, rects, 4);
}

/**
 * Dessine l'ennemi "Crabe" - Grille de pixels 11x8
 */
void draw_pixel_crab(SDL_Renderer *renderer, float x, float y, float size) {
    float p = size / 11; // Grille de 11 colonnes
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Couleur blanche

    SDL_FRect body[] = {
        {x + 2*p, y,       p,   p}, {x + 8*p, y,       p,   p}, // Antennes supérieures
        {x + 3*p, y + p,   p,   p}, {x + 7*p, y + p,   p,   p}, // Jonction des antennes
        {x + 2*p, y + 2*p, 7*p, 3*p}, // Corps principal
        {x,       y + 3*p, 2*p, 2*p}, {x + 9*p, y + 3*p, 2*p, 2*p}, // Pinces latérales
        {x + 2*p, y + 5*p, p,   p}, {x + 4*p, y + 5*p, 3*p, p}, {x + 8*p, y + 5*p, p,   p}, // Pattes inférieures
        {x + p,   y + 6*p, p,   p}, {x + 9*p, y + 6*p, p,   p},
        {x,       y + 7*p, p,   p}, {x + 10*p,y + 7*p, p,   p}  // Pointes des pattes
    };
    SDL_RenderFillRects(renderer, body, 13);

    // Dessine les yeux (Noir)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_FRect eyes[] = {{x + 3*p, y + 3*p, p, p}, {x + 7*p, y + 3*p, p, p}};
    SDL_RenderFillRects(renderer, eyes, 2);
}

/**
 * Dessine l'ennemi "Calmar" (Squid) - Grille de pixels 8x8
 */
void draw_pixel_squid(SDL_Renderer *renderer, float x, float y, float size) {
    float p = size / 8; 
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255); // Couleur rose/magenta

    SDL_FRect body[] = {
        {x + 3*p, y,       2*p, p},   // Sommet de la tête
        {x + 2*p, y + p,   4*p, p},   // Front
        {x + p,   y + 2*p, 6*p, 3*p}, // Corps de la tête
        {x + 2*p, y + 5*p, p,   p}, {x + 5*p, y + 5*p, p, p}, // Cou
        {x + p,   y + 6*p, p,   p}, {x + 3*p, y + 6*p, 2*p, p}, {x + 6*p, y + 6*p, p, p}, // Tentacules
        {x,       y + 7*p, p,   p}, {x + 2*p, y + 7*p, p, p}, {x + 5*p, y + 7*p, p, p}, {x + 7*p, y + 7*p, p, p}
    };
    SDL_RenderFillRects(renderer, body, 12);

    // Dessine các yeux (Noir)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_FRect eyes[] = {{x + 2*p, y + 3*p, p, p}, {x + 5*p, y + 3*p, p, p}};
    SDL_RenderFillRects(renderer, eyes, 2);
}

/**
 * Dessine l'ennemi "Poulpe" (Octopus) - Grille de pixels 12x8
 */
void draw_pixel_octopus(SDL_Renderer *renderer, float x, float y, float size) {
    float p = size / 12;
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255); // Couleur bleu cyan

    SDL_FRect body[] = {
        {x + 4*p, y,       4*p, p},   // Sommet plat
        {x + 2*p, y + p,   8*p, p},   // Extension de la tête
        {x + p,   y + 2*p, 10*p, 3*p}, // Corps large
        {x + 2*p, y + 5*p, 2*p, p}, {x + 5*p, y + 5*p, 2*p, p}, {x + 8*p, y + 5*p, 2*p, p}, // Base des tentacules
        {x + p,   y + 6*p, 2*p, p}, {x + 5*p, y + 6*p, 2*p, p}, {x + 9*p, y + 6*p, 2*p, p}, // Milieu des tentacules
        {x,       y + 7*p, p,   p}, {x + 3*p, y + 7*p, p,   p}, {x + 8*p, y + 7*p, p,   p}, {x + 11*p,y + 7*p, p, p} // Pointes des tentacules
    };
    SDL_RenderFillRects(renderer, body, 13);

    // Dessine les yeux (Noir)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_FRect eyes[] = {{x + 3*p, y + 3*p, 2*p, p}, {x + 7*p, y + 3*p, 2*p, p}};
    SDL_RenderFillRects(renderer, eyes, 2);
}