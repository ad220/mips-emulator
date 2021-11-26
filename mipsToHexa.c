#include <stdio.h>
#include <stdlib.h>

#define NMAX 32

void textToHexa(char T[]);

int main(int argc, char* argv[]) {
    FILE * input;
    input=fopen(argv[1],"r");
    FILE * output;
    output=fopen(argv[2],"w");
    int n,i=0,j=0;
    char T[NMAX];
    char instruction[NMAX];
    char* arguments[3];
    while (!feof(input)) {
        fscanf(input, "%c",&T);
        n=0;
        while (T[n]!=' ') {
            instruction[n]=T[n];
            n++;
        }
        n++;
        // while (n<32) {
        //     *(arguments[i])[j]=instruction[n];
        //     n++,j++;
        // }



    }
    fclose(input);
    return (0);
}

void textToHexa(char T[]) {

}