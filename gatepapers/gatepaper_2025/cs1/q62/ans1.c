//Code by Mudit
//Date: 08/10/2026
#include <stdio.h>
#include <stdlib.h>
#include "List/libs/listgen.h"

int main()
{
    avyuh *L1, *L2;

    L1 = loadList("L1.dat", 1, 9);
    L2 = loadList("L2.dat", 1, 7);

    printf("L1 = ");
    printList(L1);

    printf("L2 = ");
    printList(L2);

    return 0;
}
