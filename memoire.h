#include "conversionHexa.h"

typedef struct page page;
struct page {
    int mot[TAILLE_PAGE]; //tableau de 256 octets soit 64 entiers
    int n; //index de la page
    page* suivant; //adresse de la page suivante
};

typedef page* memoire;


int lire(memoire M, unsigned addr); //renvoie la valeur de la donnée stockée à l'adresse spécifiée
void ecrire(memoire M, unsigned addr, int x); //écrit la valeur de x à l'adresse spécifiée
page* allerPage(memoire M, unsigned addr); //renvoie l'adresse de la page dans laquelle est comprise l'adresse spécifiée et la crée si la page d'index adresse/256 n'existe pas