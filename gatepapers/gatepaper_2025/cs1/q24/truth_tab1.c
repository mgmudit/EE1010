//Code by Mudit
//Date: 07/10/2026
#include <stdio.h>

// Convert number into 3-bit binary
void printBinary(int n)
{
    int i;

    for (i = 2; i >= 0; i--)
        printf("%d", (n >> i) & 1);
}

// Generate all possible combinations
void generateTruthTable()
{
    int i, X;

    for (i = 0; i < 8; i++)
    {
        // X = AB + AC + BC
        X = (((i >> 2) & 1) && ((i >> 1) & 1))
          || (((i >> 2) & 1) && (i & 1))
          || (((i >> 1) & 1) && (i & 1));

        printBinary(i);
        printf(" | %d\n", X);
    }
}

int main()
{
    printf("A B C | X\n");
    printf("--------\n");

    generateTruthTable();

    return 0;
}
