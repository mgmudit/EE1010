//Code by Mudit
//Date: 07/10/2026
#include <stdio.h>

// Prints the 4-bit binary representation of a number
void printBinary(int n)
{
    int i;

    for (i = 3; i >= 0; i--)
        printf("%d", (n >> i) & 1);
}

int main()
{
    int a, b;

    printf("Enter two numbers: ");
    scanf("%d %d", &a, &b);

    // Display numbers in binary
    printf("\n--- Binary Representation ---\n");
    printf("a = %d = ", a);
    printBinary(a);

    printf("\nb = %d = ", b);
    printBinary(b);

    // Bitwise AND: compares the bits one by one
    printf("\n\n--- Bitwise AND (&) ---\n");
    printf("   ");
    printBinary(a);
    printf("\n&  ");
    printBinary(b);
    printf("\n   ----\n   ");
    printBinary(a & b);

    printf("\nResult of a & b = %d\n", a & b);

    // Logical AND: checks whether both numbers are non-zero
    printf("\n--- Logical AND (&&) ---\n");
    printf("a = %d -> %s\n", a, a ? "TRUE" : "FALSE");
    printf("b = %d -> %s\n", b, b ? "TRUE" : "FALSE");

    printf("\n%d && %d = %d\n", a, b, a && b);

    return 0;
}
