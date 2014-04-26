
/*
 * Author: Javier Fernandez
 * CCNY-CSC-I9604 adv data struc 
 * Fall-2010 Homework Assignment 2 
 * measure tree algorithm
 */ 


#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define BLOCKSIZE 256
#define STACKSIZE 100
#define	MEASURE_TREE_H
#define UNINITIALIZED -1


typedef int object_t;
typedef int key_t;
typedef struct m_tr_n_t{
    key_t key;
    int measure;
    int left;
    int right;
    int left_min;
    int right_max;
    int height;
    struct m_tr_n_t *pleft;
    struct m_tr_n_t *pright;
}m_tree_t;


typedef struct tr_n_t{
    key_t key;
    int ref_count;
    int height;
    struct tr_n_t *left;
    struct tr_n_t *right;
}b_tree_t;

void _mtree_right_rotation(m_tree_t *);
void _mtree_left_rotation(m_tree_t *);
int _mtree_insert_node(m_tree_t*, key_t, object_t);
void _mtree_return_node(m_tree_t *);
m_tree_t *_get_mtree_node();
int _min_number(int, int);
int _max_number(int, int);
int _mtree_update_leaf_node_fields(m_tree_t *tmp_node  );
int _mtree_internal_node_calc_measure( m_tree_t *);
int _mtree_delete_interval( m_tree_t*, int, int);
void _mtree_interior_node_field_update(m_tree_t * );
void _mtree_interior_node_field_update_on_delete(m_tree_t *);
void delete_interval( m_tree_t *, int, int );
void _mtree_close_interval_gap(m_tree_t *node, int new_right);
void insert_interval( m_tree_t *, int , int );
int query_length(m_tree_t*);
m_tree_t *create_m_tree();


void _bintree_right_rotation(b_tree_t*);
void _bintree_left_rotation(b_tree_t*);
int _bintree_find_min(b_tree_t*  );
int _bintree_find_max(b_tree_t* );
int _bintree_delete_node(b_tree_t*, key_t);
int _bintree_insert_node(b_tree_t*, key_t);
void _bintree_return_node(b_tree_t*);
b_tree_t *_bintree_get_node();


m_tree_t *create_m_tree(){
    m_tree_t *t = _get_mtree_node();
    t->height = 0;
    t->key = t->measure         = UNINITIALIZED;
    t->right_max = t->left_min  = UNINITIALIZED;
    t->left_min  = t->right_max = UNINITIALIZED;
    t->pleft = t->pright = NULL;
    return t;
}

int query_length(m_tree_t *tree){
    return tree->measure;
}

void insert_interval( m_tree_t *mtree, int a, int b){

    if( mtree->pleft == NULL) {
    /*  base case - first time inserting range  */

        /* left child of mtree */
        mtree->pleft = _get_mtree_node();
        mtree->pleft->height = 0;
        mtree->pleft->key = a;
        mtree->pleft->left = a;
        mtree->pleft->right = b;
        mtree->pleft->left_min = a;
        mtree->pleft->right_max = b;
        mtree->pleft->measure = (
          _min_number(mtree->pleft->right, mtree->pleft->right_max)
                - mtree->pleft->left
        );
        
        b_tree_t *tmp_btree = _bintree_get_node();
        if( tmp_btree == NULL ){
           perror("\nERROR: dynamically creating node\n");
           
        }

        int node_insert = _bintree_insert_node( tmp_btree, b );
        if( node_insert == UNINITIALIZED ){
            perror("\nERROR: _bin_tree_insert_node() failed inserting b_tree_t*\n");
            
        }

        mtree->pleft->pleft = (m_tree_t *) tmp_btree;

        /* right child of mtree*/
        mtree->pright = _get_mtree_node();
        mtree->pright->height = 0;
        mtree->pright->key = b;
        mtree->pright->left = b;
        mtree->pright->right = INT_MAX;
        mtree->pright->left_min = a;
        mtree->pright->right_max = b;    
        mtree->pright->measure = (
            _min_number(mtree->pright->right, mtree->pright->right_max)
                - mtree->pright->left
        );

        tmp_btree = _bintree_get_node();
        if( tmp_btree == NULL ){
            perror("\nERROR: dynamically creating node");
            
        }

        node_insert = _bintree_insert_node(tmp_btree, a);
        if( node_insert == UNINITIALIZED ){
            perror("\nERROR: _bin_tree_insert_node() failed inserting b_tree_t*\n");
            
        }

        mtree->pright->pleft = (m_tree_t *) tmp_btree;

        /* root of mtree*/
        mtree->key = b;
        mtree->height = 1;
        mtree->left = mtree->pleft->left;
        mtree->right= mtree->pright->right;
        mtree->left_min = _min_number( mtree->pleft->left_min, mtree->pright->left_min);
        mtree->right_max= _max_number( mtree->pleft->right_max,mtree->pright->right_max);
        mtree->measure = _mtree_internal_node_calc_measure( mtree );
        if( mtree->measure == UNINITIALIZED ){
            perror("\nERROR: _mtree_internal_node_calc_measure() failed\n");
            
        }

        return;
    }

    int xchg, result, cnt;
    for(cnt =0; cnt != 2; ++cnt){
        if( cnt != 0){ xchg = a; a = b; b = xchg; }
        result = _mtree_insert_node(mtree, a, (object_t) b );
        if( result == UNINITIALIZED){
            perror("\nERROR: _mtree_insert_node() failed\n");
            exit(1);
        }

    }
}


