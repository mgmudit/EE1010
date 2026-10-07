//Code by Mudit
//Date: 07/10/2026
#include <stdio.h>

// Converts i into 3-bit binary and stores the bits in A, B and C
void printBinary(int n, int *A, int *B, int *C)
{
    *A = (n >> 2) & 1;   // Get the leftmost bit
    *B = (n >> 1) & 1;   // Get the middle bit
    *C = n & 1;          // Get the rightmost bit

    printf("%d%d%d", *A, *B, *C);
}

int main()
{
    int i, A, B, C, X;

    printf("A B C | X\n");
    printf("--------\n");

    // Generate all numbers from 0 to 7
    for (i = 0; i < 8; i++)
    {
        // Convert i into binary and generate A, B and C
        printBinary(i, &A, &B, &C);

        // Boolean function: X = AB + AC + BC
        X = (A & B) | (A & C) | (B & C);

        printf(" | %d\n", X);
    }

    return 0;
}
