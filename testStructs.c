#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Structs.h"
#include <math.h>
#include "utilities/RBUtilities.h"

void testConcat()
{
	char *w = (char*)calloc(13, sizeof(char));
	char *hello = "Hello";
	concatenate(hello, w);
	w[5] = '\0';

	char *world = " World";
	concatenate(world, w);
	assert(strcmp("Hello World\n", w) == 0 && "failed string concat");
	free(w);
}

void testStrCompare()
{
	char *a = "hello", *b = "hello";
	assert(stringCompare(a, b) == 0 && "failed stringCompare");
	a = "hello", b = "world";
	assert(stringCompare(a, b) < 0 && "failed stringCompare");
	a = "hello", b = "b";
	assert(stringCompare(a, b) > 0 && "failed stringCompare");
	a = "hello", b = "";
	assert(stringCompare(a, b) > 0 && "failed stringCompare - one empty string");
	a = "", b = "";
	assert(stringCompare(a, b) == 0 && "failed stringCompare - empty strings");
}

void testStrFree()
{
	char *a = (char*)calloc(5, sizeof(char));
	freeString(a);
}

char *vectorToString(void *data)
{
	Vector *v = (Vector*)data;
	char* string = (char*)calloc(15, sizeof(char));
	string[0] = '[';
	sprintf(&string[1], "%d", v->len);
	return string;
}

void testVecTree()
{
	RBTree *tree = newRBTree(vectorCompare1By1, freeVector);
	double *data1 = (double*)malloc(5*sizeof(double));
	double *data2 = (double*)malloc(1*sizeof(double));
	double *data3 = (double*)malloc(5*sizeof(double));
	double *data4 = (double*)malloc(7*sizeof(double));

	data1[0] = 14-0.2-0.2-0.1, data1[1] = 2, data1[2] = 3, data1[3] = 4, data1[4] = 5;
	data2[0] = 1;
	data3[0] = 1, data3[1] = 2, data3[2] = 5, data3[3] = 4, data3[4] = 5;
	data4[0] = 13+0.2+0.2+0.1, data4[1] = 2, data4[2] = 3, data4[3] = 4, data4[4] = 5, data4[5] = 6, data4[6] = 7;

	Vector 	*vec1 = (Vector*)malloc(sizeof(Vector)),
			*vec2 = (Vector*)malloc(sizeof(Vector)),
			*vec3 = (Vector*)malloc(sizeof(Vector)),
			*vec4 = (Vector*)malloc(sizeof(Vector));
	vec1->len = 5;
	vec1->vector = data1;
	vec2->len = 1;
	vec2->vector = data2;
	vec3->len = 5;
	vec3->vector = data3;
	vec4->len = 7;
	vec4->vector = data4;

	insertToRBTree(tree, vec1);
	insertToRBTree(tree, vec2);
	insertToRBTree(tree, vec3);
	insertToRBTree(tree, vec4);

	assert(vectorCompare1By1(vec1, vec4) == -1);
	Vector *max = findMaxNormVectorInTree(tree);
	assert(max->len == 7);
	freeVector(max);
	assert(deleteFromRBTree(tree, vec4));
	max = findMaxNormVectorInTree(tree);
	assert(max->len == 5);
	freeVector(max);

	// check failed max - and valgrind memory leak for this case
	free(vec1->vector);
	vec1->vector = NULL;
	vec1->len = 0;
	max = findMaxNormVectorInTree(tree);
	assert(max == NULL);
	freeVector(max);

	freeVector(max);
	freeRBTree(&tree);
}

int double_compare(const double a, const double b, const double epsilon)
{
	double delta = a - b;
	if (fabs(delta) < epsilon)
	{
		return 0;
	}
	else if (0 < delta)
	{
		return 1;
	}
	else
	{
		return -1;
	}
}



int main()
{
	testConcat();
	testStrCompare();
	testStrFree();

	testVecTree();

	printf("Passed.\n");
	return 0;
}