void delete_interval( m_tree_t *mtree, int a, int b){

    int xchg, result, cnt;
    for(cnt =0; cnt != 2; ++cnt){
        if( cnt != 0){ xchg = a; a = b; b = xchg; }
        result = _mtree_delete_interval(mtree, b, a );

        if( result == UNINITIALIZED){
            perror("\nERROR: _mtree_delete_interval() failed\n");
        }
    }

}

int _mtree_delete_interval( m_tree_t *mtree, int intrvl_a, int intrvl_b){
    if( mtree == NULL || intrvl_a == intrvl_b) return UNINITIALIZED;

  
    m_tree_t *path_stack[STACKSIZE];
    for(int i=0; i < STACKSIZE; ++i) path_stack[i] = NULL;
    
    int path_st_p = 0;
    m_tree_t *dup_interior = NULL;
    m_tree_t *upper_node   = NULL;
    m_tree_t *other_node   = NULL;
    m_tree_t *tmp_node     = mtree;
    while( tmp_node->pright != NULL ){
    /* search for leaf node */
        path_stack[path_st_p++] = tmp_node;
        upper_node = tmp_node;
        if( intrvl_a < tmp_node->key ){
            tmp_node   = upper_node->pleft;
            other_node = upper_node->pright;
        }
        else{
            if(tmp_node->key == intrvl_a)
                dup_interior = tmp_node;
            tmp_node   = upper_node->pright;
            other_node = upper_node->pleft;
        }
    }

    if( tmp_node->key != intrvl_a )
        return UNINITIALIZED;
    /* failed to find key */

    if( (path_st_p == 0) && (tmp_node->pright == NULL) ){
    /* Single mtree node */
        if( tmp_node->pleft != NULL )
            _bintree_return_node( (b_tree_t *) tmp_node->pleft);
        
        _mtree_return_node(tmp_node);
        return 0;
    }


    /* delete intrvl_b from object tree */
    b_tree_t *btree_obj = (b_tree_t *) tmp_node->pleft;
    int result = _bintree_delete_node( btree_obj, (key_t) intrvl_b);
    
    if( result == UNINITIALIZED)
        return result;

    if( btree_obj->ref_count != UNINITIALIZED ){
    /* keep mtree node alive, it still has associated intervals in btree*/
         result = _mtree_update_leaf_node_fields(tmp_node);
           if( result == UNINITIALIZED)
               return result;
    }
    else{

        if( dup_interior != NULL && ( path_st_p - 2 >= 0 ) ){
        /* swap key of interior node whose is duplicate of node to delete*/
            key_t candidate_1 = (path_stack[path_st_p - 1])->key;
            key_t candidate_2 = (path_stack[path_st_p - 2])->key;

            if( candidate_1 != intrvl_a)
                dup_interior->key = candidate_1;
            else if( candidate_2 != intrvl_a)
                dup_interior->key = candidate_2;

       
        }

        upper_node->key      = other_node->key;
        upper_node->measure  = other_node->measure;
        upper_node->pleft    = other_node->pleft;
        upper_node->left     = other_node->left;
        upper_node->right    = other_node->right;
        upper_node->left_min = other_node->left_min;
        upper_node->right_max = other_node->right_max;
        upper_node->height   = other_node->height;

        if(upper_node->pright == tmp_node )
            upper_node->right = tmp_node->right;

        upper_node->pright   = other_node->pright;

        _mtree_return_node( tmp_node );
        _mtree_return_node( other_node );

        if( upper_node->pright != NULL )
            if( upper_node->pright->right != upper_node->right )
                _mtree_close_interval_gap(upper_node->pright,upper_node->right);

        if( upper_node->pright ==  NULL){
            result = _mtree_update_leaf_node_fields(upper_node);
           if( result == UNINITIALIZED)
               return result;
        }


    }

    while( path_st_p > 0 ){
    /* rebalance tree - update node values */

        int tmp_height;
        tmp_node = path_stack[--path_st_p];

        if( tmp_node->pright == NULL ) continue;
      
        if( tmp_node->pleft->height  - tmp_node->pright->height == 2 ){

            if(tmp_node->pleft->pleft->height - tmp_node->pright->height == 1){
                _mtree_right_rotation( tmp_node );
                _mtree_interior_node_field_update(tmp_node->pright);

                tmp_node->pright->height =
                tmp_node->pright->pleft->height + 1;
                tmp_node->height = tmp_node->pright->height + 1;
            }
            else{

               _mtree_left_rotation( tmp_node->pleft );
               _mtree_interior_node_field_update(tmp_node->pleft->pleft);

               _mtree_right_rotation( tmp_node );
               _mtree_interior_node_field_update(tmp_node->pright);

               tmp_height = tmp_node->pleft->pleft->height;
               tmp_node->pleft->height  = tmp_height + 1;
               tmp_node->pright->height = tmp_height + 1;
               tmp_node->height = tmp_height + 2;
           }
        }
        else if ( tmp_node->pleft->height - tmp_node->pright->height == -2 )
        {
            if(tmp_node->pright->pright->height - tmp_node->pleft->height == 1){
                  _mtree_left_rotation( tmp_node );
                  _mtree_interior_node_field_update(tmp_node->pleft);

                  tmp_node->pleft->height = tmp_node->pleft->pright->height + 1;
                  tmp_node->height = tmp_node->pleft->height + 1;
                }
                else{

                    _mtree_right_rotation( tmp_node->pright );
                    _mtree_interior_node_field_update(tmp_node->pright->pright);

                    _mtree_left_rotation( tmp_node );
                    _mtree_interior_node_field_update(tmp_node->pleft);

                    tmp_height = tmp_node->pright->pright->height;
                    tmp_node->pleft->height  = tmp_height + 1;
                    tmp_node->pright->height = tmp_height + 1;
                    tmp_node->height = tmp_height + 2;
                 }
        }
        else{
        /* update height even if there was no rotation */
           if( tmp_node->pleft->height > tmp_node->pright->height )
              tmp_node->height = tmp_node->pleft->height + 1;
           else
              tmp_node->height = tmp_node->pright->height + 1;

        }

        if( tmp_node->pright->left - tmp_node->pleft->right )
            _mtree_close_interval_gap(tmp_node->pleft, tmp_node->pright->left);

        _mtree_interior_node_field_update(tmp_node);

   }

    return 0;

}

