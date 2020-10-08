#include "RBTree.h"
#include "utilities/RBUtilities.h"
#include <stdio.h>
#include <stdlib.h>

#define FIVE 5
#define TEN 10
#define TWENTY_FIVE 25
#define FIFTY 50
#define ONE_HUNDRED 100
#define FIVE_HUNDREDS 500
#define THOUSAND 1000
#define TEN_THOUSAND 10000

char *intToString(void *data);

int compInt(void* data1, void* data2)
{
    int a = *((int*) data1);
    int b = *((int*) data2);
    if(a == b)
        return 0;
    if(a > b)
        return 1;
    return -1;
}

void freeFunction(void* data)
{
    int stam_to_not_make_error = 0;
}

void newInsert(RBTree* t, void* data)
{
    insertToRBTree(t, data);
//    viewTree(t, intToString);
    if(!isValidRBTree(t))
    {
        printf("added %d and it is not a valid tree now: ", *((int*) data));
        printRBTree(t->root);
        freeRBTree(&t);
        exit(1);
    }
}

void newDelete(RBTree* t, void* data)
{
    deleteFromRBTree(t, data);
//    printRBTree(t->root);
    if(!isValidRBTree(t))
    {
        printf("deleted %d and it is not a valid tree now: \n", *((int*) data));
        printRBTree(t->root);
        freeRBTree(&t);
        exit(1);
    }
}

char *intToString(void *data)
{
    int *d = (int*)data;
    char* string = (char*)calloc(10, sizeof(char));
    sprintf(string, "%d", *d);
    return string;
}

int main()
{

    RBTree *t = newRBTree((CompareFunc) &compInt, (FreeFunc) &freeFunction);
    int a[THOUSAND];
    for(int i = 0; i < THOUSAND; i++)
    {
        a[i] = rand();
        while(a[i] > 999)
        {
            a[i] = rand();
        }
        newInsert(t, &a[i]);
    }
//    viewTree(t, intToString);

    for(int i = 0; i < THOUSAND; i++)
    {
        newDelete(t, &a[i]);
    }
    printf("\nPassed All tests (Baruch Hashem)\n");
}
