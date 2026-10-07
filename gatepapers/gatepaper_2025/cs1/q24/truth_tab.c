//Code by Mudit
//Date: 05/10/2026
#include <stdio.h>

int main()
{
    int A, B, C, X;

    // Print the headings of the truth table
    printf("A B C | X\n");
    printf("--------\n");

    // Generate all possible values of A, B and C
    for (A = 0; A <= 1; A++)
    {
        for (B = 0; B <= 1; B++)
        {
            for (C = 0; C <= 1; C++)
            {
                // X = 1 when at least two of A, B and C are 1
                // &  -> bitwise AND
                // |  -> bitwise OR
                X = (A & B) | (B & C) | (A & C);

                // Print the current combination and its output
                printf("%d %d %d | %d\n", A, B, C, X);
            }
        }
    }

    return 0;
}
