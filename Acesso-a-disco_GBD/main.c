#include <stdio.h>
#include <stdlib.h>
#include "registro.h"

int main()
{
    FILE * fp;
    fp = CREATE_HEAP_FILE(2);
    printf("Hello world!\n");
    return 0;
}
