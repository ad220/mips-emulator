#include <stdio.h>
#include <stdlib.h>

#define NMAX 32
/* 
typedef struct {
    char* args[4][16];
} instruction; */


void strToWords(char ligne[], char* result[4][16]) {
    int i = 0, stopWord = 0, wordCount = 0;

    while (ligne[i] != '\0')
    {
        i++;
        if (ligne[i] == ' '){ // Mettre l'espace en ASCII si marche pas
            char word[16];
            for (int j = stopWord; j < i; j++)
            {
                word[j-stopWord] = ligne[j];
            }
            result[wordCount][i-stopWord] = word;
            wordCount++;
            i++;
            stopWord = i;
        }

        if (ligne[i] == ',')
        {
            char word[16];
            for (int j = stopWord; j < i; j++)
            {
                word[j-stopWord] = ligne[j];
            }
            result[wordCount][i-stopWord] = word;
            wordCount++;
            i += 2;
            stopWord = i;
        } 
    }
}

int main() {
    char* result[4][16];
    strToWords("ADD $1, $2, $3", result);
}