#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NMAX 8192

typedef struct {
    char line[NMAX][64];
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