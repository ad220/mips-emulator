#include "header.h"

#define TAILLE_PAGE 64 // 64 entiers --> 256 octets à adresser, sur un octet donc

typedef struct page page;
struct page {
    int mot[TAILLE_PAGE];
    int n;
    page* suivant;
};

typedef page* memoire;


int lire(memoire* M, unsigned addr);
void ecrire(memoire* M, unsigned addr, int x);
page* allerPage(memoire* M, unsigned addr);