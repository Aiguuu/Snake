/*
 * alea.c — voir alea.h.
 *
 * FICHIER FOURNI : ne pas le modifier.
 */
#include "alea.h"

unsigned alea_suivant(unsigned *etat)
{
    *etat = *etat * 1103515245u + 12345u;
    return (*etat >> 16) & 0x7FFFu;
}
