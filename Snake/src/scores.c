/*
 * scores.c — le tableau des meilleurs scores (étape 6).
 */
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "a_faire.h"
#include "scores.h"

int score_nom_valide(const char *nom)
{
    // TODO Étape 6 : à écrire (voir le sujet) ; effacez alors les lignes (void) et A_FAIRE.
    // Indice : isalnum(c) (ctype.h) dit si c est une lettre ou un chiffre :
    //          lui passer (unsigned char)c.
    (void)nom;
    A_FAIRE(6);
    return 0;
}

int scores_inserer(Score tableau[], int *nombre, int max, const char *nom, int score)
{
    // TODO Étape 6 : à écrire (voir le sujet) ; effacez alors les lignes (void) et A_FAIRE.
    // Indice : 1) trouver le rang ; 2) décaler vers la droite les éléments
    //          qui suivent, en partant de la fin ; 3) écrire au rang trouvé.
    (void)tableau;
    (void)nombre;
    (void)max;
    (void)nom;
    (void)score;
    A_FAIRE(6);
    return -1;
}

int scores_charger(const char *chemin, Score tableau[], int max)
{
    // TODO Étape 6 : à écrire (voir le sujet) ; effacez alors les lignes (void) et A_FAIRE.
    // Indice : Lire ligne par ligne (fgets), couper au ';' (strchr), puis
    //          confier chaque couple à scores_inserer : il trie et filtre.
    (void)chemin;
    (void)tableau;
    (void)max;
    A_FAIRE(6);
    return 0;
}

int scores_sauvegarder(const char *chemin, const Score tableau[], int nombre)
{
    // TODO Étape 6 : à écrire (voir le sujet) ; effacez alors les lignes (void) et A_FAIRE.
    (void)chemin;
    (void)tableau;
    (void)nombre;
    A_FAIRE(6);
    return 0;
}
