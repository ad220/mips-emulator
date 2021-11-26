#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    char args[4][9];
} instruction;

void clear(instruction* I) {
    for (int i=0; i<4; i++) {
        for (int j=0; j<9; j++){
            (*I).args[i][j]=NULL;
        }
    }
}

void sepLine(char ligne[], instruction* I) { //ajouter support arg1(arg2)
    int n=0,i=0,j=0;
    while (ligne[n]!='\0') {
        (*I).args[i][j]=ligne[n];
        j++;n++;
        if (ligne[n]==',') {n+=2,i++,j=0;}
        if (ligne[n]==' ') {n++,i++,j=0;}
    }
}

int nbArgs(instruction* I) {
    int i=0;
    while (i<4 && (*I).args[i][0]!=NULL) {i++;}
    return (i);
}

int strToInt(char R[]) {
    int i=0,n=0;
    if (R[0]=='$') {i=1;}
    while (R[i]!=NULL) {
        n+=(R[i]-48)*pow(10,i);
        i++;
    }
    return(n);
}

int instrToHexa(instruction* I) {
    int n;
    if (nbArgs(I)==3) { //instructions avec 3 arguments
        switch (hash((*I).args[0])) {
            case 2106: //ADD (special)
                n=32+pow(2,11)*strToInt((*I).args[1])+pow(2,21)*strToInt((*I).args[2])+pow(2,16)*strToInt((*I).args[3]);
                break;
            case 142714: //ADDI
                n=pow(2,29)+pow(2,16)*strToInt((*I).args[1])+pow(2,21)*strToInt((*I).args[2])+strToInt((*I).args[3]);
                break;
            case 2366: //AND (special)
                n=36+pow(2,11)*strToInt((*I).args[1])+pow(2,21)*strToInt((*I).args[2])+pow(2,16)*strToInt((*I).args[3]);
                break;
            case 10921: //BEQ
                n=pow(2,28)+pow(2,21)*strToInt((*I).args[1])+pow(2,16)*strToInt((*I).args[2])+strToInt((*I).args[3]);
                break;
            case 3043: //BNE
                n=5*pow(2,26)+pow(2,21)*strToInt((*I).args[1])+pow(2,16)*strToInt((*I).args[2])+strToInt((*I).args[3]);
                break;
            case 583: //LW
                n=35*pow(2,26)+pow(2,16)*strToInt((*I).args[1])+strToInt((*I).args[2])+pow(2,21)*strToInt((*I).args[3]);
                break;
            case 456: //OR (special)
                n=37+pow(2,11)*strToInt((*I).args[1])+pow(2,21)*strToInt((*I).args[2])+pow(2,16)*strToInt((*I).args[3]);
                break;
            case 312017: //ROTR (special)
                n=2+pow(2,21)+pow(2,11)*strToInt((*I).args[1])+pow(2,16)*strToInt((*I).args[2])+pow(2,6)*strToInt((*I).args[3]);
                break;
            case 7740: //SLL (special)
                n=pow(2,11)*strToInt((*I).args[1])+pow(2,16)*strToInt((*I).args[2])+pow(2,6)*strToInt((*I).args[3]);
                break;
            case 13148: //SLT (special)
                n=42+pow(2,11)*strToInt((*I).args[1])+pow(2,21)*strToInt((*I).args[2])+pow(2,16)*strToInt((*I).args[3]);
                break;
            case 7896: //SRL (special)
                n=2+pow(2,11)*strToInt((*I).args[1])+pow(2,16)*strToInt((*I).args[2])+pow(2,6)*strToInt((*I).args[3]);
                break;
            case 1214: //SUB (special)
                n=34+pow(2,11)*strToInt((*I).args[1])+pow(2,21)*strToInt((*I).args[2])+pow(2,16)*strToInt((*I).args[3]);
                break;
            case 590: //SW
                n=43*pow(2,26)+pow(2,16)*strToInt((*I).args[1])+strToInt((*I).args[2])+pow(2,21)*strToInt((*I).args[3]);
                break;
            case 11879: //XOR (special)
                n=38+pow(2,11)*strToInt((*I).args[1])+pow(2,21)*strToInt((*I).args[2])+pow(2,16)*strToInt((*I).args[3]);
                break;
            
            default:
                break;
        }
    } else {
        switch (hash((*I).args[0])) {
            case 452401: //BGTZ
                n=7*pow(2,26)+pow(2,21)*strToInt((*I).args[1])+strToInt((*I).args[2]);
                break;
            case 442391: //BLEZ
                n=3*pow(2,27)+pow(2,21)*strToInt((*I).args[1])+strToInt((*I).args[2]);
                break;
            case 14407: //DIV (special)
                n=26+pow(2,21)*strToInt((*I).args[1])+pow(2,16)*strToInt((*I).args[2]);
                break;
            case 9: //J
                n=pow(2,27)+strToInt((*I).args[1]);
                break;
            case 7445: //JAL
                n=pow(2,27)+strToInt((*I).args[1]);
                break;
            case 451: //JR (special)
                n=8+strToInt((*I).args[1]);
                break;
            case 5939: //LUI
                n=15*pow(2,26)+pow(2,16)*strToInt((*I).args[1])+strToInt((*I).args[2]);
                break;
            case 145482: //MFHI (special)
                n=16+pow(2,11)*strToInt((*I).args[1]);
                break;
            case 253642: //MFLO (special)
                n=18+pow(2,11)*strToInt((*I).args[1]);
                break;
            case 341912: //MULT (special)
                n=24*pow(2,26)+pow(2,21)*strToInt((*I).args[1])+pow(2,16)*strToInt((*I).args[2]);
                break;
            case 10517: //NOP (special)
                n=0;
                break;
            case -2147483648: //SYSCALL (special)
                n=12; //opérande code ?
                break;
            default:
                break;
            }
    }



}

int hash(char op[9]) {
    int i=0,n=0;
    while (op[i]!=NULL) {n+=(op[i]-65)*pow(26,i); i++;}
    return(n);
}

int main() {
    instruction I;
    clear(&I);
    sepLine("ADD $1, $2, $3",&I);
}
