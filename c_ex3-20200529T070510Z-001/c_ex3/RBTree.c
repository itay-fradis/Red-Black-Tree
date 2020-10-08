#include "RBTree.h"
#include "utilities/RBUtilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


#define ALLOC_FAIL_NULL NULL
#define ALLOC_FAIL_ZERO 0
#define INSERT_NODE_SUCCESS 1
#define INSERT_NODE_FAILURE 0
#define DELETE_NODE_SUCCESS 1
#define DELETE_NODE_FAILURE 0

Node *findBinaryLeaf(Node *node, CompareFunc compfunc, const void *data);
Node *findBinaryLeafToDelete(Node *node, CompareFunc compfunc, const void *data);
Node *findUncle(const Node *node);
void rotateRight(Node *node, RBTree *tree);
void rotateLeft(Node *node, RBTree *tree);
void createChainRight(Node *node, Node* parent, RBTree* tree);
void createChainLeft(Node *node, Node* parent, RBTree *tree);
void insertCase4(Node *node, RBTree *tree);
void fixTree(Node *node, RBTree *tree);
int insertRoot(RBTree *tree, void *data);
int insertNode(Node *node, RBTree *tree, void *data);
Node *findSuccessor(Node *node, RBTree *tree);
void freeNode(Node* node, Node* parent, RBTree *tree);
Node *findChild(const Node *node);
void deleteCase2(Node *node, Node *child, RBTree *tree);
Node *findBrother(const Node *node, const Node *parent);
void deleteSubCase1(Node *bro, Node *parent);
void deleteSubCase3(Node *bro, Node *parent, RBTree *tree);
Node *findCloseCousin(const Node* bro, const Node* parent);
void deleteSubCase4(Node* cousin, Node* bro, RBTree *tree);
void deleteSubCase5(Node *farCousin, Node *bro, Node *parent, RBTree *tree);
void deleteCaseB(Node* bro, Node* parent, RBTree* tree);
void deleteCase3FixTree(Node* c, Node* bro, RBTree* tree);
void deleteCase3(Node* node, Node* parent, RBTree *tree);
void deleteNodeAndFixTree(Node* node, RBTree* tree);

//void freeFunction(void* data)
//{
//    int stam_to_not_make_error = 0;
//}

/**
 *
 * @param compFunc
 * @param freeFunc
 * @return create tree
 */
RBTree *newRBTree(CompareFunc compFunc, FreeFunc freeFunc)
{
    RBTree* tree = (RBTree*) malloc (sizeof(RBTree));
    if (tree == NULL)
    {
        return ALLOC_FAIL_NULL;
    }
    tree->root = NULL;
    tree->compFunc = compFunc;
    tree->freeFunc = freeFunc;
    tree->size = 0;
    return tree;

}

/**
 *
 * @param node
 * @param compfunc
 * @param data
 * @return a pointer to a node which the new node will be hanged on it.
 */
Node *findBinaryLeaf(Node *node, CompareFunc compfunc, const void *data)
{
    if (!compfunc(data, node->data)) //equal
    {
        return NULL;
    }
    else if (compfunc(data, node->data) < 0) //go left
    {
        if (!node->left)
        {
            return node;
        }
        return  findBinaryLeaf(node->left, compfunc, data);
    }
    else //go right
    {
        if (!node->right)
        {
            return node;
        }
        return findBinaryLeaf(node->right, compfunc, data);
    }
}

/**
 *
 * @param node
 * @param compfunc
 * @param data
 * @return pointer to node to delete.
 */
Node *findBinaryLeafToDelete(Node *node, CompareFunc compfunc, const void *data)
{
    assert(node != NULL);
    if (!compfunc(data, node->data)) //equal
    {
        return node;
    }
    else if (compfunc(data, node->data) < 0) //go left
    {
        if (!node->left)
        {
            return NULL;
        }
        return  findBinaryLeafToDelete(node->left, compfunc, data);
    }
    else //go right
    {
        if (!node->right)
        {
            return NULL;
        }
        return findBinaryLeafToDelete(node->right, compfunc, data);
    }
}

/**
 * @param node
 * @return the node's uncle
 */
Node *findUncle(const Node *node)
{
    Node *grand = node->parent->parent;
    if (grand->right == node->parent)
    {
        return grand->left;
    }
    else
    {
        return grand->right;
    }
}