int _mtree_update_leaf_node_fields(m_tree_t *tmp_node  ){

    /* recalculate leaf node fields */
        int result = _bintree_find_max( (b_tree_t *) tmp_node->pleft );
        if( result == UNINITIALIZED )
            return result;
    /* greater of two: key or  btree max is the right_max*/
        tmp_node->right_max = _max_number( (int) tmp_node->key, result );

        /* search for least number in btree*/
        result = _bintree_find_min( (b_tree_t *) tmp_node->pleft);
        if( result == UNINITIALIZED )
            return result;

        /* least of two: key or btree min is the left_min */
        tmp_node->left_min = _min_number( (int) tmp_node->key, result );

        /* update leaf fields */
        tmp_node->measure =
          _min_number(tmp_node->right, tmp_node->right_max) - tmp_node->left;

        return 0;
}

void _mtree_close_interval_gap(m_tree_t *node, int new_right ){
/* gap between left-subtree's right most leaf
 * and right-subtree's left most  leaf
 */

    m_tree_t *gap_path_stack[STACKSIZE];
    for(int i=0; i < STACKSIZE; ++i) gap_path_stack[i] = NULL;
    

    int gap_path_st_p = 0;
    m_tree_t *tmp_mtree = node;
    while( tmp_mtree->pright != NULL){
    /* search for left-subtree's right most leaf */
        gap_path_stack[gap_path_st_p++] = tmp_mtree;
        tmp_mtree = tmp_mtree->pright;
    }

    /* update left-subtree's right-most leafs' field:right*/
    tmp_mtree->right = new_right;
    
    /* leaf */
    tmp_mtree->measure =
          _min_number(tmp_mtree->right, tmp_mtree->right_max) - tmp_mtree->left;


    while( gap_path_st_p > 0 ){
    /* unwind the stack, update interior node's field */
        tmp_mtree = gap_path_stack[--gap_path_st_p];
        _mtree_interior_node_field_update( tmp_mtree );
        gap_path_stack[gap_path_st_p] = NULL;
    }
}


