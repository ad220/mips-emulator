#include "conversionHexa.h"

void charFileToHexaFile(char fichierIN[], char fichierOUT[]);

int main(int argc, char* argv[]) {
    if (argc==3) {
        charFileToHexaFile(argv[1],argv[2]);
    } else {printf("error");}
    return(0);
}




int lireFichier(char nomFichier[], progIN* pgm) {
    // lit le fichier "nomFichier" et l'écrit sous forme de tableau de chaînes de caractères dans pgm.
    FILE * fichier;
    fichier=fopen(nomFichier,"r");
    int n=0;
    char line[64];
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
    while (ligne[n]!='\0' && ligne[n]!='#') {
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

int argToInt(char R[]) {
    // convertit un argument sous forme de chaîne de caractère en entier
    int i=0,n=0,p=0,s=1;
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
    endArgToInt: return(s*n);
}



int hash(char op[9]) {
    // bijection entre une chaine de caractère alphabétique et un entier.
    int i=0,n=0;
    while (op[i]!=NULL) {n+=(op[i]-65)*pow(26,i); i++;}
    return(n);
}

int instrToHexa(instruction* I) {
    // traduit une instruction en entier correspondant à sa forme hexadécimale / binaire
    unsigned n;
    if (nbArgs(I)==4) { //instructions avec 4 arguments (en comptant l'op code)
        switch (hash((*I).args[0])) {
            case 2106: //ADD (special)
                n=32+pow(2,11)*argToInt((*I).args[1])+pow(2,21)*argToInt((*I).args[2])+pow(2,16)*argToInt((*I).args[3]);
                break;
            case 142714: //ADDI
                n=pow(2,29)+pow(2,16)*argToInt((*I).args[1])+pow(2,21)*argToInt((*I).args[2])+argToInt((*I).args[3]);
                break;
            case 2366: //AND (special)
                n=36+pow(2,11)*argToInt((*I).args[1])+pow(2,21)*argToInt((*I).args[2])+pow(2,16)*argToInt((*I).args[3]);
                break;
            case 10921: //BEQ
                n=pow(2,28)+pow(2,21)*argToInt((*I).args[1])+pow(2,16)*argToInt((*I).args[2])+argToInt((*I).args[3]);
                break;
            case 3043: //BNE
                n=5*pow(2,26)+pow(2,21)*argToInt((*I).args[1])+pow(2,16)*argToInt((*I).args[2])+argToInt((*I).args[3]);
                break;
            case 583: //LW
                n=35*pow(2,26)+pow(2,16)*argToInt((*I).args[1])+argToInt((*I).args[2])+pow(2,21)*argToInt((*I).args[3]);
                break;
            case 456: //OR (special)
                n=37+pow(2,11)*argToInt((*I).args[1])+pow(2,21)*argToInt((*I).args[2])+pow(2,16)*argToInt((*I).args[3]);
                break;
            case 312017: //ROTR (special)
                n=2+pow(2,21)+pow(2,11)*argToInt((*I).args[1])+pow(2,16)*argToInt((*I).args[2])+pow(2,6)*argToInt((*I).args[3]);
                break;
            case 7740: //SLL (special)
                n=pow(2,11)*argToInt((*I).args[1])+pow(2,16)*argToInt((*I).args[2])+pow(2,6)*argToInt((*I).args[3]);
                break;
            case 13148: //SLT (special)
                n=42+pow(2,11)*argToInt((*I).args[1])+pow(2,21)*argToInt((*I).args[2])+pow(2,16)*argToInt((*I).args[3]);
                break;
            case 7896: //SRL (special)
                n=2+pow(2,11)*argToInt((*I).args[1])+pow(2,16)*argToInt((*I).args[2])+pow(2,6)*argToInt((*I).args[3]);
                break;
            case 1214: //SUB (special)
                n=34+pow(2,11)*argToInt((*I).args[1])+pow(2,21)*argToInt((*I).args[2])+pow(2,16)*argToInt((*I).args[3]);
                break;
            case 590: //SW
                n=43*pow(2,26)+pow(2,16)*argToInt((*I).args[1])+argToInt((*I).args[2])+pow(2,21)*argToInt((*I).args[3]);
                break;
            case 11879: //XOR (special)
                n=38+pow(2,11)*argToInt((*I).args[1])+pow(2,21)*argToInt((*I).args[2])+pow(2,16)*argToInt((*I).args[3]);
                break;
            
            default:
                n=pow(2,32)-1;
                break;
        }
    } else {
        switch (hash((*I).args[0])) {
            case 452401: //BGTZ
                n=7*pow(2,26)+pow(2,21)*argToInt((*I).args[1])+argToInt((*I).args[2]);
                break;
            case 442391: //BLEZ
                n=3*pow(2,27)+pow(2,21)*argToInt((*I).args[1])+argToInt((*I).args[2]);
                break;
            case 14407: //DIV (special)
                n=26+pow(2,21)*argToInt((*I).args[1])+pow(2,16)*argToInt((*I).args[2]);
                break;
            case 9: //J
                n=pow(2,27)+argToInt((*I).args[1]);
                break;
            case 7445: //JAL
                n=pow(2,27)+argToInt((*I).args[1]);
                break;
            case 451: //JR (special)
                n=8+argToInt((*I).args[1])*pow(2,21);
                break;
            case 5939: //LUI
                n=15*pow(2,26)+pow(2,16)*argToInt((*I).args[1])+argToInt((*I).args[2]);
                break;
            case 145482: //MFHI (special)
                n=16+pow(2,11)*argToInt((*I).args[1]);
                break;
            case 253642: //MFLO (special)
                n=18+pow(2,11)*argToInt((*I).args[1]);
                break;
            case 341912: //MULT (special)
                n=24+pow(2,21)*argToInt((*I).args[1])+pow(2,16)*argToInt((*I).args[2]);
                break;
            case 10517: //NOP (special)
                n=0;
                break;
            case -2147483648: //SYSCALL (special)
                n=12; //opérande code ?
                break;

            default:
                n=pow(2,32)-1;
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
    unsigned h;
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
    unsigned h;
    for (int i=0; i<n; i++) {
        initInstr(&I);
        sepLigneToInstr((*pgmIN).line[i],&I);
        (*pgmOUT).line[i]=instrToHexa(&I);
    }
}

/* void charPgmToInstrPgm(progIN* pgmIN, progInstr* pgmOUT, int n) {
    instruction I;
    unsigned h;
    for (int i=0; i<n; i++) {
        initInstr(&I);
        sepLigneToInstr((*pgmIN).line[i],&I);
        (*pgmOUT).line[i]=I;
    }
} */