/**
 * @param node
 * @param tree
 */
void rotateRight(Node *node, RBTree *tree)
{
    // node is the G vertex.
    Node* root = node->parent;
    Node* father = node->left;
    if (root != NULL)
    {
        if (root->left == node)
        {
            root->left = father;
        }
        else
        {
            root->right = father;
        }
    }
    else
    {
        tree->root = father;
    }
    node->left = father->right;
    if (father->right != NULL)
    {
        father->right->parent = node;
    }
    node->parent = father;
    father->parent = root;
    father->right = node;
}

/**
 * @param node
 * @param tree
 */
void rotateLeft(Node *node, RBTree *tree)
{
    Node* root = node->parent;
    Node* father = node->right;
    if (root != NULL)
    {
        if (root->right == node)
        {
            root->right = father;
        }
        else
        {
            root->left = father;
        }
    }
    else
    {
        tree->root = father;
    }
    node->right = father->left;
    if (father->left != NULL)
    {
        father->left->parent = node;
    }
    node->parent = father;
    father->parent  = root;
    father->left = node;

}

/**
 * case 4 - first step - in inserting node
 * @param node
 * @param parent
 * @param tree
 */
void createChainRight(Node *node, Node* parent, RBTree* tree)
{
    if (parent->right != node)
    {
        rotateRight(parent, tree);
        node->color = BLACK;
    }
    else
    {
        parent->color = BLACK;
    }
}

/**
 *
 * @param node
 * @param parent
 * @param tree
 */
void createChainLeft(Node *node, Node* parent, RBTree *tree)
{
    if (parent->left != node)
    {
        rotateLeft(parent, tree);
        node->color = BLACK;
    }
    else
    {
        parent->color = BLACK;
    }
}

/**
 *
 * @param node
 * @param tree
 */
void insertCase4(Node *node, RBTree *tree)
{
    Node *parent = node->parent;
    Node *grand = node->parent->parent;
    if (grand->right == parent)
    {
        createChainRight(node, parent, tree);
        rotateLeft(grand, tree);
    }
    else
    {
        createChainLeft(node, parent, tree);
        rotateRight(grand, tree);
    }
    grand->color = RED;

}
/**
 * fix tree after inserting a node
 * @param node
 * @param tree
 */
void fixTree(Node *node, RBTree *tree)
{
    if (node->parent == NULL) //case1
    {
        node->color = BLACK;
        return;
    }
    if (node->parent->color == BLACK) //case2
    {
        return;
    }
    Node * uncle = findUncle(node);
    if (node->parent->color == RED && uncle != NULL && uncle->color == RED) //case3
    {
        node->parent->color = BLACK;
        uncle->color = BLACK;
        node->parent->parent->color = RED;
        fixTree(node->parent->parent, tree);
        return;
    }
    if (node->parent->color == RED && (!uncle || uncle->color == BLACK))
    {
        insertCase4(node, tree);
        return;
    }

}

/**
 * creates a root-node to the tree
 * @param tree
 * @param data
 * @return
 */
int insertRoot(RBTree *tree, void *data)
{
    tree->root = (Node*) malloc (sizeof(Node));
    if (!tree->root)
    {
        return ALLOC_FAIL_ZERO;
    }
    tree->root->data = data;
    tree->root->color = RED;
    tree->root->parent = NULL;
    tree->root->right = NULL;
    tree->root->left = NULL;
    fixTree(tree->root, tree);
    return INSERT_NODE_SUCCESS;
}

/**
 * insert node to the tree
 * @param node
 * @param tree
 * @param data
 * @return
 */
int insertNode(Node *node, RBTree *tree, void *data)
{
    Node *new = (Node*) malloc (sizeof(Node));
    if (!new)
    {
        return ALLOC_FAIL_ZERO;
    }
    new->data = data;
    new->right = NULL;
    new->left = NULL;
    new->color = RED;
    new->parent = node;
    if (tree->compFunc(data, node->data) < 0)
    {
        node->left = new;
        fixTree(new, tree);
    }
    else
    {
        node->right = new;
        fixTree(new, tree);
    }
    return INSERT_NODE_SUCCESS;
}

/**
 *
 * @param tree
 * @param data
 * @return
 */
