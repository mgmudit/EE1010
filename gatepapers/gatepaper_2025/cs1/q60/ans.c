//Code by Mudit
//Date: /10/2026
#include <stdio.h>

int main(void)
{
    // Initial state of the four D flip-flops
    int q0 = 0, q1 = 0, q2 = 0, q3 = 0;
    int count = 0;

    do
    {
        printf("%d%d%d%d\n", q0, q1, q2, q3);
        count++;

        // Calculate next state before updating any flip-flop
        int next0 = q3;
        int next1 = q0;
        int next2 = q1;
        int next3 = q2;

        // Update all flip-flops simultaneously
        q0 = next0;
        q1 = next1;
        q2 = next2;
        q3 = next3;

    } while (!(q0 == 0 && q1 == 0 && q2 == 0 && q3 == 0));

    printf("Number of distinct states = %d\n", count);

    return 0;
}
