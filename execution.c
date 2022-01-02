#include "execution.h"

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
            case 15: ;//LUI
                R->rgd[arg(L,20,16)]=arg(L,16,0)<<16;
                break;
            case 35: //LW
                R->rgd[arg(L,20,16)]=lire(M,argS(L,15,0)+R->rgd[arg(L,25,21)]);
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