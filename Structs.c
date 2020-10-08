/**********************************************
 * @author: itay fradis, itay.fradis, 205812712
 * @date 3 June 2020
 * ex3 - RED-BLACK TREE
 * here some functions that can be executed on the
 * RBTree data base.
***********************************************/

//---------------- includes ------------------
#include "RBTree.h"
#include "Structs.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

//----------- const definitions ---------------
/**
 * @def EQUAL, V1_IS_LOWER,  V1_IS_BIGGER
 * all are compare result of the func vectorCompare1By1
 * @def EPSILON
 * defines the maximum deviations between two norms.
 * @def VECTOR_VALUE_IS_NULL
 * we define norm's value to be -1 when the vector's pointer is null.
 */
#define EQUAL 0
#define V1_IS_LOWER -1
#define V1_IS_BIGGER 1
#define EPSILON 0.001
#define VECTOR_VALUE_IS_NULL -1
#define FAILURE 0
#define SUCCESS 1

//---------------- functions ------------------
/**
 * pass through the elements and compare them
 * @param vec1
 * @param vec2
 * @param len
 * @return the bigger one ore zero if maybe they equal.
 */
int findIfOneHasBiggerElement(const Vector *const vec1, const Vector *const vec2, const int len)
{
    for(int i = 0; i < len; i++)
    {
        if ((vec1->vector[i] - vec2->vector[i]) > EPSILON)
        {
            return V1_IS_BIGGER;
        }
        if (vec2->vector[i] - vec1->vector[i] > EPSILON)
        {
            return V1_IS_LOWER;
        }
    }
    return 0;
}

/**
 *
 * @param a
 * @param b
 * @return
 */
int vectorCompare1By1(const void *a, const void *b)
{
    Vector *vec1 = (Vector*)a;
    Vector *vec2 = (Vector*)b;
    int retVal = 0;
    if (vec1->len < vec2->len)
    {
        retVal = findIfOneHasBiggerElement(vec1, vec2, vec1->len);
        if (retVal)
        {
            return retVal;
        }
        return V1_IS_LOWER;
    }
    else
    {
        retVal = findIfOneHasBiggerElement(vec1, vec2, vec2->len);
        if (retVal)
        {
            return retVal;
        }
        if (vec1->len > vec2->len)
        {
            return V1_IS_BIGGER;
        }
        return EQUAL;
    }
}

/**
 *
 * @param vec
 * @return
 */
double findNorm(const Vector *const vec)
{
    if (vec->vector == NULL)
    {
        return VECTOR_VALUE_IS_NULL;
    }
    double norm = 0;
    for (int i = 0; i < vec->len; i++)
    {
        norm += pow(vec->vector[i], 2);
    }
    norm = sqrt(norm);
    return norm;
}

/**
 *
 * @param pVector
 * @param pMaxVector
 * @return
 */
int copyIfNormIsLarger(const void *pVector, void *pMaxVector)
{
    if (pVector == NULL || pMaxVector == NULL)
    {
        return FAILURE;
    }
    Vector *pVec = (Vector*)pVector;
    Vector *mVec = (Vector*)pMaxVector;
    if (pVec->vector == NULL || pVec->len <= 0)
    {
        return FAILURE;
    }
    double norm1 = findNorm(pVec);
    double norm2 = findNorm(mVec);
    if (mVec->vector == NULL || norm1 - norm2 > EPSILON)
    {
        mVec->len = pVec->len;
        mVec->vector = (double*) realloc (mVec->vector, sizeof(double) * pVec->len);
        for (int i = 0; i < pVec->len; i++)
        {
            mVec->vector[i] = pVec->vector[i];
        }
    }
    return SUCCESS;
}

/**
 *
 * @param pVector
 */
void freeVector(void *pVector)
{
    if (pVector == NULL)
    {
        return;
    }
    Vector *vec = (Vector*)pVector;
    free(vec->vector);
    vec->vector = NULL;
    free(pVector);
}

/**
 *
 * @param tree
 * @return
 */
Vector *findMaxNormVectorInTree(RBTree *tree)
{
    if (tree == NULL)
    {
        return NULL;
    }
    Vector *vec = (Vector*) malloc (sizeof(Vector));
    if (vec == NULL)
    {
        return NULL;
    }
    vec->vector = NULL;
    if (!forEachRBTree(tree, copyIfNormIsLarger, vec))
    {
        freeVector(vec);
        vec = NULL;
    }
    return vec;
}

/**
 *
 * @param a
 * @param b
 * @return
 */
int stringCompare(const void *a, const void *b)
{
    if (a == NULL || b == NULL)
    {
        return FAILURE;
    }
    char *str1 = (char*)a;
    char *str2 = (char*)b;
    return strcmp(str1, str2);
}

/**
 *
 * @param s
 */
void freeString(void *s)
{
    if (s == NULL)
    {
        return;
    }
    free(s);
}

/**
 *
 * @param word
 * @param pConcatenated
 * @return
 */
int concatenate(const void *word, void *pConcatenated)
{
    if (word == NULL || pConcatenated == NULL)
    {
        return FAILURE;
    }
    strcat(pConcatenated, word);
    strcat(pConcatenated, "\n");
    return SUCCESS;
}
