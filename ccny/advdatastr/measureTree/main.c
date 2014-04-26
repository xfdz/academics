/*
 * File:   main.c
 * Author: Javier Fernandez
 *
 * Created on October 27, 2010, 12:59 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include "measure_tree.h"
#include <limits.h>

typedef struct b_tr_dst{
    int number;
    int ref_count;
}btree_data;


/* helper functions */
void print_mtree(m_tree_t * tr, char w);
void print_mtree_nodeInfo(m_tree_t * tr, char w);
void print_btree( b_tree_t *tr , char w);
void print_btree_nodeInfo( b_tree_t *tr, char w);
void free_mtree_mem(m_tree_t *tr);
void print_mtree_debug(m_tree_t *tr, char w );
void print_mtree_nodeInfo_debug(m_tree_t * tr, char w);


/* test functions */
void test_create_m_tree();
void test_insert_one_small_set();
void test_insert_one_large_set();
void test_integration_measure_tree();
void test_insert_two_set_with_gap();
void test_bintree_insert_set();
void free_btree_mem(b_tree_t *tr);
void test_bintree_insert_set_with_repeats();
void test_bintree_delete_nodes();
void test_bintree_delete_nodes_single_repeats();
void test_bintree_delete_all_nodes_of_repeats();
void test_bintree_find_max();
void test_bintree_find_min();
void test_insert_sets_of_duplicate_with_gap();
void test_insert_sets_of_duplicate_consecutively_with_gap();
void test_integration_test();
void test_insert_two_set_with_no_gap();
void test_insert_two_set_with_gap_backwards();
void test_insert_one_small_set_and_one_large_set_overlap();
void test_insert_one_large_set_and_one_small_set_overlap();
void test_delete_two_set_with_gap();
void test_insert_two_three_with_gap_and_delete_last_two_set();
void test_delete_wide_interval_and_several_inner_intervals();
void test_insert_sets_with_multiple_overlaps();
void test_insert_single_large_interval();

char *interior = "INTERIOR";
char *leaf = "LEAF";
char parent_c = 'P';

int main() {
    
    /* tests mtree*/
    //test_create_m_tree();
    //test_insert_one_small_set();
    //test_insert_one_large_set();
    //test_insert_two_set_with_gap();
    //test_insert_two_set_with_no_gap();
    //test_insert_sets_of_duplicate_with_gap();
    //test_insert_two_set_with_gap_backwards();
    //test_insert_sets_of_duplicate_consecutively_with_gap();
    //test_insert_one_small_set_and_one_large_set_overlap();
    //test_insert_one_large_set_and_one_small_set_overlap();
    //test_delete_two_set_with_gap();
    //test_insert_two_three_with_gap_and_delete_last_two_set();
    //test_delete_wide_interval_and_several_inner_intervals();
     //test_insert_sets_with_multiple_overlaps();




    test_integration_test();

     //test_insert_single_large_interval();



    /* tests btree */
    //test_bintree_insert_set();
    //test_bintree_find_max();
    //test_bintree_find_min();
    //test_bintree_insert_set_with_repeats();
    //test_bintree_delete_nodes();
    //test_bintree_delete_nodes_single_repeats();
    //test_bintree_delete_all_nodes_of_repeats();
    

    return (EXIT_SUCCESS);
}


void test_create_m_tree(){
    printf("\n1: test_create_m_tree:\n");
    m_tree_t *result = create_m_tree();
    if( result == NULL) {
        printf("\n [ FAILED ]"); exit(1);
    }

    print_mtree( result, parent_c );
    printf("\n [ PASSED ]\n");
}

void test_insert_one_small_set(){
    printf("\n insert range: [ 1 - 2 ] \n");
    m_tree_t *tree = create_m_tree();
    insert_interval(tree, 1, 2);
    print_mtree(tree, parent_c);
    //free_mtree_mem(tree);
}


void test_insert_one_large_set()
{
    printf("\n insert range: [ 0 - 100 ] \n");
    m_tree_t *tree = create_m_tree();
    insert_interval(tree, 0, 100);
    print_mtree(tree, parent_c);   

}

void test_insert_one_small_set_and_one_large_set_overlap(){
    printf("\n insert range: [2 - 3], then [0 - 4]");
    m_tree_t *tree = create_m_tree();
    insert_interval(tree, 2,3 );
    insert_interval(tree, 0,4 );
    print_mtree(tree, parent_c);

    printf("\n measure : (%d)", query_length(tree));

}

