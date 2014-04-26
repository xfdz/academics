/* 
 * Author: Javier Fernandez
 * CCNY-CSCI-I9604 Advanced data structure
 * Fall 2010 HW# 1
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "angularset_hbalanced_tree.h"
#include "pbrass_testplan.h"

#define MAXRANDNUM 100
#define LIST_MAXSIZE 75
#define SORTED 10

/*
 * Test for angularset_balanced_btree.c
 */
void test_insert_aset();
void test_delete_aset();

void test_count_aset_positive_range();
void test_count_aset_negative_range();

void test_first_aset_positive_range();
void test_last_aset_positive_range();

void test_first_aset_negative_range();
void test_last_aset_negative_range();



/* Helper functions */
float randfloat();
int compare (const void * a, const void * b);
void check_tree( angle_set_t *tr, int depth, float lower, float upper );

int main(int argc, char** argv) {
    test_insert_aset();
    test_delete_aset();
    test_count_aset_positive_range();
    test_count_aset_negative_range();
    test_first_aset_positive_range();
    test_last_aset_positive_range();
    test_first_aset_negative_range();
    test_last_aset_negative_range();
    test_pbrass();

    return (EXIT_SUCCESS);
}

void test_insert_aset(){
   
    srand( time(NULL) );
    float set[LIST_MAXSIZE];
    
    int i;
    /* Generate random floats*/
    for(i = 0; i != LIST_MAXSIZE; i++)
        set[i] = randfloat();

    /* Sort the set*/
    printf("\nSorting random generated float array\n");
    qsort (set, SORTED, sizeof(float), compare);

    printf("* BEGIN: List:\n *");
    /* print sorted set */
    for(i=0; i != LIST_MAXSIZE; i++)
        printf("\n%f,%d\n", set[i],i);
     printf("* END: List:\n\n *");

    /* insert set into tree */
    angle_set_t *tr = create_aset();
    for(i=0; i != LIST_MAXSIZE; i++)
        insert_aset(tr,set[i]);


    printf("* BEGIN: Tree:\n\n");
    check_tree(tr, 0, 0.0,  (float) MAXRANDNUM + 15 );
    printf("* END: Tree:\n\n");
 
    
}

void test_delete_aset(){

    object_t set[] = { 2.0, 2.99, 8.76, 10.9, 11.88, 15.01, 17.7 };

    angle_set_t *tr = create_aset();

    printf("\n*BEGIN: Inserting set into tree\n");
    /* insert set into tree */
    for(int i=0; i != 7; i++){
         printf("\n(%f,%d)", set[i],i);
        insert_aset(tr,set[i]);
    }
    printf("\n\n*END: Inserting set into tree\n\n");

    printf("* BEGIN: Tree:\n\n");
    check_tree(tr, 0, 0.0,  17.7 );
    printf("\n * END: Tree:\n\n");

    float delkey = 8.76;
    printf("* BEGIN: Delete\n\n");
    object_t *delobj = _delete(tr, delkey );
    printf("Delete %f from tree\n", delkey);
    printf("\n* END: Delete\n\n");

    if( delobj == NULL )
        printf("  delete failed for key %f\n", delkey);
    else
        printf("delete successful, object %f, height is now %d\n", 
                (float ) *delobj , tr->height);

    check_tree(tr, 0, 0.0,  17.7 );

}

void test_count_aset_positive_range(){

    float set[] = {
        2.0, 2.99, 8.76, 10.9, 11.88, 15.01, 17.7, 28.9, 
        29.9, 44.5, 44.9, 51.8, 52.2, 55.8, 57.6, 76.6
    };

    angle_set_t *tr = create_aset();

    printf("\n*BEGIN: Inserting set into tree\n");
    /* insert set into tree */
    for(int i=0; i != 16; i++){
         printf("\n(%f,%d)", set[i],i);
        insert_aset(tr,set[i]);
    }
    printf("\n\n*END: Inserting set into tree\n\n");

    printf("* BEGIN: Tree:\n\n");
    check_tree(tr, 0, 0.0,  80.0 );
    printf("\n * END: Tree:\n\n");

    int count_return = count_aset(tr, 15.01, 44.9 );

    printf("\n\nRange: [15.01,44.9[ produced: %d\n angles", count_return);
}

void test_count_aset_negative_range(){
     float set[] = {
        2.0, 2.99, 8.76, 10.9, 11.88, 15.01, 17.7, 28.9,
        29.9, 44.5, 44.9, 51.8, 52.2, 55.8, 57.6, 76.6
    };

    angle_set_t *tr = create_aset();

    printf("\n*BEGIN: Inserting set into tree\n");
    /* insert set into tree */
    for(int i=0; i != 16; i++){
         printf("\n(%f,%d)", set[i],i);
        insert_aset(tr,set[i]);
    }
    printf("\n\n*END: Inserting set into tree\n\n");

    printf("* BEGIN: Tree:\n\n");
    check_tree(tr, 0, 0.0,  80.0 );
    printf("\n * END: Tree:\n\n");

    int count_return = count_aset(tr, 44.9, 15.01 );

    printf("\n\nRange: [44.9, 15.01[  produced: %d\n angles", count_return);

}

