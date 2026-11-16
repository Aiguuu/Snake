/*
 * tests.c — tests automatiques du Serpent, étape par étape.
 *
 * FICHIER FOURNI : ne pas le modifier (le correcteur utilisera le sien).
 *
 *     tests          lance tous les tests
 *     tests 3        lance seulement ceux de l'étape 3
 *
 * [ OK ]      le test passe
 * [ECHEC]     le test échoue : la ligne et la condition fausse sont affichées
 * [A FAIRE]   le test appelle une fonction encore marquée A_FAIRE
 *
 * Les tests écrivent quelques fichiers temporaires (test_*.txt) dans le
 * dossier courant et les effacent. Lancez-les depuis le dossier du projet.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "a_faire.h"
#include "alea.h"
#include "partie.h"
#include "scores.h"

// --- Mini-cadre de test ------------------------------------------------------

static int test_ok;
static int test_ligne;
static const char *test_condition;

// Si la condition est fausse, le test s'arrête et est marqué [ECHEC].
#define VERIFIER(cond)                                                       \
    do {                                                                     \
        if (!(cond)) {                                                       \
            test_ok = 0;                                                     \
            test_ligne = __LINE__;                                           \
            test_condition = #cond;                                          \
            return;                                                          \
        }                                                                    \
    } while (0)

typedef struct {
    const char *nom;
    void (*fonction)(void);
} Test;

#define FICHIER_TMP "test_tmp.txt"
#define FICHIER_TMP2 "test_tmp2.txt"

static void ecrire_fichier(const char *chemin, const char *contenu)
{
    FILE *f = fopen(chemin, "wb");     // binaire : on écrit les octets tels quels
    if (f != NULL) {
        fputs(contenu, f);
        fclose(f);
    }
}

// Lit tout le fichier (en mode texte) dans tampon. Renvoie 0 si absent.
static int lire_fichier(const char *chemin, char *tampon, size_t taille)
{
    FILE *f = fopen(chemin, "r");
    if (f == NULL) {
        return 0;
    }
    size_t n = fread(tampon, 1, taille - 1, f);
    tampon[n] = '\0';
    fclose(f);
    return 1;
}

static Position pos(int x, int y)
{
    Position p = {x, y};
    return p;
}

static int egal(Position a, int x, int y)
{
    return a.x == x && a.y == y;
}

// Carte de test commune aux étapes 4 et 5 : 8 x 6, départ en (3, 2).
static const char *CARTE_TEST =
    "8 6\n"
    "########\n"
    "#......#\n"
    "#..@...#\n"
    "#......#\n"
    "#......#\n"
    "########\n";

// =============================================================================
// Étape 1 — positions et carte en mémoire
// =============================================================================

static void t1_voisine(void)
{
    Position p = pos(5, 5);
    VERIFIER(egal(position_voisine(p, HAUT), 5, 4));
    VERIFIER(egal(position_voisine(p, BAS), 5, 6));
    VERIFIER(egal(position_voisine(p, GAUCHE), 4, 5));
    VERIFIER(egal(position_voisine(p, DROITE), 6, 5));
}

static void t1_egales(void)
{
    VERIFIER(positions_egales(pos(3, 7), pos(3, 7)) == 1);
    VERIFIER(positions_egales(pos(3, 7), pos(7, 3)) == 0);
    VERIFIER(positions_egales(pos(3, 7), pos(3, 8)) == 0);
}

static void t1_opposee(void)
{
    VERIFIER(direction_opposee(HAUT) == BAS);
    VERIFIER(direction_opposee(BAS) == HAUT);
    VERIFIER(direction_opposee(GAUCHE) == DROITE);
    VERIFIER(direction_opposee(DROITE) == GAUCHE);
}

static void t1_depuis_nom(void)
{
    Direction d = HAUT;
    VERIFIER(direction_depuis_nom("DROITE", &d) == 1 && d == DROITE);
    VERIFIER(direction_depuis_nom("BAS", &d) == 1 && d == BAS);
    VERIFIER(direction_depuis_nom("GAUCHE", &d) == 1 && d == GAUCHE);
    VERIFIER(direction_depuis_nom("HAUT", &d) == 1 && d == HAUT);
    VERIFIER(direction_depuis_nom("haut", &d) == 0 && d == HAUT);
    VERIFIER(direction_depuis_nom("DROITES", &d) == 0 && d == HAUT);
    VERIFIER(direction_depuis_nom("", &d) == 0 && d == HAUT);
}

static void t1_creer(void)
{
    Carte *c = carte_creer(4, 3);
    VERIFIER(c != NULL);
    int ok = c->largeur == 4 && c->hauteur == 3 && c->cases != NULL
             && c->depart.x == 0 && c->depart.y == 0;
    for (int i = 0; ok && i < 12; i++) {
        ok = c->cases[i] == CASE_VIDE;
    }
    carte_liberer(c);
    VERIFIER(ok);
}

static void t1_creer_refuse(void)
{
    VERIFIER(carte_creer(0, 5) == NULL);
    VERIFIER(carte_creer(5, 0) == NULL);
    VERIFIER(carte_creer(-3, 5) == NULL);
}

static void t1_modifier_lire(void)
{
    Carte *c = carte_creer(4, 3);
    VERIFIER(c != NULL);
    carte_modifier(c, pos(2, 1), CASE_MUR);
    int ok = carte_case(c, pos(2, 1)) == CASE_MUR
             && carte_case(c, pos(1, 2)) == CASE_VIDE
             && carte_case(c, pos(1, 1)) == CASE_VIDE
             && c->cases[1 * 4 + 2] == CASE_MUR;      // rangement ligne par ligne
    carte_liberer(c);
    VERIFIER(ok);
}

static void t1_hors_limites(void)
{
    Carte *c = carte_creer(4, 3);
    VERIFIER(c != NULL);
    int ok = carte_case(c, pos(-1, 0)) == CASE_MUR && carte_case(c, pos(4, 0)) == CASE_MUR
             && carte_case(c, pos(0, -1)) == CASE_MUR && carte_case(c, pos(0, 3)) == CASE_MUR
             && carte_contient(c, pos(0, 0)) && carte_contient(c, pos(3, 2))
             && !carte_contient(c, pos(4, 2)) && !carte_contient(c, pos(3, 3))
             && !carte_contient(c, pos(-1, 1));
    carte_liberer(c);
    VERIFIER(ok);
}

static void t1_modifier_hors_limites(void)
{
    Carte *c = carte_creer(4, 3);
    VERIFIER(c != NULL);
    carte_modifier(c, pos(4, 0), CASE_MUR);
    carte_modifier(c, pos(-1, 2), CASE_MUR);
    carte_modifier(c, pos(0, 3), CASE_MUR);
    int ok = 1;
    for (int i = 0; i < 12; i++) {
        ok = ok && c->cases[i] == CASE_VIDE;
    }
    carte_liberer(c);
    VERIFIER(ok);
}

static void t1_liberer_null(void)
{
    carte_liberer(NULL);      // ne doit pas planter
    VERIFIER(1);
}

static const Test ETAPE1[] = {
    {"position_voisine dans les 4 directions", t1_voisine},
    {"positions_egales", t1_egales},
    {"direction_opposee", t1_opposee},
    {"direction_depuis_nom (noms valides et invalides)", t1_depuis_nom},
    {"carte_creer : dimensions, cases vides, depart (0,0)", t1_creer},
    {"carte_creer refuse les dimensions <= 0", t1_creer_refuse},
    {"carte_modifier puis carte_case, rangement ligne par ligne", t1_modifier_lire},
    {"carte_case hors limites = mur ; carte_contient", t1_hors_limites},
    {"carte_modifier hors limites ne change rien", t1_modifier_hors_limites},
    {"carte_liberer(NULL) ne plante pas", t1_liberer_null},
};

// =============================================================================
// Étape 2 — lecture d'une carte
// =============================================================================

// Charge `contenu` depuis un fichier temporaire ; vérifie que le fichier a
// été refermé (sous Windows, on ne peut pas effacer un fichier ouvert).
static Carte *charger_texte(const char *contenu, int *referme)
{
    ecrire_fichier(FICHIER_TMP, contenu);
    Carte *c = carte_charger(FICHIER_TMP);
    *referme = remove(FICHIER_TMP) == 0;
    return c;
}

static void t2_jardin(void)
{
    Carte *c = carte_charger("cartes/jardin.txt");
    VERIFIER(c != NULL);
    int ok = c->largeur == 24 && c->hauteur == 16 && egal(c->depart, 6, 8)
             && carte_case(c, pos(0, 0)) == CASE_MUR && carte_case(c, pos(23, 15)) == CASE_MUR
             && carte_case(c, pos(0, 7)) == CASE_MUR && carte_case(c, pos(1, 1)) == CASE_VIDE
             && carte_case(c, pos(6, 8)) == CASE_VIDE;
    carte_liberer(c);
    VERIFIER(ok);
}

static void t2_petite(void)
{
    int referme;
    Carte *c = charger_texte("6 5\n"
                             "######\n"
                             "#....#\n"
                             "#.#@.#\n"
                             "#....#\n"
                             "######\n", &referme);
    VERIFIER(c != NULL);
    int ok = c->largeur == 6 && c->hauteur == 5 && egal(c->depart, 3, 2)
             && carte_case(c, pos(2, 2)) == CASE_MUR && carte_case(c, pos(3, 2)) == CASE_VIDE
             && carte_case(c, pos(4, 3)) == CASE_VIDE && carte_case(c, pos(5, 4)) == CASE_MUR;
    carte_liberer(c);
    VERIFIER(ok);
    VERIFIER(referme);
}

static void t2_crlf(void)
{
    int referme;
    Carte *c = charger_texte("5 5\r\n#####\r\n#...#\r\n#..@#\r\n#...#\r\n#####\r\n", &referme);
    VERIFIER(c != NULL);
    int ok = c->largeur == 5 && egal(c->depart, 3, 2);
    carte_liberer(c);
    VERIFIER(ok);
}

static void t2_absent(void)
{
    VERIFIER(carte_charger("cartes/n_existe_pas.txt") == NULL);
}

// Le contenu doit être refusé, et le fichier refermé.
static int refuse(const char *contenu)
{
    int referme;
    Carte *c = charger_texte(contenu, &referme);
    if (c != NULL) {
        carte_liberer(c);
        return 0;
    }
    return referme;
}

static void t2_entete(void)
{
    VERIFIER(refuse("cinq cinq\n#####\n#...#\n#..@#\n#...#\n#####\n"));
    VERIFIER(refuse(""));
    VERIFIER(refuse("4 5\n####\n#..#\n#.@#\n#..#\n####\n"));          // trop étroite
    VERIFIER(refuse("61 5\n"));                                        // trop large
    VERIFIER(refuse("5 41\n"));                                        // trop haute
}

static void t2_longueurs(void)
{
    VERIFIER(refuse("5 5\n#####\n#...#\n#..@\n#...#\n#####\n"));      // ligne courte
    VERIFIER(refuse("5 5\n#####\n#...#\n#..@##\n#...#\n#####\n"));    // ligne longue
    VERIFIER(refuse("5 5\n#####\n#...#\n#..@#\n#...#\n"));            // ligne manquante
}

static void t2_caracteres(void)
{
    VERIFIER(refuse("5 5\n#####\n#.x.#\n#..@#\n#...#\n#####\n"));     // caractère inconnu
    VERIFIER(refuse("5 5\n#####\n#...#\n#...#\n#...#\n#####\n"));     // pas de départ
    VERIFIER(refuse("5 5\n#####\n#@..#\n#..@#\n#...#\n#####\n"));     // deux départs
}

static void t2_toutes(void)
{
    const char *cartes[] = {"cartes/jardin.txt", "cartes/arene.txt", "cartes/couloirs.txt",
                            "cartes/labyrinthe.txt", "cartes/cagibi.txt"};
    for (int i = 0; i < 5; i++) {
        Carte *c = carte_charger(cartes[i]);
        VERIFIER(c != NULL);
        carte_liberer(c);
    }
}

static const Test ETAPE2[] = {
    {"charge cartes/jardin.txt (24 x 16, depart (6,8))", t2_jardin},
    {"charge une petite carte case par case, fichier referme", t2_petite},
    {"accepte les fins de ligne Windows (\\r\\n)", t2_crlf},
    {"fichier absent -> NULL", t2_absent},
    {"en-tete invalide ou dimensions hors bornes -> NULL", t2_entete},
    {"ligne trop courte, trop longue ou manquante -> NULL", t2_longueurs},
    {"caractere inconnu, 0 ou 2 departs -> NULL", t2_caracteres},
    {"les 5 cartes du jeu se chargent", t2_toutes},
};

// =============================================================================
// Étape 3 — le serpent
// =============================================================================

static void t3_creer(void)
{
    Serpent *s = serpent_creer(pos(5, 2), 3, DROITE);
    VERIFIER(s != NULL);
    int ok = s->longueur == 3 && s->capacite == SERPENT_CAPACITE_INITIALE
             && egal(s->corps[0], 5, 2) && egal(s->corps[1], 4, 2) && egal(s->corps[2], 3, 2)
             && s->direction == DROITE && s->derniere == DROITE;
    serpent_liberer(s);
    VERIFIER(ok);
}

static void t3_creer_haut(void)
{
    Serpent *s = serpent_creer(pos(5, 2), 3, HAUT);
    VERIFIER(s != NULL);
    int ok = egal(s->corps[0], 5, 2) && egal(s->corps[1], 5, 3) && egal(s->corps[2], 5, 4)
             && s->direction == HAUT;
    serpent_liberer(s);
    VERIFIER(ok);
}

static void t3_creer_refuse(void)
{
    VERIFIER(serpent_creer(pos(5, 2), 0, DROITE) == NULL);
    VERIFIER(serpent_creer(pos(5, 2), -2, DROITE) == NULL);
    serpent_liberer(NULL);
}

static void t3_long(void)
{
    Serpent *s = serpent_creer(pos(20, 3), 10, DROITE);
    VERIFIER(s != NULL);
    int ok = s->longueur == 10 && s->capacite == 16 && egal(s->corps[9], 11, 3);
    serpent_liberer(s);
    VERIFIER(ok);
}

static void t3_ajouter(void)
{
    Serpent *s = serpent_creer(pos(0, 0), 1, BAS);
    VERIFIER(s != NULL);
    int ok = 1;
    for (int i = 1; i <= 8 && ok; i++) {
        ok = serpent_ajouter_queue(s, pos(i, 0)) == 1;
    }
    // 1 + 8 = 9 cases : la capacité est passée de 4 à 8, puis à 16.
    ok = ok && s->longueur == 9 && s->capacite == 16;
    for (int i = 0; i < 9 && ok; i++) {
        ok = egal(s->corps[i], i, 0);
    }
    serpent_liberer(s);
    VERIFIER(ok);
}

static void t3_tete_contient(void)
{
    Serpent *s = serpent_creer(pos(5, 2), 3, DROITE);
    VERIFIER(s != NULL);
    int ok = egal(serpent_tete(s), 5, 2)
             && serpent_contient(s, pos(5, 2)) && serpent_contient(s, pos(3, 2))
             && !serpent_contient(s, pos(6, 2)) && !serpent_contient(s, pos(4, 3));
    serpent_liberer(s);
    VERIFIER(ok);
}

static void t3_prochaine(void)
{
    Serpent *s = serpent_creer(pos(5, 2), 3, DROITE);
    VERIFIER(s != NULL);
    int ok = egal(serpent_prochaine_tete(s), 6, 2);
    serpent_changer_direction(s, BAS);
    ok = ok && egal(serpent_prochaine_tete(s), 5, 3) && egal(serpent_tete(s), 5, 2);
    serpent_liberer(s);
    VERIFIER(ok);
}

static void t3_avancer(void)
{
    Serpent *s = serpent_creer(pos(5, 2), 3, DROITE);
    VERIFIER(s != NULL);
    int ok = serpent_avancer(s, 0) == 1 && s->longueur == 3
             && egal(s->corps[0], 6, 2) && egal(s->corps[1], 5, 2) && egal(s->corps[2], 4, 2);
    serpent_changer_direction(s, BAS);
    ok = ok && serpent_avancer(s, 0) == 1
         && egal(s->corps[0], 6, 3) && egal(s->corps[1], 6, 2) && egal(s->corps[2], 5, 2);
    serpent_liberer(s);
    VERIFIER(ok);
}

static void t3_grandir(void)
{
    Serpent *s = serpent_creer(pos(5, 2), 4, DROITE);
    VERIFIER(s != NULL);
    int ok = serpent_avancer(s, 1) == 1 && s->longueur == 5 && s->capacite == 8
             && egal(s->corps[0], 6, 2) && egal(s->corps[4], 2, 2) && egal(s->corps[3], 3, 2);
    ok = ok && serpent_avancer(s, 1) == 1 && s->longueur == 6 && egal(s->corps[5], 2, 2)
         && egal(s->corps[0], 7, 2);
    serpent_liberer(s);
    VERIFIER(ok);
}

static void t3_demi_tour(void)
{
    Serpent *s = serpent_creer(pos(5, 2), 3, DROITE);
    VERIFIER(s != NULL);
    int ok = serpent_changer_direction(s, GAUCHE) == 0 && s->direction == DROITE
             && serpent_changer_direction(s, HAUT) == 1 && s->direction == HAUT
             && serpent_changer_direction(s, DROITE) == 1 && s->direction == DROITE;
    serpent_liberer(s);
    VERIFIER(ok);
}

static void t3_piege(void)
{
    // Le piège : HAUT puis GAUCHE très vite, AVANT le pas suivant.
    // GAUCHE est l'opposé du dernier pas (DROITE) : il doit être refusé,
    // sinon le serpent se retourne sur son propre cou.
    Serpent *s = serpent_creer(pos(5, 2), 3, DROITE);
    VERIFIER(s != NULL);
    int ok = serpent_changer_direction(s, HAUT) == 1
             && serpent_changer_direction(s, GAUCHE) == 0 && s->direction == HAUT;
    ok = ok && serpent_avancer(s, 0) == 1 && s->derniere == HAUT
         && serpent_changer_direction(s, GAUCHE) == 1;
    serpent_liberer(s);
    VERIFIER(ok);
}

static const Test ETAPE3[] = {
    {"serpent_creer vers la DROITE : corps, capacite, directions", t3_creer},
    {"serpent_creer vers le HAUT : le corps s'etend vers le bas", t3_creer_haut},
    {"serpent_creer refuse une longueur < 1", t3_creer_refuse},
    {"serpent_creer de longueur 10 : capacite 4 -> 8 -> 16", t3_long},
    {"serpent_ajouter_queue : la capacite double quand c'est plein", t3_ajouter},
    {"serpent_tete et serpent_contient", t3_tete_contient},
    {"serpent_prochaine_tete suit la direction demandee", t3_prochaine},
    {"serpent_avancer sans grandir : chaque case suit la precedente", t3_avancer},
    {"serpent_avancer en grandissant : la queue reste en place", t3_grandir},
    {"serpent_changer_direction refuse le demi-tour", t3_demi_tour},
    {"piege du double virage : comparer au DERNIER PAS", t3_piege},
};

// =============================================================================
// Étape 4 — la partie
// =============================================================================

static Partie *partie_test(unsigned graine)
{
    ecrire_fichier(FICHIER_TMP, CARTE_TEST);
    Partie *p = partie_creer(FICHIER_TMP, graine);
    remove(FICHIER_TMP);
    return p;
}

static void t4_creer(void)
{
    Partie *p = partie_test(1u);
    VERIFIER(p != NULL);
    VERIFIER(p->carte != NULL && p->serpent != NULL);
    int ok = p->carte->largeur == 8 && p->serpent->longueur == LONGUEUR_INITIALE
             && egal(p->serpent->corps[0], 3, 2) && egal(p->serpent->corps[2], 1, 2)
             && p->serpent->direction == DROITE
             && p->score == 0 && p->pommes == 0 && p->etat == PARTIE_EN_COURS
             && strcmp(p->chemin_carte, FICHIER_TMP) == 0
             && carte_case(p->carte, p->pomme) == CASE_VIDE
             && !serpent_contient(p->serpent, p->pomme);
    partie_liberer(p);
    VERIFIER(ok);
}

static void t4_creer_refuse(void)
{
    VERIFIER(partie_creer("cartes/n_existe_pas.txt", 1u) == NULL);
    // Départ collé au mur de gauche : la queue tomberait dans le mur.
    ecrire_fichier(FICHIER_TMP, "6 5\n######\n#....#\n#@...#\n#....#\n######\n");
    Partie *p = partie_creer(FICHIER_TMP, 1u);
    remove(FICHIER_TMP);
    partie_liberer(p);
    VERIFIER(p == NULL);
    partie_liberer(NULL);
}

static void t4_case_libre(void)
{
    Partie *p = partie_test(1u);
    VERIFIER(p != NULL && p->carte != NULL && p->serpent != NULL);
    int ok = !partie_case_libre(p, pos(0, 0))        // mur
             && !partie_case_libre(p, pos(2, 2))     // serpent
             && !partie_case_libre(p, pos(-1, 3))    // dehors
             && !partie_case_libre(p, pos(8, 3))     // dehors
             && partie_case_libre(p, pos(6, 4));
    partie_liberer(p);
    VERIFIER(ok);
}

// La règle du sujet, réécrite indépendamment : la k-ième case libre.
static Position pomme_attendue(const Partie *p, unsigned graine)
{
    int libres = 0;
    for (int y = 0; y < p->carte->hauteur; y++) {
        for (int x = 0; x < p->carte->largeur; x++) {
            if (carte_case(p->carte, pos(x, y)) == CASE_VIDE
                && !serpent_contient(p->serpent, pos(x, y))) {
                libres++;
            }
        }
    }
    int k = (int)(alea_suivant(&graine) % (unsigned)libres);
    for (int y = 0; y < p->carte->hauteur; y++) {
        for (int x = 0; x < p->carte->largeur; x++) {
            if (carte_case(p->carte, pos(x, y)) == CASE_VIDE
                && !serpent_contient(p->serpent, pos(x, y)) && k-- == 0) {
                return pos(x, y);
            }
        }
    }
    return pos(-1, -1);
}

static void t4_pomme_regle(void)
{
    unsigned graines[] = {1u, 42u, 2026u, 123456789u};
    for (int i = 0; i < 4; i++) {
        Partie *p = partie_test(graines[i]);
        VERIFIER(p != NULL && p->carte != NULL && p->serpent != NULL);
        Position attendue = pomme_attendue(p, graines[i]);
        unsigned graine_suivante = graines[i];
        alea_suivant(&graine_suivante);
        int ok = positions_egales(p->pomme, attendue) && p->graine == graine_suivante;
        partie_liberer(p);
        VERIFIER(ok);
    }
}

static void t4_pomme_pleine(void)
{
    ecrire_fichier(FICHIER_TMP, "5 5\n#####\n#...#\n#..@#\n#...#\n#####\n");
    Partie *p = partie_creer(FICHIER_TMP, 3u);
    remove(FICHIER_TMP);
    VERIFIER(p != NULL && p->serpent != NULL);
    Position autres[] = {{1, 1}, {2, 1}, {3, 1}, {1, 3}, {2, 3}, {3, 3}};
    for (int i = 0; i < 6; i++) {
        serpent_ajouter_queue(p->serpent, autres[i]);
    }
    int ok = partie_placer_pomme(p) == 0;
    partie_liberer(p);
    VERIFIER(ok);
}

static void t4_avance(void)
{
    Partie *p = partie_test(5u);
    VERIFIER(p != NULL && p->serpent != NULL);
    p->pomme = pos(6, 4);
    partie_etape(p);
    int ok = p->etat == PARTIE_EN_COURS && p->serpent->longueur == 3
             && egal(p->serpent->corps[0], 4, 2) && egal(p->serpent->corps[2], 2, 2)
             && p->score == 0;
    partie_liberer(p);
    VERIFIER(ok);
}

static void t4_mange(void)
{
    Partie *p = partie_test(5u);
    VERIFIER(p != NULL && p->serpent != NULL);
    p->pomme = pos(4, 2);
    unsigned graine = p->graine;
    partie_etape(p);
    int ok = p->etat == PARTIE_EN_COURS && p->serpent->longueur == 4
             && egal(p->serpent->corps[0], 4, 2) && egal(p->serpent->corps[3], 1, 2)
             && p->score == POINTS_PAR_POMME && p->pommes == 1
             && positions_egales(p->pomme, pomme_attendue(p, graine));
    partie_liberer(p);
    VERIFIER(ok);
}

static void t4_mur(void)
{
    Partie *p = partie_test(5u);
    VERIFIER(p != NULL && p->serpent != NULL);
    p->pomme = pos(6, 4);
    serpent_changer_direction(p->serpent, HAUT);
    partie_etape(p);                                   // (3,1)
    VERIFIER(p->etat == PARTIE_EN_COURS);
    partie_etape(p);                                   // (3,0) : mur
    int ok = p->etat == PARTIE_PERDUE && egal(p->serpent->corps[0], 3, 1);
    partie_etape(p);                                   // plus rien ne bouge
    ok = ok && egal(p->serpent->corps[0], 3, 1);
    partie_liberer(p);
    VERIFIER(ok);
}

// Remplace le serpent de la partie par celui décrit par `corps`.
static int poser_serpent(Partie *p, const Position corps[], int n, Direction d)
{
    serpent_liberer(p->serpent);
    p->serpent = serpent_creer(corps[0], 1, d);
    if (p->serpent == NULL) {
        return 0;
    }
    for (int i = 1; i < n; i++) {
        serpent_ajouter_queue(p->serpent, corps[i]);
    }
    return p->serpent->longueur == n;
}

static void t4_soi_meme(void)
{
    Partie *p = partie_test(5u);
    VERIFIER(p != NULL && p->serpent != NULL);
    // Tête en (2,3) qui monte vers (2,2), occupée par le corps.
    Position corps[] = {{2, 3}, {3, 3}, {3, 2}, {2, 2}, {1, 2}};
    VERIFIER(poser_serpent(p, corps, 5, HAUT));
    p->pomme = pos(6, 4);
    partie_etape(p);
    int ok = p->etat == PARTIE_PERDUE;
    partie_liberer(p);
    VERIFIER(ok);
}

static void t4_queue(void)
{
    Partie *p = partie_test(5u);
    VERIFIER(p != NULL && p->serpent != NULL);
    // Un carré : la tête (2,2) va sur la queue (3,2), qui s'en va au même pas.
    Position corps[] = {{2, 2}, {2, 3}, {3, 3}, {3, 2}};
    VERIFIER(poser_serpent(p, corps, 4, DROITE));
    p->pomme = pos(6, 4);
    partie_etape(p);
    int ok = p->etat == PARTIE_EN_COURS && egal(p->serpent->corps[0], 3, 2)
             && egal(p->serpent->corps[3], 3, 3);
    partie_liberer(p);
    VERIFIER(ok);
}

static void t4_gagne(void)
{
    ecrire_fichier(FICHIER_TMP, "5 5\n#####\n#...#\n#..@#\n#...#\n#####\n");
    Partie *p = partie_creer(FICHIER_TMP, 3u);
    remove(FICHIER_TMP);
    VERIFIER(p != NULL && p->serpent != NULL);
    // Il ne reste qu'une case libre, (3,1), juste au-dessus de la tête.
    Position autres[] = {{1, 3}, {2, 3}, {3, 3}, {1, 1}, {2, 1}};
    for (int i = 0; i < 5; i++) {
        serpent_ajouter_queue(p->serpent, autres[i]);
    }
    serpent_changer_direction(p->serpent, HAUT);
    p->pomme = pos(3, 1);
    partie_etape(p);
    int ok = p->etat == PARTIE_GAGNEE && p->serpent->longueur == 9 && p->score == 10;
    partie_liberer(p);
    VERIFIER(ok);
}

static void t4_delai(void)
{
    Partie *p = partie_test(5u);
    VERIFIER(p != NULL);
    int ok = partie_delai_ms(p) == 150;
    p->pommes = 6;
    ok = ok && partie_delai_ms(p) == 120;
    p->pommes = 18;
    ok = ok && partie_delai_ms(p) == 60;
    p->pommes = 100;
    ok = ok && partie_delai_ms(p) == 60;
    partie_liberer(p);
    VERIFIER(ok);
}

static const Test ETAPE4[] = {
    {"partie_creer : serpent, score, etat, chemin, pomme", t4_creer},
    {"partie_creer refuse une carte absente ou un depart colle au mur", t4_creer_refuse},
    {"partie_case_libre : mur, serpent, dehors, case libre", t4_case_libre},
    {"partie_placer_pomme suit exactement la regle du sujet", t4_pomme_regle},
    {"partie_placer_pomme renvoie 0 s'il n'y a plus de place", t4_pomme_pleine},
    {"partie_etape : le serpent avance", t4_avance},
    {"partie_etape : manger une pomme (grandir, score, nouvelle pomme)", t4_mange},
    {"partie_etape : un mur fait perdre, puis plus rien ne bouge", t4_mur},
    {"partie_etape : se mordre fait perdre", t4_soi_meme},
    {"partie_etape : entrer dans la case que la queue quitte est permis", t4_queue},
    {"partie_etape : derniere case libre mangee -> gagne", t4_gagne},
    {"partie_delai_ms : 150 ms, -5 ms par pomme, 60 ms au minimum", t4_delai},
};

// =============================================================================
// Étape 5 — sauvegarde et reprise
// =============================================================================

// Une partie un peu avancée : 2 pommes mangées, serpent de 5 cases.
static Partie *partie_avancee(void)
{
    ecrire_fichier(FICHIER_TMP2, CARTE_TEST);
    Partie *p = partie_creer(FICHIER_TMP2, 77u);
    if (p == NULL || p->serpent == NULL) {
        return p;
    }
    p->pomme = pos(4, 2);
    partie_etape(p);
    p->pomme = pos(5, 2);
    partie_etape(p);
    serpent_changer_direction(p->serpent, BAS);
    p->pomme = pos(6, 4);
    partie_etape(p);
    return p;
}

static void t5_format(void)
{
    Partie *p = partie_avancee();
    VERIFIER(p != NULL && p->serpent != NULL && p->serpent->longueur == 5);
    p->graine = 4242u;
    int ok = partie_sauvegarder(p, FICHIER_TMP) == 1;
    partie_liberer(p);
    remove(FICHIER_TMP2);
    VERIFIER(ok);
    char texte[512];
    VERIFIER(lire_fichier(FICHIER_TMP, texte, sizeof texte));
    remove(FICHIER_TMP);
    VERIFIER(strcmp(texte,
                    "SERPENT\n"
                    "carte test_tmp2.txt\n"
                    "score 20\n"
                    "pommes 2\n"
                    "graine 4242\n"
                    "direction BAS\n"
                    "pomme 6 4\n"
                    "longueur 5\n"
                    "5 3\n5 2\n4 2\n3 2\n2 2\n") == 0);
}

static void t5_aller_retour(void)
{
    Partie *p = partie_avancee();
    VERIFIER(p != NULL && p->serpent != NULL);
    VERIFIER(partie_sauvegarder(p, FICHIER_TMP) == 1);
    Partie *q = partie_reprendre(FICHIER_TMP);
    int referme = remove(FICHIER_TMP) == 0;
    int ok = q != NULL && q->carte != NULL && q->serpent != NULL
             && strcmp(q->chemin_carte, p->chemin_carte) == 0
             && q->score == p->score && q->pommes == p->pommes && q->graine == p->graine
             && positions_egales(q->pomme, p->pomme) && q->etat == PARTIE_EN_COURS
             && q->serpent->longueur == p->serpent->longueur
             && q->serpent->direction == p->serpent->direction
             && q->serpent->derniere == p->serpent->direction
             && q->carte->largeur == p->carte->largeur;
    for (int i = 0; ok && i < p->serpent->longueur; i++) {
        ok = positions_egales(q->serpent->corps[i], p->serpent->corps[i]);
    }
    if (ok) {
        // La partie reprise continue normalement.
        partie_etape(q);
        ok = q->etat == PARTIE_EN_COURS && egal(q->serpent->corps[0], 5, 4);
    }
    partie_liberer(p);
    partie_liberer(q);
    remove(FICHIER_TMP2);
    VERIFIER(ok);
    VERIFIER(referme);
}

static void t5_absent(void)
{
    VERIFIER(partie_reprendre("n_existe_pas.txt") == NULL);
}

// Une sauvegarde valide, modifiée par le test pour la rendre incohérente.
static const char *SAUVEGARDE_OK =
    "SERPENT\n"
    "carte test_tmp2.txt\n"
    "score 20\n"
    "pommes 2\n"
    "graine 4242\n"
    "direction BAS\n"
    "pomme 6 4\n"
    "longueur 3\n"
    "5 3\n5 2\n4 2\n";

static int reprise_refusee(const char *texte)
{
    ecrire_fichier(FICHIER_TMP2, CARTE_TEST);
    ecrire_fichier(FICHIER_TMP, texte);
    Partie *p = partie_reprendre(FICHIER_TMP);
    int referme = remove(FICHIER_TMP) == 0;
    remove(FICHIER_TMP2);
    if (p != NULL) {
        partie_liberer(p);
        return 0;
    }
    return referme;
}

static void t5_valide(void)
{
    ecrire_fichier(FICHIER_TMP2, CARTE_TEST);
    ecrire_fichier(FICHIER_TMP, SAUVEGARDE_OK);
    Partie *p = partie_reprendre(FICHIER_TMP);
    remove(FICHIER_TMP);
    remove(FICHIER_TMP2);
    int ok = p != NULL && p->serpent != NULL && p->serpent->longueur == 3
             && egal(p->serpent->corps[2], 4, 2) && p->serpent->direction == BAS;
    partie_liberer(p);
    VERIFIER(ok);
}

static void t5_crlf(void)
{
    ecrire_fichier(FICHIER_TMP2, CARTE_TEST);
    ecrire_fichier(FICHIER_TMP, "SERPENT\r\ncarte test_tmp2.txt\r\nscore 20\r\npommes 2\r\n"
                                "graine 4242\r\ndirection BAS\r\npomme 6 4\r\nlongueur 3\r\n"
                                "5 3\r\n5 2\r\n4 2\r\n");
    Partie *p = partie_reprendre(FICHIER_TMP);
    remove(FICHIER_TMP);
    remove(FICHIER_TMP2);
    int ok = p != NULL && p->carte != NULL && p->serpent != NULL && p->serpent->longueur == 3;
    partie_liberer(p);
    VERIFIER(ok);
}

// Remplace la première occurrence de `avant` par `apres` dans SAUVEGARDE_OK.
static int refuse_si(const char *avant, const char *apres)
{
    char texte[512];
    const char *ou = strstr(SAUVEGARDE_OK, avant);
    if (ou == NULL) {
        return 0;
    }
    size_t debut = (size_t)(ou - SAUVEGARDE_OK);
    snprintf(texte, sizeof texte, "%.*s%s%s", (int)debut, SAUVEGARDE_OK, apres,
             ou + strlen(avant));
    return reprise_refusee(texte);
}

static void t5_entete(void)
{
    VERIFIER(refuse_si("SERPENT", "SERPENTS"));
    VERIFIER(refuse_si("carte test_tmp2.txt", "carte absente.txt"));
    VERIFIER(refuse_si("score 20", "score vingt"));
    VERIFIER(refuse_si("score 20", "score -20"));
    VERIFIER(refuse_si("direction BAS", "direction SUD"));
    VERIFIER(refuse_si("longueur 3", "longueur 0"));
}

static void t5_corps(void)
{
    VERIFIER(refuse_si("longueur 3", "longueur 4"));          // une position manque
    VERIFIER(refuse_si("4 2\n", "0 2\n"));                     // dans un mur
    VERIFIER(refuse_si("4 2\n", "9 2\n"));                     // hors de la carte
    VERIFIER(refuse_si("4 2\n", "5 3\n"));                     // case déjà occupée
    VERIFIER(refuse_si("pomme 6 4", "pomme 5 2"));             // pomme sur le serpent
    VERIFIER(refuse_si("pomme 6 4", "pomme 0 0"));             // pomme dans un mur
}

static const Test ETAPE5[] = {
    {"partie_sauvegarder ecrit exactement le format du sujet", t5_format},
    {"sauvegarder puis reprendre redonne la meme partie", t5_aller_retour},
    {"partie_reprendre : fichier absent -> NULL", t5_absent},
    {"partie_reprendre lit une sauvegarde ecrite a la main", t5_valide},
    {"partie_reprendre accepte les fins de ligne \\r\\n", t5_crlf},
    {"en-tete incoherent -> NULL", t5_entete},
    {"corps ou pomme incoherents -> NULL", t5_corps},
};

// =============================================================================
// Étape 6 — meilleurs scores
// =============================================================================

static void t6_noms(void)
{
    VERIFIER(score_nom_valide("Alice") == 1);
    VERIFIER(score_nom_valide("a") == 1);
    VERIFIER(score_nom_valide("bob_2-X") == 1);
    VERIFIER(score_nom_valide("abcdefghijklmno") == 1);      // 15 caractères
    VERIFIER(score_nom_valide("abcdefghijklmnop") == 0);     // 16 caractères
    VERIFIER(score_nom_valide("") == 0);
    VERIFIER(score_nom_valide("a b") == 0);
    VERIFIER(score_nom_valide("a;b") == 0);
    VERIFIER(score_nom_valide("\xc3\xa9va") == 0);           // "éva" en UTF-8
}

static void t6_inserer_ordre(void)
{
    Score t[SCORES_MAX];
    int n = 0;
    VERIFIER(scores_inserer(t, &n, SCORES_MAX, "bob", 50) == 0 && n == 1);
    VERIFIER(scores_inserer(t, &n, SCORES_MAX, "alice", 80) == 0 && n == 2);
    VERIFIER(scores_inserer(t, &n, SCORES_MAX, "carl", 20) == 2 && n == 3);
    VERIFIER(strcmp(t[0].nom, "alice") == 0 && t[0].score == 80);
    VERIFIER(strcmp(t[1].nom, "bob") == 0 && t[1].score == 50);
    VERIFIER(strcmp(t[2].nom, "carl") == 0 && t[2].score == 20);
}

static void t6_egalite(void)
{
    Score t[SCORES_MAX];
    int n = 0;
    scores_inserer(t, &n, SCORES_MAX, "A", 50);
    VERIFIER(scores_inserer(t, &n, SCORES_MAX, "B", 50) == 1);
    VERIFIER(n == 2 && strcmp(t[0].nom, "A") == 0 && strcmp(t[1].nom, "B") == 0);
}

static void t6_plein(void)
{
    Score t[3];
    int n = 0;
    scores_inserer(t, &n, 3, "a", 80);
    scores_inserer(t, &n, 3, "b", 50);
    scores_inserer(t, &n, 3, "c", 20);
    VERIFIER(n == 3);
    VERIFIER(scores_inserer(t, &n, 3, "d", 10) == -1 && n == 3);
    VERIFIER(scores_inserer(t, &n, 3, "e", 20) == -1 && n == 3);    // égalité : après c
    VERIFIER(scores_inserer(t, &n, 3, "f", 60) == 1 && n == 3);
    VERIFIER(strcmp(t[0].nom, "a") == 0 && strcmp(t[1].nom, "f") == 0
             && strcmp(t[2].nom, "b") == 0 && t[2].score == 50);
}

static void t6_refus(void)
{
    Score t[SCORES_MAX];
    int n = 0;
    VERIFIER(scores_inserer(t, &n, SCORES_MAX, "a b", 50) == -1 && n == 0);
    VERIFIER(scores_inserer(t, &n, SCORES_MAX, "bob", -1) == -1 && n == 0);
    VERIFIER(scores_inserer(t, &n, SCORES_MAX, "bob", 0) == 0 && n == 1);
}

static void t6_charger_absent(void)
{
    Score t[SCORES_MAX];
    VERIFIER(scores_charger("n_existe_pas.txt", t, SCORES_MAX) == 0);
}

static void t6_charger(void)
{
    ecrire_fichier(FICHIER_TMP, "bob;30\nalice;120\nmauvaise ligne\n;40\ncarl;abc\n"
                                "dan;-5\nfred;12x\neve;75\r\n");
    Score t[SCORES_MAX];
    int n = scores_charger(FICHIER_TMP, t, SCORES_MAX);
    int referme = remove(FICHIER_TMP) == 0;
    VERIFIER(n == 3);
    VERIFIER(strcmp(t[0].nom, "alice") == 0 && t[0].score == 120);
    VERIFIER(strcmp(t[1].nom, "eve") == 0 && t[1].score == 75);
    VERIFIER(strcmp(t[2].nom, "bob") == 0 && t[2].score == 30);
    VERIFIER(referme);
}

static void t6_charger_max(void)
{
    char texte[512] = "";
    for (int i = 1; i <= 12; i++) {
        char ligne[32];
        snprintf(ligne, sizeof ligne, "j%d;%d\n", i, i * 10);
        strcat(texte, ligne);
    }
    ecrire_fichier(FICHIER_TMP, texte);
    Score t[SCORES_MAX];
    int n = scores_charger(FICHIER_TMP, t, SCORES_MAX);
    remove(FICHIER_TMP);
    VERIFIER(n == SCORES_MAX);
    VERIFIER(t[0].score == 120 && t[9].score == 30 && strcmp(t[9].nom, "j3") == 0);
}

static void t6_sauvegarder(void)
{
    Score t[SCORES_MAX];
    int n = 0;
    scores_inserer(t, &n, SCORES_MAX, "eve", 75);
    scores_inserer(t, &n, SCORES_MAX, "alice", 120);
    VERIFIER(scores_sauvegarder(FICHIER_TMP, t, n) == 1);
    char texte[256];
    VERIFIER(lire_fichier(FICHIER_TMP, texte, sizeof texte));
    Score u[SCORES_MAX];
    int m = scores_charger(FICHIER_TMP, u, SCORES_MAX);
    remove(FICHIER_TMP);
    VERIFIER(strcmp(texte, "alice;120\neve;75\n") == 0);
    VERIFIER(m == 2 && strcmp(u[1].nom, "eve") == 0 && u[1].score == 75);
}

static const Test ETAPE6[] = {
    {"score_nom_valide", t6_noms},
    {"scores_inserer garde le tableau trie et renvoie le rang", t6_inserer_ordre},
    {"scores_inserer : a egalite, le nouveau se place apres", t6_egalite},
    {"scores_inserer dans un tableau plein", t6_plein},
    {"scores_inserer refuse un nom invalide ou un score < 0", t6_refus},
    {"scores_charger : fichier absent -> 0 score", t6_charger_absent},
    {"scores_charger trie et ignore les lignes mal formees", t6_charger},
    {"scores_charger garde les max meilleurs", t6_charger_max},
    {"scores_sauvegarder : format nom;score, relu a l'identique", t6_sauvegarder},
};

// =============================================================================

typedef struct {
    int numero;
    const char *titre;
    const Test *tests;
    int nombre;
} Etape;

#define ETAPE(n, titre, t) {n, titre, t, (int)(sizeof t / sizeof t[0])}

static const Etape ETAPES[] = {
    ETAPE(1, "Positions et carte en memoire", ETAPE1),
    ETAPE(2, "Lecture d'une carte", ETAPE2),
    ETAPE(3, "Le serpent", ETAPE3),
    ETAPE(4, "La partie", ETAPE4),
    ETAPE(5, "Sauvegarde et reprise", ETAPE5),
    ETAPE(6, "Meilleurs scores", ETAPE6),
};
#define NB_ETAPES ((int)(sizeof ETAPES / sizeof ETAPES[0]))

static int lancer_etape(const Etape *e)
{
    int reussis = 0;
    printf("\nEtape %d - %s\n", e->numero, e->titre);
    for (int i = 0; i < e->nombre; i++) {
        printf("  %-66s ", e->tests[i].nom);
        fflush(stdout);                  // si le test plante, on sait lequel
        a_faire_effacer();
        test_ok = 1;
        e->tests[i].fonction();
        if (a_faire_etape() != 0) {
            printf("[A FAIRE] %s()\n", a_faire_fonction());
        } else if (test_ok) {
            printf("[ OK ]\n");
            reussis++;
        } else {
            printf("[ECHEC]\n      tests.c ligne %d : %s\n", test_ligne, test_condition);
        }
    }
    printf("  -> %d / %d\n", reussis, e->nombre);
    return reussis;
}

int main(int argc, char *argv[])
{
    int seule = (argc > 1) ? atoi(argv[1]) : 0;
    int reussis[NB_ETAPES] = {0};
    int total = 0, total_reussis = 0;

    for (int i = 0; i < NB_ETAPES; i++) {
        if (seule == 0 || seule == ETAPES[i].numero) {
            reussis[i] = lancer_etape(&ETAPES[i]);
            total += ETAPES[i].nombre;
            total_reussis += reussis[i];
        }
    }
    if (total == 0) {
        printf("Etape inconnue : %s (de 1 a %d)\n", argv[1], NB_ETAPES);
        return 2;
    }

    printf("\n========== Bilan ==========\n");
    for (int i = 0; i < NB_ETAPES; i++) {
        if (seule == 0 || seule == ETAPES[i].numero) {
            printf("  Etape %d %-32s %2d / %2d\n", ETAPES[i].numero, ETAPES[i].titre,
                   reussis[i], ETAPES[i].nombre);
        }
    }
    printf("  TOTAL %-34s %2d / %2d\n", "", total_reussis, total);
    return total_reussis == total ? 0 : 1;
}
