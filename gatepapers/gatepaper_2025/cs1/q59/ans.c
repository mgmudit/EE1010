//Code by Mudit
//Date: 12/10/2026
#include <stdio.h>

int main(void)
{
    char state[8] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};

    // Next-state table: columns correspond to X = 0 and X = 1
    int next[8][2] = {
        {5, 1},  // A -> F, B
        {3, 2},  // B -> D, C
        {5, 4},  // C -> F, E
        {6, 0},  // D -> G, A
        {3, 2},  // E -> D, C
        {5, 1},  // F -> F, B
        {6, 7},  // G -> G, H
        {6, 0}   // H -> G, A
    };

    // Output table for each state and input
    int output[8][2] = {
        {0, 0}, {0, 0}, {0, 0}, {1, 0},
        {0, 0}, {1, 1}, {0, 1}, {1, 0}
    };

    int group[8], newGroup[8];
    int groups = 0, changed;

    // Initially, group states having identical output pairs
    for (int i = 0; i < 8; i++) {
        group[i] = -1;

        for (int j = 0; j < i; j++) {
            if (output[i][0] == output[j][0] &&
                output[i][1] == output[j][1]) {
                group[i] = group[j];
                break;
            }
        }

        if (group[i] == -1)
            group[i] = groups++;
    }

    // Refine groups until no further separation is needed
    do {
        changed = 0;
        int count = 0;

        for (int i = 0; i < 8; i++) {
            newGroup[i] = -1;

            for (int j = 0; j < i; j++) {
                // States remain equivalent only if their outputs
                // and the groups of both next states match
                if (output[i][0] == output[j][0] &&
                    output[i][1] == output[j][1] &&
                    group[next[i][0]] == group[next[j][0]] &&
                    group[next[i][1]] == group[next[j][1]]) {
                    newGroup[i] = newGroup[j];
                    break;
                }
            }

            if (newGroup[i] == -1)
                newGroup[i] = count++;
        }

        // Check whether the partition has changed
        for (int i = 0; i < 8; i++) {
            if (newGroup[i] != group[i])
                changed = 1;

            group[i] = newGroup[i];
        }

        groups = count;

    } while (changed);

    // Print the original state-transition table
    printf("State | X=0 (Next/Output) | X=1 (Next/Output)\n");

    for (int i = 0; i < 8; i++) {
        printf("  %c   |       %c / %d       |       %c / %d\n",
               state[i],
               state[next[i][0]], output[i][0],
               state[next[i][1]], output[i][1]);
    }

    // Display the final equivalent-state groups
    printf("\nEquivalent state groups:\n");

    for (int g = 0; g < groups; g++) {
        printf("Group %d: ", g + 1);

        for (int i = 0; i < 8; i++) {
            if (group[i] == g)
                printf("%c ", state[i]);
        }

        printf("\n");
    }

    printf("\nMinimum number of states = %d\n", groups);

    return 0;
}
