#include <stdlib.h>
#include <stdio.h>
#include <check.h>

#include "avltree.h"

int cmp( void *data0, void *data1 )
{
  return *(int *)data0 - *(int *)data1;
}

void free_data( void *data ) {}

START_TEST (test_insert)
{
  int data[10];

  AvlTree tree;
  AvlTree_init( &tree );
  int i;
  for( i = 0; i < 10; i++ ) {
    data[i] = i;
    AvlTree_insert( &tree, (void*)&data[i], cmp );
    ck_assert( *(int *)AvlTree_find( &tree, (void*)&data[i], cmp ) == i );
  }
  AvlTree_clean( &tree, free_data );
}
END_TEST;

START_TEST (test_delete)
{
  int data[10];

  AvlTree tree;
  AvlTree_init( &tree );
  int i;
  for( i = 1; i < 10; i++ ) {
    data[i] = i;
    AvlTree_insert( &tree, (void*)&data[i], cmp );
    ck_assert( *(int *)AvlTree_find( &tree, (void*)&data[i], cmp ) == i );
    AvlTree_delete( &tree, (void*)&data[i], cmp, free_data );
    ck_assert( AvlTree_find( &tree, (void*)&data[i], cmp ) == NULL );
  }
  AvlTree_clean( &tree, free_data );
}
END_TEST;

START_TEST (test_valid_inorder_iteration)
{
  int data[10];
  AvlTree tree;
  AvlTree_init( &tree );
  int i;
  for( i = 0; i < 10; i++ ) {
    data[i] = i;
    AvlTree_insert( &tree, (void*)&data[i], cmp );
  }

  AvlTreeIter it;
  AvlTree_iter_init( &it, &tree );
  void *next = AvlTreeIter_next( &it );
  for(i = 0 ; next != NULL; next = AvlTreeIter_next( &it ), i++ ) {
    ck_assert( *(int *)next == data[i] );
  }
}
END_TEST;

Suite *common_suite()
{
  Suite *s;
  TCase *tc_common;

  s = suite_create( "Common testing AVL trees." );
  tc_common = tcase_create( "Inserting" );
  tcase_add_test( tc_common, test_insert );
  tcase_add_test( tc_common, test_delete );
  tcase_add_test( tc_common, test_valid_inorder_iteration );
  suite_add_tcase( s, tc_common );
  return s;
}

int main()
{
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = common_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
