#include "memoire.h"

int lire(memoire M, unsigned addr) {
    page* pageAddr=allerPage(M,addr);
    return((*pageAddr).mot[addr/4]);
}

void ecrire(memoire M, unsigned addr, int x) {
    page* pageAddr=allerPage(M,addr);
    (*pageAddr).mot[addr%64]=x;
}

page* allerPage(memoire M, unsigned addr) {
    page *i=M, *j;
    int x=addr/256;
    do {
        j=i;
        i=(*i).suivant;
    } while (i!=M && (*i).n<x);
    if ((*i).n!=x) {
        page *p = (page* ) malloc(sizeof(page));
        (*p).n=x;
        (*j).suivant=p;
        (*p).suivant=i;
        i=p;
    }
    return(i);
}