//Code by Mudit
//Date: 07/10/2026
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 30
#define MAX 100

// Generate a random number between 0 and 100
int uniform()
{
    return rand() % (MAX + 1);
}

// Fill the array with random values
void generate(int A[])
{
    int i;

    for (i = 0; i < N; i++)
        A[i] = uniform();
}

// Function given in the question
// It sorts the array using adjacent swaps
// and returns the total number of swaps
int fun(int A[])
{
    int i, j, temp;
    int swaps = 0;

    for (i = 0; i <= N - 2; i++)
    {
        for (j = 0; j <= N - i - 2; j++)
        {
            // Swap if the elements are in the wrong order
            if (A[j] > A[j + 1])
            {
                temp = A[j];
                A[j] = A[j + 1];
                A[j + 1] = temp;

                swaps++;
            }
        }
    }

    return swaps;
}

int main()
{
    int A[N];
    int i, swaps;

    // Different random values each time the program runs
    srand(time(NULL));

    // Generate the random array
    generate(A);

    printf("Random array:\n");

    for (i = 0; i < N; i++)
        printf("%d ", A[i]);

    // Pass the generated array to the given function
    swaps = fun(A);

    printf("\n\nNumber of swaps = %d\n", swaps);

    return 0;
}