/* pg 164-165 - Advc_dtstr - P.Brass */
int _mtree_internal_node_calc_measure( m_tree_t *mtree ){

    if( mtree == NULL ) return UNINITIALIZED;

    int result;
    int right = ( mtree->right == INT_MAX)? mtree->right_max : mtree->right;
    int left  = mtree->left;

    if( mtree->pright->left_min < left &&
            mtree->pleft->right_max >= mtree->right )
            result = ( right - left );
    else
        if( mtree->pright->left_min >= left &&
                mtree->pleft->right_max >= mtree->right )
            result = ( ( right - mtree->key ) + mtree->pleft->measure);
    else
        if( mtree->pright->left_min < left &&
                mtree->pleft->right_max < mtree->right )
            result = (mtree->pright->measure + ( mtree->key - left ));
    else
        if( mtree->pright->left_min >= left &&
                mtree->pleft->right_max < mtree->right )
            result = ( mtree->pright->measure + mtree->pleft->measure );

    return result;
}

/* pg 164 - Advc_dtstr - P.Brass */
int _max_number(int a, int b ){
    return ( a < b ) ? b : a;
}

int _min_number(int a, int b){
    return ( a < b) ? a : b;
}


int mtree_size_left =0;
m_tree_t *mtree_currentblock = NULL;
m_tree_t *mtree_free_list = NULL;

m_tree_t *_get_mtree_node(){

  m_tree_t *tmp = NULL;
  if( mtree_free_list != NULL )
  {  tmp = mtree_free_list;
     mtree_free_list = mtree_free_list->pleft;
  }
  else
  {  if( mtree_currentblock == NULL || mtree_size_left == 0)
     {  mtree_currentblock =
                (m_tree_t *) malloc( BLOCKSIZE * sizeof(m_tree_t) );
        mtree_size_left = BLOCKSIZE;
     }
     tmp = mtree_currentblock++;
     mtree_size_left -= 1;
  }
  tmp->pleft = tmp->pright = NULL;
  return tmp;
}

void _mtree_return_node(m_tree_t *node){
   if( node == NULL) return;

   /* clear all fields */
   node->measure = node->key     =
   node->left    = node->right   =
   node->height  = node->left_min=
   node->right_max = 0;
   node->pright = node->pleft = NULL;

   node->pleft = mtree_free_list;
   mtree_free_list = node;
}

