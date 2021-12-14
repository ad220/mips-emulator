#include "header.h"

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