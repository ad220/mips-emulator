#include "../include/execution.h"

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
    lPgm*=4;
    k=lPgm%256;
    page* x=allerPage(M,lPgm);
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
    } while (x->n!=0);
}

int strComp(char C1[], char C2[]) {
    int i=0;
    while (C1[i]==C2[i] && C1[i]!='\0' && C2[i]!='\0') {i++;}
    if (C1[i]==C2[i]) {i=1;}
    else {i=0;}
    return (i);
}

int main(int argc, char* argv[]) {
    printf("\n==== Projet MIPS - Arthur DAVID & Alexandre BARRAL ====\n\n");
    page* p = (page* ) malloc(sizeof(page));
    memoire M=p;
    (*p).suivant=p;
    (*p).n=0;

    registre reg;
    initProc(&reg);
    
    int lPgm;
    if (argc > 1) { // mode non-interactif
        progIN pgmChar;
        lPgm=lireFichier(argv[1],&pgmChar);
        progOUT pgmHexa;
        charPgmToHexaPgm(&pgmChar,&pgmHexa,lPgm);
        printf("Traduction du programme assembleur en instructions hexadecimales :\n");
        for (int i=0; i<lPgm; i++) {
            printf("0x%04X %08X (%s)\n",4*i,pgmHexa.line[i],pgmChar.line[i]);
        }
        printf("\n");
        pgmDansMemoire(&pgmHexa,M,lPgm);

        int pas=(argc>2 && strComp("--pas",argv[2]));
    
        while(reg.PC!=lPgm*4) {
            if (pas) {
                getchar();
                printf("\n=== Etat intermediaire ===\n");
                affichage(&reg,M,lPgm);
            }
            printf("\n=== Instruction %08X (%s) ===\n\n",lire(M,reg.PC),pgmChar.line[reg.PC/4]);
            execLigne(M,&reg,lire(M,reg.PC));
            reg.rgd[0]=0;
            reg.PC+=4;
        }
    } else { // mode interactif
        lPgm=0;
        instruction I;
        unsigned instrHexa;
        char input[TAILLE_LIGNE_MAX];
        printf("Mode interactif ('exit' pour fermer le programme):\n> ");
        scanf("%[^\n]",&input);
        do {
            initInstr(&I);
            sepLigneToInstr(input,&I);
            instrHexa=instrToHexa(&I);
            printf("\n=== Instruction %08X ===\n\n",instrHexa);
            execLigne(M,&reg,instrHexa);
            printf("\n=== Etat intermediaire ===\n");
            affichage(&reg,M,lPgm);
            for (int i=0; input[i]!='\0'; i++) {input[i]=NULL;}
            printf("> ");
            scanf(" %[^\n]",&input);
        } while (strComp(input,"exit")!=1);
    }
    printf("\n=== Etat final ===\n");
    affichage(&reg,M,lPgm);
    return(0);
}