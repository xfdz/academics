/* 
 * File:   main.c
 * Author: Javier Fernandez
 * CCNY-CSC-I9604 adv data struc
 * Fall-2010 Homework Assignment 3
 * Structure maintains linear Ordered 
*/


#include <stdio.h>
#include <stdlib.h>
#include "ordered_tree.h"


char *interior = "INTERIOR";
char *leaf = "LEAF";
char parent_c = 'P';

long p(long q);
void test_integrated();

void print_o_t(o_t *t, char w );
void print_o_t_nodeInfo(o_t * t, char w);

void print_l_o_t_list(l_o_t *t, char w);
void print_l_o_t_tree(l_o_t *t, char w);
void print_l_o_t_node_info(l_o_t *t, char w);
void print_l_o_t_node_list(l_o_t *t, char w);

void test_create_order();
void test_insert_bottom_single_value();
void test_insert_bottom_two_values();
void test_insert_bottom_three_values();
void test_insert_bottom_four_values();

void test_insert_top_single_value();
void test_insert_bottom_hundred_fifty_num();
void test_insert_top_two_values();
void test_insert_top_three_values();
void test_insert_top_four_values();
void test_insert_top_hundred_fifty_values();
void test_compare_insert_bottom_twenty_num();
void test_compare_insert_top_twenty_num();

void test_insert_bottom_twenty_values();
void test_insert_bottom_six_values();

void test_insert_bottom_delete_one_node();
void test_insert_bottom_delete_two_nodes();
void test_insert_bottom_delete_three_nodes();
void test_insert_bottom_delete_four_nodes();

void test_insert_top_delete_one_node();
void test_insert_top_delete_two_nodes();
void test_insert_top_delete_three_nodes();
void test_insert_top_delete_three_nodes();
void test_insert_top_delete_four_nodes();
void test_insert_before_small_list();
void test_insert_before_near_top();
void test_insert_before_in_middle_of_ol();
void test_insert_after_small_list();
void test_insert_after_small_list_end_of_lot();

int main(int argc, char** argv) {
    //test_create_order();
    //test_insert_bottom_single_value();
    //test_insert_bottom_two_values();
    //test_insert_bottom_three_values();
    //test_insert_bottom_four_values();
    //test_insert_bottom_hundred_fifty_num();
    //test_insert_top_four_values();
    //test_insert_top_hundred_fifty_values();
    //test_insert_bottom_twenty_values();
    //test_insert_bottom_six_values();

    
    //test_insert_top_single_value();
    //test_insert_top_two_values();
    //test_insert_top_three_values();
    
   
    //test_compare_insert_top_twenty_num();
    //test_compare_insert_bottom_twenty_num();

    //test_insert_bottom_delete_one_node();
    //test_insert_bottom_delete_two_nodes();
    //test_insert_bottom_delete_three_nodes();
    //test_insert_bottom_delete_four_nodes();

    //test_insert_top_delete_one_node();
    //test_insert_top_delete_two_nodes();
    //test_insert_top_delete_three_nodes();
    //test_insert_top_delete_four_nodes();
    //test_insert_before_small_list();
    //test_insert_before_near_top();
    //test_insert_before_in_middle_of_ol();
    //test_insert_after_small_list();
    //test_insert_after_small_list_end_of_lot();
    

    test_integrated();
    return (EXIT_SUCCESS);
}

void test_create_order(){
   o_t *t = create_order();
   print_o_t(t,'P');

}

void test_insert_top_single_value(){
    o_t *t = create_order();
    insert_top(t,27);
    print_o_t(t,'P');
}

void test_insert_top_two_values(){
    o_t *t = create_order();
    insert_top(t,99);
    insert_top(t,5);
    print_o_t(t,'P');
}

void test_insert_top_three_values(){
    o_t *t = create_order();
    insert_top(t,99);
    insert_top(t,50);
    insert_top(t,10);
    print_o_t(t,'P');
}

