//Code by Mudit
//Date: 24/09/2026
#include <stdio.h>
#include <math.h>

int main()
{
    int n = 0;
    double an, error;

    // From Z-transform:
    // a[n] = 1 - (1/2)^n

    while (1)
    {
        an = 1.0 - pow(0.5, n);
        error = fabs(1.0 - an);

        if (error < pow(0.5, 10))
            break;

        n++;
    }

    printf("Least value of n = %d\n", n);
    printf("a[%d] = %.10f\n", n, an);
    printf("|1 - a[%d]| = %.10f\n", n, error);

    return 0;
}