int _mtree_insert_node(m_tree_t *tree, key_t new_key, object_t new_object)
{

    m_tree_t *tmp_node;
    /* int finished; */

    m_tree_t *path_stack[STACKSIZE];
    for(int i=0; i <= STACKSIZE; ++i)  path_stack[i] = NULL;

    int  path_st_p = 0;
    tmp_node = tree;
    while( tmp_node->pright != NULL ){
    /* search for node leaf matching key */
      path_stack[path_st_p++] = tmp_node;
      tmp_node = (new_key < tmp_node->key) ? tmp_node->pleft : tmp_node->pright;
    }

    if( tmp_node->key == new_key ){
    /*  duplicate key found, save associated interval */

        int result =
        _bintree_insert_node( (b_tree_t *) tmp_node->pleft, (key_t) new_object);

        if( result == UNINITIALIZED)
            return result;

         result =_mtree_update_leaf_node_fields(tmp_node);
         if( result == UNINITIALIZED )
             return result;


    }
    else{
        /* key is distinct, now perform the insert */
        m_tree_t *old_leaf = NULL, *new_leaf = NULL;
        old_leaf            = _get_mtree_node();
        old_leaf->key       = tmp_node->key;
        old_leaf->measure   = tmp_node->measure;
        old_leaf->height    = tmp_node->height;
        old_leaf->left      = tmp_node->left;
        old_leaf->right     = tmp_node->right;
        old_leaf->pleft     = tmp_node->pleft;
        old_leaf->pright    = tmp_node->pright;
        old_leaf->left_min  = tmp_node->left_min;
        old_leaf->right_max = tmp_node->right_max;

        new_leaf = _get_mtree_node();
        new_leaf->pleft = (m_tree_t *) _bintree_get_node();
        new_leaf->key = new_key;
        new_leaf->height = 0;     
        new_leaf->left = (int) new_key;

        /* insert associated interval */
        int result
         =_bintree_insert_node((b_tree_t *)new_leaf->pleft, (key_t) new_object);
        if( result == UNINITIALIZED)
            return result;

        result =_mtree_update_leaf_node_fields(new_leaf);
         if( result == UNINITIALIZED )
             return result;

        /* tmp_node at tis point is a parent */
        if( tmp_node->key < new_key ){
        /* new node placed to the right */
            tmp_node->pleft  = old_leaf;
            tmp_node->pright = new_leaf;
            tmp_node->key = new_key;
            new_leaf->right = old_leaf->right;
            old_leaf->right = new_leaf->key;

        }
        else{
        /* new node placed to the left*/
            tmp_node->pleft  = new_leaf;
            tmp_node->pright = old_leaf;
            new_leaf->right = old_leaf->key;
        }

        /* leafs - update measures only after updating all other fields*/
        tmp_node->pleft->measure 
         = ( _min_number( tmp_node->pleft->right, tmp_node->pleft->right_max)
                - tmp_node->pleft->left );

         tmp_node->pright->measure
         = ( _min_number( tmp_node->pright->right, tmp_node->pright->right_max)
                - tmp_node->pright->left );

         _mtree_interior_node_field_update(tmp_node);

         tmp_node->height = 1;
    }

    while( path_st_p > 0 ){
    /* rebalance tree - update node values */

        int tmp_height;
        tmp_node = path_stack[--path_st_p];

        if( tmp_node->pleft->height  - tmp_node->pright->height == 2 ){

            if(tmp_node->pleft->pleft->height - tmp_node->pright->height == 1){
                _mtree_right_rotation( tmp_node );
                _mtree_interior_node_field_update(tmp_node->pright);

                tmp_node->pright->height =
                tmp_node->pright->pleft->height + 1;
                tmp_node->height = tmp_node->pright->height + 1;
            }
            else{

               _mtree_left_rotation( tmp_node->pleft );
               _mtree_interior_node_field_update(tmp_node->pleft->pleft);
               
               _mtree_right_rotation( tmp_node );
               _mtree_interior_node_field_update(tmp_node->pright);

               tmp_height = tmp_node->pleft->pleft->height;
               tmp_node->pleft->height  = tmp_height + 1;
               tmp_node->pright->height = tmp_height + 1;
               tmp_node->height = tmp_height + 2;
           }
        }
        else if ( tmp_node->pleft->height - tmp_node->pright->height == -2 )
        {
            if(tmp_node->pright->pright->height - tmp_node->pleft->height == 1){
                  _mtree_left_rotation( tmp_node );
                  _mtree_interior_node_field_update(tmp_node->pleft);
                  
                  tmp_node->pleft->height = tmp_node->pleft->pright->height + 1;
                  tmp_node->height = tmp_node->pleft->height + 1;
                }
                else{

                    _mtree_right_rotation( tmp_node->pright );
                    _mtree_interior_node_field_update(tmp_node->pright->pright);

                    _mtree_left_rotation( tmp_node );
                    _mtree_interior_node_field_update(tmp_node->pleft);

                    tmp_height = tmp_node->pright->pright->height;
                    tmp_node->pleft->height  = tmp_height + 1;
                    tmp_node->pright->height = tmp_height + 1;
                    tmp_node->height = tmp_height + 2;
                 }
        }
        else{
        /* update height even if there was no rotation */
           if( tmp_node->pleft->height > tmp_node->pright->height )
              tmp_node->height = tmp_node->pleft->height + 1;
           else
              tmp_node->height = tmp_node->pright->height + 1;

       }

       _mtree_interior_node_field_update(tmp_node);
    }

   return 0;
}

