#include <stdio.h>

#include "RBTree.h"

#include "utilities/RBUtilities.h"

#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int comp(const void *a, const void *b)
{
	int *ob1 = (int*)a;
	int *ob2 = (int*)b;
	return *ob1 - *ob2;
}

int AddToArray(Node *node, Node **arr, int i)
{
	if (node == NULL)
	{
		arr[i] = NULL;
		return ++i;
	}
	if(node->left != NULL)
		AddToArray(node->left, arr, i+1);
	arr[i] = node;
	if(node->right != NULL)
		AddToArray(node->right, arr, i+1);

	return ++i;
}

void noDelete(void *n)
{
	return;
}

void test1()
{
	RBTree *tree = newRBTree(&comp, noDelete);
	int n1 = 15, n2 = 5, n3 = 10, n4 = 20, n5 = 30, n6 = 3, n7 = 7, n8 = 25,
			n9 = 35;
	insertToRBTree(tree, &n1);
	insertToRBTree(tree, &n2);
	insertToRBTree(tree, &n3);
	insertToRBTree(tree, &n4);
	insertToRBTree(tree, &n5);
	insertToRBTree(tree, &n5);
	insertToRBTree(tree, &n6);
	insertToRBTree(tree, &n7);
	insertToRBTree(tree, &n8);
	insertToRBTree(tree, &n9);
	insertToRBTree(tree, &n10);

//	printRBTree(tree->root);
	deleteFromRBTree(tree, &n4);
//	printRBTree(tree->root);

	assert(isValidRBTree(tree));
	freeRBTree(&tree);
}

void test_insert_ordered(int len)
{
	RBTree *tree = newRBTree(&comp, noDelete);

	int *arr = (int*)malloc(len * sizeof(int));
	for (int i = 0; i < len; i++)
	{
		arr[i] = i+1;
	}

	for (int i = 0; i < len; i++)
	{
		insertToRBTree(tree, &(arr[i]));
	}

//	printRBTree(tree->root);

	assert(isValidRBTree(tree));
	freeRBTree(&tree);
	free(arr);
}

void test_insert_random(int len)
{
	RBTree *tree = newRBTree(&comp, noDelete);

	int *arr = (int*)malloc(len * sizeof(int));
	for (int i = 0; i < len; i++)
	{
		arr[i] = (rand()%1000) - 500;
	}

	for (int i = 0; i < len; i++)
	{
		insertToRBTree(tree, &(arr[i]));
	}

//	printRBTree(tree->root);

	assert(isValidRBTree(tree));
	freeRBTree(&tree);
	free(arr);
}

void test_delete1()
{
	RBTree *tree = newRBTree(&comp, noDelete);
	int n1 = 10, n2 = 7, n3 = 20, n4 = 30;
	insertToRBTree(tree, &n1);
	insertToRBTree(tree, &n2);
	insertToRBTree(tree, &n3);
	insertToRBTree(tree, &n4);

	deleteFromRBTree(tree, &n4);

	assert(isValidRBTree(tree));
	freeRBTree(&tree);
}

void test_delete2()
{
	RBTree *tree = newRBTree(&comp, noDelete);
	int n1 = 10, n2 = 7, n3 = 20, n4 = 30, n5 = 35, n6 = 28;
	insertToRBTree(tree, &n1);
	insertToRBTree(tree, &n2);
	insertToRBTree(tree, &n3);
	insertToRBTree(tree, &n4);
	insertToRBTree(tree, &n5);
	insertToRBTree(tree, &n6);

	deleteFromRBTree(tree, &n1);

	assert(isValidRBTree(tree));
	freeRBTree(&tree);
}

void test_delete3()
{
	RBTree *tree = newRBTree(&comp, noDelete);
	int n1 = 10, n2 = 5, n3 = 20, n4 = 15, n5 = 30;
	insertToRBTree(tree, &n1);
	insertToRBTree(tree, &n2);
	insertToRBTree(tree, &n3);
	insertToRBTree(tree, &n4);
	insertToRBTree(tree, &n5);

	tree->root->right->color = RED;
	tree->root->right->left->color = BLACK;
	tree->root->right->right->color = BLACK;

//	printRBTree(tree->root);
	deleteFromRBTree(tree, &n3);
//	printRBTree(tree->root);

	assert(isValidRBTree(tree));
	freeRBTree(&tree);
}

