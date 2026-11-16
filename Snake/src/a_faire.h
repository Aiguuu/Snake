/*
 * a_faire.h — repérer les fonctions qui ne sont pas encore écrites.
 *
 * FICHIER FOURNI : ne pas le modifier.
 *
 * Dans le kit, chaque fonction à écrire contient la ligne
 *     A_FAIRE(3);
 * qui signale « étape 3 pas encore faite ». Les tests affichent alors
 * [A FAIRE] au lieu de [ECHEC], et le jeu affiche l'étape à écrire en bas de
 * la fenêtre. Quand vous écrivez une fonction, SUPPRIMEZ cette ligne.
 */
#ifndef A_FAIRE_H
#define A_FAIRE_H

#define A_FAIRE(etape) a_faire_signaler((etape), __func__)

void a_faire_signaler(int etape, const char *fonction);

// Plus petite étape signalée depuis le dernier a_faire_effacer() (0 : aucune).
int a_faire_etape(void);

// Nom de la fonction correspondante ("" si aucune).
const char *a_faire_fonction(void);

void a_faire_effacer(void);

#endif