int insertToRBTree(RBTree *tree, void *data)
{
    if (!tree->root)
    {
        if(!insertRoot(tree, data))
        {
            return INSERT_NODE_FAILURE;
        }
        tree->size++;
        return INSERT_NODE_SUCCESS;
    }
    Node* leaf = findBinaryLeaf(tree->root, tree->compFunc, data);
    if (!leaf)
    {
        return INSERT_NODE_FAILURE;
    }
    if (!insertNode(leaf, tree, data))
    {
        return INSERT_NODE_FAILURE;
    }
    tree->size ++;
    return INSERT_NODE_SUCCESS;
}

/**
 * finds node's successor and switch their data pointers
 * @param node
 * @param tree
 * @return
 */
Node *findSuccessor(Node *node, RBTree *tree)
{
    Node *successor = node->right;
    while (successor->left != NULL)
    {
        successor = successor->left;
    }
    void *data = node->data;
    node->data = successor->data;
    successor->data = data;
    return successor;
}

/**
 * deletes a node from the tree
 * @param node
 * @param parent
 * @param tree
 */
void freeNode(Node* node, Node* parent, RBTree *tree)
{
    if (!parent)
    {
        tree->freeFunc(node->data);
        free(node);
        tree->root = NULL;
    }
    else if (parent->left == node)
    {
        tree->freeFunc(node->data);
        free(node);
        parent->left = NULL;
    }
    else
    {
        tree->freeFunc(node->data);
        free(node);
        parent->right = NULL;
    }
}

/**
 *
 * @param node
 * @return
 */
Node *findChild(const Node *node)
{
    if (!node->left && node->right != NULL)
    {
        return node->right;
    }
    if (node->left != NULL && !node->right)
    {
        return node->left;
    }
    return NULL;
}

/**
 *
 * @param node
 * @param child
 * @param tree
 */
void deleteCase2(Node *node, Node *child, RBTree *tree)
{
    void *data = node->data;
    node->data = child->data;
    child->data = data;
    freeNode(child, node, tree);
}

/**
 *
 * @param node
 * @param parent
 * @return
 */
Node *findBrother(const Node *node, const Node *parent)
{
    if (!parent)
    {
        return NULL;
    }
    if (parent->right == node)
    {
        return parent->left;
    }
    else
    {
        return parent->right;
    }
}

/**
 * subcase 1 of case B in delete
 * @param bro - brother
 * @param parent
 */
void deleteSubCase1(Node *bro, Node *parent)
{
    parent->color = BLACK;
    bro->color = RED;
}

/**
 * subcase of case C in fix-tree after delete
 * @param bro
 * @param parent
 * @param tree
 */
void deleteSubCase3(Node *bro, Node *parent, RBTree *tree)
{
    bro->color = BLACK;
    parent->color = RED;
    if (parent->right == bro)
    {
        rotateLeft(parent, tree);
    }
    else
    {
        rotateRight(parent, tree);
    }
}

/**
 *
 * @param bro
 * @param parent
 * @return
 */
Node *findCloseCousin(const Node* bro, const Node* parent)
{
    if (parent->right == bro)
    {
        return bro->left;
    }
    else
    {
        return bro->right;
    }
}

/**
 * suncase of case D in delete
 * @param cousin
 * @param bro
 * @param tree
 */
void deleteSubCase4(Node* cousin, Node* bro, RBTree *tree)
{
    cousin->color = BLACK;
    bro->color = RED;
    if (bro->parent->right == bro)
    {
        rotateRight(bro, tree);
    }
    else
    {
        rotateLeft(bro, tree);
    }
}

/**
 * case E
 * @param farCousin
 * @param bro
 * @param parent
 * @param tree
 */
void deleteSubCase5(Node *farCousin, Node *bro, Node *parent, RBTree *tree)
{
    Color x = parent->color;
    parent->color = bro->color;
    bro->color = x;
    if (parent->right == bro)
    {
        rotateLeft(parent, tree);
    }
    else
    {
        rotateRight(parent, tree);
    }
    farCousin->color = BLACK;
}

/**
 *
 * @param bro
 * @param parent
 * @param tree
 */
void deleteCaseB(Node* bro, Node* parent, RBTree* tree)
{
    if (parent->color == RED)
    {
        deleteSubCase1(bro, parent);
        return;
    }
    else
    {
        bro->color = RED;
        Node* newBro = findBrother(parent, parent->parent);
        deleteCase3FixTree(parent, newBro, tree);
    }
}

