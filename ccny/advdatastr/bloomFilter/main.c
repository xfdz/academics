/*
 * Author: Javier Fernandez
 * CCNY-CSC-I9604 adv data struc
 * Fall-2010 Homework Assignment 4
 * Bloom filter hash table, 200k hashable strings
 */

#include <stdio.h>
#include <stdlib.h>
#include "bloomfilter_hash.h"

#define ITERATIONS 10

/*
 * Prof Brass' Test code
 */

long integraton_test();
void sample_string_A(char *s, int i);
void sample_string_B(char *s, int i);
void sample_string_C(char *s, int i);
void sample_string_D(char *s, int i);
void sample_string_E(char *s, int i);

/*
 * My test code
 */
 void test_create_bf();
void display_bits(unsigned char s);


int main(int argc, char** argv) {

    //test_create_bf();
    long sum=0;
    for(int i=0; i<ITERATIONS; ++i){
        sum += integraton_test();
    }
    
    float avg = (float)sum/ITERATIONS;
    printf("\n Ran code %d times, sum is: %ld, average is: %ld/%d = %f",
            ITERATIONS, sum, sum,ITERATIONS, avg);

    float percnt =  (avg / 400000) * 100;

    printf("\n False positive error percentage: %f/%d is:  %f",
            avg,400000, percnt);
   
    
    return (EXIT_SUCCESS);
}
void test_create_bf(){
    bf_t *bf = create_bf();

    for(int i=0; i < BF_TABLES; ++i){
        for(int j=0; j < MAX_HASH_STRINGS; ++j){
            display_bits((bf->abits[i])[j]);
        }
    }
}


void display_bits(unsigned char s){
    int value;
    int i;;
    for (i=0; i<MAX_HASH_STRINGS; i++){
        value = s & (1 != 0);
        printf(" %u ", value);
        s = s >> 1;
    }
    printf("\n #%d#\n", i);
}

/*
 *NOTE: I modified Dr.Brass code to return a long, his doesn't
 */
long integraton_test(){


 long i,j;
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

   return j;

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