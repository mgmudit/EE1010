#include <stdio.h>

int gate(int n)
{
    int d, t, newnum, turn;

    newnum = turn = 0;
    t = 1;

    while (n >= t)
        t *= 10;

    t /= 10;

    while (t > 0)
    {
        d = n / t;
        n = n % t;
        t /= 10;

        if (turn)
            newnum = 10 * newnum + d;

        turn = (turn + 1) % 2;
    }

    return newnum;
}

int main()
{
    printf("%d\n", gate(14362));

    return 0;
}