/**
 * all case 3 in delete
 * @param c
 * @param bro
 * @param tree
 */
void deleteCase3FixTree(Node* c, Node* bro, RBTree* tree)
{
    if (c == tree->root) //case A
    {
        return;
    }
    Node* parent = bro->parent;
    if (bro->color == BLACK && (!bro->left || bro->left->color == BLACK) //case B
        && (!bro->right || bro->right->color == BLACK))
    {
        deleteCaseB(bro, parent, tree);
        return;
    }
    if (bro->color == RED) //CASE C
    {
        deleteSubCase3(bro, parent, tree);
        bro = findBrother(c, parent);
        deleteCase3FixTree(c, bro, tree);
        return;
    }
    Node* cousin = findCloseCousin(bro, parent);
    Node* farCousin = findBrother(cousin, bro);
    if (bro->color == BLACK && (cousin != NULL && cousin->color == RED) && //CASE D
        (!farCousin || farCousin->color == BLACK))
    {
        deleteSubCase4(cousin, bro, tree);
    }
    bro = findBrother(c, parent);
    cousin = findCloseCousin(bro, parent);
    farCousin = findBrother(cousin, bro);
    if (bro->color == BLACK && (farCousin != NULL && farCousin->color == RED)) //case E
    {
        deleteSubCase5(farCousin, bro, parent, tree);
    }
}

/**
 * go to case 3
 * @param node
 * @param parent
 * @param tree
 */
void deleteCase3(Node* node, Node* parent, RBTree *tree)
{
    Node *bro = findBrother(node, parent);
    freeNode(node, parent, tree);
    deleteCase3FixTree(NULL, bro, tree);
}

/**
 * all delete cases
 * @param node
 * @param tree
 */
void deleteNodeAndFixTree(Node* node, RBTree* tree)
{
    if (node->left != NULL && node->right != NULL)
    {
        node = findSuccessor(node, tree);
    }
    Node* parent = node->parent;
    Node* child = findChild(node);
    if (node->color == RED)
    {
        freeNode(node, parent, tree);
        return;
    }
    if (child != NULL && child->color == RED)
    {
        deleteCase2(node, child, tree);
        return;
    }
    //M is black, C is Black
    deleteCase3(node, parent, tree);
}

/**
 *
 * @param tree
 * @param data
 * @return
 */
int deleteFromRBTree(RBTree* tree, void *data)
{
    if (tree == NULL || tree->root == NULL)
    {
        return DELETE_NODE_FAILURE;
    }
    Node* leaf = findBinaryLeafToDelete(tree->root, tree->compFunc, data);
    if (leaf == NULL)
    {
        return DELETE_NODE_FAILURE;
    }
    deleteNodeAndFixTree(leaf, tree);
    tree->size--;
    return DELETE_NODE_SUCCESS;
}

int RBTreeContains(const RBTree *tree, const void *data)
{
    return 0;
}

int intcompare(const void *a, const void *b)
{
    int *x = (int*)a;
    int *y = (int*)b;
    return *x - *y;
}


//char *intToString(void *data)
//{
//    int *d = (int*)data;
//    char* string = (char*)calloc(10, sizeof(char));
//    sprintf(string, "%d", *d);
//    return string;
//}


//int main()
//{
//    int arr[100] = {0};
//    for (int i = 0; i<100; i++)
//    {
//        arr[i] = i + 1;
//    }
//    RBTree* tree = newRBTree(intcompare, freeFunction);
//    for (int j = 0; j <25; j++)
//    {
//        insertToRBTree(tree, &arr[j]);
//    }
//    printRBTree(tree->root);
//    deleteFromRBTree(tree, &arr[6]);
//    viewTree(tree, intToString);
//    printf("%lu", tree->size);
//    insertToRBTree(tree, &arr[19]);
//    insertToRBTree(tree, &arr[15]);
//    insertToRBTree(tree, &arr[17]);
//    insertToRBTree(tree, &arr[17]);
//    insertToRBTree(tree, &arr[16]);
//    insertToRBTree(tree, &arr[19]);
//    insertToRBTree(tree, &arr[17]);
//    assert(isValidRBTree(tree));
//}