void test_delete4()
{
	RBTree *tree = newRBTree(&comp, noDelete);
	int n1 = 10, n2 = 5, n3 = 20, n4 = 15, n5 = 30;
	insertToRBTree(tree, &n1);
	insertToRBTree(tree, &n2);
	insertToRBTree(tree, &n3);
	insertToRBTree(tree, &n4);
	insertToRBTree(tree, &n5);

	tree->root->right->color = BLACK;
	tree->root->right->left->color = BLACK;
	tree->root->right->right->color = BLACK;

//	printRBTree(tree->root);
	deleteFromRBTree(tree, &n3);
//	printRBTree(tree->root);

	assert(tree->root->color == BLACK && *(int*)tree->root->data == 10);
	assert(tree->root->left->color == RED && *(int*)tree->root->left->data == 5);
	assert(tree->root->right->color == BLACK && *(int*)tree->root->right->data == 30);
	assert(tree->root->right->left->color == RED && *(int*)tree->root->right->left->data == 15);
	freeRBTree(&tree);
}

void test_delete5()
{
	RBTree *tree = newRBTree(&comp, noDelete);
	int n1 = 10, n2 = 5, n3 = 1, n4 = 7, n5 = 20, n6 = 15, n7 = 30, n8 = 25, n9 = 40;
	insertToRBTree(tree, &n1);
	insertToRBTree(tree, &n2);
	insertToRBTree(tree, &n3);
	insertToRBTree(tree, &n4);
	insertToRBTree(tree, &n5);
	insertToRBTree(tree, &n6);
	insertToRBTree(tree, &n7);
	insertToRBTree(tree, &n8);
	insertToRBTree(tree, &n9);

	tree->root->right->color = BLACK;
	tree->root->left->color = BLACK;
	tree->root->right->right->color = RED;
	tree->root->right->right->right->color = BLACK;
	tree->root->right->right->left->color = BLACK;

	deleteFromRBTree(tree, &n6);

	assert(isValidRBTree(tree));
	freeRBTree(&tree);
}

void test_delete6()
{
	RBTree *tree = newRBTree(&comp, noDelete);
	int n1 = 10, n2 = 5, n3 = 1, n4 = 7, n5 = 30, n6 = 25, n7 = 40, n8 = 20, n9 = 28;
	insertToRBTree(tree, &n1);
	insertToRBTree(tree, &n2);
	insertToRBTree(tree, &n3);
	insertToRBTree(tree, &n4);
	insertToRBTree(tree, &n5);
	insertToRBTree(tree, &n6);
	insertToRBTree(tree, &n7);
	insertToRBTree(tree, &n8);
	insertToRBTree(tree, &n9);

	tree->root->right->color = BLACK;
	tree->root->left->color = BLACK;
	tree->root->right->left->right->color = BLACK;
	tree->root->right->left->left->color = BLACK;
	tree->root->right->left->color = RED;

	deleteFromRBTree(tree, &n3);

	assert(isValidRBTree(tree));
	freeRBTree(&tree);
}

void test_delete7()
{
	RBTree *tree = newRBTree(&comp, noDelete);
	int n1 = 10, n2 = 5, n3 = 1, n4 = 7, n5 = 30, n6 = 25, n7 = 40, n8 = 20, n9 = 28;
	insertToRBTree(tree, &n1);
	insertToRBTree(tree, &n2);
	insertToRBTree(tree, &n3);
	insertToRBTree(tree, &n4);
	insertToRBTree(tree, &n5);
	insertToRBTree(tree, &n6);
	insertToRBTree(tree, &n7);
	insertToRBTree(tree, &n8);
	insertToRBTree(tree, &n9);

	deleteFromRBTree(tree, &n9);
	assert(isValidRBTree(tree));
	deleteFromRBTree(tree, &n3);
	assert(isValidRBTree(tree));
	insertToRBTree(tree, &n3);

	assert(isValidRBTree(tree));
	freeRBTree(&tree);
}

