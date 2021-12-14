#include "conversionHexa.h"

typedef struct page page;
struct page {
    int mot[TAILLE_PAGE];
    int n;
    page* suivant;
};

typedef page* memoire;


int lire(memoire M, unsigned addr);
void ecrire(memoire M, unsigned addr, int x);
page* allerPage(memoire M, unsigned addr);