void test_first_aset_positive_range(){

    float set[] = {
        2.0, 2.99, 8.76, 10.9, 11.88, 15.01, 17.7, 28.9,
        29.9, 44.5, 44.9, 51.8, 52.2, 55.8, 57.6, 76.6
    };

    angle_set_t *tr = create_aset();

    printf("\n*BEGIN: Inserting set into tree\n");
    /* insert set into tree */
    for(int i=0; i != 16; i++){
         printf("\n(%f,%d)", set[i],i);
        insert_aset(tr,set[i]);
    }
    printf("\n\n*END: Inserting set into tree\n\n");

    printf("* BEGIN: Tree:\n\n");
    check_tree(tr, 0, 0.0,  80.0 );
    printf("\n * END: Tree:\n\n");

    float first_angle = first_aset(tr, 15.01, 44.9 );

    printf("\n\nRange: [15.01, 44.9[  first_aset() produced angle: %f\n",
            first_angle);

}

void test_last_aset_positive_range(){
    float set[] = {
        2.0, 2.99, 8.76, 10.9, 11.88, 15.01, 17.7, 28.9,
        29.9, 44.5, 44.9, 51.8, 52.2, 55.8, 57.6, 76.6
    };

    angle_set_t *tr = create_aset();

    printf("\n*BEGIN: Inserting set into tree\n");
    /* insert set into tree */
    for(int i=0; i != 16; i++){
         printf("\n(%f,%d)", set[i],i);
        insert_aset(tr,set[i]);
    }
    printf("\n\n*END: Inserting set into tree\n\n");

    printf("* BEGIN: Tree:\n\n");
    check_tree(tr, 0, 0.0,  80.0 );
    printf("\n * END: Tree:\n\n");

    float last_angle = last_aset(tr, 15.01, 44.9 );

    printf("\n\nRange: [15.01, 44.9[  last_aset() produced angle: %f\n",
            last_angle);
}

void test_first_aset_negative_range(){
     float set[] = {
        2.0, 2.99, 8.76, 10.9, 11.88, 15.01, 17.7, 28.9,
        29.9, 44.5, 44.9, 51.8, 52.2, 55.8, 57.6, 76.6
    };

    angle_set_t *tr = create_aset();

    printf("\n*BEGIN: Inserting set into tree\n");
    /* insert set into tree */
    for(int i=0; i != 16; i++){
         printf("\n(%f,%d)", set[i],i);
        insert_aset(tr,set[i]);
    }
    printf("\n\n*END: Inserting set into tree\n\n");

    printf("* BEGIN: Tree:\n\n");
    check_tree(tr, 0, 0.0,  80.0 );
    printf("\n * END: Tree:\n\n");

    float last_angle = first_aset(tr, 44.9, 15.01 );

    printf("\n\nRange: [44.9, 15.01[  first_aset() produced angle: %f\n",
            last_angle);

}

void test_last_aset_negative_range(){
     float set[] = {
        2.0, 2.99, 8.76, 10.9, 11.88, 15.01, 17.7, 28.9,
        29.9, 44.5, 44.9, 51.8, 52.2, 55.8, 57.6, 76.6
    };

    angle_set_t *tr = create_aset();

    printf("\n*BEGIN: Inserting set into tree\n");
    /* insert set into tree */
    for(int i=0; i != 16; i++){
         printf("\n(%f,%d)", set[i],i);
        insert_aset(tr,set[i]);
    }
    printf("\n\n*END: Inserting set into tree\n\n");

    printf("* BEGIN: Tree:\n\n");
    check_tree(tr, 0, 0.0,  80.0 );
    printf("\n * END: Tree:\n\n");

    float last_angle = last_aset(tr, 44.9, 15.01 );

    printf("\n\nRange: [44.9, 15.01, [  last_aset() produced angle: %f\n",
            last_angle);
}

/* generates a psuedo-random float */
float
randfloat()
{
    return ( rand() % MAXRANDNUM) + ( (float) ( rand() % 100) / 10 );
 
}

/* std::qsort() uses this helper function to sort*/
int
compare (const void * a, const void * b){
  return ( *(float*)a - *(float*)b );
}

/* 
 * Code recycled from Peter Brass:
 * http://www-cs.ccny.cuny.edu/~peter/dstest/h_bl_tree.c
 */
void
check_tree( angle_set_t *tr, int depth, float lower, float upper ){

   if( tr->left == NULL ){
       printf("\n\nTree Empty\n"); return;
   }

   if( tr->key < lower || tr->key > upper ){
         printf("\n\nWrong Key Order:  key(%f) lower(%f) upper(%f)\n",
                 tr->key, lower, upper );
   }

   if( tr->right == NULL ){
       printf("(key: %f ,depth: %d)\n", tr->key, depth );
   }
   else{
       check_tree(tr->left, depth+1, lower, tr->key );
       check_tree(tr->right, depth+1, tr->key, upper );
   }
}
