#ifndef AVLTREE_H
#define AVLTREE_H

#include <stdlib.h>
#include <stdint.h>

typedef int32_t (*AvlTreeCmpFn)(void *data0, void *data1);
typedef void (*AvlTreeFreeFn)(void *data);

typedef struct _sAvlTree {
  struct _sAvlNode {
    void *data;
    struct _sAvlNode *left;
    struct _sAvlNode *right;
    uint8_t height;
  } *root;
} AvlTree;

/**
 * Initialize empty AvlTree
 */
void AvlTree_init(AvlTree *tree);
/**
 * Insert node to avl tree using compare function.
 * * Data couldn`t be NULL pointer.
 *
 * Return 0 if insert is success and != 0 if problem with memory allocation
 */
int32_t AvlTree_insert(AvlTree *tree, void *data, AvlTreeCmpFn cmp);
/**
 * Find element of tree using cmp function.
 * First argument of callback function is current node, second data for compare.
 * Function must return -1 if data0 > key, 1 if data0 < key, 0 if data0 == key
 *
 * Function return NULL if function could not find node.
 */
void *AvlTree_find(AvlTree *tree, void *key, AvlTreeCmpFn cmp);
/**
 * Remove element of tree using cmp function.
 * First argument of callback function is current node, second data for compare.
 * Function must return -1 if data0 > key, 1 if data0 < key, 0 if data0 == key
 */
void AvlTree_delete(AvlTree *tree, void *key, AvlTreeCmpFn cmp,
                    AvlTreeFreeFn free_data);
/**
 * Remove all elements of tree
 */
void AvlTree_clean(AvlTree *tree, AvlTreeFreeFn free_data);

/**
 * For balanset b-tree with height 120
 */
#define MAX_DEPTH_SIZE 120

typedef struct _sAvlTreeIter {
  struct _sAvlNode *stack[MAX_DEPTH_SIZE];
  int32_t stack_head;
  struct _sAvlNode *current;
} AvlTreeIter;

/**
 * Get iterator for tree. Return NULL if cannot allocate memory.
 */
AvlTreeIter *AvlTree_iter_init(AvlTreeIter *it, AvlTree *tree);
/**
 * Return data for next node. If it's over, it will return NULL.
 */
void *AvlTreeIter_next(AvlTreeIter *it);

#endif /* AVLTREE_H */
