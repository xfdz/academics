/*
 * Author: Javier Fernandez
 * CCNY-CSC-I9604 adv data struc
 * Fall-2010 Homework Assignment 4
 * Bloom filter hash table, 200k hashable strings
 */


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "bloomfilter_hash.h"

#define STRNGSIZE 8

char HEXREFTABLE[8] = {
   0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80
};


/* creates a Bloom filter, specification 2000,000 strings 1% error*/
bf_t *create_bf(){

    bf_t *bf_htable  = (bf_t*)malloc( sizeof(bf_t) );
    
    srand( time(NULL) );
    for(int i=0; i < BF_TABLES; ++i){
        bf_htable->bf_param[i].a_list       = get_node();
        bf_htable->bf_param[i].a_list->next = NULL;
        bf_htable->bf_param[i].b            = rand() % MAXP;
        bf_htable->abits[i] = 
                (unsigned char*) malloc(sizeof(char)*MAX_HASH_STRINGS);
        for(int j=0; j < MAX_HASH_STRINGS; ++j){
            (bf_htable->abits[i])[j] = 0;
        }
    }
    

    return bf_htable;
}

/* insert the string s into the bloom filter b */
void insert_bf(bf_t *b, char *s){
    if( b == NULL ) return;

    for(int i=0; i < BF_TABLES; ++i){
        int bucketbit  = universalhashfunction(s, b->bf_param[i] );
        int index_byte = bucketbit / STRNGSIZE;
        int index_bit  = bucketbit % STRNGSIZE;
        (b->abits[i])[index_byte]  =
                (b->abits[i])[index_byte] | HEXREFTABLE[index_bit];
    }
}



/* return 1 if string q  accpted by Bloom filter else 0*/
int is_element(bf_t *b, char *q){
    if( b == NULL ){
        printf("ERROR: in is_element(), NULL bf_t object");
        exit(-1);
    }

    int result = 0;
    for(int i=0; i < BF_TABLES; ++i){
    /* flip bit in corresponding table for the string */
        int bucketbit  = universalhashfunction(q, b->bf_param[i] );
        int index_byte = bucketbit / STRNGSIZE;
        int index_bit  = bucketbit % STRNGSIZE;
        result += (((b->abits[i])[index_byte] & HEXREFTABLE[index_bit]) != 0x00);
    }

    return ( result != 10 ) ? 0 : 1;
}


int universalhashfunction(char *key, bf_param_t bfp){

   int sum = bfp.b;
   htp_l_node_t *al = NULL;
   al = bfp.a_list;
   while( *key != '\0' ){
       if( al->next == NULL ){
          al->next = (htp_l_node_t *) get_node();
          al->next->next = NULL;
          al->a = rand()%MAXP;
       }
       sum += abs( ( (al->a) * (( int)*key))%MAXP);
       key += 1;
       al = al->next;
   }
   
   return ( sum % MAXNUM_STRINGS) ;
}



int size_left;
htp_l_node_t *currentblock = NULL;
htp_l_node_t *free_list = NULL;

htp_l_node_t *get_node(){
  htp_l_node_t *tmp;
  if( free_list != NULL )
  {  tmp = free_list;
     free_list = free_list -> next;
  }
  else
  {  if( currentblock == NULL || size_left == 0)
     {  currentblock =
                (htp_l_node_t *) malloc( BLOCKSIZE * sizeof(htp_l_node_t) );
        size_left = BLOCKSIZE;
     }
     tmp = currentblock++;
     size_left -= 1;
  }
  return( tmp );
}

void return_node(htp_l_node_t *node)
{  node->next = free_list;
   free_list = node;
}