void test_insert_top_hundred_fifty_values(){
    o_t *t = create_order();
    for(long i=150; i>=0; i-- )
        insert_top(t,i);

    //print_o_t(t,'P');
     print_l_o_t_list(t->lot_root, 'P');

}

void test_insert_top_four_values(){
    o_t *t = create_order();
    insert_top(t,99);
    insert_top(t,50);
    insert_top(t,10);
    insert_top(t,5);
    print_o_t(t,'P');
}

void test_insert_bottom_hundred_fifty_num(){
    o_t *t = create_order();
    for(long i=1000; i>=0; i-- )
        insert_bottom(t,i);

    printf("%ld\n" ,t->lot_root->top->key);
    
    //print_o_t(t,'P');
    //printf("\n ********* Linear Ordered List ******** \n");
   //print_l_o_t_list(t->lot_root, 'P');
   //print_l_o_t_tree(t->lot_root,'P');
}

void test_compare_insert_bottom_twenty_num(){

    o_t *t = create_order();
    for(long i=19; i>=0; i-- )
    /*  bottom: 0,1,2...19 :top*/
        insert_bottom(t,i);

     //print_l_o_t_tree(t->lot_root,'P');
     //exit(0);

    int result = -1;
    for(long i=19; i>0; i--){
        result = compare(t,i-1,i);
        if( result == 0){
            printf("\n\n key(%ld) is NOT before (%ld) \n", i-1, i);
            exit(1);
        }
        else
            printf("\n key(%ld) is before (%ld)\n",i-1,i);
    }
    
}

void test_compare_insert_top_twenty_num(){
    o_t *t = create_order();
    for(long i=19; i>=0; i-- )
    /* bottom: 19,18,17...0 :top */
        insert_top(t,i);

    int result = -1;
    for(long i=19; i>0; i--){
        result = compare(t,i,i-1);
        if( result == 0){
            printf("\n\n key(%ld) is NOT before (%ld) \n", i, i-1);
            exit(1);
        }
        else
            printf("\n key(%ld) is before (%ld)\n",i,i-1);
    }

}

void test_insert_bottom_single_value(){
    o_t *t = create_order();
    insert_bottom(t,27);
    print_o_t(t,'P');

}

void test_insert_bottom_two_values(){
    o_t *t = create_order();
    insert_bottom(t,50);
    insert_bottom(t,99);
    print_o_t(t,'P');
}

void test_insert_bottom_three_values(){
    o_t *t = create_order();
    insert_bottom(t,27);
    insert_bottom(t,65);
    insert_bottom(t,108);
    print_o_t(t,'P');
    print_l_o_t_list(t->lot_root, 'P');
}

void test_insert_bottom_four_values(){
    o_t *t = create_order();
    insert_bottom(t,1);
    insert_bottom(t,2);
    insert_bottom(t,3);
    insert_bottom(t,4);

    //print_o_t(t,'P');
    //print_l_o_t_list(t->lot_root,'P');
    print_l_o_t_tree(t->lot_root,'P');
}

void test_insert_bottom_six_values(){
    o_t *t = create_order();
    insert_bottom(t,1);
    insert_bottom(t,2);
    insert_bottom(t,3);
    insert_bottom(t,4);
    insert_bottom(t,5);
    insert_bottom(t,6);

    //print_o_t(t,'P');
    //print_l_o_t_list(t->lot_root,'P');
    print_l_o_t_tree(t->lot_root,'P');
}

