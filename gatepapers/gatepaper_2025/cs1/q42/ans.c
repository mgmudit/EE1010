//Code by Mudit
//Date: 07/10/2026
#include <stdio.h>

// Convert number into 4-bit binary
void int_to_binary(int num, int bit[])
{
    int i;

    for (i = 0; i < 4; i++)
        bit[3 - i] = (num >> i) & 1;
}

// Generate the truth table
void truth_table()
{
    int i, bit[4], F;

    for (i = 0; i < 16; i++)
    {
        int_to_binary(i, bit);

        // F = (0,2,4,8,10,11,12)
        F = ((!bit[0] && !bit[1] && !bit[2] && !bit[3]) ||
             (!bit[0] && !bit[1] && bit[2] && !bit[3]) ||
             (!bit[0] && bit[1] && !bit[2] && !bit[3]) ||
             (bit[0] && !bit[1] && !bit[2] && !bit[3]) ||
             (bit[0] && !bit[1] && bit[2] && !bit[3]) ||
             (bit[0] && !bit[1] && bit[2] && bit[3]) ||
             (bit[0] && bit[1] && !bit[2] && !bit[3]));

        printf("%d  %d  %d  %d  |  %d\n",
               bit[0], bit[1], bit[2], bit[3], F);
    }
}

int main()
{
    printf("b3 b2 b1 b0 | F\n");
    printf("----------------\n");

    truth_table();

    return 0;
}
