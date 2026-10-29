//Code by Mudit
//Date: 26/10/2026
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main(void)
{
    FILE *fp = fopen("samples.dat", "w");
    if (fp == NULL) {
        printf("Error creating samples.dat\n");
        return 1;
    }

    srand((unsigned int)time(NULL));

    // Generate 100 random samples using inverse CDF
    for (int i = 0; i < 100; i++) {
        double U = (double)rand() / ((double)RAND_MAX + 1.0);
        double X = cbrt(63.0 * U + 1.0);

        fprintf(fp, "%.10f %.10f\n", U, X);
    }

    fclose(fp);
    printf("Random samples saved in samples.dat\n");

    return 0;
}
