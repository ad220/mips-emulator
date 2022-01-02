#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NB_LIGNE_MAX 1024
#define TAILLE_LIGNE_MAX 256
#define TAILLE_PAGE 64 // 64 entiers --> 256 octets à adresser, sur un octet donc

typedef struct {
    char line[NB_LIGNE_MAX][TAILLE_LIGNE_MAX];
} progIN;

typedef struct {
    unsigned line[NB_LIGNE_MAX];
} progOUT;

typedef struct {
    char args[4][9];
} instruction;

typedef struct {
    instruction line[NB_LIGNE_MAX];
} progInstr;

int lireFichier(char nomFichier[], progIN* pgm);
void ecrireFichier(char nomFichier[], progOUT* pgm, int n);
void charPgmToHexaPgm(progIN* pgmIN, progOUT* pgmOUT, int n);

int lireFichier(char nomFichier[], progIN* pgm);
void ecrireFichier(char nomFichier[], progOUT* pgm, int n);
void charPgmToHexaPgm(progIN* pgmIN, progOUT* pgmOUT, int n);

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

typedef struct {
    int rgd[32];
    unsigned PC;
    int HI;
    int LO;
} registre;

void initProc(registre* R); //met tous les registres à 0
int lireReg(registre* R, int i); //renvoie la valeur du registre i
void ecrireReg(registre* R, int i, int x); //écrit la valeur x dans le registre i

void execLigne(memoire M, registre* R);
void pgmDansMemoire(progOUT* pgm, memoire mem, int lPgm);

unsigned PWR2[32]={1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536,131072,262144,524288,1048576,2097152,4194304,8388608,16777216,33554432,67108864,134217728,268435456,536870912,1073741824,2147483648};

/* int main(int argc, char* argv[]) {
    if (argc==3) {
        charFileToHexaFile(argv[1],argv[2]);
    } else {printf("error");}
    // charFileToHexaFile("in4convRgd.txt","out4convRgd.txt");
    return(0);
} */


int lireFichier(char nomFichier[], progIN* pgm) {
    // lit le fichier "nomFichier" et l'écrit sous forme de tableau de chaînes de caractères dans pgm.
    FILE * fichier;
    fichier=fopen(nomFichier,"r");
    int n=0;
    while (!feof(fichier)) {
        fscanf(fichier, "%[^\n]\n",&((*pgm).line[n]));
        if ((*pgm).line[n][0]!='#') {n++;}
    }
    fclose(fichier);
    return(n);
}

void ecrireFichier(char nomFichier[], progOUT* pgm, int n) {
    // lit le tableau d'entiers pgm et l'écrit sous forme hexadécimale dans le fichier "nomFichier"
    FILE * fichier;
    fichier=fopen(nomFichier,"w");
    int i=0;
    while (i<n) {
        fprintf(fichier, "%08X\n",(*pgm).line[i++]);
    }
    fclose(fichier);
}

void initInstr(instruction* I) {
    // met l'instruction I à 0
    for (int i=0; i<4; i++) {
        for (int j=0; j<9; j++){
            (*I).args[i][j]=NULL;
        }
    }
}

void sepLigneToInstr(char ligne[], instruction* I) {
    // sépare une instruction assembleur sous forme de chaîne de caractère en tableau d'arguments.
    int n=0,i=0,j=0;
    while (ligne[n]!='\0' && ligne[n]!='#' && ligne[n]!='\r') { // condition 'r' nécessaire sous UNIX : caractère retour à la ligne
        (*I).args[i][j]=ligne[n];
        j++;n++;
        if(ligne[n]==')') {n++;}
        if (ligne[n]==' ' || ligne[n]==',' || ligne[n]=='(') {
            do {
                n++;
            } while (ligne[n]==' ' || ligne[n]==',');
            i++; j=0;
        }
    }
    ligne[n]='\0';
}

