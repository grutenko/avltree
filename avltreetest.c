#include <stdlib.h>
#include <stdio.h>

#include "avltree.h"

int cmp( void *data0, void *data1 )
{
  return *(char *)data0 - *(char *)data1;
}

void free_data( void *data ) {}

int main()
{
  char table[48] = {
    'A',
    'B', 'C', 'D', 'E', 'F',
    'G', 'H', 'J', 'K', 'L',
    'M', 'N', 'O', 'P', 'R',
    'S', 'T', 'U', 'V', 'W',
    'X', 'Y', 'Z',
    'a', 'b', 'c', 'd', 'e',
    'f', 'g', 'h', 'j', 'k',
    'l', 'm', 'n', 'o', 'p',
    'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z'
  };

  AvlTree tree;
  AvlTree_init( &tree );

  int i;
  for( i = 0; i < 48; i++ ) {
    AvlTree_insert( &tree, (void *)&(table[i]), cmp );
    void *item;
    int j;
    AvlTreeIter it;
    AvlTree_iter_init( &it, &tree );
    for( j = 0, item = AvlTreeIter_next( &it ); item != NULL; j++, item = AvlTreeIter_next( &it ) ) {
      if( *(char *)item != table[j] ) {
        fprintf( stderr, "Test Failure tree ordering.\n");
        return EXIT_FAILURE;
      }
    }
  }

  for( i = 0; i < 48; i++ ) {
    AvlTree_delete( &tree, &table[i], cmp, free_data );
  }

  AvlTree tree1;
  AvlTree_init( &tree1 );

  char table1[8] = { 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a' };
  for( i = 0; i < 8; i++ ) {
    AvlTree_insert( &tree1, (void *)&(table1[i]), cmp );
  }

  char a = 'a';
  AvlTree_delete( &tree1, (void *)&a, cmp, free_data );

  return EXIT_SUCCESS;
}
