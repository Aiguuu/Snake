/*
 * partie.c — une partie de Serpent (étapes 4 et 5).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "a_faire.h"
#include "alea.h"
#include "partie.h"

Partie *partie_creer(const char *chemin_carte, unsigned graine)
{
    // TODO Étape 4 : à écrire (voir le sujet) ; effacez alors les lignes (void) et A_FAIRE.
    // Indice : Mettre carte et serpent à NULL dès le malloc de la structure :
    //          en cas d'erreur, un seul appel à partie_liberer suffit alors.
    (void)chemin_carte;
    (void)graine;
    A_FAIRE(4);
    return NULL;
}

void partie_liberer(Partie *p)
{
    // TODO Étape 4 : à écrire (voir le sujet) ; effacez alors les lignes (void) et A_FAIRE.
    (void)p;
    A_FAIRE(4);
}

int partie_case_libre(const Partie *p, Position pos)
{
    // TODO Étape 4 : à écrire (voir le sujet) ; effacez alors les lignes (void) et A_FAIRE.
    (void)p;
    (void)pos;
    A_FAIRE(4);
    return 0;
}

int partie_placer_pomme(Partie *p)
{
    // TODO Étape 4 : à écrire (voir le sujet) ; effacez alors les lignes (void) et A_FAIRE.
    // Indice : Deux parcours de la grille : le premier compte les cases
    //          libres, le second s'arrête sur la k-ième.
    (void)p;
    A_FAIRE(4);
    return 0;
}

void partie_etape(Partie *p)
{
    // TODO Étape 4 : à écrire (voir le sujet) ; effacez alors les lignes (void) et A_FAIRE.
    (void)p;
    A_FAIRE(4);
}

int partie_delai_ms(const Partie *p)
{
    // TODO Étape 4 : à écrire (voir le sujet) ; effacez alors les lignes (void) et A_FAIRE.
    (void)p;
    A_FAIRE(4);
    return DELAI_INITIAL_MS;
}

int partie_sauvegarder(const Partie *p, const char *chemin)
{
    // TODO Étape 5 : à écrire (voir le sujet) ; effacez alors les lignes (void) et A_FAIRE.
    (void)p;
    (void)chemin;
    A_FAIRE(5);
    return 0;
}

Partie *partie_reprendre(const char *chemin)
{
    // TODO Étape 5 : à écrire (voir le sujet) ; effacez alors les lignes (void) et A_FAIRE.
    // Indice : Commencez par lire TOUT l'en-tête (de « SERPENT » à
    //          « longueur ») avant de créer quoi que ce soit.
    (void)chemin;
    A_FAIRE(5);
    return NULL;
}
