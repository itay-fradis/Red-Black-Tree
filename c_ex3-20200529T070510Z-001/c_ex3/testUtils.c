#include <stdio.h>


int intCompFunc(void* a, void* b)
{
    return *((int *)a)-*((int*)b);
}

void freeFunc(void* data)
{
    return;
}