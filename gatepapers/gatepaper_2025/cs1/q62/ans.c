//Code by Mudit
//Date: 08/10/2026
#include <stdio.h>
#include <stdlib.h>
#include "List/libs/listgen.h"

int main()
{
    FILE *fp1, *fp2;
    sadish *L1, *L2;

    fp1 = fopen("L1.dat", "r");
    fp2 = fopen("L2.dat", "r");

    if (fp1 == NULL || fp2 == NULL)
    {
        printf("Error opening file\n");
        return 1;
    }

    L1 = loadVec(fp1, 9);
    L2 = loadVec(fp2, 7);

    printf("L1: ");
    printVec(L1);

    printf("L2: ");
    printVec(L2);

    fclose(fp1);
    fclose(fp2);

    return 0;
}
