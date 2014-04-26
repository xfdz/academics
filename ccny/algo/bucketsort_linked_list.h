/// Author: Javier Fernandez
/// CCNY-CSC 220 Algorithms, Fall Semester Homework Assignment 2 
#ifndef GUARD_bucketsort_linked_list 
#define GUARD_bucketsort_linked_list

#include <stdlib.h>
#include <stdio.h>

struct listnode {
    char *word; 
    struct listnode *next;
};

struct front_back {
    struct listnode *head, *tail;
};

struct bucket{
    struct front_back *data;
    struct bucket *next;
    int ascii_value;
};

struct listnode 
*stringsort( struct listnode *data);

struct front_back 
*ssort(struct front_back *data, int next);

struct bucket 
*create_buckets();

#endif
