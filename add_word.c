#include <stdio.h>
#include <stdlib.h>


int add_word(int rd, int rs, int rt)
{
    int trente_un[5] = {0, 0, 0, 0, 0, 0};

    int vingt_cinq[4] = conv_vers_bin(rs, 5);

    int vingt_zero[4] = conv_vers_bin(rt, 5);

    int quinze[4]  = conv_vers_bin(rd, 5);

    int dix[4] = {0, 0, 0, 0, 0};

    int cinq[5] = {1, 0, 0, 0, 0, 0};



    int tot[31];

        int k;
        for (k=0; k<=5; k++)
            tot[k]=cinq[k];

        for (k=6; k<=10; k++)
            tot[k]=dix[k-6];

        for (k=11; k<=15; k++)
            tot[k]=quinze[k-11];

        for (k=16; k<=20; k++)
            tot[k]=vingt_zero[k-16];

        for (k=21; k<=25; k++)
            tot[k]=vingt_cinq[k-21];

        for (k=26; k<=31; k++)
            tot[k]=trente_un[k-26];




    return (tot[31]) ;

}
