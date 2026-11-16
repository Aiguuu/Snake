/*
 * serpent.h — le serpent : un tableau dynamique de positions.
 *
 * EN-TÊTE FOURNI : ne pas le modifier. Vous écrivez serpent.c.
 */
#ifndef SERPENT_H
#define SERPENT_H

#include "types.h"

#define SERPENT_CAPACITE_INITIALE 4

typedef struct {
    Position *corps;      // corps[0] est la TÊTE, corps[longueur-1] la QUEUE
    int longueur;         // nombre de cases occupées par le serpent
    int capacite;         // nombre de Position allouées dans corps (>= longueur)
    Direction direction;  // direction demandée pour le prochain pas
    Direction derniere;   // direction du dernier pas réellement effectué
} Serpent;

// Étape 3 — crée un serpent de `longueur` cases dont la tête est en `tete`
// et qui avance vers `direction` : le corps s'étend donc dans la direction
// OPPOSÉE. Exemple : tête (5, 2), longueur 3, DROITE -> (5,2) (4,2) (3,2).
// direction et derniere valent `direction`. La capacité de départ est
// SERPENT_CAPACITE_INITIALE. Renvoie NULL si longueur < 1 ou si
// l'allocation échoue.
Serpent *serpent_creer(Position tete, int longueur, Direction direction);

// Étape 3 — libère le serpent. s peut valoir NULL.
void serpent_liberer(Serpent *s);

// Étape 3 — ajoute p au bout de la queue. Si le tableau est plein, sa
// capacité DOUBLE (realloc). Renvoie 1 si l'ajout a réussi, 0 sinon (le
// serpent est alors inchangé).
int serpent_ajouter_queue(Serpent *s, Position p);

// Étape 3 — la position de la tête.
Position serpent_tete(const Serpent *s);

// Étape 3 — 1 si l'une des cases du serpent (tête comprise) est p.
int serpent_contient(const Serpent *s, Position p);

// Étape 3 — demande à tourner vers d. Refusé (renvoie 0, rien ne change) si
// d est l'opposé de la direction du DERNIER PAS (un serpent ne fait pas
// demi-tour sur lui-même). Sinon direction prend la valeur d et la
// fonction renvoie 1.
int serpent_changer_direction(Serpent *s, Direction d);

// Étape 3 — la case où sera la tête après le prochain pas.
Position serpent_prochaine_tete(const Serpent *s);

// Étape 3 — fait un pas dans la direction demandée : chaque case prend la
// place de la précédente, la tête avance. Si grandir vaut 1, la queue reste
// en place (le serpent gagne une case). Met derniere à jour.
// Renvoie 1, ou 0 si l'agrandissement a échoué (serpent inchangé).
int serpent_avancer(Serpent *s, int grandir);

#endif
