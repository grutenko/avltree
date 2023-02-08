#include <stdlib.h>
#include "avltree.h"

void AvlTree_init( AvlTree *tree )
{
  tree->root = NULL;
}

#define AVL_FIXED_HEIGHT(node) ((((node)->left ? (node)->left->height : 0) > ((node)->right ? (node)->right->height : 0) \
    ? ((node)->left ? (node)->left->height : 0) : ((node)->right ? (node)->right->height : 0)) + 1)

#define AVL_BFACTOR(node) (((node)->right ? (node)->right->height : 0) - ((node)->left ? (node)->left->height : 0))

static inline struct _sAvlNode *__AvlTree_rotate_left( struct _sAvlNode *q )
{
  struct _sAvlNode *p = q->right;
	q->right = p->left;
	p->left = q;
  q->height = AVL_FIXED_HEIGHT(q);
  p->height = AVL_FIXED_HEIGHT(p);
	return p;
}

static inline struct _sAvlNode *__AvlTree_rotate_right( struct _sAvlNode *p )
{
  struct _sAvlNode *q = p->left;
  p->left = q->right;
  q->right = p;
  p->height = AVL_FIXED_HEIGHT(p);
  q->height = AVL_FIXED_HEIGHT(q);
  return q;
}

static struct _sAvlNode *__AvlTree_balance( struct _sAvlNode *node )
{
  node->height = AVL_FIXED_HEIGHT(node);
  int bfactor = AVL_BFACTOR(node);
  if( bfactor == 2 ) {
    if( AVL_BFACTOR(node->right) < 0 ) {
      node->right = __AvlTree_rotate_right(node->right);
    }
		return __AvlTree_rotate_left(node);
  } else if( bfactor == -2 ) {
    if( AVL_BFACTOR(node->left) > 0 ) {
      node->left = __AvlTree_rotate_left(node->left);
    }
		return __AvlTree_rotate_right(node);
  }
  return node;
}

static struct _sAvlNode *__AvlTree_init_node( void *data )
{
  struct _sAvlNode *node = malloc( sizeof(struct _sAvlNode) );
  if( !node ) {
    return NULL;
  }
  node->data = data;
  node->height = 0;
  node->left = NULL;
  node->right = NULL;
  return node;
}

static struct _sAvlNode *__AvlTree_insert( struct _sAvlNode *node, void *data, AvlTreeCmpFn cmp, int *err )
{
  if( !node ) {
    struct _sAvlNode *new_node = __AvlTree_init_node( data );
    if( !new_node ) {
      *err = -1;
    } else {
      *err = 0;
    }
    return new_node;
  }
  int cmp_result = cmp( node->data, data );
  if( cmp_result >= 0 ) {
    node->left = __AvlTree_insert( node->left, data, cmp, err );
  } else {
    node->right = __AvlTree_insert( node->right, data, cmp, err );
  }
  return __AvlTree_balance( node );
}

int AvlTree_insert( AvlTree *tree, void *data, AvlTreeCmpFn cmp )
{
  int err = 0;
  tree->root = __AvlTree_insert( tree->root, data, cmp, &err );
  return err;
}

static void *__AvlTree_find( struct _sAvlNode *node, void *key, AvlTreeCmpFn cmp )
{
  if( !node ) {
    return NULL;
  }
  int cmp_result = cmp( node->data, key );
  if( cmp_result > 0 ) {
    return __AvlTree_find( node->left, key, cmp );
  } else if( cmp_result < 0 ) {
    return __AvlTree_find( node->right, key, cmp );
  } else {
    return node->data;
  }
}

void *AvlTree_find( AvlTree *tree, void *key, AvlTreeCmpFn cmp )
{
  return __AvlTree_find( tree->root, key, cmp );
}

static struct _sAvlNode *__AvlTree_leftmost_node( struct _sAvlNode *node )
{
  struct _sAvlNode *current = node;
  while( current->left ) {
    current = node->left;
  }
  return current;
}

static struct _sAvlNode *__AvlTree_leftmost_remove( struct _sAvlNode *node, AvlTreeCmpFn cmp )
{
  if( !node->left ) {
    return node->right;
  }
	node->left = __AvlTree_leftmost_remove( node->left, cmp );
	return __AvlTree_balance( node );
}

static struct _sAvlNode *__AvlTree_delete(
  struct _sAvlNode *node, void *key, AvlTreeCmpFn cmp, AvlTreeFreeFn free_data )
{
  if( !node ) {
    return NULL;
  }
  int cmp_result = cmp( node->data, key );
  if( cmp_result > 0 ) {
    node->left = __AvlTree_delete( node->left, key, cmp, free_data );
  } else if( cmp_result < 0 ) {
    node->right = __AvlTree_delete( node->right, key, cmp, free_data );
  } else {
    struct _sAvlNode *left = node->left;
    struct _sAvlNode *right = node->right;
    free_data( node->data );
    free( node );

    if( !right ) {
      return left;
    }
    struct _sAvlNode *leftmost = __AvlTree_leftmost_node( right );
    leftmost->right = __AvlTree_leftmost_remove( right, cmp );
    leftmost->left = left;
    return __AvlTree_balance( leftmost );
  }
  return __AvlTree_balance( node );
}

void AvlTree_delete( AvlTree *tree, void *key, AvlTreeCmpFn cmp, AvlTreeFreeFn free_data )
{
  tree->root = __AvlTree_delete( tree->root, key, cmp, free_data );
}

static void __AvlTree_clean( struct _sAvlNode *node, AvlTreeFreeFn free_data )
{
  if( node->left ) {
    __AvlTree_clean( node->left, free_data );
  }
  if( node->right ) {
    __AvlTree_clean( node->right, free_data );
  }
  free_data( node->data );
  free( node );
}

void AvlTree_clean( AvlTree *tree, AvlTreeFreeFn free_data )
{
  if( !tree->root ) {
    return;
  }
  __AvlTree_clean( tree->root, free_data );
}

#define IT_STACK_PUSH(it, n) ((it)->stack[++it->stack_head] = (n))

#define IT_STACK_POP(it) ((it)->stack_head == -1 ? NULL : (it)->stack[(it)->stack_head--] )

#define IT_STACK_HEAD(it) ((it)->stack_head == -1 ? NULL : (it)->stack[(it)->stack_head] )

#define IT_STACK_EMPTY(it) ((it)->stack_head == -1)

AvlTreeIter *AvlTree_iter_init( AvlTreeIter *it, AvlTree *tree )
{
  if( it == NULL ) {
    return NULL;
  }
  it->stack_head = -1;
  if( tree->root != NULL ) {
    it->current = tree->root;
  }
  return it;
}

void *AvlTreeIter_next( AvlTreeIter *it )
{
  if( !it->current && IT_STACK_EMPTY(it) ) {
    return NULL;
  }

  /**
   * Lookup left branch of tree until end and push each node to stack.
   * @see https://www.geeksforgeeks.org/inorder-tree-traversal-without-recursion/
  */
  while(it->current) {
    IT_STACK_PUSH(it, it->current);
    it->current = it->current->left;
  }

  /**
   * Pop last left element of tree from stack.
   * and set current element as right branch
  */

  it->current = IT_STACK_POP(it);

  void * data = it->current->data;
  it->current = it->current->right;

  return data;
}