int nbArgs(instruction* I) {
    // renvoie le nombre d'arguments d'une instruction
    int i=0;
    while (i<4 && (*I).args[i][0]!=NULL) {i++;}
    return (i);
}



int hashRgd(char R[]) {
    // bijection entre une chaîne de caractère alphanumérique minuscule et un entier.
    int i=0,n=0;
    while (R[++i]!=NULL) {
        n+=(R[i]-48)*pow(75,i-1);
    }
    return (n);
}

int idRgd(char R[]) {
    // identifie le n° de registre à partir du résultat de la bijection hashRgd.
    int convRegistres[32]={26953424,5149,70,145,49,124,199,274,68,143,218,293,368,443,518,593,67,142,217,292,367,442,517,592,668,743,59,134,4855,4867,4854,3741};
    // convRegistres = hash({"$zero","$at","$v0","$v1","$a0","$a1","$a2","$a3","$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7","$s0","$s1","$s2","$s3","$s4","$s5","$s6","$s7","$t8","$t9","$k0","$k1","$gp","$sp","$fp","$ra"})
    int i=0,n=hashRgd(R);
    while (convRegistres[i]!=n && i<32) {i++;}
    return(i);
}

unsigned argToInt(char R[]) {
    // convertit un argument sous forme de chaîne de caractère en entier
    int i=0,p=0,s=1;
    unsigned n=0;
    if (R[0]=='-') {i=1; p=1; s=-1;}
    else if (R[0]=='$') {
        n=idRgd(R);
        if (n!=32) {
            goto endArgToInt;
        } else {
            i=1; p=1; n=0;
        }
    }
    while (R[i]!=NULL) {i++;}
    int j=i;
    i-=p;
    while (i!=0) {
        n+=(R[j-i]-48)*pow(10,i-1);
        i--;
    }
    if (s==-1) {
        unsigned short k=-n;
        n=k;
    }
    endArgToInt: return(n);
}



int hash(char op[9]) {
    // bijection entre une chaine de caractère alphabétique et un entier.
    int i=0,n=0;
    while (op[i]!=NULL) {n+=(op[i]-65)*pow(26,i); i++;}
    return(n);
}