void test_insert_bottom_twenty_values(){
    o_t *t = create_order();
    insert_bottom(t,1);
    insert_bottom(t,2);
    insert_bottom(t,3);
    insert_bottom(t,4);

    insert_bottom(t,5);
    insert_bottom(t,6);
    insert_bottom(t,7);
    insert_bottom(t,8);

    insert_bottom(t,9);
    insert_bottom(t,10);
    insert_bottom(t,11);
    insert_bottom(t,12);

    insert_bottom(t,13);
    insert_bottom(t,14);
    insert_bottom(t,15);
    insert_bottom(t,16);

    insert_bottom(t,17);
    insert_bottom(t,18);
    insert_bottom(t,19);
    insert_bottom(t,20);

    //print_o_t(t,'P');
    //print_l_o_t_list(t->lot_root,'P');
    print_l_o_t_tree(t->lot_root,'P');
    //print_o_t(t,'P');
}

void test_insert_bottom_delete_one_node(){

    o_t *t = create_order();
    insert_bottom(t,1);
    printf("inserting single node key: %d\n\n", 1);
    printf("\n\n Before deletion:\n\n");
    print_o_t(t,'P');

    
    delete_o(t,1);
    printf("\n\n After deletion 1\n\n");
    print_o_t(t,'P');
}

void test_insert_bottom_delete_two_nodes(){
    o_t *t = create_order();
    printf("inserting single node key: %d\n\n", 1);
    printf("inserting single node key: %d\n\n", 2);
    printf("inserting single node key: %d\n\n", 3);
    insert_bottom(t,1);
    insert_bottom(t,2);
    printf("\n\n Before deletion:\n\n");
    print_o_t(t,'P');

    delete_o(t,1);
    printf("\n\n After deletion 1\n\n");
    print_o_t(t,'P');


    delete_o(t,2);
    printf("\n\n After deletion 2\n\n");
    print_o_t(t,'P');

}

void test_insert_bottom_delete_three_nodes(){
    o_t *t = create_order();
    printf("inserting single node key: %d\n\n", 1);
    printf("inserting single node key: %d\n\n", 2);
    printf("inserting single node key: %d\n\n", 3);
    insert_bottom(t,1);
    insert_bottom(t,2);
    insert_bottom(t,3);
    printf("\n\n Before deletion:\n\n");
    print_o_t(t,'P');

   
    delete_o(t,1);
    printf("\n\n After deletion 1\n\n");
    print_o_t(t,'P');

    delete_o(t,2);
    printf("\n\n After deletion 2\n\n");
    print_o_t(t,'P');
    
    delete_o(t,3);
    printf("\n\n After deletion 3\n\n");
    print_o_t(t,'P');
}

void test_insert_bottom_delete_four_nodes(){
    o_t *t = create_order();
    printf("inserting single node key: %d\n\n", 1);
    printf("inserting single node key: %d\n\n", 2);
    printf("inserting single node key: %d\n\n", 3);
    insert_bottom(t,1);
    insert_bottom(t,2);
    insert_bottom(t,3);
    insert_bottom(t,4);
    printf("\n\n Before deletion:\n\n");
    print_o_t(t,'P');


    delete_o(t,1);
    printf("\n\n After deletion 1\n\n");
    print_o_t(t,'P');

    delete_o(t,2);
    printf("\n\n After deletion 2\n\n");
    print_o_t(t,'P');

    delete_o(t,3);
    printf("\n\n After deletion 3\n\n");
    print_o_t(t,'P');

    delete_o(t,4);
    printf("\n\n After deletion 4\n\n");
    print_o_t(t,'P');
}

void test_insert_top_delete_one_node(){

    o_t *t = create_order();
    insert_top(t,1);
    
    printf("inserting single node key: %d\n\n", 1);
    printf("\n\n Before deletion:\n\n");
    print_o_t(t,'P');


    delete_o(t,1);
    printf("\n\n After deletion 1\n\n");
    print_o_t(t,'P');
}

void test_insert_top_delete_two_nodes(){
    o_t *t = create_order();
    printf("inserting single node key: %d\n\n", 1);
    printf("inserting single node key: %d\n\n", 2);
 
    insert_top(t,1);
    insert_top(t,2);
    printf("\n\n Before deletion:\n\n");
    print_o_t(t,'P');

    delete_o(t,1);
    printf("\n\n After deletion 1\n\n");
    print_o_t(t,'P');


    delete_o(t,2);
    printf("\n\n After deletion 2\n\n");
    print_o_t(t,'P');

}

