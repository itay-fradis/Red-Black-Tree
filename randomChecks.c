//
// Created by isaac on 20/05/2020.
//

#ifndef RAND_CHECKS
#define RAND_CHECKS


#include "RBTree.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "utilities/RBUtilities.h"
int c =0;
int intComparator(const void *a, const void *b){
    int f= *((int*) a);
    int s= *((int*) b);
    return f==s ? 0 : f<s ? -1: 1;
}
void freef(const void *a){
    free(a);
}
void add_1(const void *a){
    c++;
}
int main() {
    int insertion, val, prevSize;
    int *data;
    RBTree *tree = newRBTree(intComparator, freef);
    for (int i = 0; i < 10000; i++) {
        insertion = rand() % 2;
        val = rand() % 1000;
        prevSize = tree->size;
        if (insertion == 1) {
            data = malloc(sizeof(int));
            if (data == NULL) {
                printf("malloc f***ed up");
                continue;
            }
            *data = val;
            printf("%d \n", val);
            if (insertToRBTree(tree, data) == 1) {
                if (tree->size != prevSize + 1) {
                    printf("You inserted but the size didn't change\n");
                }
            } else {
                if (RBTreeContains(tree, data) == 0){
                    printf("shouldv'e inserted but didn't\n");
                }
                free(data);
            }
           if (val == 750 || val == 179)
                viewTree(tree, intToString);
        }

		else
		{
			if(deleteFromRBTree(tree, &val) == 1){
				if (tree->size != prevSize -1){
					printf("You deleted but the size didn't change\n");
				}
			}
			else{
				if (RBTreeContains(tree, &val) == 1){
					printf("shouldv'e deleted but didn't\n");
				}
			}
		}
            if (isValidRBTree(tree) == 0) {
                NULL;
            }
        }
	forEachRBTree(tree, add_1, NULL);
	if (c != tree->size)
	{
		printf("forEachRBTree isn't working\n");
	}
        printf("If no messages appeared and Dor is correct you passed the random test (which "
               "probably isn't that good...)\n");
	freeRBTree(&tree);
    }
}
#endif //RAND_CHECKS
