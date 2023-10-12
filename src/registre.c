#include "../include/registre.h"

void initProc(registre* R) {
    for (int i=0; i<32; i++) {(*R).rgd[i]=0;}
    (*R).HI=0;
    (*R).LO=0;
    (*R).PC=0;
}

int lireReg(registre* R, int i) {
    int n;
    switch (i) {
    case 32:
        n=(*R).HI;
        break;
    case 33:
        n=(*R).LO;
        break;
    case 34:
        n=(*R).PC;    
    default:
        n=(*R).rgd[i];
        break;
    }
    return(n);
}

void ecrireReg(registre* R, int i, int x) {
    switch (i) {
    case 32:
        (*R).HI=x;
        break;
    case 33:
        (*R).LO=x;
        break;
    case 34:
        (*R).PC=x;    
    default:
        (*R).rgd[i]=x;
        break;
    }
}