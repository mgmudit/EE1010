//Code by Mudit
//Date: 05/10/2026
#include <stdio.h>

int main()
{
    int A, B, C, X;

    printf("A B C | X\n");
    printf("--------\n");

    for (A = 0; A <= 1; A++)
    {
        for (B = 0; B <= 1; B++)
        {
            for (C = 0; C <= 1; C++)
            {
                X = (A & B) | (B & C) | (A & C);

                printf("%d %d %d | %d\n", A, B, C, X);
            }
        }
    }

    return 0;
}
