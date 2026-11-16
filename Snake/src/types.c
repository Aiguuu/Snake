/*
 * types.c — positions et directions (étape 1).
 */
#include <string.h>

#include "a_faire.h"
#include "types.h"

Position position_voisine(Position p, Direction d)
{
    // TODO Étape 1 : à écrire (voir le sujet) ; effacez alors les lignes (void) et A_FAIRE.
    // Indice : Une copie de p, dont on modifie x ou y selon d (switch).
    (void)p;
    (void)d;
    A_FAIRE(1);
    return p;
}

int positions_egales(Position a, Position b)
{
    // TODO Étape 1 : à écrire (voir le sujet) ; effacez alors les lignes (void) et A_FAIRE.
    (void)a;
    (void)b;
    A_FAIRE(1);
    return 0;
}

Direction direction_opposee(Direction d)
{
    // TODO Étape 1 : à écrire (voir le sujet) ; effacez alors les lignes (void) et A_FAIRE.
    (void)d;
    A_FAIRE(1);
    return d;
}

// Fournie : un exemple de fonction qui renvoie une chaîne CONSTANTE.
// La chaîne vit pendant tout le programme : on peut la renvoyer sans malloc.
const char *direction_nom(Direction d)
{
    switch (d) {
    case HAUT:   return "HAUT";
    case BAS:    return "BAS";
    case GAUCHE: return "GAUCHE";
    case DROITE: return "DROITE";
    }
    return "?";
}

int direction_depuis_nom(const char *nom, Direction *d)
{
    // TODO Étape 1 : à écrire (voir le sujet) ; effacez alors les lignes (void) et A_FAIRE.
    // Indice : On ne compare pas deux chaînes avec == : voir strcmp.
    (void)nom;
    (void)d;
    A_FAIRE(1);
    return 0;
}
