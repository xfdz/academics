/*
* Author: Javier Fernandez
* CCNY-CSCI-I9604 Advanced data structure
* Fall 2010 HW# 1
*/

#include <stdio.h>
#include <stdlib.h>
#include "angularset_hbalanced_tree.h"

#define BLOCKSIZE 256
#define STACKSIZE 100



angle_set_t *create_aset(){

   angle_set_t *tmp_node = _get_node();
   tmp_node->left = NULL;
   return( tmp_node );
}

void insert_aset(angle_set_t *s, float a){

    float *objf = (float *) malloc( sizeof(float));
    if(objf == NULL )
        perror("ERROR: allocating dynamic memory with malloc()");

    *objf = a;
    int status = _insert( s, (key_t )a, objf);
    if( status != 0)
        printf("\nINFO: insert failed for key %f\n", a);
}

void delete_aset(angle_set_t *s , float a ){
    object_t *delobj = _delete(s, (object_t) a);
    if( delobj == NULL )
        printf("\nINFO: delete failed for key %f\n", a);
    free(delobj);
}

int count_aset(angle_set_t *tree, float a, float b){

    int count;
    if( a < b )
        /* range [a,b[ */
        count =  count_aset_range( tree, a, b);
    else
        /* range [a,360[ and [0,b[ */
        count = count_aset_range(tree, a, 360) + count_aset_range( tree, 0, b);
    
    return count;
}

int count_aset_range(angle_set_t *tree, float a, float b){
 
    int count =0, stksize=0;
    angle_set_t *stack[100];
    stack[stksize++] = tree;
    angle_set_t *tmp_tree = NULL;

    while( stksize > 0){
        tmp_tree = stack[--stksize];
        if( tmp_tree->right == NULL ){
            /* reached a leaf*/
            if( (a <= tmp_tree->key) && ( tmp_tree->key < b) )
                ++count;
        }
        else if( b <= tmp_tree->key ){
            /* entire left interval*/
            stack[stksize++] = tmp_tree->left;
        }
        else if( tmp_tree->key <= a){
            /* entire right interval */
            stack[stksize++] = tmp_tree->right;
        }
        else{
            /* within interval, follow both right, left*/
            stack[stksize++] = tmp_tree->right;
            stack[stksize++] = tmp_tree->left;
        }
    }
    return count;
    
}

float first_aset(angle_set_t *tree, float a, float b ){

    float *first_angle = NULL, angle_range[] = {a,b};
    
    /* when a > b in range [a,b[ then search two ranges */
    int is_loop = ( a < b) ? 1 : 2;

    int count_loop =0;
    for(; count_loop != is_loop; ++count_loop ){

        if( is_loop == 2 ){
            if( count_loop == 0){
                angle_range[0] = a; angle_range[1] = 360.0;
            }
            else{
                angle_range[0] = 0.0; angle_range[1] = b;
            }
        }
        
        int stksize=0;
        angle_set_t *stack[100];
        stack[stksize++] = tree;
        angle_set_t *tmp_tree = NULL;

        while( stksize > 0){
            tmp_tree = stack[--stksize];
            if( tmp_tree->right == NULL ){
                /* reached a leaf - remember the interval is [a,b[ */
                if( (angle_range[0] <= tmp_tree->key)
                        && ( tmp_tree->key < angle_range[1]) )
                {
                    first_angle = (float*)malloc( sizeof(float) );
                    *first_angle = tmp_tree->key;
                    break;
                }
            }
            else if( angle_range[1] < tmp_tree->key ){
                /* entire left interval*/
                stack[stksize++] = tmp_tree->left;
            }
            else if( tmp_tree->key <= angle_range[0]){
                /* entire right interval */
                stack[stksize++] = tmp_tree->right;
            }
            else{
                /* within interval, follow both right, left*/
                stack[stksize++] = tmp_tree->right;
                stack[stksize++] = tmp_tree->left;
            }
        }

        if( first_angle != NULL ) break;
    }

    float ret_value = *first_angle;
    free( first_angle);
  
    return  ret_value;
}

