#include "header.h"

typedef struct {
    int rgd[32];
    int PC;
    int HI;
    int LO;
} registre;

void initProc(registre* R);
int lireReg(registre* R, int i);
void ecrireReg(registre* R, int i, int x);