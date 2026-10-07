//Code by Mudit
//Date: 07/10/2026
#include <stdio.h>

int foo(int S[], int size)
{
    if (size == 0)
        return 0;

    if (size == 1)
        return 1;

    if (S[0] != S[1])
        return 1 + foo(S + 1, size - 1);

    return foo(S + 1, size - 1);
}

int main()
{
    int A[] = {0, 1, 2, 2, 0, 0, 1, 1};

    printf("%d\n", foo(A, 9));

    return 0;
}
