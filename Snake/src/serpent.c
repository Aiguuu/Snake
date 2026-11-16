/*
 * serpent.c — le serpent (étape 3).
 */
#include <stdlib.h>

#include "a_faire.h"
#include "serpent.h"

Serpent *serpent_creer(Position tete, int longueur, Direction direction)
{
    // TODO Étape 3 : à écrire (voir le sujet) ; effacez alors les lignes (void) et A_FAIRE.
    // Indice : Allouer la structure et un tableau de SERPENT_CAPACITE_INITIALE
    //          positions (longueur = 0), puis appeler serpent_ajouter_queue.
    (void)tete;
    (void)longueur;
    (void)direction;
    A_FAIRE(3);
    return NULL;
}

void serpent_liberer(Serpent *s)
{
    // TODO Étape 3 : à écrire (voir le sujet) ; effacez alors les lignes (void) et A_FAIRE.
    (void)s;
    A_FAIRE(3);
}

int serpent_ajouter_queue(Serpent *s, Position p)
{
    // TODO Étape 3 : à écrire (voir le sujet) ; effacez alors les lignes (void) et A_FAIRE.
    // Indice : realloc dans une variable TEMPORAIRE : si elle vaut NULL,
    //          l'ancien tableau est toujours valide (et toujours à vous).
    (void)s;
    (void)p;
    A_FAIRE(3);
    return 0;
}

Position serpent_tete(const Serpent *s)
{
    // TODO Étape 3 : à écrire (voir le sujet) ; effacez alors les lignes (void) et A_FAIRE.
    (void)s;
    A_FAIRE(3);
    Position p = {0, 0}; return p;
}

int serpent_contient(const Serpent *s, Position p)
{
    // TODO Étape 3 : à écrire (voir le sujet) ; effacez alors les lignes (void) et A_FAIRE.
    (void)s;
    (void)p;
    A_FAIRE(3);
    return 0;
}

int serpent_changer_direction(Serpent *s, Direction d)
{
    // TODO Étape 3 : à écrire (voir le sujet) ; effacez alors les lignes (void) et A_FAIRE.
    (void)s;
    (void)d;
    A_FAIRE(3);
    return 0;
}

Position serpent_prochaine_tete(const Serpent *s)
{
    // TODO Étape 3 : à écrire (voir le sujet) ; effacez alors les lignes (void) et A_FAIRE.
    (void)s;
    A_FAIRE(3);
    Position p = {0, 0}; return p;
}

int serpent_avancer(Serpent *s, int grandir)
{
    // TODO Étape 3 : à écrire (voir le sujet) ; effacez alors les lignes (void) et A_FAIRE.
    // Indice : Décaler en partant de la QUEUE : corps[i] = corps[i - 1].
    (void)s;
    (void)grandir;
    A_FAIRE(3);
    return 0;
}
