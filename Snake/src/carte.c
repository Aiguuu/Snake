/*
 * carte.c — le terrain de jeu (étapes 1 et 2).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "a_faire.h"
#include "carte.h"

Carte *carte_creer(int largeur, int hauteur)
{
    // TODO Étape 1 : à écrire (voir le sujet) ; effacez alors les lignes (void) et A_FAIRE.
    // Indice : Deux malloc : la structure, puis le tableau de cases.
    //          Si le second échoue, il faut libérer le premier.
    largeur;
    hauteur;
    return NULL;
}

void carte_liberer(Carte *carte)
{
    // TODO Étape 1 : à écrire (voir le sujet) ; effacez alors les lignes (void) et A_FAIRE.
    (void)carte;
    A_FAIRE(1);
}

int carte_contient(const Carte *carte, Position p)
{
    // TODO Étape 1 : à écrire (voir le sujet) ; effacez alors les lignes (void) et A_FAIRE.
    (void)carte;
    (void)p;
    A_FAIRE(1);
    return 0;
}

Case carte_case(const Carte *carte, Position p)
{
    // TODO Étape 1 : à écrire (voir le sujet) ; effacez alors les lignes (void) et A_FAIRE.
    (void)carte;
    (void)p;
    A_FAIRE(1);
    return CASE_MUR;
}

void carte_modifier(Carte *carte, Position p, Case valeur)
{
    // TODO Étape 1 : à écrire (voir le sujet) ; effacez alors les lignes (void) et A_FAIRE.
    (void)carte;
    (void)p;
    (void)valeur;
    A_FAIRE(1);
}

Carte *carte_charger(const char *chemin)
{
    // TODO Étape 2 : à écrire (voir le sujet) ; effacez alors les lignes (void) et A_FAIRE.
    // Indice : Lire l'en-tête avec fgets puis sscanf ; créer la carte avec
    //          carte_creer ; puis une ligne de fichier par ligne de grille.
    //          Chaque « return NULL » doit d'abord fermer le fichier et
    //          libérer la carte déjà créée.
    (void)chemin;
    A_FAIRE(2);
    return NULL;
}