void _mtree_interior_node_field_update(m_tree_t * tmp_node){

    /* interior fields */
    tmp_node->left = tmp_node->pleft->left;
    tmp_node->right= tmp_node->pright->right;

    tmp_node->left_min =
        _min_number(tmp_node->pleft->left_min, tmp_node->pright->left_min);

    tmp_node->right_max =
        _max_number(tmp_node->pleft->right_max, tmp_node->pright->right_max);

    /* measure of interior nodes calculated differntly*/
    tmp_node->measure = _mtree_internal_node_calc_measure(tmp_node);
    if( tmp_node->measure == UNINITIALIZED){
        perror("\nERROR: _mtree_internal_node_calc_measure() failed\n");
        
    }

}

void _mtree_left_rotation(m_tree_t *n){
   m_tree_t *tmp_node;
   key_t     tmp_key;
   tmp_node = n->pleft;
   tmp_key  = n->key;
   n->pleft  = n->pright;
   n->key   = n->pright->key;
   n->pright = n->pleft->pright;
   n->pleft->pright = n->pleft->pleft;
   n->pleft->pleft  = tmp_node;
   n->pleft->key   = tmp_key;
}

void _mtree_right_rotation(m_tree_t *n){
   m_tree_t *tmp_node;
   key_t        tmp_key;
   tmp_node = n->pright;
   tmp_key  = n->key;
   n->pright = n->pleft;
   n->key   = n->pleft->key;
   n->pleft  = n->pright->pleft;
   n->pright->pleft = n->pright->pright;
   n->pright->pright  = tmp_node;
   n->pright->key   = tmp_key;
}


int btree_size_left =0;
b_tree_t *btree_current_block = NULL;
b_tree_t *btree_free_list = NULL;

b_tree_t *_bintree_get_node(){

  b_tree_t *tmp = NULL;
  if( btree_free_list != NULL ) {
      tmp = btree_free_list;
      btree_free_list = btree_free_list->left;
  }
  else
  {  if( btree_current_block == NULL || btree_size_left == 0){
         btree_current_block
           = (b_tree_t *) malloc(BLOCKSIZE * sizeof(b_tree_t));
         btree_size_left = BLOCKSIZE;
      }
      tmp = btree_current_block++;
      btree_size_left -= 1;
  }
  tmp->left = tmp->right = NULL;
  return tmp;
}

void _bintree_return_node(b_tree_t *b_node){
    if( b_node == NULL) return;
    b_node->key       =
    b_node->height    =
    b_node->ref_count = UNINITIALIZED;
    b_node->right     = b_node->left = NULL;
    b_node->left = btree_free_list;
    btree_free_list = b_node;
}