void writeTest()
{
	RBTree *tree = newRBTree(&comp, noDelete);
	int n1 = 10, n2 = 5, n3 = 1, n4 = 7, n5 = -15, n6 = 25, n7 = 40, n8 = 20, n9 = 28;
	insertToRBTree(tree, &n1);
	insertToRBTree(tree, &n2);
	insertToRBTree(tree, &n3);
	insertToRBTree(tree, &n4);
	insertToRBTree(tree, &n5);
	insertToRBTree(tree, &n6);
	insertToRBTree(tree, &n7);
	insertToRBTree(tree, &n8);
	insertToRBTree(tree, &n9);

	assert(isValidRBTree(tree));
	printRBTree(tree->root);
//	int status = RBTreeToJSON(tree, "tree.json", intToString);
	int status = viewTree(tree, intToString);
	printf("status is - %d", status);
}

int isInArr(int val, const int *arr, int len)
{
	for (int j = 0; j < len; j++)
	{
		if (val == arr[j])
			return 1;
	}
	return 0;
}

void fillUniqueArr(int *arr, int len, int maxNum, int minNum)
{
	int tmp;
	for (int i = 0; i < len; i++)
	{
		tmp = (rand() % (maxNum - minNum + 1)) + minNum;
		while (isInArr(tmp, arr, i))
		{
			tmp = (rand() % (maxNum - minNum + 1)) + minNum;
		}
		arr[i] = tmp;
	}
}

#define MAX_NUM 500
#define MIN_NUM -500

void test_general(int len)
{
	RBTree *tree = newRBTree(&comp, noDelete);

	int *arr = (int*)malloc(len * sizeof(int));
	fillUniqueArr(arr, len, MAX_NUM, MIN_NUM);

	for (int i = 0; i < len; i++)
	{
		assert(insertToRBTree(tree, &(arr[i])));
		assert(isValidRBTree(tree));
	}

	for (int i = 0; i < len; i++)
	{
		assert(!insertToRBTree(tree, &(arr[i])));
		assert(isValidRBTree(tree));
	}

//	viewTree(tree, intToString);

	int notInTree = MIN_NUM - 5;
	assert(!deleteFromRBTree(tree, &notInTree));
	assert(isValidRBTree(tree));

	for (int i = 0; i < len; i++)
	{
//		printRBTree(tree->root);
		assert(RBTreeContains(tree, &(arr[i])));
		assert(deleteFromRBTree(tree, &(arr[i])));
		if (!isValidRBTree(tree))
		{
//			printRBTree(tree->root);
			assert(0 && "invalid delete");
		}
	}

	// remove from empty tree
	notInTree = MIN_NUM - 5;
	assert(!deleteFromRBTree(tree, &notInTree));
	assert(isValidRBTree(tree));

	assert(isValidRBTree(tree));
	freeRBTree(&tree);
	free(arr);
}

int pushToBuffer(const void *data, void *pBuffer)
{
	int *buffer = (int*)pBuffer;
	buffer[buffer[0]] = *(int*)data;
	buffer[0]++;
	return 1;
}

void test_foreach(int len)
{
	RBTree *tree = newRBTree(&comp, noDelete);

	int *arr = (int*)malloc(len * sizeof(int));
	for (int i = 0; i < len; i++)
	{
		arr[i] = (rand()%1000) - 500;
	}

	for (int i = 0; i < len; i++)
	{
		if (!insertToRBTree(tree, &(arr[i])))
		{
			assert(RBTreeContains(tree, &(arr[i])));
		}
		assert(isValidRBTree(tree));
	}

	int *buffer = (int*)malloc((tree->size+1) * sizeof(int));
	buffer[0] = 1;
	forEachRBTree(tree, pushToBuffer, buffer);
	for (int i = 2; i < (int)tree->size; i++)
	{
		if (buffer[i - 1] > buffer[i])
		{
			viewTree(tree, intToString);
			assert(0 && "forEach isn't in-order");
		}
	}

	assert(isValidRBTree(tree));
	freeRBTree(&tree);
	free(arr);
	free(buffer);
}

