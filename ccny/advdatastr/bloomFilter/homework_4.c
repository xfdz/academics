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

#define STRNGSIZE        8
#define BLOCKSIZE        256
#define MAXP             227832
#define BF_TABLES        10
#define MAXNUM_STRINGS   200000
#define MAX_HASH_STRINGS 25000

typedef struct htp_l_node {
    int a;
    struct htp_l_node *next;
} htp_l_node_t;

typedef struct bf_param {
    int b;
    htp_l_node_t *a_list;
} bf_param_t;


typedef struct bf {
    bf_param_t bf_param[BF_TABLES];
    unsigned char *abits[BF_TABLES];
} bf_t;

bf_t *create_bf();
htp_l_node_t *get_node();
void insert_bf(bf_t *b, char *s);
int is_element(bf_t *b, char *q);
int universalhashfunction(char *, bf_param_t);

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

void sample_string_A(char *s, int i)
{  s[0] = (char)(1 + (i%254));
   s[1] = (char)(1 + ((i/254)%254));
   s[2] = (char)(1 + (((i/254)/254)%254));
   s[3] = 'a';
   s[4] = 'b';
   s[5] = (char)(1 + ((i*(i-3)) %217));
   s[6] = (char)(1 + ((17*i+129)%233 ));
   s[7] = '\0';
}
void sample_string_B(char *s, int i)
{  s[0] = (char)(1 + (i%254));
   s[1] = (char)(1 + ((i/254)%254));
   s[2] = (char)(1 + (((i/254)/254)%254));
   s[3] = 'a';
   s[4] = (char)(1 + ((i*(i-3)) %217));
   s[5] = (char)(1 + ((17*i+129)%233 ));
   s[6] = '\0';
}
void sample_string_C(char *s, int i)
{  s[0] = (char)(1 + (i%254));
   s[1] = (char)(1 + ((i/254)%254));
   s[2] = 'a';
   s[3] = (char)(1 + ((i*(i-3)) %217));
   s[4] = (char)(1 + ((17*i+129)%233 ));
   s[5] = '\0';
}
void sample_string_D(char *s, int i)
{  s[0] = (char)(1 + (i%254));
   s[1] = (char)(1 + ((i/254)%254));
   s[2] = (char)(1 + (((i/254)/254)%254));
   s[3] = 'b';
   s[4] = 'b';
   s[5] = (char)(1 + ((i*(i-3)) %217));
   s[6] = (char)(1 + ((17*i+129)%233 ));
   s[7] = '\0';
}
void sample_string_E(char *s, int i)
{  s[0] = (char)(1 + (i%254));
   s[1] = (char)(1 + ((i/254)%254));
   s[2] = (char)(1 + (((i/254)/254)%254));
   s[3] = 'a';
   s[4] = (char)(2 + ((i*(i-3)) %217));
   s[5] = (char)(1 + ((17*i+129)%233 ));
   s[6] = '\0';
}

int main() 
{ long i,j;
   bf_t *bloom;
   bloom = create_bf();
   printf("Created Filter\n");
   for( i= 0; i< 100000; i++ )
   {  char s[8];
      sample_string_A(s,i);
      insert_bf( bloom, s );
   }
   for( i= 0; i< 50000; i++ )
   {  char s[7];
      sample_string_B(s,i);
      insert_bf( bloom, s );
   }
   for( i= 0; i< 50000; i++ )
   {  char s[6];
      sample_string_C(s,i);
      insert_bf( bloom, s );
   }
   printf("inserted 200000 strings of length 8,7,6.\n");

   for( i= 0; i< 100000; i++ )
   {  char s[8];
      sample_string_A(s,i);
      if( is_element( bloom, s ) != 1 )
	{  printf("found negative error (1)\n"); exit(0); }
   }
   for( i= 0; i< 50000; i++ )
   {  char s[7];
      sample_string_B(s,i);
      if( is_element( bloom, s ) != 1 )
	{  printf("found negative error (2)\n"); exit(0); }
   }
   for( i= 0; i< 50000; i++ )
   {  char s[6];
      sample_string_C(s,i);
      if( is_element( bloom, s ) != 1 )
	{  printf("found negative error (3)\n"); exit(0); }
   }
   j = 0;
   for( i= 0; i< 200000; i++ )
   {  char s[8];
      sample_string_D(s,i);
      if( is_element( bloom, s ) != 0 )
	j+=1;
   }
   for( i= 0; i< 200000; i++ )
   {  char s[8];
      sample_string_E(s,i);
      if( is_element( bloom, s ) != 0 )
	j+=1;
   }
   printf("Found %ld positive errors out of 400000 tests.n",j);

   return 0;
}
