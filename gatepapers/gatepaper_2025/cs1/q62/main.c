//Code by Mudit
//Date: 08/10/2026
#include <stdio.h>
#include <stdlib.h>
#include "List/libs/listgen.h"

int find(int query, sadish *list)
{
    while (list != NULL)
    {
        if (list->data == query)
            return 1;

        list = list->next;
    }

    return 0;
}

int main()
{
    FILE *fp;
    avyuh *L1, *L2;
    sadish *p, *prev, *temp;
    int i;

    fp = fopen("L12.dat", "r");

    if (fp == NULL)
    {
        printf("File cannot be opened\n");
        return 1;
    }

    // Create the two vector-lists
    L1 = createList(1, 9);
    L2 = createList(1, 7);

    // Read L1
    p = L1->vector;
    for (i = 0; i < 9; i++)
    {
        fscanf(fp, "%lf", &p->data);
        p = p->next;
    }

    // Read L2
    p = L2->vector;
    for (i = 0; i < 7; i++)
    {
        fscanf(fp, "%lf", &p->data);
        p = p->next;
    }

    printf("L1 before deletion:\n");
    printList(L1);

    printf("L2:\n");
    printList(L2);

    // Check L1 except the first node
    prev = L1->vector;
    p = prev->next;

    while (p != NULL)
    {
        if (find(p->data, L2->vector))
        {
            temp = p;
            prev->next = p->next;
            p = p->next;
            free(temp);
        }
        else
        {
            prev = p;
            p = p->next;
        }
    }

    printf("L1 after deletion:\n");
    printList(L1);

    fclose(fp);

    return 0;
}
