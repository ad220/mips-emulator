#include "header.h"
#include "memoire.h"
#include "registre.h"
#include "conversionHexa.h"

void pgmDansMemoire(progOUT* pgm, memoire* mem, int lPgm) {
    int i=0,j,n;
    while (i<lPgm) {
        n=allerPage(mem,i);
        j=0;
        while (j<64 && i<lPgm) {
            ecrire(mem,n+j,(*pgm).line[i]);
            i++,j++;
        }
    }
}

int main(int argc, char* argv[]) {
    memoire *M = (memoire* ) malloc(sizeof(memoire));
    page* p=M;
    (*p).suivant=p;
    (*p).n=0;

    registre reg;
    initProc(&reg);

    progIN pgmHexa;
    int lPgm=lireFichier(argv[1],&pgmHexa);
    progOUT pgmChar;
    charPgmToHexaPgm(&pgmHexa,&pgmChar,lPgm);
    pgmDansMemoire(&pgmHexa,&M,lPgm);
}

