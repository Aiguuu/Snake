/*
 * carte.h — le terrain de jeu : une grille de cases vides et de murs.
 *
 * EN-TÊTE FOURNI : ne pas le modifier. Vous écrivez carte.c.
 */
#ifndef CARTE_H
#define CARTE_H

#include "types.h"

#define CARTE_LARGEUR_MIN 5
#define CARTE_LARGEUR_MAX 60
#define CARTE_HAUTEUR_MIN 5
#define CARTE_HAUTEUR_MAX 40

typedef enum {
    CASE_VIDE,
    CASE_MUR
} Case;

typedef struct {
    int largeur;
    int hauteur;
    Case *cases;       // largeur * hauteur cases, rangées ligne par ligne :
                       // la case (x, y) est cases[y * largeur + x]
    Position depart;   // case de la tête du serpent en début de partie
} Carte;

// Étape 1 — alloue une carte largeur x hauteur dont toutes les cases sont
// vides ; depart vaut (0, 0). Renvoie NULL si une dimension est <= 0 ou si
// l'allocation échoue (sans fuite de mémoire).
Carte *carte_creer(int largeur, int hauteur);

// Étape 1 — libère tout ce qu'a alloué carte_creer. carte peut valoir NULL.
void carte_liberer(Carte *carte);

// Étape 1 — 1 si p est dans la grille, 0 sinon.
int carte_contient(const Carte *carte, Position p);

// Étape 1 — le contenu de la case p. Une case HORS de la grille est un mur.
Case carte_case(const Carte *carte, Position p);

// Étape 1 — change le contenu de la case p ; ne fait rien si p est hors de
// la grille.
void carte_modifier(Carte *carte, Position p, Case valeur);

// Étape 2 — lit une carte dans un fichier texte (format : voir le sujet).
// Renvoie NULL si le fichier est absent ou mal formé, sans fuite de mémoire.
Carte *carte_charger(const char *chemin);

#endif