void test_insert_single_large_interval(){
    printf("\n insert range: [0-100]");
    m_tree_t *tree = create_m_tree();
    insert_interval(tree, 0,100 );
    print_mtree(tree, parent_c);

}


void test_insert_one_large_set_and_one_small_set_overlap(){
    printf("\n insert range: [0 - 4], then [ 2 - ]");
    m_tree_t *tree = create_m_tree();
     insert_interval(tree, 0,4 );
    insert_interval(tree, 2,3 );
    print_mtree(tree, parent_c);

    printf("\n measure : (%d)", query_length(tree));

}


void test_insert_two_set_with_gap(){
    printf("\n insert range: [ 1 - 2 ] and [ 3 - 4 ] ");
    m_tree_t *tree = create_m_tree();
    insert_interval(tree, 1, 2);
    insert_interval(tree, 3, 4);
    print_mtree(tree, parent_c);
    //free_mtree_mem(tree);
}

void test_delete_two_set_with_gap(){
    printf("\n insert range: [ 1 - 2 ] and [ 3 - 4 ] ");
    m_tree_t *tree = create_m_tree();
    insert_interval(tree, 1, 2);
    insert_interval(tree, 3, 4);
    print_mtree(tree, parent_c);

    printf("\n delete range: [ 1 - 2 ]\n");
    delete_interval(tree, 1, 2);
    delete_interval(tree, 3, 4);
    print_mtree(tree, parent_c);
    //free_mtree_mem(tree);
}

void test_insert_two_three_with_gap_and_delete_last_two_set(){
    printf("\n insert range: [ 1 - 2 ] and [ 3 - 4 ] and [ 5 - 6]\n");
    m_tree_t *tree = create_m_tree();
    insert_interval(tree, 1, 2);
    insert_interval(tree, 3, 4);
    insert_interval(tree, 5, 6);

    printf("\n delete range: [ 3 - 4 ] and [ 5 - 6]\n");
    delete_interval(tree,3,4);
    delete_interval(tree,5,6);


    print_mtree(tree, parent_c);
    //free_mtree_mem(tree);
}

void test_delete_wide_interval_and_several_inner_intervals(){
printf("\n insert range: [ 1 - 2 ] and [ 3 - 4 ] and [ 5 - 6] and [ 1- 6 ]\n");
    m_tree_t *tree = create_m_tree();
    insert_interval(tree, 1, 2);
    insert_interval(tree, 3, 4);
    insert_interval(tree, 5, 6);
    insert_interval(tree, 1, 6);

    printf("\n delete interval: [5 - 6] and [3 - 4] \n");
    delete_interval(tree, 5,6);
    delete_interval(tree, 3,4);
    delete_interval(tree, 1,6);
    print_mtree(tree, parent_c);
}


void test_insert_two_set_with_gap_backwards(){
    printf("\n insert range: [ 3 - 4] and [ 2 - 1 ] ");
    m_tree_t *tree = create_m_tree();
    insert_interval(tree, 4, 3);
    insert_interval(tree, 2, 1);
    print_mtree(tree, parent_c);
    //free_mtree_mem(tree);
}


void test_insert_two_set_with_no_gap(){
    printf("\n insert range: [ 1 - 2 ] and [ 2 - 3 ] ");
    m_tree_t *tree = create_m_tree();
    insert_interval(tree, 1, 2);
    insert_interval(tree, 2, 3);
    print_mtree(tree, parent_c);
    //free_mtree_mem(tree);
}

void test_insert_sets_with_multiple_overlaps(){
    printf("\n insert interval [0-1] and [2-3] and [0-100] and [3-98]");
    m_tree_t *tree = create_m_tree();
    insert_interval(tree,0,1);
    insert_interval(tree,2,3);
    insert_interval(tree,0,100);
    insert_interval(tree,3,98);
    insert_interval(tree,98,99);
    print_mtree(tree, parent_c);

}

void test_insert_sets_of_duplicate_with_gap(){
    printf(
     "\ninsert range: [ 1 - 2 ] and [ 3 - 4 ], then [ 1- 2 ] and [ 3 -4 ]"
    );
    m_tree_t *tree = create_m_tree();
    insert_interval(tree, 1, 2);
    insert_interval(tree, 3, 4);
    insert_interval(tree, 1, 2);
    insert_interval(tree, 3, 4);
    print_mtree(tree, parent_c);
    //free_mtree_mem(tree);

}


