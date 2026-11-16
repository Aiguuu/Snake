/*
 * scores.h — le tableau des meilleurs scores, conservé dans un fichier.
 *
 * EN-TÊTE FOURNI : ne pas le modifier. Vous écrivez scores.c.
 *
 * Fichier : une ligne par score, sous la forme  nom;score
 *     Alice;230
 *     bob_2;120
 */
#ifndef SCORES_H
#define SCORES_H

#define SCORES_MAX 10
#define NOM_MAX 16        // 15 caractères au plus, plus le '\0'

typedef struct {
    char nom[NOM_MAX];
    int score;
} Score;

// Étape 6 — 1 si nom a de 1 à NOM_MAX-1 caractères, tous lettres (a-z,
// A-Z), chiffres, '-' ou '_' ; 0 sinon.
int score_nom_valide(const char *nom);

// Étape 6 — insère (nom, score) dans tableau, trié par score DÉCROISSANT,
// qui contient *nombre éléments et au plus max. À score égal, le nouveau
// venu se place APRÈS les anciens. Si le tableau est plein, le dernier est
// éliminé. Renvoie le rang obtenu (0 = premier), ou -1 si le score n'entre
// pas dans le tableau ou si nom n'est pas valide ou si score < 0.
int scores_inserer(Score tableau[], int *nombre, int max, const char *nom, int score);

// Étape 6 — lit le fichier et remplit tableau (au plus max scores), trié
// par score décroissant même si le fichier ne l'est pas. Les lignes mal
// formées sont ignorées. Renvoie le nombre de scores lus ; un fichier
// absent n'est pas une erreur (0 score).
int scores_charger(const char *chemin, Score tableau[], int max);

// Étape 6 — écrit les nombre scores dans le fichier. Renvoie 1, ou 0 en
// cas d'erreur.
int scores_sauvegarder(const char *chemin, const Score tableau[], int nombre);

#endif