void test_insert_top_delete_three_nodes(){
    o_t *t = create_order();
    printf("inserting single node key: %d\n\n", 1);
    printf("inserting single node key: %d\n\n", 2);
    printf("inserting single node key: %d\n\n", 3);
    
    insert_top(t,1);
    insert_top(t,2);
    insert_top(t,3);
    printf("\n\n Before deletion:\n\n");
    print_o_t(t,'P');


    delete_o(t,1);
    printf("\n\n After deletion 1\n\n");
    print_o_t(t,'P');

    delete_o(t,2);
    printf("\n\n After deletion 2\n\n");
    print_o_t(t,'P');

    delete_o(t,3);
    printf("\n\n After deletion 3\n\n");
    print_o_t(t,'P');
}

void test_insert_top_delete_four_nodes(){
    o_t *t = create_order();
    printf("inserting single node key: %d\n\n", 1);
    printf("inserting single node key: %d\n\n", 2);
    printf("inserting single node key: %d\n\n", 3);
    printf("inserting single node key: %d\n\n", 4);

    insert_top(t,1);
    insert_top(t,2);
    insert_top(t,3);
    insert_top(t,4);
    printf("\n\n Before deletion:\n\n");
    print_o_t(t,'P');


    delete_o(t,1);
    printf("\n\n After deletion 1\n\n");
    print_o_t(t,'P');

    delete_o(t,2);
    printf("\n\n After deletion 2\n\n");
    print_o_t(t,'P');

    delete_o(t,3);
    printf("\n\n After deletion 3\n\n");
    print_o_t(t,'P');

    delete_o(t,4);
    printf("\n\n After deletion 4\n\n");
    print_o_t(t,'P');
}

void test_delete_insert_hundred_delete_hundred(){
     //o_t *t = create_order();
     //for(int i=0; i < 100; ++i);
    
}

void test_insert_before_small_list(){
    o_t *t = create_order();
    insert_top(t,1);
    insert_top(t,2);
    insert_top(t,3);
    insert_top(t,4);
    
    printf("\n After insert_top: 1,2,3,4\n");
    print_o_t(t,'P');

    printf("\n Insert 8 before 1\n");
    insert_before(t,8,1);
    print_o_t(t,'P');


}

void test_insert_before_near_top(){

    o_t *t = create_order();
    insert_top(t,1);
    insert_top(t,2);
    insert_top(t,3);
    insert_top(t,4);

    printf("\n After insert_top: 1,2,3,4\n");
    print_o_t(t,'P');

    printf("\n Insert 8 before 4\n");
    insert_before(t,8,4);
    print_o_t(t,'P');


}

void test_insert_before_in_middle_of_ol(){

    o_t *t = create_order();
    insert_top(t,1);
    insert_top(t,2);
    insert_top(t,3);
    insert_top(t,4);

    insert_before(t,777,3);
    print_l_o_t_tree(t->lot_root,'P');
}


void test_insert_after_small_list(){
    o_t *t = create_order();
    insert_top(t,1);
    insert_top(t,2);
    insert_top(t,3);
    insert_top(t,4);

    printf("\n After insert_top: 1,2,3,4\n");
    print_l_o_t_tree(t->lot_root,'P');

    printf("\n Insert 888 after 1\n");
    insert_after(t,888,1);
    print_l_o_t_tree(t->lot_root,'P');

}


void test_insert_after_small_list_end_of_lot(){
    o_t *t = create_order();
    insert_top(t,1);
    insert_top(t,2);
    insert_top(t,3);
    insert_top(t,4);

    printf("\n After insert_top: 1,2,3,4\n");
    print_l_o_t_tree(t->lot_root,'P');

    printf("\n Insert 888 after 1\n");
    insert_after(t,888,4);
    print_l_o_t_tree(t->lot_root,'P');

}




