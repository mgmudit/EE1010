//Code by Mudit
//Date: 08/10/2026
#include <stdio.h>
#include <stdlib.h>
#include "List/libs/listgen.h"

int main()
{
    avyuh *L;

    L = loadList("lists.dat", 2, 9);

    printf("The two lists are:\n");
    printList(L);

    return 0;
}
