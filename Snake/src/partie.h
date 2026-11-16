/*
 * partie.h — une partie : la carte, le serpent, la pomme et le score.
 *
 * EN-TÊTE FOURNI : ne pas le modifier. Vous écrivez partie.c.
 */
#ifndef PARTIE_H
#define PARTIE_H

#include "carte.h"
#include "serpent.h"

#define LONGUEUR_INITIALE 3
#define POINTS_PAR_POMME 10
#define DELAI_INITIAL_MS 150
#define DELAI_MINIMUM_MS 60
#define CHEMIN_MAX 256

typedef enum {
    PARTIE_EN_COURS,
    PARTIE_PERDUE,
    PARTIE_GAGNEE
} EtatPartie;

typedef struct {
    Carte *carte;
    Serpent *serpent;
    Position pomme;
    int score;
    int pommes;                      // nombre de pommes mangées
    EtatPartie etat;
    unsigned graine;                 // état du générateur (voir alea.h)
    char chemin_carte[CHEMIN_MAX];   // fichier d'où vient la carte
} Partie;

// Étape 4 — charge la carte, crée un serpent de LONGUEUR_INITIALE cases sur
// la case de départ, tourné vers la DROITE, place la première pomme.
// Renvoie NULL (sans fuite) si la carte ne se charge pas, si le chemin est
// trop long pour chemin_carte, ou si le corps du serpent tombe sur un mur
// ou hors de la carte.
Partie *partie_creer(const char *chemin_carte, unsigned graine);

// Étape 4 — libère toute la partie. p peut valoir NULL.
void partie_liberer(Partie *p);

// Étape 4 — 1 si pos est dans la carte, n'est pas un mur et n'est pas
// occupée par le serpent.
int partie_case_libre(const Partie *p, Position pos);

// Étape 4 — place la pomme sur une case libre tirée au hasard (règle exacte
// dans le sujet). Renvoie 1, ou 0 s'il ne reste aucune case libre.
int partie_placer_pomme(Partie *p);

// Étape 4 — fait avancer le jeu d'un pas (règles dans le sujet).
void partie_etape(Partie *p);

// Étape 4 — délai entre deux pas, en millisecondes : DELAI_INITIAL_MS moins
// 5 ms par pomme mangée, sans descendre sous DELAI_MINIMUM_MS.
int partie_delai_ms(const Partie *p);

// Étape 5 — écrit la partie dans un fichier texte. Renvoie 1, ou 0 en cas
// d'erreur.
int partie_sauvegarder(const Partie *p, const char *chemin);

// Étape 5 — recrée une partie à partir d'un fichier écrit par
// partie_sauvegarder. Renvoie NULL (sans fuite) si le fichier est absent ou
// incohérent.
Partie *partie_reprendre(const char *chemin);

#endif
