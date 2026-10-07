//Code by Mudit
//Date: 07/10/2026
#include <stdio.h>

void foo(int *p, int x)
{
    // Store x at the address pointed to by p
    *p = x;
}

int main()
{
    int *z;
    int a = 20, b = 25;

    // z stores the address of a
    z = &a;

    // Pass address of a through z
    // and pass value of b
    foo(z, b);

    printf("%d", a);

    return 0;
}
