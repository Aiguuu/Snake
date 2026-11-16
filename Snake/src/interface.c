/*
 * interface.c — l'interface graphique du Serpent (bibliothèque raylib).
 *
 * FICHIER FOURNI : vous n'avez pas à le modifier... mais lisez-le ! Il
 * n'utilise QUE les fonctions déclarées dans les en-têtes (partie.h,
 * scores.h...) : c'est votre code qui fait tourner le jeu.
 *
 * Touches : flèches (ou ZQSD sur un clavier AZERTY) pour diriger le serpent,
 * P ou Échap pour la pause, S (en pause) pour sauvegarder et quitter.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "raylib.h"

#include "a_faire.h"
#include "partie.h"
#include "scores.h"

#define LARGEUR_FENETRE 960
#define HAUTEUR_FENETRE 720
#define HAUT_BANDEAU 64
#define HAUT_PIED 34
#define FICHIER_SAUVEGARDE "sauvegarde.txt"

typedef struct {
    const char *nom;
    const char *chemin;
} InfoCarte;

static const InfoCarte CARTES[] = {
    {"Le jardin",     "cartes/jardin.txt"},
    {"L'arène",       "cartes/arene.txt"},
    {"Les couloirs",  "cartes/couloirs.txt"},
    {"Le labyrinthe", "cartes/labyrinthe.txt"},
    {"Le cagibi",     "cartes/cagibi.txt"},
};
#define NB_CARTES ((int)(sizeof CARTES / sizeof CARTES[0]))

typedef enum { ECRAN_MENU, ECRAN_JEU, ECRAN_PAUSE, ECRAN_FIN, ECRAN_SCORES } Ecran;

typedef struct {
    Ecran ecran;
    int choix;                     // carte sélectionnée dans le menu
    Carte *apercus[NB_CARTES];     // cartes chargées pour les miniatures
    Partie *partie;
    int demo;                      // 1 : le serpent joue tout seul
    double prochain_pas;           // instant (secondes) du prochain pas
    char fichier_scores[CHEMIN_MAX];
    Score scores[SCORES_MAX];
    int nb_scores;
    int rang;                      // rang du record en cours de saisie, -1 sinon
    int surligne;                  // ligne du tableau à mettre en couleur, -1 sinon
    char nom[NOM_MAX];
    char message[160];
    double fin_message;
} App;

// --- Couleurs -------------------------------------------------------------
static const Color FOND        = {28, 32, 40, 255};
static const Color BANDEAU     = {20, 23, 30, 255};
static const Color HERBE_1     = {170, 215, 81, 255};
static const Color HERBE_2     = {162, 209, 73, 255};
static const Color MUR         = {74, 84, 99, 255};
static const Color MUR_HAUT    = {104, 116, 135, 255};
static const Color SERPENT_TETE  = {66, 110, 230, 255};
static const Color SERPENT_QUEUE = {120, 165, 250, 255};
static const Color POMME       = {231, 71, 29, 255};
static const Color TEXTE       = {236, 239, 244, 255};
static const Color TEXTE_PALE  = {150, 158, 172, 255};
static const Color ACCENT      = {255, 196, 61, 255};
static const Color ALERTE      = {200, 50, 50, 255};

// --- Petits outils ----------------------------------------------------------

static void texte_centre(const char *texte, int cx, int y, int taille, Color c)
{
    DrawText(texte, cx - MeasureText(texte, taille) / 2, y, taille, c);
}

static Color melange(Color a, Color b, float t)
{
    Color c;
    c.r = (unsigned char)(a.r + (b.r - a.r) * t);
    c.g = (unsigned char)(a.g + (b.g - a.g) * t);
    c.b = (unsigned char)(a.b + (b.b - a.b) * t);
    c.a = 255;
    return c;
}

static void afficher_message(App *app, const char *texte)
{
    snprintf(app->message, sizeof app->message, "%s", texte);
    app->fin_message = GetTime() + 3.0;
}

// "cartes/jardin.txt" -> "scores_jardin.txt"
static void nom_fichier_scores(const char *chemin_carte, char *dest, size_t taille)
{
    const char *base = chemin_carte;
    for (const char *c = chemin_carte; *c != '\0'; c++) {
        if (*c == '/' || *c == '\\') {
            base = c + 1;
        }
    }
    char nom[CHEMIN_MAX];
    snprintf(nom, sizeof nom, "%s", base);
    char *point = strrchr(nom, '.');
    if (point != NULL) {
        *point = '\0';
    }
    snprintf(dest, taille, "scores_%s.txt", nom);
}

static const char *nom_de_carte(const char *chemin)
{
    for (int i = 0; i < NB_CARTES; i++) {
        if (strcmp(CARTES[i].chemin, chemin) == 0) {
            return CARTES[i].nom;
        }
    }
    return chemin;
}

static void charger_scores(App *app, const char *chemin_carte)
{
    nom_fichier_scores(chemin_carte, app->fichier_scores, sizeof app->fichier_scores);
    app->nb_scores = scores_charger(app->fichier_scores, app->scores, SCORES_MAX);
    if (app->nb_scores < 0 || app->nb_scores > SCORES_MAX) {
        app->nb_scores = 0;
    }
}

// --- Géométrie du plateau ---------------------------------------------------

typedef struct {
    int x0, y0;     // coin haut gauche, en pixels
    int cote;       // taille d'une case
} Plateau;

static Plateau plateau_pour(const Carte *carte, int x, int y, int largeur, int hauteur)
{
    Plateau pl;
    int cl = largeur / carte->largeur;
    int ch = hauteur / carte->hauteur;
    pl.cote = cl < ch ? cl : ch;
    if (pl.cote < 1) {
        pl.cote = 1;
    }
    pl.x0 = x + (largeur - pl.cote * carte->largeur) / 2;
    pl.y0 = y + (hauteur - pl.cote * carte->hauteur) / 2;
    return pl;
}

static Rectangle case_rect(Plateau pl, Position p)
{
    Rectangle r = {(float)(pl.x0 + p.x * pl.cote), (float)(pl.y0 + p.y * pl.cote),
                   (float)pl.cote, (float)pl.cote};
    return r;
}

// --- Dessin du jeu ----------------------------------------------------------

static void dessiner_carte(const Carte *carte, Plateau pl)
{
    for (int y = 0; y < carte->hauteur; y++) {
        for (int x = 0; x < carte->largeur; x++) {
            Position p = {x, y};
            Rectangle r = case_rect(pl, p);
            if (carte_case(carte, p) == CASE_MUR) {
                DrawRectangleRec(r, MUR);
                DrawRectangle((int)r.x, (int)r.y, pl.cote, pl.cote / 5 + 1, MUR_HAUT);
            } else {
                DrawRectangleRec(r, (x + y) % 2 == 0 ? HERBE_1 : HERBE_2);
            }
        }
    }
}

static void dessiner_pomme(Plateau pl, Position p)
{
    Rectangle r = case_rect(pl, p);
    float cx = r.x + r.width / 2;
    float cy = r.y + r.height / 2 + r.height * 0.06f;
    float rayon = r.width * 0.38f;
    DrawRectangle((int)(cx - 1), (int)(cy - rayon - r.height * 0.18f), 3,
                  (int)(r.height * 0.2f), (Color){110, 70, 40, 255});
    DrawEllipse((int)(cx + rayon * 0.45f), (int)(cy - rayon * 0.95f),
                rayon * 0.45f, rayon * 0.22f, (Color){70, 150, 50, 255});
    DrawCircle((int)cx, (int)cy, rayon, POMME);
    DrawCircle((int)(cx - rayon * 0.35f), (int)(cy - rayon * 0.35f), rayon * 0.22f,
               (Color){255, 140, 110, 255});
}

static void dessiner_serpent(const Serpent *s, Plateau pl)
{
    if (s == NULL || s->corps == NULL || s->longueur < 1) {
        return;
    }
    float marge = pl.cote * 0.12f;
    // Du bout de la queue vers la tête, pour que la tête soit dessinée dessus.
    for (int i = s->longueur - 1; i >= 0; i--) {
        float t = s->longueur > 1 ? (float)i / (float)(s->longueur - 1) : 0.0f;
        Color c = melange(SERPENT_TETE, SERPENT_QUEUE, t);
        Rectangle r = case_rect(pl, s->corps[i]);
        Rectangle in = {r.x + marge, r.y + marge, r.width - 2 * marge, r.height - 2 * marge};
        DrawRectangleRounded(in, 0.45f, 6, c);
        if (i > 0) {
            // Remplir l'espace entre deux cases voisines du corps.
            Rectangle r2 = case_rect(pl, s->corps[i - 1]);
            int dx = s->corps[i - 1].x - s->corps[i].x;
            int dy = s->corps[i - 1].y - s->corps[i].y;
            if (dx * dx + dy * dy == 1) {
                Rectangle pont;
                pont.x = (dx != 0 ? (r.x < r2.x ? r.x : r2.x) + r.width / 2 : in.x);
                pont.y = (dy != 0 ? (r.y < r2.y ? r.y : r2.y) + r.height / 2 : in.y);
                pont.width = (dx != 0 ? r.width : in.width);
                pont.height = (dy != 0 ? r.height : in.height);
                DrawRectangleRec(pont, c);
            }
        }
    }
    // Les yeux regardent dans la direction du dernier pas.
    Rectangle t = case_rect(pl, s->corps[0]);
    float cx = t.x + t.width / 2, cy = t.y + t.height / 2;
    float ax = 0, ay = 0, ex = 0, ey = 0;
    switch (s->derniere) {
    case HAUT:   ay = -1; ex = 1; break;
    case BAS:    ay = 1;  ex = 1; break;
    case GAUCHE: ax = -1; ey = 1; break;
    case DROITE: ax = 1;  ey = 1; break;
    }
    float d = t.width * 0.18f, avant = t.width * 0.12f;
    float oeil = t.width * 0.13f;
    for (int k = -1; k <= 1; k += 2) {
        float x = cx + ax * avant + ex * d * k;
        float y = cy + ay * avant + ey * d * k;
        DrawCircle((int)x, (int)y, oeil, RAYWHITE);
        DrawCircle((int)(x + ax * oeil * 0.4f), (int)(y + ay * oeil * 0.4f), oeil * 0.5f, BLACK);
    }
}

static void dessiner_bandeau(const App *app)
{
    const Partie *p = app->partie;
    DrawRectangle(0, 0, LARGEUR_FENETRE, HAUT_BANDEAU, BANDEAU);
    DrawText("SERPENT", 20, 18, 30, ACCENT);
    DrawText(nom_de_carte(p->chemin_carte), 196, 24, 20, TEXTE_PALE);
    char t[64];
    snprintf(t, sizeof t, "Score %d", p->score);
    texte_centre(t, LARGEUR_FENETRE / 2 + 60, 18, 30, TEXTE);
    int record = app->nb_scores > 0 ? app->scores[0].score : 0;
    snprintf(t, sizeof t, "Record %d", record > p->score ? record : p->score);
    DrawText(t, LARGEUR_FENETRE - 20 - MeasureText(t, 20), 12, 20, TEXTE_PALE);
    snprintf(t, sizeof t, "Pommes %d", p->pommes);
    DrawText(t, LARGEUR_FENETRE - 20 - MeasureText(t, 20), 36, 20, TEXTE_PALE);
    if (app->demo) {
        DrawText("DEMO", 400, 24, 20, ACCENT);
    }
}

static void dessiner_jeu(const App *app)
{
    const Partie *p = app->partie;
    dessiner_bandeau(app);
    Plateau pl = plateau_pour(p->carte, 16, HAUT_BANDEAU + 8, LARGEUR_FENETRE - 32,
                              HAUTEUR_FENETRE - HAUT_BANDEAU - HAUT_PIED - 12);
    dessiner_carte(p->carte, pl);
    if (carte_contient(p->carte, p->pomme) && p->etat != PARTIE_GAGNEE) {
        dessiner_pomme(pl, p->pomme);
    }
    dessiner_serpent(p->serpent, pl);
}

static void voile(void)
{
    DrawRectangle(0, 0, LARGEUR_FENETRE, HAUTEUR_FENETRE, Fade(BLACK, 0.6f));
}

static void dessiner_tableau_scores(const App *app, int cx, int y)
{
    if (app->nb_scores == 0) {
        texte_centre("Aucun score enregistré.", cx, y, 20, TEXTE_PALE);
        return;
    }
    for (int i = 0; i < app->nb_scores; i++) {
        Color c = (i == app->surligne) ? ACCENT : TEXTE;
        char t[64];
        snprintf(t, sizeof t, "%2d.", i + 1);
        DrawText(t, cx - 170, y + 30 * i, 20, TEXTE_PALE);
        DrawText(app->scores[i].nom, cx - 120, y + 30 * i, 20, c);
        snprintf(t, sizeof t, "%d", app->scores[i].score);
        DrawText(t, cx + 170 - MeasureText(t, 20), y + 30 * i, 20, c);
    }
}

static void dessiner_fin(const App *app)
{
    const Partie *p = app->partie;
    voile();
    int cx = LARGEUR_FENETRE / 2;
    texte_centre(p->etat == PARTIE_GAGNEE ? "Gagné !" : "Perdu !", cx, 90, 60,
                 p->etat == PARTIE_GAGNEE ? ACCENT : TEXTE);
    char t[96];
    snprintf(t, sizeof t, "Score : %d   (%d pommes)", p->score, p->pommes);
    texte_centre(t, cx, 165, 30, TEXTE);

    if (app->rang >= 0) {
        snprintf(t, sizeof t, "Nouveau record : %d%s place !", app->rang + 1,
                 app->rang == 0 ? "re" : "e");
        texte_centre(t, cx, 230, 30, ACCENT);
        snprintf(t, sizeof t, "Votre nom : %s%s", app->nom,
                 ((int)(GetTime() * 2) % 2 == 0) ? "_" : " ");
        texte_centre(t, cx, 280, 30, TEXTE);
        texte_centre("lettres, chiffres, - et _   puis Entrée", cx, 325, 20, TEXTE_PALE);
    } else {
        texte_centre("Meilleurs scores", cx, 225, 30, ACCENT);
        dessiner_tableau_scores(app, cx, 270);
        texte_centre("Entrée : rejouer      M : menu", cx, HAUTEUR_FENETRE - 80, 20,
                     TEXTE_PALE);
    }
}

static void dessiner_pause(void)
{
    voile();
    int cx = LARGEUR_FENETRE / 2;
    texte_centre("Pause", cx, 230, 60, TEXTE);
    texte_centre("P : reprendre", cx, 320, 20, TEXTE);
    texte_centre("S : sauvegarder et revenir au menu", cx, 350, 20, TEXTE);
    texte_centre("M : abandonner et revenir au menu", cx, 380, 20, TEXTE);
}

static void dessiner_menu(const App *app)
{
    DrawText("SERPENT", 60, 50, 60, ACCENT);
    DrawText("Projet de langage C - INSA Strasbourg", 64, 115, 20, TEXTE_PALE);

    for (int i = 0; i < NB_CARTES; i++) {
        int y = 190 + 56 * i;
        if (i == app->choix) {
            DrawRectangleRounded((Rectangle){50, (float)y - 10, 360, 46}, 0.3f, 6,
                                 (Color){50, 58, 72, 255});
            DrawText(">", 64, y, 30, ACCENT);
        }
        DrawText(CARTES[i].nom, 96, y, 30, i == app->choix ? TEXTE : TEXTE_PALE);
    }

    // Miniature de la carte choisie.
    Rectangle cadre = {450, 170, 460, 330};
    DrawRectangleRounded(cadre, 0.04f, 6, (Color){40, 46, 58, 255});
    const Carte *c = app->apercus[app->choix];
    if (c != NULL) {
        Plateau pl = plateau_pour(c, (int)cadre.x + 15, (int)cadre.y + 15,
                                  (int)cadre.width - 30, (int)cadre.height - 30);
        dessiner_carte(c, pl);
        Rectangle r = case_rect(pl, c->depart);
        DrawRectangleRec(r, SERPENT_TETE);
        char t[64];
        snprintf(t, sizeof t, "%d x %d cases", c->largeur, c->hauteur);
        texte_centre(t, (int)(cadre.x + cadre.width / 2), (int)(cadre.y + cadre.height + 10),
                     20, TEXTE_PALE);
    } else {
        texte_centre("carte illisible", (int)(cadre.x + cadre.width / 2),
                     (int)(cadre.y + cadre.height / 2 - 10), 20, TEXTE_PALE);
    }

    int y = 545;
    DrawText("Haut/Bas : choisir la carte     Entrée : jouer", 60, y, 20, TEXTE);
    DrawText("T : meilleurs scores     D : démonstration     Échap : quitter", 60, y + 30, 20,
             TEXTE);
    if (FileExists(FICHIER_SAUVEGARDE)) {
        DrawText("R : reprendre la partie sauvegardée", 60, y + 60, 20, ACCENT);
    }
}

static void dessiner_ecran_scores(const App *app)
{
    int cx = LARGEUR_FENETRE / 2;
    texte_centre("Meilleurs scores", cx, 60, 40, ACCENT);
    texte_centre(CARTES[app->choix].nom, cx, 115, 30, TEXTE_PALE);
    dessiner_tableau_scores(app, cx, 180);
    texte_centre("Échap : retour", cx, HAUTEUR_FENETRE - 80, 20, TEXTE_PALE);
}

static void dessiner_pied(const App *app)
{
    if (a_faire_etape() != 0) {
        char t[160];
        snprintf(t, sizeof t, "Étape %d à écrire : %s()  -  lancez les tests pour suivre vos progrès",
                 a_faire_etape(), a_faire_fonction());
        DrawRectangle(0, HAUTEUR_FENETRE - HAUT_PIED, LARGEUR_FENETRE, HAUT_PIED, ALERTE);
        DrawText(t, 14, HAUTEUR_FENETRE - HAUT_PIED + 8, 20, RAYWHITE);
    }
    if (app->message[0] != '\0' && GetTime() < app->fin_message) {
        int l = MeasureText(app->message, 20) + 30;
        int y = HAUTEUR_FENETRE - HAUT_PIED - 50;
        DrawRectangleRounded((Rectangle){(float)(LARGEUR_FENETRE - l) / 2, (float)y,
                                         (float)l, 36}, 0.4f, 6, (Color){50, 58, 72, 240});
        texte_centre(app->message, LARGEUR_FENETRE / 2, y + 8, 20, TEXTE);
    }
}

// --- Démonstration : un joueur automatique simple ---------------------------

// Nombre de cases libres atteignables depuis `depart` (parcours en largeur).
static int zone_libre(const Partie *p, Position depart)
{
    static Position file[CARTE_LARGEUR_MAX * CARTE_HAUTEUR_MAX];
    static unsigned char vu[CARTE_LARGEUR_MAX * CARTE_HAUTEUR_MAX];
    const Carte *c = p->carte;
    memset(vu, 0, sizeof vu);
    int debut = 0, fin = 0;
    file[fin++] = depart;
    vu[depart.y * c->largeur + depart.x] = 1;
    while (debut < fin) {
        Position q = file[debut++];
        for (int d = HAUT; d <= DROITE; d++) {
            Position v = position_voisine(q, (Direction)d);
            if (partie_case_libre(p, v) && !vu[v.y * c->largeur + v.x]) {
                vu[v.y * c->largeur + v.x] = 1;
                file[fin++] = v;
            }
        }
    }
    return fin;
}

static void demo_choisir(Partie *p)
{
    Serpent *s = p->serpent;
    Position tete = serpent_tete(s);
    int meilleur = -1, meilleure_note = 1 << 30;
    for (int d = HAUT; d <= DROITE; d++) {
        if ((Direction)d == direction_opposee(s->derniere)) {
            continue;
        }
        Position v = position_voisine(tete, (Direction)d);
        if (!partie_case_libre(p, v)) {
            continue;
        }
        int note = abs(v.x - p->pomme.x) + abs(v.y - p->pomme.y);
        if (zone_libre(p, v) < s->longueur + 2) {
            note += 1000;                   // impasse probable
        }
        if (note < meilleure_note) {
            meilleure_note = note;
            meilleur = d;
        }
    }
    if (meilleur >= 0) {
        serpent_changer_direction(s, (Direction)meilleur);
    }
}

// --- Transitions --------------------------------------------------------------

static void quitter_partie(App *app)
{
    partie_liberer(app->partie);
    app->partie = NULL;
    app->demo = 0;
    app->ecran = ECRAN_MENU;
}

static void lancer_partie(App *app, Partie *p, int demo)
{
    partie_liberer(app->partie);
    app->partie = p;
    app->demo = demo;
    app->rang = -1;
    app->surligne = -1;
    app->nom[0] = '\0';
    charger_scores(app, p->chemin_carte);
    app->prochain_pas = GetTime() + 0.4;
    app->ecran = ECRAN_JEU;
}

static void nouvelle_partie(App *app, int demo)
{
    Partie *p = partie_creer(CARTES[app->choix].chemin, (unsigned)time(NULL));
    if (p == NULL || p->carte == NULL || p->serpent == NULL) {
        afficher_message(app, "Impossible de créer la partie (partie_creer a renvoyé NULL).");
        return;
    }
    lancer_partie(app, p, demo);
}

static void fin_de_partie(App *app)
{
    app->ecran = ECRAN_FIN;
    app->rang = -1;
    app->surligne = -1;
    if (app->demo || app->partie->score <= 0) {
        return;
    }
    // Le score entrerait-il dans le tableau ? On essaie sur une copie.
    Score copie[SCORES_MAX];
    int n = app->nb_scores;
    memcpy(copie, app->scores, sizeof copie);
    app->rang = scores_inserer(copie, &n, SCORES_MAX, "essai", app->partie->score);
    app->nom[0] = '\0';
}

static void valider_record(App *app)
{
    if (!score_nom_valide(app->nom)) {
        afficher_message(app, "Nom invalide.");
        return;
    }
    app->rang = scores_inserer(app->scores, &app->nb_scores, SCORES_MAX, app->nom,
                               app->partie->score);
    if (!scores_sauvegarder(app->fichier_scores, app->scores, app->nb_scores)) {
        afficher_message(app, "Impossible d'écrire le fichier des scores.");
    }
    app->surligne = app->rang;      // la ligne à mettre en couleur
    app->rang = -1;                 // fin de la saisie : on montre le tableau
}

// --- Gestion du clavier ----------------------------------------------------------

static int touche_direction(Direction *d)
{
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))    { *d = HAUT;   return 1; }
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))  { *d = BAS;    return 1; }
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A))  { *d = GAUCHE; return 1; }
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) { *d = DROITE; return 1; }
    return 0;
}

static void mettre_a_jour(App *app)
{
    switch (app->ecran) {
    case ECRAN_MENU:
        if (IsKeyPressed(KEY_DOWN)) app->choix = (app->choix + 1) % NB_CARTES;
        if (IsKeyPressed(KEY_UP)) app->choix = (app->choix + NB_CARTES - 1) % NB_CARTES;
        if (IsKeyPressed(KEY_ENTER)) nouvelle_partie(app, 0);
        if (IsKeyPressed(KEY_D)) nouvelle_partie(app, 1);
        if (IsKeyPressed(KEY_T)) {
            charger_scores(app, CARTES[app->choix].chemin);
            app->surligne = -1;
            app->ecran = ECRAN_SCORES;
        }
        if (IsKeyPressed(KEY_R) && FileExists(FICHIER_SAUVEGARDE)) {
            Partie *p = partie_reprendre(FICHIER_SAUVEGARDE);
            if (p == NULL || p->carte == NULL || p->serpent == NULL) {
                afficher_message(app, "Impossible de reprendre la sauvegarde.");
            } else {
                lancer_partie(app, p, 0);
                app->ecran = ECRAN_PAUSE;
            }
        }
        break;

    case ECRAN_SCORES:
        if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER)) app->ecran = ECRAN_MENU;
        break;

    case ECRAN_JEU: {
        Partie *p = app->partie;
        if (IsKeyPressed(KEY_P) || IsKeyPressed(KEY_ESCAPE)) {
            app->ecran = app->demo ? ECRAN_MENU : ECRAN_PAUSE;
            if (app->demo) quitter_partie(app);
            break;
        }
        Direction d;
        if (!app->demo && touche_direction(&d)) {
            serpent_changer_direction(p->serpent, d);
        }
        if (GetTime() >= app->prochain_pas) {
            if (app->demo) demo_choisir(p);
            partie_etape(p);
            int delai = partie_delai_ms(p);
            if (delai < 30) delai = 30;
            if (delai > 1000) delai = 1000;
            app->prochain_pas = GetTime() + delai / (app->demo ? 2000.0 : 1000.0);
            if (p->etat != PARTIE_EN_COURS) fin_de_partie(app);
        }
        break;
    }

    case ECRAN_PAUSE:
        if (IsKeyPressed(KEY_P) || IsKeyPressed(KEY_ESCAPE)) {
            app->ecran = ECRAN_JEU;
            app->prochain_pas = GetTime() + 0.3;
        } else if (IsKeyPressed(KEY_S)) {
            if (partie_sauvegarder(app->partie, FICHIER_SAUVEGARDE)) {
                afficher_message(app, "Partie sauvegardée.");
                quitter_partie(app);
            } else {
                afficher_message(app, "La sauvegarde a échoué.");
            }
        } else if (IsKeyPressed(KEY_M)) {
            quitter_partie(app);
        }
        break;

    case ECRAN_FIN:
        if (app->rang >= 0) {
            int c = GetCharPressed();
            while (c > 0) {
                size_t n = strlen(app->nom);
                int permis = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
                             || (c >= '0' && c <= '9') || c == '-' || c == '_';
                if (permis && n < NOM_MAX - 1) {
                    app->nom[n] = (char)c;
                    app->nom[n + 1] = '\0';
                }
                c = GetCharPressed();
            }
            if ((IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE))
                && app->nom[0] != '\0') {
                app->nom[strlen(app->nom) - 1] = '\0';
            }
            if (IsKeyPressed(KEY_ENTER)) valider_record(app);
            if (IsKeyPressed(KEY_ESCAPE)) app->rang = -1;
        } else {
            if (IsKeyPressed(KEY_ENTER)) {
                int demo = app->demo;
                for (int i = 0; i < NB_CARTES; i++) {
                    if (strcmp(CARTES[i].chemin, app->partie->chemin_carte) == 0) app->choix = i;
                }
                nouvelle_partie(app, demo);
            } else if (IsKeyPressed(KEY_M) || IsKeyPressed(KEY_ESCAPE)) {
                quitter_partie(app);
            }
        }
        break;
    }
}

static void dessiner(const App *app)
{
    BeginDrawing();
    ClearBackground(FOND);
    switch (app->ecran) {
    case ECRAN_MENU:   dessiner_menu(app); break;
    case ECRAN_SCORES: dessiner_ecran_scores(app); break;
    case ECRAN_JEU:    dessiner_jeu(app); break;
    case ECRAN_PAUSE:  dessiner_jeu(app); dessiner_pause(); break;
    case ECRAN_FIN:    dessiner_jeu(app); dessiner_fin(app); break;
    }
    dessiner_pied(app);
    EndDrawing();
}

// --- Captures d'écran pour le sujet (option cachée --capture) -------------------

static void capture(App *app, const char *ecran, const char *fichier)
{
    if (strcmp(ecran, "jeu") == 0 || strcmp(ecran, "fin") == 0) {
        Partie *p = partie_creer(CARTES[0].chemin, 2026u);
        if (p != NULL && p->serpent != NULL) {
            lancer_partie(app, p, 1);
            int pommes_voulues = strcmp(ecran, "jeu") == 0 ? 14 : 1000;
            for (int i = 0; i < 20000 && p->etat == PARTIE_EN_COURS
                            && p->pommes < pommes_voulues; i++) {
                demo_choisir(p);
                partie_etape(p);
            }
            app->demo = 0;
            if (strcmp(ecran, "fin") == 0) {
                p->etat = PARTIE_PERDUE;
                app->nb_scores = 0;
                fin_de_partie(app);
                snprintf(app->nom, sizeof app->nom, "Alice");
            }
        }
    } else if (strcmp(ecran, "pause") == 0) {
        Partie *p = partie_creer(CARTES[1].chemin, 7u);
        if (p != NULL && p->serpent != NULL) {
            lancer_partie(app, p, 0);
            for (int i = 0; i < 12; i++) partie_etape(p);
            app->ecran = ECRAN_PAUSE;
        }
    }
    for (int i = 0; i < 3; i++) {
        dessiner(app);
    }
    TakeScreenshot(fichier);
}

int main(int argc, char *argv[])
{
    SetTraceLogLevel(LOG_WARNING);
    InitWindow(LARGEUR_FENETRE, HAUTEUR_FENETRE, "Serpent - projet de langage C");
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);

    static App app;                  // static : grosse structure, mise à zéro
    app.ecran = ECRAN_MENU;
    app.rang = -1;
    app.surligne = -1;
    for (int i = 0; i < NB_CARTES; i++) {
        app.apercus[i] = carte_charger(CARTES[i].chemin);
    }

    if (argc == 4 && strcmp(argv[1], "--capture") == 0) {
        capture(&app, argv[2], argv[3]);
    } else {
        int quitter = 0;
        while (!quitter && !WindowShouldClose()) {
            if (app.ecran == ECRAN_MENU && IsKeyPressed(KEY_ESCAPE)) {
                quitter = 1;
            }
            mettre_a_jour(&app);
            dessiner(&app);
        }
    }

    partie_liberer(app.partie);
    for (int i = 0; i < NB_CARTES; i++) {
        carte_liberer(app.apercus[i]);
    }
    CloseWindow();
    return 0;
}
