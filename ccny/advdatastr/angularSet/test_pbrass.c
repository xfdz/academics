#include <stdio.h>
#include <stdlib.h>
#include "pbrass_testplan.h"
#include "angularset_hbalanced_tree.h"

void print_me(){
    printf("YOU'RE HERE");
}

void test_pbrass()
{
  long i; float x, y;
  angle_set_t *S, *T;
  S = create_aset();
  T = create_aset();
  printf("Started: created two angle sets.\n");
  for( i= 0; i< 100000; i++ )
  { x = ( 3.0 * ((float) i)/ 1000.0 ) + 30.0;
    y = ( 3.0 * ( (float) ((81*i)%100000))/1000.0 ) + 30.0;
    insert_aset( S, x );
    insert_aset( T, y );
  }
  printf("Inserted same 100,000 angle values in both sets\n");
  x = first_aset(S, 0.0, 50.0);
  y = first_aset(T, 0.0, 50.0);
  if( x!=y)
    printf("Error: first angle in S is %f, in T is %f\n",x,y);
  x = last_aset(S, 320.0, 350.0);
  y = last_aset(T, 320.0, 350.0);
  if( x!=y)
    printf("Error: last angle in S is %f, in T is %f\n",x,y);

  for(i= 0; i<50; i++ )
  {  x = first_aset(S, 0.0, 100.0);
     delete_aset(S,x);
     y = first_aset(T, 0.0, 100.0);
     delete_aset(T,y);
     if( x!= y  )
       {  printf("Error in delete loop\n"); exit(-1); }
  }
  printf("Deleted first 50 elements\n");
  i = count_aset(S, 120.3, 175.6 );
  if( i != count_aset(T, 120.3, 175.6 ) )
    printf("Error in counting (1)\n");
  insert_aset(S, 11.7);   insert_aset(T, 11.7);
  insert_aset(S, 13.0);   insert_aset(T, 13.0);
  insert_aset(S, 9.9);   insert_aset(T, 9.9);
  insert_aset(S, 355.0);   insert_aset(T, 355.0);
  if( count_aset(S, 10.0, 20.0 ) != 2 ||
      count_aset(T, 10.0, 20.0 ) !=2 )
    printf("Error in counting (2)\n");
  if( count_aset(S, 350.0, 12.0 ) !=3 ||
      count_aset(T, 350.0, 12.0 ) !=3 )
    printf("Error in counting (3)\n");
  if( first_aset(S, 340.0, 20.0 ) != 355.0 ||
      first_aset(T, 340.0, 20.0 ) != 355.0 )
  {  printf("Error in wrap-around interval (1)\n");
     printf(" first in interval [340, 20) in S is %f, should be 355.0\n",
	 first_aset(S, 357.0, 20.0 ) );
     printf(" first in interval [340, 20) in T is %f, should be 355.0\n",
	 first_aset(T, 357.0, 20.0 ) );
  }
  if( last_aset(S, 340.0, 20.0 ) != 13.0 ||
      last_aset(T, 340.0, 20.0 ) != 13.0 )
  {  printf("Error in wrap-around interval (2)\n");
     printf(" last in interval [340, 20) in S is %f, should be 13.0\n",
	 first_aset(S, 357.0, 20.0 ) );
     printf(" last in interval [340, 20) in T is %f, should be 13.0\n",
	 first_aset(T, 357.0, 20.0 ) );
  }
  if( first_aset(S, 357.0, 20.0 ) !=  (float) 9.9 ||
      first_aset(T, 357.0, 20.0 ) !=  (float) 9.9 )
  {  printf("Error in wrap-around interval (3)\n");
     printf(" first in interval [357, 20) in S is %f, should be 9.9\n",
	 first_aset(S, 357.0, 20.0 ) );
     printf(" first in interval [357, 20) in T is %f, should be 9.9\n",
	 first_aset(T, 357.0, 20.0 ) );
  }
  if( last_aset(S, 357.0, 20.0 ) != 13.0 ||
      last_aset(T, 357.0, 20.0 ) != 13.0 )
  {  printf("Error in wrap-around interval (4)\n");
     printf(" last in interval [357, 20) in S is %f, should be 13.0\n",
	 last_aset(S, 357.0, 20.0 ) );
     printf(" last in interval [357, 20) in T is %f, should be 13.0\n",
	 last_aset(T, 357.0, 20.0 ) );
  }
  printf("End Test\n");

}
