///
/// Author: Javier Fernandez
/// CCNY-CSC 220 Algorithms, Fall Semester Homework Assignment 1
#ifndef GUARD_qsort_linked_list 
#define GUARD_qsort_linked_list
#include <cstdlib>
#include <time.h>

struct listnode{
    int value; struct listnode *next; 
};

// Main function once used fires recursive calls to itself
struct listnode 
*quicksort( struct listnode *data, int len );

struct listnode
*divide_list( struct listnode **data, struct listnode **too_small,
    struct listnode **too_big);

void 
node_classification( struct listnode **newlist, struct listnode **newlist_tail,  struct listnode *data);

void 
concatinate_list( struct listnode **data, struct listnode *too_small,
 struct listnode *pivot, struct listnode *too_big);

int  
count_list( struct listnode *list );

struct listnode
*get_pivot( struct listnode **data, int len );


#endif
