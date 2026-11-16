/*
 * alea.h — générateur pseudo-aléatoire du jeu.
 *
 * FICHIER FOURNI : ne pas le modifier.
 *
 * Pourquoi pas rand() ? Parce que rand() ne donne pas la même suite sur
 * Windows, Linux et macOS : les tests ne pourraient pas prévoir où tombe la
 * pomme. Ce générateur (congruentiel linéaire) donne la même suite partout.
 *
 *     unsigned graine = 42;
 *     unsigned a = alea_suivant(&graine);   // graine est mise à jour
 *     unsigned b = alea_suivant(&graine);   // donc b est différent de a
 */
#ifndef ALEA_H
#define ALEA_H

// Renvoie un entier entre 0 et 32767, et fait avancer *etat.
unsigned alea_suivant(unsigned *etat);

#endif