void test_empty()
{
	RBTree *tree = newRBTree(&comp, noDelete);
	assert(isValidRBTree(tree) && "initialized tree isn't valid");
	freeRBTree(&tree);

	tree = newRBTree(&comp, noDelete);
	assert(!deleteFromRBTree(tree, NULL) && "deleting NULL pointer");
	assert(!insertToRBTree(tree, NULL) && "inserting NULL pointer");

	int val = 5;
	assert(!deleteFromRBTree(tree, &val) && "deleting non-existing value");
	assert(isValidRBTree(tree) && "tree isn't valid after delete");

	int buffer[5] = {0};
	assert(forEachRBTree(tree, pushToBuffer, buffer) && "failed forEach for empty tree");
	assert(isValidRBTree(tree) && "tree isn't valid after forEach");

	assert(!forEachRBTree(tree, NULL, buffer) && "forEach for NULL should be failing");

	//todo check school sol
	assert(!RBTreeContains(tree, NULL) && "contains for NULL should be 0");

	//
	assert(!RBTreeContains(tree, &val) && "didn't fail for contains for empty tree");
	freeRBTree(&tree);
}

int isInArrPointer(int val, int **arr, int len)
{
	for (int j = 0; j < len; j++)
	{
		if (val == *arr[j])
			return 1;
	}
	return 0;
}

void fillUniqueArrDyn(int **arr, int len, int maxNum, int minNum)
{
	int *tmp;
	for (int i = 0; i < len; i++)
	{
		tmp = (int*)malloc(sizeof(int));
		assert(tmp != NULL);
		*tmp = (rand() % (maxNum - minNum + 1)) + minNum;
		while (isInArrPointer(*tmp, arr, i))
		{
			*tmp = (rand() % (maxNum - minNum + 1)) + minNum;
		}
		arr[i] = tmp;
	}
}

void pfree(void *p)
{
	free(p);
}

void test_dynamic(int len)
{
	RBTree *tree = newRBTree(comp, pfree);

	int **arr = (int**)malloc(len * sizeof(void*));
	fillUniqueArrDyn(arr, len, MAX_NUM, MIN_NUM);

	for (int i = 0; i < len; i++)
	{
		assert(insertToRBTree(tree, arr[i]));
		assert(isValidRBTree(tree));
	}

	for (int i = 0; i < len; i++)
	{
		assert(!insertToRBTree(tree, arr[i]));
		assert(isValidRBTree(tree));
	}

	int *notInTree = (int*)malloc(sizeof(int));
	*notInTree = MIN_NUM - 5;
	assert(!deleteFromRBTree(tree, notInTree));
	assert(isValidRBTree(tree));

	for (int i = 0; i < len; i++)
	{
		assert(RBTreeContains(tree, arr[i]));
		assert(deleteFromRBTree(tree, arr[i]));
		assert(isValidRBTree(tree));
	}

	// remove from empty tree
	assert(!deleteFromRBTree(tree, notInTree));
	assert(isValidRBTree(tree));

	assert(isValidRBTree(tree));
	freeRBTree(&tree);
	free(arr);
	free(notInTree);
}

void weirdTest()
{
	RBTree *tree = newRBTree(&comp, noDelete);
	int n1 = -129, n2 = -133, n3 = 223, n4 = 134, n5 = 331;
	insertToRBTree(tree, &n1);
	insertToRBTree(tree, &n2);
	insertToRBTree(tree, &n3);
	insertToRBTree(tree, &n4);
	insertToRBTree(tree, &n5);
	insertToRBTree(tree, &n5);

	viewTree(tree, intToString);
	assert(isValidRBTree(tree));
	printRBTree(tree->root);
	deleteFromRBTree(tree, &n2);
	printRBTree(tree->root);

	assert(isValidRBTree(tree));
	freeRBTree(&tree);
}

int main()
{
	time_t start = clock();
	int seed = time(NULL);
	srand(seed);

	test_empty();

	test1();
	test_insert_ordered(15);
	test_insert_random(10);
	test_delete1();
	test_delete2();
	test_delete3();
	test_delete4();
	test_delete5();
	test_delete6();
	test_delete7();

	int size, counter = 0;
	for (int i = 0; i < 1000; i++)
	{
		size = rand() % 500;
		test_general(size);
		test_foreach(size);
		test_dynamic(size);
		counter++;
	}

	time_t end = clock();
	double total = (double)(end - start) / CLOCKS_PER_SEC;
	printf("tests took %.2f seconds.\n", total);
	printf("preformed %d random tests. \n", counter);
	printf("All tests passed. \n");

	return 0;
}