void test_insert_sets_of_duplicate_consecutively_with_gap(){
      printf(
     "\ninsert range: [ 1 - 2 ] and [ 1 - 2 ], then [ 3 - 4 ] and [ 3 -4 ]"
    );
    m_tree_t *tree = create_m_tree();
    insert_interval(tree, 1, 2);
    insert_interval(tree, 1, 2);
    insert_interval(tree, 3, 4);
    insert_interval(tree, 3, 4);
    print_mtree(tree, parent_c);

}

void print_mtree(m_tree_t *tr, char w ){
/* recursivly walks the tree in pre-order*/
    print_mtree_nodeInfo(tr, w);
    if (tr->pright != NULL) {
        print_mtree(tr->pleft,'L');
        print_mtree(tr->pright, 'R');
    }
}

void print_mtree_nodeInfo(m_tree_t * tr, char w){
/* prints node's fields */
    char *which = (tr->pright == NULL) ? leaf : interior;
    if( which == leaf){
        printf("\n\t");
    }
    else{
        printf("\n");
    }
    
    printf("[ %s (%c) ]\n\t\tkey(%d),  m(%d),  l(%d),  r(%d),  lm(%d),  rm(%d)\n",
            which, w, tr->key, tr->measure, tr->left, tr->right, tr->left_min, tr->right_max);
}

void print_mtree_nodeInfo_debug(m_tree_t * tr, char w){
/* prints node's fields */
    char *which = (tr->pright == NULL) ? leaf : interior;
    
    if( which == leaf){
        printf("\n\t");
        if( w == 'L' && tr->right == INT_MAX){
            //printf("[ %s (%c) ]\n\t\tkey(%d),  m(%d),  l(%d),  r(%d),  lm(%d),  rm(%d)\n",
            //which, w, tr->key, tr->measure, tr->left, tr->right, tr->left_min, tr->right_max);
            //return;
        }
    }
    else{
        printf("\n");
    }

    printf("[ %s (%c) ]\n\t\tkey(%d),  m(%d),  l(%d),  r(%d),  lm(%d),  rm(%d)\n",
           which, w, tr->key, tr->measure, tr->left, tr->right, tr->left_min, tr->right_max);
}

void print_mtree_debug(m_tree_t *tr, char w ){
/* recursivly walks the tree in pre-order*/
    print_mtree_nodeInfo_debug(tr, w);
    if (tr->pright != NULL) {
        print_mtree_debug(tr->pleft,'L');
        print_mtree_debug(tr->pright, 'R');
    }
}

void test_bintree_insert_set(){

    b_tree_t *b_tree = NULL;
    int measure_set[] = {1,2,3,4};
    b_tree = _bintree_get_node();

    printf("\ninserting set {1,2,3,4} in to binary tree\n");
    for(int i =0; i != 4; ++i){
        int result =_bintree_insert_node( b_tree, measure_set[i]);
        if(result == UNINITIALIZED ){
            printf("ERROR: inserting node in to tree");
            exit(1);
        }
    }

    print_btree( b_tree , parent_c);
    free_btree_mem(b_tree);

}

void test_bintree_insert_set_with_repeats(){

    b_tree_t *b_tree = NULL;
    int measure_set[] = {1,1,2,2,3,3,4,4};
    b_tree = _bintree_get_node();

    printf("\ninserting set {1,1,2,2,3,3,4,4}; in to binary tree\n");
    for(int i =0; i != 8; ++i){
        int result =_bintree_insert_node( b_tree, measure_set[i]);
        if(result == UNINITIALIZED ){
            printf("ERROR: inserting node in to tree");
            exit(1);
        }
    }

    print_btree( b_tree , parent_c);
    free_btree_mem(b_tree);
}