int _bintree_insert_node(b_tree_t *tree, key_t new_key ){

    if( tree == NULL ) return UNINITIALIZED;

    b_tree_t *tmp_node = NULL;
    object_t *obj = NULL;

    if( tree->left == NULL ){
    /* single node tree*/
        tree->key  = new_key;
        tree->height = 0;
        tree->ref_count = 1;

        /* store value on heap */
        obj = (object_t *) malloc( sizeof(object_t) );
        if( obj == NULL ){
            perror("\nERROR: allocating dynamic memory\n");
            
        }

        *obj = (object_t ) new_key;
        tree->left = (b_tree_t *) obj;
        return 0;
    }
    else
    {
        tmp_node = tree;
        int  path_st_p = 0;
        b_tree_t *path_stack[STACKSIZE];
        while( tmp_node->right != NULL ){
        /* find leaf */
            path_stack[path_st_p++] = tmp_node;
            tmp_node=(new_key < tmp_node->key)? tmp_node->left: tmp_node->right;
        }

        if( tmp_node->key == new_key ){
        /* key already exists, increment refernce */
            ++(tmp_node->ref_count);
            return 0;
        }

        /* copy all of left's fields to tmp node */
        b_tree_t *old_leaf = NULL, *new_leaf = NULL;
        old_leaf = _bintree_get_node();
        old_leaf->left = tmp_node->left;
        old_leaf->key = tmp_node->key;
        old_leaf->ref_count = tmp_node->ref_count;
        old_leaf->height = 0;

        /* store object on heap*/
        new_leaf = _bintree_get_node();
        obj = (object_t *) malloc( sizeof(object_t) );
        if( obj == NULL ){
            perror("\nERROR: allocating dynamic memory\n");
            
        }

        /* new node, save incomming fields*/
        *obj = (object_t )new_key;
        new_leaf->left = (b_tree_t *) obj;
        new_leaf->ref_count = 1;
        new_leaf->key = new_key;
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
        tmp_node->ref_count = 0;
        tmp_node->height = 1;
       
       int finished = 0;
       while( path_st_p > 0 && !finished ){
       /* rebalance */
           int tmp_height, old_height;
           tmp_node = path_stack[--path_st_p];
           old_height= tmp_node->height;

           if( tmp_node->left->height - tmp_node->right->height == 2 ){
               if( tmp_node->left->left->height - tmp_node->right->height == 1 ){
                    _bintree_right_rotation( tmp_node );
                    tmp_node->right->height =
                    tmp_node->right->left->height + 1;
                    tmp_node->height = tmp_node->right->height + 1;
               }
               else{
                   _bintree_left_rotation( tmp_node->left );
                   _bintree_right_rotation( tmp_node );
                   tmp_height = tmp_node->left->left->height;
                   tmp_node->left->height  = tmp_height + 1;
                   tmp_node->right->height = tmp_height + 1;
                   tmp_node->height = tmp_height + 2;
              }
           }
           else if ( tmp_node->left->height - tmp_node->right->height == -2 ){
               if( tmp_node->right->right->height -
                       tmp_node->left->height == 1 )
               {
                   _bintree_left_rotation( tmp_node );
                   tmp_node->left->height
                    = tmp_node->left->right->height + 1;
                    tmp_node->height = tmp_node->left->height + 1;
               }
               else
               {
                   _bintree_right_rotation( tmp_node->right );
                   _bintree_left_rotation( tmp_node );
                   tmp_height = tmp_node->right->right->height;
                   tmp_node->left->height  = tmp_height + 1;
                   tmp_node->right->height = tmp_height + 1;
                   tmp_node->height = tmp_height + 2;
               }
          }
          else{
          /* update height even when no rotation */

              if( tmp_node->left->height > tmp_node->right->height )
                  tmp_node->height = tmp_node->left->height + 1;
              else
                  tmp_node->height = tmp_node->right->height + 1;
          }

          if( tmp_node->height == old_height ) finished = 1;
       }

    }
    return 0;
}

