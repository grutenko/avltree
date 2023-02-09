#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "avltree.h"

inline static int cmp( void *data0, void *data1 )
{
  return *(int *)data0 > *(int *)data1 ? 1 : (*(int *)data0 == *(int *)data1 ? 0 : -1);
}

void free_data( void *data ) {}

#define AVL_BFACTOR(node) (((node)->right ? (node)->right->height : 0) - ((node)->left ? (node)->left->height : 0))

int main()
{
  int table[1000000];

  AvlTree tree;
  AvlTree_init( &tree );

  clock_t start, end;
  double iops = -1;

  srand( time(NULL) ) ;

  int i;
  for( i = 0; i < 1000000; i++ ) {
    table[i] = rand() % INT16_MAX;
    start = clock();

    AvlTree_insert( &tree, (void *)&(table[i]), cmp );
    end = clock();
    if( iops > -1 ) {
      iops = iops*i/(i+1) + ((double) (end - start)) / (i+1);
    } else {
      iops = ((double) (end - start));
    }
  }

  printf("Insertion: %f i/usec \n", iops);

  iops = -1;
  for( i = 0; i < 1000000; i++ ) {
    start = clock();

    AvlTree_find( &tree, (void *)&(table[i]), cmp );
    end = clock();
    if( iops > -1 ) {
      iops = iops*i/(i+1) + ((double) (end - start)) / (i+1);
    } else {
      iops = ((double) (end - start));
    }
  }

  printf("Lookup: %f i/usec \n", iops);

  iops = -1;
  for( i = 0; i < 1000000; i++ ) {
    start = clock();

    AvlTree_delete( &tree, (void *)&(table[i]), cmp, free_data );

    end = clock();
    if( iops > -1 ) {
      iops = iops*i/(i+1) + ((double) (end - start)) / (i+1);
    } else {
      iops = ((double) (end - start));
    }
  }

  printf("Deleting: %f i/usec \n", iops);

  return EXIT_SUCCESS;
}
