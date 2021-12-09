#include "execution.h"
#include "debug.c"

int arg(unsigned ligne, int bitSup, int bitInf) {
    return((ligne%PWR2[bitSup+1])/PWR2[bitInf]);
}

short argS(unsigned ligne, int bitSup, int bitInf) {
    return((ligne%PWR2[bitSup+1])/PWR2[bitInf]);
}

void execLigne(memoire M, registre* R) {
    unsigned L=lire(M,R->PC);
    int opcode=L/PWR2[26];
    if (opcode!=0) { // pas SPECIAL
        switch (opcode) {
            case 8: //ADDI
                R->rgd[arg(L,20,16)]=R->rgd[arg(L,25,21)]+argS(L,15,0); //gérer overflow
                break;
            case 4: //BEQ
                break;
            case 5: //BNE
                break;
            case 35: //LW
                break;
            case 43: //SW
                break;
            case 7: //BGTZ
                break;
            case 6: //BLEZ
                break;
            case 2: //J
                break;
            case 3: //JAL
                break;
            case 15: //LUI
                break;
            
            default:
                break;
        }
    } else { // SPECIAL
        opcode=ligne%PWR2[6];
        switch (opcode) {
            case 32: //ADD
                break;
            case 36: //AND
                break;
            case 37: //OR
                break;
            case 2: //ROTR & SRL
                break;
            case 0: //SLL & NOP
                break;
            case 42: //SLT
                break;
            case 34: //SUB
                break;
            case 38: //XOR
                break;
            case 26: //DIV
                break;
            case 8: //JR
                break;
            case 16: //MFHI
                break;
            case 18: //MFLO
                break;
            case 24: //MULT
                break;
            case 12: //SYSCALL op code ?
                break;

            default:
                break;
        }
    }
}