float last_aset(angle_set_t *tree, float a, float b ){

     float *last_angle = NULL, angle_range[] = {a,b};

    /* when a > b in range [a,b[ then search two ranges */
    int is_loop = ( a < b) ? 1 : 2;

    int count_loop =0;
    for(; count_loop != is_loop; ++count_loop ){

        if( is_loop == 2 ){
            if( count_loop == 0){
                angle_range[0] = 0.0; angle_range[1] = b;
            }
            else{
                
                angle_range[0] = a; angle_range[1] = 360.0;
            }
        }

        int stksize=0;
        angle_set_t *stack[100];
        stack[stksize++] = tree;
        angle_set_t *tmp_tree = NULL;

        while( stksize > 0){
            tmp_tree = stack[--stksize];
            if( tmp_tree->right == NULL ){
                /* reached a leaf*/
                if( (angle_range[0] <= tmp_tree->key)
                        && ( tmp_tree->key < angle_range[1]) )
                {
                    last_angle = (float*)malloc( sizeof(float) );
                    *last_angle = tmp_tree->key;
                    break;
                }
            }
            else if( angle_range[1] < tmp_tree->key ){  /*** not here check*/
                /* go left if [a, b[ < node->key */
                stack[stksize++] = tmp_tree->left;
            }
            else if( tmp_tree->key <= angle_range[0]){
                /* right if node->key ≤ [a, b[ */
                stack[stksize++] = tmp_tree->right;
            }
            else{
                /* within interval, follow both right, left*/
                stack[stksize++] = tmp_tree->left;
                stack[stksize++] = tmp_tree->right;
            }
        }

        if( last_angle != NULL ) break;
    }

    float ret_value = *last_angle;
    free(last_angle);
    
    return ret_value;
}

/* functions begining with underscore are recycled from:
 * http://www-cs.ccny.cuny.edu/~peter/dstest/h_bl_tree.c
 */
int size_left;
angle_set_t *currentblock = NULL;
angle_set_t *free_list = NULL;

angle_set_t *_get_node(){

  angle_set_t *tmp;
  if( free_list != NULL )
  {  tmp = free_list;
     free_list = free_list->left;
  }
  else
  {  if( currentblock == NULL || size_left == 0)
     {  currentblock = 
                (angle_set_t *) malloc( BLOCKSIZE * sizeof(angle_set_t) );
        size_left = BLOCKSIZE;
     }
     tmp = currentblock++;
     size_left -= 1;
  }
  return( tmp );
}

void _return_node(angle_set_t *node){

   node->left = free_list;
   free_list = node;
}

int _insert(angle_set_t *tree, key_t new_key, object_t *new_object){
    
   angle_set_t *tmp_node;
   int finished;

   if( tree->left == NULL ){
      /* single node tree*/
      tree->key  = new_key;
      tree->height = 0;
      tree->right  = NULL;
      tree->left = (angle_set_t *) new_object;
      
   }
   else
   {
      angle_set_t *path_stack[STACKSIZE];
      int  path_st_p = 0;
      tmp_node = tree;

      while( tmp_node->right != NULL )
      {   path_stack[path_st_p++] = tmp_node;
          if( new_key < tmp_node->key )
               tmp_node = tmp_node->left;
          else
               tmp_node = tmp_node->right;
      }
      /* found the candidate leaf. Test whether key distinct */
      if( tmp_node->key == new_key ) return( -1 );
      
      /* key is distinct, now perform the insert */
      {  angle_set_t *old_leaf, *new_leaf;
         old_leaf = create_aset();
         old_leaf->left = tmp_node->left;
         old_leaf->key = tmp_node->key;
         old_leaf->right  = NULL;
         old_leaf->height = 0;

         new_leaf = create_aset();
         new_leaf->left = (angle_set_t *) new_object;
         new_leaf->key = new_key;
         new_leaf->right  = NULL;
         new_leaf->height = 0;

         if( tmp_node->key < new_key )
         {   tmp_node->left  = old_leaf;
             tmp_node->right = new_leaf;
             tmp_node->key = new_key;
         }
         else
         {   tmp_node->left  = new_leaf;
             tmp_node->right = old_leaf;
         }
         tmp_node->height = 1;
      }

      /* rebalance */
      finished = 0;
      while( path_st_p > 0 && !finished )
      {  int tmp_height, old_height;
         tmp_node = path_stack[--path_st_p];
         old_height= tmp_node->height;

         if( tmp_node->left->height 
              - tmp_node->right->height == 2 )
         {
             if( tmp_node->left->left->height -
                                 tmp_node->right->height == 1 )
            {  _right_rotation( tmp_node );
               tmp_node->right->height =
                            tmp_node->right->left->height + 1;
               tmp_node->height = tmp_node->right->height + 1;
            }
            else
            {  _left_rotation( tmp_node->left );
               _right_rotation( tmp_node );
               tmp_height = tmp_node->left->left->height;
               tmp_node->left->height  = tmp_height + 1;
               tmp_node->right->height = tmp_height + 1;
               tmp_node->height = tmp_height + 2;
            }
         }
         else if ( tmp_node->left->height
                    - tmp_node->right->height == -2 )
         {
             if( tmp_node->right->right->height -
                                  tmp_node->left->height == 1 )
            {  _left_rotation( tmp_node );
               tmp_node->left->height =
                           tmp_node->left->right->height + 1;
               tmp_node->height = tmp_node->left->height + 1;
            }
            else
            {  _right_rotation( tmp_node->right );
               _left_rotation( tmp_node );
               tmp_height = tmp_node->right->right->height;
               tmp_node->left->height  = tmp_height + 1;
               tmp_node->right->height = tmp_height + 1;
               tmp_node->height = tmp_height + 2;
            }
         }
         else /* update height even if there was no rotation */
         {  if( tmp_node->left->height > tmp_node->right->height )
               tmp_node->height = tmp_node->left->height + 1;
            else
               tmp_node->height = tmp_node->right->height + 1;
         }

         if( tmp_node->height == old_height )
            finished = 1;
      }

   }
   return( 0 );
    
}

