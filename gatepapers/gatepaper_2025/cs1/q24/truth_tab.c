//Code by Mudit
//Date: 05/10/2026
#include <stdio.h>

int main()
{
    int i, A, B, C, X;

    printf("A B C | X\n");
    printf("--------\n");

    for (i = 0; i < 8; i++)
    {
        // Extract the 3 bits of i
        A = (i >> 2) & 1;
        B = (i >> 1) & 1;
        C = i & 1;

        // Majority function: X = AB + AC + BC
        X = (A & B) | (A & C) | (B & C);

        printf("%d %d %d | %d\n", A, B, C, X);
    }

    return 0;
}
