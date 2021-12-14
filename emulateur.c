#include "execution.h"

void affichage(registre *R, memoire M,int lPgm) {
    printf("Etat registres :\n");
    unsigned k;
    for (int i=0; i<4; i++) {
        for (int j=0; j<8; j++) {
            k=j+i*8;
            printf(" R%02d:% -11d ",k,R->rgd[k]);
        }
        printf("\n");
    }
    printf("  PC:0x%08X",R->PC);
    printf("    HI:% -11d",R->HI);
    printf("   LO:% -11d\n",R->LO);

    printf("\nEtat memoire :\n");
    k=lPgm*4;
    page* x=allerPage(M,lPgm);
    page* y=x;
    int i;
    do {
        i=0;
        printf("Page %d :\n",x->n);
        while(k<256) {
            printf("0x%02X:% -11d ",k,x->mot[k/4]);
            if (++i==8) {i=0; printf("\n");}
            k+=4;
        }
        k=0;
        x=x->suivant;
        printf("\n");
    } while (x!=y);
    printf("\n");
}


int main(int argc, char* argv[]) {
    page* p = (page* ) malloc(sizeof(page));
    memoire M=p;
    (*p).suivant=p;
    (*p).n=0;

    registre reg;
    initProc(&reg);

    progIN pgmChar;
    int lPgm=lireFichier("test1.mips",&pgmChar);
    progOUT pgmHexa;
    charPgmToHexaPgm(&pgmChar,&pgmHexa,lPgm);
    pgmDansMemoire(&pgmHexa,M,lPgm);
    
    while(reg.PC!=lPgm*4) {
        execLigne(M,&reg);
        reg.rgd[0]=0;
        // affichage(&reg,M,lPgm);
        reg.PC+=4;
    }
    affichage(&reg,M,lPgm);
    return(0);
}