int instrToHexa(instruction* I) { // fonctionne pas avec les entiers négatifs
    // traduit une instruction en entier correspondant à sa forme hexadécimale / binaire
    unsigned n;
    if (nbArgs(I)==4) { //instructions avec 4 arguments (en comptant l'op code)
        switch (hash((*I).args[0])) {
            case 2106: //ADD (special)
                n=32+PWR2[11]*argToInt((*I).args[1])+PWR2[21]*argToInt((*I).args[2])+PWR2[16]*argToInt((*I).args[3]);
                break;
            case 142714: //ADDI
                n=PWR2[29]+PWR2[16]*argToInt((*I).args[1])+PWR2[21]*argToInt((*I).args[2])+argToInt((*I).args[3]);
                break;
            case 2366: //AND (special)
                n=36+PWR2[11]*argToInt((*I).args[1])+PWR2[21]*argToInt((*I).args[2])+PWR2[16]*argToInt((*I).args[3]);
                break;
            case 10921: //BEQ
                n=PWR2[28]+PWR2[21]*argToInt((*I).args[1])+PWR2[16]*argToInt((*I).args[2])+argToInt((*I).args[3]);
                break;
            case 3043: //BNE
                n=5*PWR2[26]+PWR2[21]*argToInt((*I).args[1])+PWR2[16]*argToInt((*I).args[2])+argToInt((*I).args[3]);
                break;
            case 583: //LW
                n=35*PWR2[26]+PWR2[16]*argToInt((*I).args[1])+argToInt((*I).args[2])+PWR2[21]*argToInt((*I).args[3]);
                break;
            case 456: //OR (special)
                n=37+PWR2[11]*argToInt((*I).args[1])+PWR2[21]*argToInt((*I).args[2])+PWR2[16]*argToInt((*I).args[3]);
                break;
            case 312017: //ROTR (special)
                n=2+PWR2[21]+PWR2[11]*argToInt((*I).args[1])+PWR2[16]*argToInt((*I).args[2])+PWR2[6]*argToInt((*I).args[3]);
                break;
            case 7740: //SLL (special)
                n=PWR2[11]*argToInt((*I).args[1])+PWR2[16]*argToInt((*I).args[2])+PWR2[6]*argToInt((*I).args[3]);
                break;
            case 13148: //SLT (special)
                n=42+PWR2[11]*argToInt((*I).args[1])+PWR2[21]*argToInt((*I).args[2])+PWR2[16]*argToInt((*I).args[3]);
                break;
            case 7896: //SRL (special)
                n=2+PWR2[11]*argToInt((*I).args[1])+PWR2[16]*argToInt((*I).args[2])+PWR2[6]*argToInt((*I).args[3]);
                break;
            case 1214: //SUB (special)
                n=34+PWR2[11]*argToInt((*I).args[1])+PWR2[21]*argToInt((*I).args[2])+PWR2[16]*argToInt((*I).args[3]);
                break;
            case 590: //SW
                n=43*PWR2[26]+PWR2[16]*argToInt((*I).args[1])+argToInt((*I).args[2])+PWR2[21]*argToInt((*I).args[3]);
                break;
            case 11879: //XOR (special)
                n=38+PWR2[11]*argToInt((*I).args[1])+PWR2[21]*argToInt((*I).args[2])+PWR2[16]*argToInt((*I).args[3]);
                break;
            
            default:
                n=2*PWR2[31]-1;
                break;
        }
    } else {
        switch (hash((*I).args[0])) {
            case 452401: //BGTZ
                n=7*PWR2[26]+PWR2[21]*argToInt((*I).args[1])+argToInt((*I).args[2]);
                break;
            case 442391: //BLEZ
                n=3*PWR2[27]+PWR2[21]*argToInt((*I).args[1])+argToInt((*I).args[2]);
                break;
            case 14407: //DIV (special)
                n=26+PWR2[21]*argToInt((*I).args[1])+PWR2[16]*argToInt((*I).args[2]);
                break;
            case 9: //J
                n=PWR2[27]+argToInt((*I).args[1]);
                break;
            case 7445: //JAL
                n=3*PWR2[26]+argToInt((*I).args[1]);
                break;
            case 451: //JR (special)
                n=8+argToInt((*I).args[1])*PWR2[21];
                break;
            case 5939: //LUI
                n=15*PWR2[26]+PWR2[16]*argToInt((*I).args[1])+argToInt((*I).args[2]);
                break;
            case 145482: //MFHI (special)
                n=16+PWR2[11]*argToInt((*I).args[1]);
                break;
            case 253642: //MFLO (special)
                n=18+PWR2[11]*argToInt((*I).args[1]);
                break;
            case 341912: //MULT (special)
                n=24+PWR2[21]*argToInt((*I).args[1])+PWR2[16]*argToInt((*I).args[2]);
                break;
            case 10517: //NOP (special)
                n=0;
                break;
            case -2147483648: //SYSCALL (special)
                n=12; //opérande code ?
                break;

            default:
                n=2*PWR2[31]-1;
                break;
        }   
    }
    return(n);
}

void charFileToHexaFile(char fichierIN[], char fichierOUT[]) {
    // traduit un fichier d'instructions MIPS en un fichier d'instruction MIPS traduites en forme hexadécimale
    progIN pgmIN;
    progOUT pgmOUT;
    int n=lireFichier(fichierIN,&pgmIN);
    instruction I;
    for (int i=0; i<n; i++) {
        initInstr(&I);
        sepLigneToInstr(pgmIN.line[i],&I);
        pgmOUT.line[i]=instrToHexa(&I);
    }
    ecrireFichier(fichierOUT,&pgmOUT,n);
}