void print_o_t(o_t *t, char w ){
/* recursivly walks the tree in pre-order*/
    if (t == NULL ) return;
    print_o_t_nodeInfo(t, w);
    if (t->right != NULL) {
        print_o_t(t->left,'L');
        print_o_t(t->right, 'R');
    }
}

void print_o_t_nodeInfo(o_t *t, char w){
/* prints node's fields */
    if( t == NULL) return;
    char *which = (t->right == NULL) ? leaf : interior;

    if( which == leaf)
        printf("\n\t");
    else
        printf("\n");
    
    int key_1 = (long) t->key;
    printf("[ %s (%c) ]\n\t\tkey(%d) heigh(%d)\n",
            which, w, key_1, t->height );

    if( w == 'P'){
        if( t->lot_root != NULL  && t->lot_root->top != NULL)
            printf("\t\t\t field (top) in l_o_t: (%ld)\n", t->lot_root->top->key);
        else
            printf("\t\t\t field (top) in l_o_t empty\n");

        if( t->lot_root != NULL && t->lot_root->bottom != NULL)
            printf("\t\t\t field (bottom) in l_o_t: (%ld)\n", t->lot_root->bottom->key);
        else
            printf("\t\t\t field (bottom) in l_o_t empty\n");
    }

}
void print_l_o_t_list(l_o_t *t, char w){
    /*recursivly walks the tree in pre-order*/
    if (t->right != NULL) {
        print_l_o_t_list(t->left,'L');
        print_l_o_t_list(t->right, 'R');
    }
    else{
        print_l_o_t_node_list(t, w);
    }

}

void print_l_o_t_node_list(l_o_t *t, char w){
    printf(" (%ld) ", (long)t->key);
}

void print_l_o_t_tree(l_o_t *t, char w){
    print_l_o_t_node_info(t,w);
    if (t->right != NULL) {
        print_l_o_t_tree(t->left,'L');
        print_l_o_t_tree(t->right, 'R');
    }

}

void print_l_o_t_node_info(l_o_t *t, char w){
    /* prints node's fields */
    char *which = (t->right == NULL) ? leaf : interior;

    if( which == leaf)
        printf("\n\t");
    else
        printf("\n");

    int key_1 = (long) t->key;
    printf("[ %s (%c) ]\n\t\tkey(%d) height(%d) \n",
            which, w, key_1, t->height);

}


// if( p(i+1) == 4311 ) { printf("%ld\n",i); exit(0); }


/* Professor Brass test code*/
void test_integrated(){

   long i; o_t *o;
   printf("starting \n");
   o = create_order();
   for(i=100000; i>=0; i-- )
      insert_bottom( o, p(i) );

   for(i=100001; i< 300007; i+=2 ){
      insert_after(o, p(i+1), p(i-1) );
      insert_before( o, p(i), p(i+1) );
   }
   printf("inserted 300000 elements. ");
   
   for(i = 250000; i < 300007; i++ )
      delete_o( o, p(i) );
   printf("deleted 50000 elements. ");

   insert_top( o, p(300006) );
   for(i = 250000; i < 300006; i++ )
      insert_before( o, p(i) , p(300006) );
   printf("reinserted. now testing order\n");
  

   for( i=0; i < 299000; i +=42 ){
       if( compare( o, p(i), p(i+23) ) != 1 ){
           printf(" found error (1) \n"); exit(0);
      }
   } 
   for( i=300006; i >57; i -=119 ){
       if( compare( o, p(i), p(i-57) ) != 0 ){
           printf(" found error (0) \n"); exit(0);
      }
   }
   printf("finished. no problem found.\n");

}


long p(long q){
    return( (1247*q +104729) % 300007 );
}

