/*
 * types.h — positions et directions sur la grille.
 *
 * EN-TÊTE FOURNI : ne pas le modifier (les tests du correcteur s'appuient
 * dessus). Vous écrivez types.c.
 */
#ifndef TYPES_H
#define TYPES_H

// Une case de la grille. L'origine (0, 0) est en HAUT à gauche :
// x augmente vers la droite, y augmente vers le BAS (comme à l'écran).
typedef struct {
    int x;
    int y;
} Position;

typedef enum {
    HAUT,
    BAS,
    GAUCHE,
    DROITE
} Direction;

// Étape 1 — la case voisine de p dans la direction d.
Position position_voisine(Position p, Direction d);

// Étape 1 — 1 si a et b désignent la même case, 0 sinon.
int positions_egales(Position a, Position b);

// Étape 1 — HAUT <-> BAS, GAUCHE <-> DROITE.
Direction direction_opposee(Direction d);

// Fournie — "HAUT", "BAS", "GAUCHE" ou "DROITE".
const char *direction_nom(Direction d);

// Étape 1 — l'inverse de direction_nom : si nom vaut "HAUT", "BAS", "GAUCHE"
// ou "DROITE", range la direction dans *d et renvoie 1 ; sinon renvoie 0
// sans toucher à *d.
int direction_depuis_nom(const char *nom, Direction *d);

#endif