void test_bintree_delete_nodes() {

    b_tree_t *b_tree = NULL;
    int measure_set[] = {1,2,3,4};
    b_tree = _bintree_get_node();

    printf("\ninserting set {1,2,3,4} in to binary tree\n");
    for(int i =0; i != 4; ++i){
        int result =_bintree_insert_node( b_tree, measure_set[i]);
        if(result == UNINITIALIZED ){
            printf("ERROR: inserting node in to tree");
            exit(1);
        }
    }

    printf("\n\n **** Tree with full set *****\n");
    print_btree( b_tree , parent_c);

    printf("\n\n **** Begin deletions *****\n");
    for(int i=1; i !=5; ++i){
        int result = _bintree_delete_node( b_tree, i );
        if( result == UNINITIALIZED )
            //perror("ERROR: _bintree_delete_node() failed");
        printf("\nDeleted \t [ %d ]\n", i);
        print_btree( b_tree , parent_c );
    }

    printf("\n\n Nodes Remaining\n");
    print_btree( b_tree , parent_c);
}

void test_bintree_delete_nodes_single_repeats() {

    b_tree_t *b_tree = NULL;
    int measure_set[] = {1,1,2,2,3,3,4,4};
    b_tree = _bintree_get_node();

    printf("\ninserting set {1,1,2,2,3,3,4,4}; in to binary tree\n");
    for(int i =0; i != 8; ++i){
        int result =_bintree_insert_node( b_tree, measure_set[i]);
        if(result == UNINITIALIZED ){
            printf("ERROR: inserting node in to tree");
            exit(1);
        }
    }

    printf("\n\n **** Tree with full set *****\n");
    print_btree( b_tree , parent_c );

    printf("\n\n **** Begin deletions *****\n");
    for(int i=1; i !=5; ++i){
        int result = _bintree_delete_node( b_tree, i );
        if( result == UNINITIALIZED )
            //perror("ERROR: _bintree_delete_node() failed");
        printf("\nDeleted \t [ %d ]\n", i);
        print_btree( b_tree , parent_c );
    }

    printf("\n\n Nodes Remaining\n");
    print_btree( b_tree , parent_c);
}


void test_bintree_delete_all_nodes_of_repeats() {

    b_tree_t *b_tree = NULL;
    int measure_set[] = {1,1,2,2,3,3,4,4};
    b_tree = _bintree_get_node();

    printf("\ninserting set {1,1,2,2,3,3,4,4}; in to binary tree\n");
    for(int i =0; i != 8; ++i){
        int result =_bintree_insert_node( b_tree, measure_set[i]);
        if(result == UNINITIALIZED ){
            printf("ERROR: inserting node in to tree");
            exit(1);
        }
    }

    printf("\n\n **** Tree with full set *****\n");
    print_btree( b_tree , parent_c );

    printf("\n\n **** Begin deletions *****\n");
    for(int i=1; i !=5; ++i){
        int result = _bintree_delete_node( b_tree, i );
        if( result == UNINITIALIZED )
            //perror("ERROR: _bintree_delete_node() failed");
        printf("\nDeleted \t [ %d ]\n", i);
        print_btree( b_tree  , parent_c);
    }
     for(int i=1; i !=5; ++i){
        int result = _bintree_delete_node( b_tree, i );
        if( result == UNINITIALIZED )
            //perror("ERROR: _bintree_delete_node() failed");
        printf("\nDeleted \t [ %d ]\n", i);
        print_btree( b_tree , parent_c );
    }

    printf("\n\n Nodes Remaining\n");
    print_btree( b_tree , parent_c);
}

void test_bintree_find_max(){
     b_tree_t *b_tree = NULL;
    int measure_set[] = {1,1,2,2,3,3,4,4};
    b_tree = _bintree_get_node();

    printf("\ninserting set {1,1,2,2,3,3,4,4}; in to binary tree\n");
    for(int i =0; i != 8; ++i){
        int result =_bintree_insert_node( b_tree, measure_set[i]);
        if(result == UNINITIALIZED ){
            printf("ERROR: inserting node in to tree");
            exit(1);
        }
    }

    printf("\n\n **** Tree with full set *****\n");
    print_btree( b_tree , parent_c);

    int max = _bintree_find_max(b_tree);
    if( max == UNINITIALIZED)
        //perror("ERROR: _bintree_find_max() failed");
    printf("\n [ MAX NUMBER ] => %d\n", max);

    free_btree_mem(b_tree);
}

