/*
 * a_faire.c — voir a_faire.h.
 *
 * FICHIER FOURNI : ne pas le modifier.
 */
#include "a_faire.h"

static int etape_signalee = 0;
static const char *fonction_signalee = "";

void a_faire_signaler(int etape, const char *fonction)
{
    if (etape_signalee == 0 || etape < etape_signalee) {
        etape_signalee = etape;
        fonction_signalee = fonction;
    }
}

int a_faire_etape(void)
{
    return etape_signalee;
}

const char *a_faire_fonction(void)
{
    return fonction_signalee;
}

void a_faire_effacer(void)
{
    etape_signalee = 0;
    fonction_signalee = "";
}