int _bintree_delete_node(b_tree_t *tree, key_t delete_key){

    if( tree == NULL )  return UNINITIALIZED;

    b_tree_t  *tmp_node = tree, *upper_node = NULL, *other_node = NULL;
    b_tree_t *path_stack[STACKSIZE]; int path_st_p = 0;

    for(int i=0; i <= STACKSIZE; ++i)  path_stack[i] = NULL;

    while( tmp_node->right != NULL ){
    /* search for leaf node */
        path_stack[path_st_p++] = tmp_node;
        upper_node = tmp_node;
        if( delete_key < tmp_node->key ){
            tmp_node   = upper_node->left;
            other_node = upper_node->right;
        }
        else{
            tmp_node   = upper_node->right;
            other_node = upper_node->left;
        }
    }

    if( tmp_node->key != delete_key ){
    /* failed to find key */
        return UNINITIALIZED;
    }
    else if( tmp_node->left == NULL ){
    /* empty root node */
        _bintree_return_node( tmp_node);
        return 0;
    }

     --(tmp_node->ref_count);
    if( tmp_node->ref_count >= 1  ){
    /* a reference to this key still exists*/
        return 0;
    }

    if( upper_node == NULL ){
    /* right pointer is null - single leaf */
        free( tmp_node->left );
        _bintree_return_node( tmp_node );
        return 0;

    }
    else{
    /* has neighboring node */
        
        /* neighbor node is copied to parent */
        upper_node->key   = other_node->key;
        upper_node->left  = other_node->left;
        upper_node->right = other_node->right;
        upper_node->ref_count = other_node->ref_count;
        upper_node->height = other_node->height;

        /* delete node associated to key, and its neighbor */
        free ( tmp_node->left );
        _bintree_return_node( tmp_node );
        _bintree_return_node( other_node );

    }

    /*start rebalance*/
    int finished = 0; path_st_p -= 1;
    while( path_st_p > 0 && !finished ){
        int tmp_height, old_height;
        tmp_node = path_stack[--path_st_p];
        old_height= tmp_node->height;

        if( tmp_node->left->height - tmp_node->right->height == 2 ){
            if( tmp_node->left->left->height - tmp_node->right->height == 1 ){
                 _bintree_right_rotation( tmp_node );
                 tmp_node->right->height = tmp_node->right->left->height + 1;
                 tmp_node->height = tmp_node->right->height + 1;
             }
             else {
                 _bintree_left_rotation( tmp_node->left );
                 _bintree_right_rotation( tmp_node );
                 tmp_height = tmp_node->left->left->height;
                 tmp_node->left->height  = tmp_height + 1;
                 tmp_node->right->height = tmp_height + 1;
                 tmp_node->height = tmp_height + 2;
             }
        }
        else if ( tmp_node->left->height - tmp_node->right->height == -2 ){
            if( tmp_node->right->right->height - tmp_node->left->height == 1 ){
                _bintree_left_rotation( tmp_node );
                tmp_node->left->height = tmp_node->left->right->height + 1;
                tmp_node->height = tmp_node->left->height + 1;
            }
            else
            {  _bintree_right_rotation( tmp_node->right );
               _bintree_left_rotation( tmp_node );
               tmp_height = tmp_node->right->right->height;
               tmp_node->left->height  = tmp_height + 1;
               tmp_node->right->height = tmp_height + 1;
               tmp_node->height = tmp_height + 2;
            }
        }
        else{
        /* update height even if there was no rotation */
            if( tmp_node->left->height > tmp_node->right->height )
                tmp_node->height = tmp_node->left->height + 1;
            else
                tmp_node->height = tmp_node->right->height + 1;
        }

        if( tmp_node->height == old_height )  finished = 1;
    }


    return 0;

}

int _bintree_find_max( b_tree_t *tree ){

    int num = UNINITIALIZED;
    if( tree->right != NULL )
    /* recursively find right most key */
        num = _bintree_find_max( tree->right);
    else if( tree != NULL )
        num = *((int *)tree->left);
        
    return num;
}

int _bintree_find_min( b_tree_t * tree ){
    
    int num = UNINITIALIZED;
    if( tree->right != NULL )
    /* recursively find right most key */
        num  = _bintree_find_min( tree->left);
    else if( tree != NULL )
        num = *((int *)tree->left);

    return num;
}

void _bintree_left_rotation(b_tree_t *n){
   b_tree_t *tmp_node;
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

void _bintree_right_rotation(b_tree_t *n){
   b_tree_t *tmp_node;
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
