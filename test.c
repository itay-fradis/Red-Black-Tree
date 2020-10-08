//
// Created by user on 27/05/2020.
//

////todo remember to delete
////
//// Created by 97254 on 20/05/2020.
////
#include "RBTree.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "utilities/RBUtilities.h"

int comp(const void* a, const void* b){
    int one = *((int*)a), two = *((int*)b);
    int result = one - two;
    return result;
}



typedef void* pVoid;
void noFree(pVoid data){}
void toFree(pVoid data){free(data);}

#define RIGHT_BLACK 1
#define LEFT_BLACK 2

#define BAD_SON(tree,dir, sign) ((tree->dir != NULL && comp(tree->data, tree->dir->data) sign 0))

int checkTree(Node* tree, Node* father)
{
    int nBlack = 0;
    if(tree == NULL) return 1;
    if(tree->parent != father) return 0;
    int data = *(int*)(tree->data);
    if(tree->parent == NULL && tree->color == RED) return 0;
    if(tree->color == RED){
        if((tree->right != NULL && tree->right->color == RED) ||
           (tree->left != NULL && tree->left->color == RED)) return 0;
    }
    else nBlack = 1;
    if(BAD_SON(tree, left, <) || BAD_SON(tree, right, >)) return 0;
    int rCheck = checkTree(tree->right, tree), lCheck = checkTree(tree->left, tree);
    if(rCheck != lCheck || rCheck == 0) return 0;
    int result = rCheck + nBlack;
    return result;
}

int sumTree(const void* object, void* args)
{
    if(object == NULL || args == NULL) return 0;
    *(int*)args += *(int const*)object;
    return 1;
}

int* pti(int i){
    int* out = (int*)malloc(sizeof(int));
    *out = i;
    return out;
}

int main()
{
    int s = 0, a = 5,b = 6,c = 13,d = 10,e = 2,f = 6, g = 5,h = 9,k = -1,l = 101
    ,m = 40, n = 70, o = -15, p = -3, q = 4, r = 90, t = 17, u = 1003, v = 200
    ,w = 756, y = 88, z = -100;
    int c1, c2,c3,c4,c5,c6,c7,c8,c9,c10, c11;
    RBTree* tree = newRBTree(comp, noFree);
#define siz  1500
    int arr[siz] = {0};
    int nums[2000] = {0};
    for (int i = 0; i < siz; ++i)
    {
        arr[i] = rand()%2000;
        insertToRBTree(tree, &arr[i]);
        t = checkTree(tree->root, NULL);
        printf("%lu, %d\n", tree->size, t);
    }
    while(tree->root != NULL)
    {
        s = rand()%2000;
        deleteFromRBTree(tree, &s);
        if(a > tree->size)
        {
            t = checkTree(tree->root, NULL);
            printf("%lu, %d\n", tree->size, t);
        }
        a = (int)tree->size;
    }
    freeRBTree(&tree);
}