void charPgmToHexaPgm(progIN* pgmIN, progOUT* pgmOUT, int n) {
    // idem mais sans l'écrire dans un fichier
    instruction I;
    for (int i=0; i<n; i++) {
        initInstr(&I);
        sepLigneToInstr((*pgmIN).line[i],&I);
        (*pgmOUT).line[i]=instrToHexa(&I);
    }
}

int lire(memoire M, unsigned addr) {
    page* pageAddr=allerPage(M,addr);
    return((*pageAddr).mot[(addr%256)/4]);
}

void ecrire(memoire M, unsigned addr, int x) {
    page* pageAddr=allerPage(M,addr);
    (*pageAddr).mot[(addr%256)/4]=x;
}

page* allerPage(memoire M, unsigned addr) {
    page *i=M, *j;
    int x=addr/256;
    if (x!=0) {
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
    }
    return(i);
}

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

unsigned PWR2_2[32]={1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536,131072,262144,524288,1048576,2097152,4194304,8388608,16777216,33554432,67108864,134217728,268435456,536870912,1073741824,2147483648};

int arg(unsigned ligne, int bitSup, int bitInf) {
    return((ligne%PWR2_2[bitSup+1])/PWR2_2[bitInf]);
}

short argS(unsigned ligne, int bitSup, int bitInf) {
    return((ligne%PWR2_2[bitSup+1])/PWR2_2[bitInf]);
}

void pgmDansMemoire(progOUT* pgm, memoire mem, int lPgm) {
    int i=0,j;
    page* p;
    while (i<lPgm) {
        p=allerPage(mem,i*4);
        j=0;
        while (j<64 && i<lPgm) {
            (*p).mot[j]=(*pgm).line[i];
            i++,j++;
        }
    }
}

