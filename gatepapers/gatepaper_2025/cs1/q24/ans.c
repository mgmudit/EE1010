//Code by Mudit
//Date: 07/10/2026
#include <stdio.h>

// Convert an integer into its binary bits
void int_to_binary(int num, int bits[], int size)
{
    int i;

    for (i = 0; i < size; i++)
        bits[size - 1 - i] = (num >> i) & 1;
}

// Majority function: output is 1 when at least two inputs are 1
int X(int p, int q, int r)
{
    return (p && q) || (q && r) || (p && r);
}

// Check every possible combination
void verify()
{
    int i, bit[5];
    int optionA = 1, optionB = 1, optionC = 1, optionD = 1;

    // 5 bits give 2^5 = 32 combinations
    for (i = 0; i < 32; i++)
    {
        int_to_binary(i, bit, 5);

        // Check option A
        if (X(bit[0], bit[1], X(bit[2], bit[3], bit[4])) !=
            X(X(bit[0], bit[1], bit[2]), bit[3], bit[4]))
            optionA = 0;

        // Check option B
        if (X(bit[0], bit[1], X(bit[0], bit[1], bit[2])) !=
            X(bit[0], bit[1], bit[2]))
            optionB = 0;

        // Check option C
        if (X(bit[0], bit[1], X(bit[0], bit[2], bit[3])) !=
            (X(bit[0], bit[1], bit[0]) &&
             X(bit[2], bit[3], bit[2])))
            optionC = 0;

        // Check option D
        if (X(bit[0], bit[1], bit[2]) !=
            X(bit[0],
              X(bit[0], bit[1], bit[2]),
              X(bit[0], bit[2], bit[2])))
            optionD = 0;
    }

    printf("\nCorrect options:\n");

    if (optionA)
        printf("A is CORRECT\n");
    else
        printf("A is INCORRECT\n");

    if (optionB)
        printf("B is CORRECT\n");
    else
        printf("B is INCORRECT\n");

    if (optionC)
        printf("C is CORRECT\n");
    else
        printf("C is INCORRECT\n");

    if (optionD)
        printf("D is CORRECT\n");
    else
        printf("D is INCORRECT\n");
}

int main()
{
    printf("Checking all 32 combinations...\n");

    verify();

    return 0;
}
