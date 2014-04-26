#include <stdio.h>
#include <stdlib.h>
#include "bucketsort_linked_list.h"

//struct listnode { struct listnode * next; char * word; } ;

int main(void)
{  
   int i,j,k,l;
   struct listnode *node, *tmpnode; 
   node = NULL;
   for( i=0; i < 10; i++)
   {
     for( j=0; j < 10; j++)
     {
       for( k=0; k < 10; k++)
       {
           tmpnode = node;
           node = (struct listnode *) malloc( sizeof(struct listnode));
           node->word = (char *) malloc( 50*sizeof(char));
           sprintf (node->word, "st %d %d %d",  (3*i)%10, (3*j)%10, (7*k)%10 ); 
           node->next = tmpnode;
       }
     }
   }
   tmpnode = node;
   node = (struct listnode *) malloc( sizeof(struct listnode));
   node->word = "first string";
   node->next = tmpnode;
   tmpnode = node;
   node = (struct listnode *) malloc( sizeof(struct listnode));
   node->word = "second string";
   node->next = tmpnode;
   tmpnode = node;
   node = (struct listnode *) malloc( sizeof(struct listnode));
   node->word = "the last string";
   node->next = tmpnode;
   printf("\n prepared list, now starting sort\n");

   node = stringsort(node);


   printf("\n checking sorted list\n");
   printf("1: %s\n", node->word);
   node = node->next;
   printf("2: %s\n", node->word);
   node=node->next;

   for( l=0; l < 1000; l++)
   {  if( node == NULL )
      {  printf("List ended early\n"); exit(0);
      }
      sscanf( node->word, "st %d %d %d", &i, &j, &k);
      if( 100*i + 10*j +k != l )
      {  printf("Node contains wrong value\n"); exit(0);
      }
      node = node->next;
   }
   printf("tested %d strings, found in sorted order.\n",l);
   printf("last: %s\n", node->word);
   if( node->next != NULL )
   {  printf("List too long\n"); exit(0);
   }

   exit(0);
}