void execLigne(memoire M, registre* R) {
    unsigned L=lire(M,R->PC);
    int opcode=L/PWR2_2[26];
    if (opcode!=0) { // pas SPECIAL
        switch (opcode) {
            case 8: //ADDI
                R->rgd[arg(L,20,16)]=R->rgd[arg(L,25,21)]+argS(L,15,0); //gérer overflow
                break;
            case 4: //BEQ
                if (R->rgd[arg(L,20,16)]==R->rgd[arg(L,25,21)]) { //branch delay slot ??
                    R->PC+=4*argS(L,15,0);
                }
                break;
            case 7: //BGTZ
                if (R->rgd[arg(L,25,21)]>R->rgd[0]) { //branch delay slot ??
                    R->PC+=4*argS(L,15,0);
                }
                break;
            case 6: //BLEZ
                if (R->rgd[arg(L,25,21)]<=R->rgd[0]) { //branch delay slot ??
                    R->PC+=4*argS(L,15,0);
                }
                break;
            case 5: //BNE
                if (R->rgd[arg(L,20,16)]!=R->rgd[arg(L,25,21)]) { //branch delay slot ??
                    R->PC+=4*argS(L,15,0);
                }
                break;
            case 2: //J
                R->PC=4*arg(L,25,0)+(((R->PC+4)>>28)<<28)-4; //branch delay slot ?
                break;
            case 3: //JAL
                R->rgd[31]=R->PC+4; //branch delay slot ?? --> return adress après
                R->PC=4*arg(L,25,0)+(((R->PC+4)>>28)<<28)-4;
                break;
            case 15: //LUI
                R->rgd[arg(L,20,16)]=arg(L,16,0)<<16;
                break;
            case 35: ;//LW
                int lwOf7=argS(L,15,0);
                unsigned lwBase=R->rgd[arg(L,25,21)];
                R->rgd[arg(L,20,16)]=lire(M,lwBase+lwOf7);
                break;
            case 43: //SW
                ecrire(M,R->rgd[arg(L,25,21)]+argS(L,15,0),R->rgd[arg(L,20,16)]);
                break;
            
            default:
                break;
        }
    } else { // SPECIAL
        opcode=L%PWR2_2[6];
        switch (opcode) {
            case 32: //ADD
                R->rgd[arg(L,15,11)]=R->rgd[arg(L,25,21)]+R->rgd[arg(L,20,16)]; // gérer overflow
                break;
            case 36: //AND
                R->rgd[arg(L,15,11)]=R->rgd[arg(L,25,21)]&R->rgd[arg(L,20,16)];
                break;
            case 26: //DIV gérer /0 ?
                R->LO=R->rgd[arg(L,25,21)]/R->rgd[arg(L,20,16)];
                R->HI=R->rgd[arg(L,25,21)]%R->rgd[arg(L,20,16)];
                break;
            case 8: //JR
                R->PC=R->rgd[arg(L,25,21)]-4;
                break;
            case 16: //MFHI
                R->rgd[arg(L,15,11)]=R->HI;
                break;
            case 18: //MFLO
                R->rgd[arg(L,15,11)]=R->LO;
                break;
            case 24: ;//MULT
                long long unsigned n=R->rgd[arg(L,25,21)]*R->rgd[arg(L,20,16)];
                R->HI=n>>32;
                R->LO=(n<<32)>>32;
                break;
            case 37: //OR
                R->rgd[arg(L,15,11)]=R->rgd[arg(L,25,21)] | R->rgd[arg(L,20,16)];
                break;
            case 2:
                if (arg(L,21,21)) { //ROTR
                    int saSRL=arg(L,10,6),rtSRL=arg(L,20,16);
                    R->rgd[arg(L,15,11)]=(R->rgd[rtSRL]<<(32-saSRL))+(R->rgd[rtSRL]>>saSRL);
                } else { //SRL
                    R->rgd[arg(L,15,11)]=R->rgd[arg(L,20,16)]>>arg(L,10,6);
                }
                break;
            case 0: //SLL & NOP
                if (L!=0) {
                    R->rgd[arg(L,15,11)]=R->rgd[arg(L,20,16)]<<arg(L,10,6);
                }
                break;
            case 42: //SLT
                R->rgd[arg(L,15,11)]=R->rgd[arg(L,25,21)]<R->rgd[arg(L,20,16)];
                break;
            case 34: //SUB
                R->rgd[arg(L,15,11)]=R->rgd[arg(L,25,21)]-R->rgd[arg(L,20,16)]; // gérer overflow
                break;
            case 38: //XOR
                R->rgd[arg(L,15,11)]=R->rgd[arg(L,25,21)]^R->rgd[arg(L,20,16)];
                break;
            case 12: //SYSCALL op code ?
                break;

            default:
                break;
        }
    }
}

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

int compStr(char C1[], char C2[]) {
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

    progIN pgmChar;
    int lPgm=lireFichier(argv[1],&pgmChar);
    progOUT pgmHexa;
    charPgmToHexaPgm(&pgmChar,&pgmHexa,lPgm);
    printf("Traduction du programme assembleur en instructions hexadecimales :\n");
    for (int i=0; i<lPgm; i++) {
        printf("0x%04X %08X (%s)\n",4*i,pgmHexa.line[i],pgmChar.line[i]);
    }
    printf("\n");
    pgmDansMemoire(&pgmHexa,M,lPgm);
    
    int pas=1;
    while(reg.PC!=lPgm*4) {
        if (pas) {
            getchar();
            printf("\n=== Etat intermediaire ===\n");
            affichage(&reg,M,lPgm);
        }
        printf("\n=== Instruction %08X (%s) ===\n\n",lire(M,reg.PC),pgmChar.line[reg.PC/4]);
        execLigne(M,&reg);
        reg.rgd[0]=0;
        reg.PC+=4;
    }
    printf("\n=== Etat final ===\n");
    affichage(&reg,M,lPgm);
    return(0);
}