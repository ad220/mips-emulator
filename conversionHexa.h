#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NMAX 1024
unsigned PWR2[32]={1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536,131072,262144,524288,1048576,2097152,4194304,8388608,16777216,33554432,67108864,134217728,268435456,536870912,1073741824,2147483648};

typedef struct {
    char line[NMAX][256];
} progIN;

typedef struct {
    unsigned line[NMAX];
} progOUT;

typedef struct {
    char args[4][9];
} instruction;

typedef struct {
    instruction line[NMAX];
} progInstr;

int lireFichier(char nomFichier[], progIN* pgm);
void ecrireFichier(char nomFichier[], progOUT* pgm, int n);
void charPgmToHexaPgm(progIN* pgmIN, progOUT* pgmOUT, int n);