void test_bintree_find_min(){
     b_tree_t *b_tree = NULL;
    int measure_set[] = {1,1,2,2,3,3,4,4};
    b_tree = _bintree_get_node();

    printf("\ninserting set {1,1,2,2,3,3,4,4}; in to binary tree\n");
    for(int i =0; i != 8; ++i){
        int result =_bintree_insert_node( b_tree, measure_set[i]);
        if(result == UNINITIALIZED ){
            printf("ERROR: inserting node in to tree");
            exit(1);
        }
    }

    printf("\n\n **** Tree with full set *****\n");
    print_btree( b_tree , parent_c);

    int min = _bintree_find_min(b_tree);
    if( min == UNINITIALIZED)
        //perror("ERROR: _bintree_find_max() failed");
    printf("\n [ MIN NUMBER ] => %d\n", min);
    
    free_btree_mem(b_tree);

}


void print_btree( b_tree_t *tr, char w){
    print_btree_nodeInfo( tr , w);
    if( tr->right != NULL){
        print_btree(tr->left, 'L');
        print_btree(tr->right, 'R');
    }
}

void print_btree_nodeInfo( b_tree_t *tr, char w){
    int *obj = (int *) tr->left;
    if( tr->right != NULL ){
        printf( "\n[ INTERIOR (%c) ]\n\tkey(%d), ref_count(%d)\n",
                w ,tr->key, tr->ref_count );
    }
    else{
        printf( "\n[ LEAF (%c) ]\n\tkey(%d), object(%d), ref_count(%d)\n",
                w, tr->key, *obj, tr->ref_count );
    }
}


void free_btree_mem(b_tree_t *tr){
    if( tr->right != NULL){
        free_btree_mem(tr->left);
        free_btree_mem(tr->right);
    }
    _bintree_return_node(tr);
}


void free_mtree_mem(m_tree_t *tr){
    //if( tr->pright != NULL){
        //free_mtree_mem(tr->left);
        //free_mtree_mem(tr->right);
    //}
    //free_btree_mem( (b_tree_t *) tr->left);
    //_return_mtree_node(tr);
}



void test_integration_test()
{

	int             i;
	m_tree_t       *t;;
	printf("starting \n");
	t = create_m_tree();

	for (i = 0; i < 50; i++) insert_interval(t, 2 * i, 2 * i + 1);
	printf("inserted first 50 intervals, total length is %d, should be 50.\n", query_length(t));

        insert_interval(t, 0, 100);
	printf("inserted another interval, total length is %d, should be 100.\n", query_length(t));

	for (i = 1; i < 50; i++)
		insert_interval(t, 199 - (3 * i), 200);	/* [52,200] is longest */
	printf("inserted further 49 intervals, total length is %d, should be 200.\n", query_length(t));

	for (i = 2 ; i < 50; i++)
		delete_interval(t, 2 * i, 2 * i + 1);
	delete_interval(t, 0, 100);
	printf("deleted some intervals, total length is %d, should be 150.\n", query_length(t));

        
      
        insert_interval(t, 1, 2);
        
	for (i = 49; i > 0; i--)
		delete_interval(t, 199 - (3 * i), 200);
        
    
    

        

	insert_interval(t, 0, 2);
	insert_interval(t, 1, 5);
	printf("deleted some intervals, total length is %d, should be 5.\n", query_length(t));

       


	insert_interval(t, 0, 100);
	printf("inserted another interval, total length is %d, should be 100.\n", query_length(t));

	for (i = 0; i <= 3000; i++)
		insert_interval(t, 2000 + i, 3000 + i);
	printf("inserted 3000 intervals, total length is %d, should be 4100.\n", query_length(t));

	for (i = 0; i <= 3000; i++)
		delete_interval(t, 2000 + i, 3000 + i);
	printf("deleted 3000 intervals, total length is %d, should be 100.\n", query_length(t));
    


	for (i = 0; i <= 100; i++)
	    insert_interval(t, 10 * i, 10 * i + 100);
	printf("inserted another 100 intervals, total length is %d, should be 1100.\n", query_length(t));

        

        delete_interval(t, 1, 2);  // bye
        delete_interval(t, 0, 2);  // bye*
	delete_interval(t, 2, 3);

	delete_interval(t, 0, 1);
	delete_interval(t, 1, 5); //bye

   

	printf("deleted some intervals, total length is %d, should be still 1100.\n", query_length(t));

	for (i = 0; i <= 100; i++)
		delete_interval(t, 10 * i, 10 * i + 100);

	delete_interval(t, 0, 100);
	printf("deleted last interval, total length is %d, should be 0.\n", query_length(t));

}