object_t *_delete(angle_set_t *tree, key_t delete_key){
    
   angle_set_t *tmp_node, *upper_node, *other_node;
   object_t *deleted_object;
   int finished;

   if( tree->left == NULL )
      return( NULL );
   else if( tree->right == NULL )
   {  if(  tree->key == delete_key )
      {  deleted_object = (object_t *) tree->left;
         tree->left = NULL;
         return( deleted_object );
      }
      else
         return( NULL );
   }
   else
   {  angle_set_t * path_stack[100]; int path_st_p = 0;
      tmp_node = tree;
      while( tmp_node->right != NULL )
      {   path_stack[path_st_p++] = tmp_node;
          upper_node = tmp_node;
          if( delete_key < tmp_node->key )
          {  tmp_node   = upper_node->left;
             other_node = upper_node->right;
          }
          else
          {  tmp_node   = upper_node->right;
             other_node = upper_node->left;
          }
      }

      if( tmp_node->key != delete_key )
         deleted_object = NULL;
      else
      {  upper_node->key   = other_node->key;
         upper_node->left  = other_node->left;
         upper_node->right = other_node->right;
         upper_node->height = other_node->height;
         deleted_object = (object_t *) tmp_node->left;
         _return_node( tmp_node );
         _return_node( other_node );

      }

      /*start rebalance*/
      finished = 0; path_st_p -= 1;
      while( path_st_p > 0 && !finished )
      {  int tmp_height, old_height;
         tmp_node = path_stack[--path_st_p];
         old_height= tmp_node->height;

         if( tmp_node->left->height 
                 - tmp_node->right->height == 2 )
         {  if( tmp_node->left->left->height 
                 - tmp_node->right->height == 1 )
	    {  _right_rotation( tmp_node );
               tmp_node->right->height =
                            tmp_node->right->left->height + 1;
               tmp_node->height = tmp_node->right->height + 1;
            }
            else
	    {  _left_rotation( tmp_node->left );
               _right_rotation( tmp_node );
               tmp_height = tmp_node->left->left->height;
               tmp_node->left->height  = tmp_height + 1;
               tmp_node->right->height = tmp_height + 1;
               tmp_node->height = tmp_height + 2;
            }
         }
         else if ( tmp_node->left->height 
                     - tmp_node->right->height == -2 )
         {  if( tmp_node->right->right->height -
                                  tmp_node->left->height == 1 )
	    {  _left_rotation( tmp_node );
               tmp_node->left->height =
                           tmp_node->left->right->height + 1;
               tmp_node->height = tmp_node->left->height + 1;
            }
            else
            {  _right_rotation( tmp_node->right );
               _left_rotation( tmp_node );
               tmp_height = tmp_node->right->right->height;
               tmp_node->left->height  = tmp_height + 1;
               tmp_node->right->height = tmp_height + 1;
               tmp_node->height = tmp_height + 2;
            }
         }
         else /* update height even if there was no rotation */
         {  if( tmp_node->left->height > tmp_node->right->height )
               tmp_node->height = tmp_node->left->height + 1;
            else
               tmp_node->height = tmp_node->right->height + 1;
         }

         if( tmp_node->height == old_height )
	    finished = 1;
      }
      
      /*end rebalance*/
      return( deleted_object );
   }
}

void _left_rotation(angle_set_t *n){
   angle_set_t *tmp_node;
   key_t        tmp_key;
   tmp_node = n->left;
   tmp_key  = n->key;
   n->left  = n->right;
   n->key   = n->right->key;
   n->right = n->left->right;
   n->left->right = n->left->left;
   n->left->left  = tmp_node;
   n->left->key   = tmp_key;
}

void _right_rotation(angle_set_t *n){
   angle_set_t *tmp_node;
   key_t        tmp_key;
   tmp_node = n->right;
   tmp_key  = n->key;
   n->right = n->left;
   n->key   = n->left->key;
   n->left  = n->right->left;
   n->right->left = n->right->right;
   n->right->right  = tmp_node;
   n->right->key   = tmp_key;
}




