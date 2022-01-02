#include "memoire.h"

typedef struct {
    int rgd[32];
    unsigned PC;
    int HI;
    int LO;
} registre;

void initProc(registre* R); //met tous les registres à 0
int lireReg(registre* R, int i); //renvoie la valeur du registre i
void ecrireReg(registre* R, int i, int x); //écrit la valeur x dans le registre i