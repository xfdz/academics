/*
 * File: ordered_tree.c
 * Author: Javier Fernandez
 * CCNY-CSC-I9604 adv data struc
 * Fall-2010 Homework Assignment 3
 * Structure maintains linear Ordered
 */



#include <stdio.h>
#include <stdlib.h>
#include "ordered_tree.h"


#define BLOCKSIZE 256
#define STACKSIZE 100
#define UNINITIALIZED -1
#define ERROR -1


/* creates an empty linear ordered set */
o_t* create_order(){

    o_t *o_t_root      = _o_t_get_node();
    o_t_root->lot_root = _l_o_t_get_node();
    o_t_root->left     = (o_t *)  o_t_root->lot_root;
    
    o_t_root->lot_root->top    = o_t_root->lot_root;
    o_t_root->lot_root->bottom = o_t_root->lot_root;

    return o_t_root;
}

/* inserts the key a immediately before key b in the ordered set */
void insert_before(o_t *ord, key_t a, key_t b){
     if( ord == NULL ) return;

    if( ord->right == NULL && ord->height == 0 ){
    /* single o_t node */
        insert_bottom(ord,a);
        return;
    }

    int stack_p = 0;
    o_t *path_st_p[STACKSIZE];
    for(int i=0; i <STACKSIZE; ++i ) path_st_p[i] = NULL;

    o_t *b_ot_leaf = ord;
    while( b_ot_leaf->right != NULL){
    /* find o_t leaf */
        path_st_p[stack_p++] = b_ot_leaf;
        if( b < b_ot_leaf->key  )
            b_ot_leaf = b_ot_leaf->left;
        else
            b_ot_leaf  = b_ot_leaf->right;
    }

    if( b_ot_leaf->key != b ){
        printf("ERROR: in insert_before(), key (%ld) not found in o_t \n", b);
        exit(1);
    }

    int is_bottom_need_update = ( ord->lot_root->bottom->key == b ) ? 1 : 0;

    /* begin insertion in l_o_t tree*/
    l_o_t *b_lot_leaf    = NULL;
    l_o_t *new_lot_leaf  = NULL;
    l_o_t *new_lot_upper = NULL;
    l_o_t *lot_parent    = NULL;

    b_lot_leaf = (l_o_t *) b_ot_leaf->left;
    lot_parent = b_lot_leaf->parent;

    /* new parent leaf */
    new_lot_upper = _l_o_t_get_node();
    new_lot_upper->top    = NULL;
    new_lot_upper->bottom = NULL;
    new_lot_upper->parent = lot_parent;

    if( lot_parent != NULL ){
        if( lot_parent->left != b_lot_leaf )
            lot_parent->right = new_lot_upper;
        else
            lot_parent->left  = new_lot_upper;
    }

    /* new leaf */
    new_lot_leaf        = _l_o_t_get_node();
    new_lot_leaf->key   = a;
    new_lot_leaf->height= 0;
    new_lot_leaf->right = NULL;
    key_t *obj = (key_t *) malloc( sizeof(key_t));
    *obj = a;
    new_lot_leaf->left   = (l_o_t *) obj;
    new_lot_leaf->top    = NULL;
    new_lot_leaf->bottom = NULL;
    new_lot_leaf->parent = new_lot_upper;

    /* update- leaf's parent*/
    b_lot_leaf->parent = new_lot_upper;
    if( a < b )
        new_lot_upper->key = b;
    else
        new_lot_upper->key = a;

    /* a comes before b*/
    new_lot_upper->left  = new_lot_leaf;
    new_lot_upper->right = b_lot_leaf;

    if( is_bottom_need_update )
        ord->lot_root->bottom = new_lot_leaf;

    _l_o_t_rebalance_tree(new_lot_leaf);


    stack_p = 0;
    for(int i=0; i <STACKSIZE; ++i ) path_st_p[i] = NULL;

    b_ot_leaf = ord;
    while( b_ot_leaf->right != NULL){
    /* find o_t leaf */
        path_st_p[stack_p++] = b_ot_leaf;
        if( a < b_ot_leaf->key  )
            b_ot_leaf = b_ot_leaf->left;
        else
            b_ot_leaf  = b_ot_leaf->right;
    }


    /* insert new key in o_t tree*/
    o_t *new_ot_leaf   = NULL;
    o_t *other_ot_leaf = NULL;

    /* a new leaf to stand for the old one */
    other_ot_leaf           = _o_t_get_node();
    other_ot_leaf->key      = b_ot_leaf->key;
    other_ot_leaf->height   = b_ot_leaf->height;
    other_ot_leaf->lot_root = NULL;
    other_ot_leaf->left     = b_ot_leaf->left;
    other_ot_leaf->right    = b_ot_leaf->right;

    new_ot_leaf           = _o_t_get_node();
    new_ot_leaf->key      = a;
    new_ot_leaf->height   = 0;
    new_ot_leaf->lot_root = NULL;
    new_ot_leaf->left     = (o_t *) new_lot_leaf;
    new_ot_leaf->right    = NULL;

    if( a < other_ot_leaf->key){
        b_ot_leaf->left  = new_ot_leaf;
        b_ot_leaf->right = other_ot_leaf;
    }
    else{
        b_ot_leaf->key  = new_ot_leaf->key;
        b_ot_leaf->left = other_ot_leaf;
        b_ot_leaf->right= new_ot_leaf;
        
    }
    
     path_st_p[stack_p++] = b_ot_leaf;

    _o_t_rebalance_tree(path_st_p, stack_p );


}

/* inserts the key a immediately after key b in the ordered set */
void insert_after(o_t *ord, key_t a, key_t b){
    if( ord == NULL ) return;

    if( ord->right == NULL && ord->height == 0 ){
    /* single o_t node */
        insert_top(ord,a);
        return;
    }

    int stack_p = 0;
    o_t *path_st_p[STACKSIZE];
    for(int i=0; i <STACKSIZE; ++i ) path_st_p[i] = NULL;

    o_t *b_ot_leaf = ord;
    while( b_ot_leaf->right != NULL){
    /* find o_t leaf */
        path_st_p[stack_p++] = b_ot_leaf;
        if( b < b_ot_leaf->key  )
            b_ot_leaf = b_ot_leaf->left;
        else
            b_ot_leaf  = b_ot_leaf->right;
    }

    if( b_ot_leaf->key != b ){
        printf("ERROR: in insert_after(), key (%ld) not found in o_t \n", b);
        exit(1);
    }

    int is_top_need_update = ( ord->lot_root->top->key == b ) ? 1 : 0;

    /* begin insertion in l_o_t tree*/
    l_o_t *b_lot_leaf    = NULL;
    l_o_t *new_lot_leaf  = NULL;
    l_o_t *new_lot_upper = NULL;
    l_o_t *lot_parent    = NULL;

    b_lot_leaf = (l_o_t *) b_ot_leaf->left;
    lot_parent = b_lot_leaf->parent;

    /* new parent leaf */
    new_lot_upper = _l_o_t_get_node();
    new_lot_upper->top    = NULL;
    new_lot_upper->bottom = NULL;
    new_lot_upper->parent = lot_parent;

    if( lot_parent != NULL ){
        if( lot_parent->left != b_lot_leaf )
            lot_parent->right = new_lot_upper;
        else
            lot_parent->left  = new_lot_upper;
    }

    /* new leaf */
    new_lot_leaf        = _l_o_t_get_node();
    new_lot_leaf->key   = a;
    new_lot_leaf->height= 0;
    new_lot_leaf->right = NULL;
    key_t *obj = (key_t *) malloc( sizeof(key_t));
    *obj = a;
    new_lot_leaf->left   = (l_o_t *) obj;
    new_lot_leaf->top    = NULL;
    new_lot_leaf->bottom = NULL;
    new_lot_leaf->parent = new_lot_upper;

    /* update- leaf's parent*/
    b_lot_leaf->parent = new_lot_upper;
    if( a < b )
        new_lot_upper->key = b;
    else
        new_lot_upper->key = a;

    /* a comes after b*/
    new_lot_upper->left  = b_lot_leaf;
    new_lot_upper->right = new_lot_leaf;

    if( is_top_need_update )
        ord->lot_root->top = new_lot_leaf;

    _l_o_t_rebalance_tree(new_lot_leaf);


    stack_p = 0;
    for(int i=0; i <STACKSIZE; ++i ) path_st_p[i] = NULL;

    b_ot_leaf = ord;
    while( b_ot_leaf->right != NULL){
    /* find o_t leaf */
        path_st_p[stack_p++] = b_ot_leaf;
        if( a < b_ot_leaf->key  )
            b_ot_leaf = b_ot_leaf->left;
        else
            b_ot_leaf  = b_ot_leaf->right;
    }


    /* insert new key in o_t tree*/
    o_t *new_ot_leaf   = NULL;
    o_t *other_ot_leaf = NULL;

    /* a new leaf to stand for the old one */
    other_ot_leaf           = _o_t_get_node();
    other_ot_leaf->key      = b_ot_leaf->key;
    other_ot_leaf->height   = b_ot_leaf->height;
    other_ot_leaf->lot_root = NULL;
    other_ot_leaf->left     = b_ot_leaf->left;
    other_ot_leaf->right    = b_ot_leaf->right;

    new_ot_leaf           = _o_t_get_node();
    new_ot_leaf->key      = a;
    new_ot_leaf->height   = 0;
    new_ot_leaf->lot_root = NULL;
    new_ot_leaf->left     = (o_t *) new_lot_leaf;
    new_ot_leaf->right    = NULL;

    if( a < other_ot_leaf->key){
        b_ot_leaf->left  = new_ot_leaf;
        b_ot_leaf->right = other_ot_leaf;
    }
    else{
        b_ot_leaf->key  = new_ot_leaf->key;
        b_ot_leaf->left = other_ot_leaf;
        b_ot_leaf->right= new_ot_leaf;

    }

     path_st_p[stack_p++] = b_ot_leaf;

    _o_t_rebalance_tree(path_st_p, stack_p );
    
}

/* inserts the key a as largest element in the ordered set */
void insert_top(o_t *ord, key_t a){

    key_t *obj = NULL;
    if( ord->right == NULL &&  (ord->key == UNINITIALIZED) ){
    /* single node o_t tree, associated l_o_t mirrors o_t in height */
        l_o_t *tmp_loc = NULL;
        ord->key    = a;
        ord->height = 0;
        tmp_loc = (l_o_t *) ord->left;

        /* update associated l_o_t node */
        tmp_loc->key = a;
        tmp_loc->height = 0;
        obj = (key_t *) malloc(sizeof(key_t) );
        *obj = a;
        tmp_loc->left   = (l_o_t *) obj;

        /* make top/bottom point to itself*/
        tmp_loc->top    = tmp_loc;
        tmp_loc->bottom = tmp_loc;

        return;
    }

     /* get the current leaf representing top*/
    l_o_t *old_lot_leaf = ord->lot_root->top;

    int is_restore_lot_ptr = ( ord->lot_root->height == 0) ? 1 : 0;

    l_o_t *new_lot_leaf  = NULL;
    l_o_t *new_lot_upper = NULL;
    l_o_t *old_lot_leaf_parent = old_lot_leaf->parent;

   /* new interior, parent of new and old top*/
    new_lot_upper  = _l_o_t_get_node();

    if( old_lot_leaf_parent != NULL ){
        if( old_lot_leaf_parent->right == old_lot_leaf )
        /* determine from whence we came: left or right? */
            old_lot_leaf_parent->right = new_lot_upper;
        else
            old_lot_leaf_parent->left = new_lot_upper;
    }

    new_lot_upper->parent = old_lot_leaf_parent;
    old_lot_leaf->parent  = new_lot_upper;
    /* leaf's can't access these attributes, only root node*/
    old_lot_leaf->top    = NULL;
    old_lot_leaf->bottom = NULL;


    /* leaf*/
    new_lot_leaf         = _l_o_t_get_node();
    new_lot_leaf->parent = new_lot_upper;
    new_lot_leaf->key    = a;
    new_lot_leaf->height = 0;

    obj = (key_t *) malloc(sizeof(key_t));
    *obj = a;
    new_lot_leaf->left = (l_o_t *) obj;
    new_lot_leaf->right= NULL;
    /* leaf's can't access these attributes, only root node*/
    new_lot_leaf->top    = NULL;
    new_lot_leaf->bottom = NULL;

    if(new_lot_leaf->key < old_lot_leaf->key )
    /*interior node key choses largest of two*/
        new_lot_upper->key = old_lot_leaf->key;
    else
        new_lot_upper->key = new_lot_leaf->key;

    /* new_lot, comes after old_lot*/
    new_lot_upper->left = old_lot_leaf;
    new_lot_upper->right= new_lot_leaf;


     if( is_restore_lot_ptr == 1  ){
     /* special case-pointer to root of l_o_t restoration*/
        ord->lot_root = new_lot_upper;
        /* also relinking l_o_t right most as top*/
        ord->lot_root->bottom = old_lot_leaf;
     }


    /* update l_o_t's new bottom object */
    ord->lot_root->top = new_lot_leaf;

    /* update field: bottom*/
    _l_o_t_rebalance_tree(new_lot_leaf);


    /* insert into o_t key associated to new l_o_t bottom*/
    int path_st_p = 0;
    o_t *path_stack[STACKSIZE];
    for(int i=0; i !=STACKSIZE; ++i ) path_stack[i] = NULL;

    o_t *tmp_ord = NULL;
    tmp_ord = ord;
    while( tmp_ord->right != NULL){
        path_stack[path_st_p++] = tmp_ord;
        if(  a < tmp_ord->key )
            tmp_ord = tmp_ord->left;
        else
            tmp_ord = tmp_ord->right;
    }

    if( tmp_ord->key == a){
        printf("WARN: Duplicate key inserted: (%d)", (int) tmp_ord->key);
        exit(1);
    }

    o_t *new_ord = NULL, *old_ord = NULL;
    old_ord = _o_t_get_node();
    old_ord->key     = tmp_ord->key;
    old_ord->height  = 0;
    old_ord->lot_root= NULL;
    old_ord->right   = tmp_ord->right;
    old_ord->left    = tmp_ord->left;

    new_ord = _o_t_get_node();
    new_ord->key      = a;
    new_ord->height   = 0;
    new_ord->lot_root = NULL;
    new_ord->right    = NULL;

    /* new o_t leaf points to new bottom l_o_t leaf*/
    new_ord->left = (o_t *)new_lot_leaf;

    if( a < tmp_ord->key){
        tmp_ord->left = new_ord;
        tmp_ord->right = old_ord;
    }
    else{
        tmp_ord->key  = a;
        tmp_ord->left = old_ord;
        tmp_ord->right= new_ord;
    }

    path_stack[path_st_p++] = tmp_ord;

    _o_t_rebalance_tree(path_stack, path_st_p );

}

/* inserts they key a as smallest element in the ordered set*/
void insert_bottom(o_t *ord, key_t a){

    key_t *obj = NULL;
    if( ord->right == NULL &&  (ord->key == UNINITIALIZED) ){
    /* single node o_t tree, associated l_o_t mirrors o_t in height */
        l_o_t *tmp_loc = NULL;
        ord->key    = a;
        ord->height = 0;
        tmp_loc = (l_o_t *) ord->left;

        /* update associated l_o_t node */
        tmp_loc->key = a;
        tmp_loc->height = 0;
        obj = (key_t *) malloc(sizeof(key_t) );
        *obj = a;
        tmp_loc->left   = (l_o_t *) obj;

        /* make top/bottom point to itself*/
        tmp_loc->top    = tmp_loc;
        tmp_loc->bottom = tmp_loc;

        return;
    }

    /* get the current leaf representing bottom*/
    l_o_t *old_lot_leaf = ord->lot_root->bottom;

    int is_restore_lot_ptr = ( ord->lot_root->height == 0) ? 1 : 0;
    
    l_o_t *new_lot_leaf  = NULL;
    l_o_t *new_lot_upper = NULL;
    l_o_t *old_lot_leaf_parent = old_lot_leaf->parent;

   /* new interior, parent of new and old bottom*/
    new_lot_upper  = _l_o_t_get_node();

    if( old_lot_leaf_parent != NULL ){
        if( old_lot_leaf_parent->right == old_lot_leaf )
        /* determine from whence we came: left or right? */
            old_lot_leaf_parent->right = new_lot_upper;
        else
            old_lot_leaf_parent->left = new_lot_upper;
    }

    new_lot_upper->parent = old_lot_leaf_parent;
    old_lot_leaf->parent  = new_lot_upper;
    /* leaf's can't access these attributes, only root node*/
    old_lot_leaf->top    = NULL;
    old_lot_leaf->bottom = NULL;

    /* leaf*/
    new_lot_leaf         = _l_o_t_get_node();
    new_lot_leaf->parent = new_lot_upper;
    new_lot_leaf->key    = a;
    new_lot_leaf->height = 0;

    obj = (key_t *) malloc(sizeof(key_t));
    *obj = a;
    new_lot_leaf->left = (l_o_t *) obj;
    new_lot_leaf->right= NULL;
    /* leaf's can't access these attributes, only root node*/
    new_lot_leaf->top    = NULL;
    new_lot_leaf->bottom = NULL;

    if(new_lot_leaf->key < old_lot_leaf->key )
    /*interior node key choses largest of two*/
        new_lot_upper->key = old_lot_leaf->key;
    else
        new_lot_upper->key = new_lot_leaf->key;

    /* new_lot, comes before old_lot*/
    new_lot_upper->left = new_lot_leaf;
    new_lot_upper->right= old_lot_leaf;


     if( is_restore_lot_ptr == 1  ){
     /* special case-pointer to root of l_o_t restoration*/
        ord->lot_root = new_lot_upper;
        /* also relinking l_o_t right most as top*/
        ord->lot_root->top = old_lot_leaf;
     }
  
  
    /* update l_o_t's new bottom object */
    ord->lot_root->bottom = new_lot_leaf;

    /* update field: bottom*/
    _l_o_t_rebalance_tree(new_lot_leaf);


    /* insert into o_t key associated to new l_o_t bottom*/
    int path_st_p = 0;
    o_t *path_stack[STACKSIZE];
    for(int i=0; i !=STACKSIZE; ++i ) path_stack[i] = NULL;
    
    o_t *tmp_ord = NULL;
    tmp_ord = ord;
    while( tmp_ord->right != NULL){
        path_stack[path_st_p++] = tmp_ord;
        if(  a < tmp_ord->key )
            tmp_ord = tmp_ord->left;
        else
            tmp_ord = tmp_ord->right;
    }

    if( tmp_ord->key == a){
        printf("\nWARN: Duplicate key inserted: (%d)\n", (int) tmp_ord->key);
    }

    o_t *new_ord = NULL, *old_ord = NULL;
    old_ord = _o_t_get_node();
    old_ord->key     = tmp_ord->key;
    old_ord->height  = 0;
    old_ord->lot_root= NULL;
    old_ord->right   = tmp_ord->right;
    old_ord->left    = tmp_ord->left;

    new_ord = _o_t_get_node();
    new_ord->key      = a;
    new_ord->height   = 0;
    new_ord->lot_root = NULL;
    new_ord->right    = NULL;

    /* new o_t leaf points to new bottom l_o_t leaf*/
    new_ord->left = (o_t *)new_lot_leaf;

    if( a < tmp_ord->key){
        tmp_ord->left = new_ord;
        tmp_ord->right = old_ord;
    }
    else{
        tmp_ord->key  = a;
        tmp_ord->left = old_ord;
        tmp_ord->right= new_ord;
    }

    path_stack[path_st_p++] = tmp_ord;

    _o_t_rebalance_tree(path_stack, path_st_p );

}

/* deletes the key a from the ordered set */
void delete_o(o_t *ord, key_t a){
    if( ord == NULL ) return;

    int stack_p = 0;
    o_t *path_st_p[STACKSIZE];
    for(int i=0; i <STACKSIZE; ++i ) path_st_p[i] = NULL;

    o_t *a_ot_delete = ord;
    o_t *a_ot_other = NULL;
    while( a_ot_delete->right != NULL){
        path_st_p[stack_p++] = a_ot_delete;
        if( a < a_ot_delete->key  ){
            a_ot_other  = a_ot_delete->right;
            a_ot_delete = a_ot_delete->left;
        }
        else{
            a_ot_other  = a_ot_delete->left;
            a_ot_delete = a_ot_delete->right;
        }
    }

    if( a_ot_delete->key != a ){
        printf("ERROR: in delete_o(), key (%ld) not found in o_t \n", a);
        exit(1);
    }

    /* find key: a in o_t tree*/
    l_o_t *a_lot_delete_node = NULL;
    a_lot_delete_node = (l_o_t *) a_ot_delete->left;

    /*
     * l_o_t tree node deleted in follwing order:
     * leaf, then parent of the leaf. Causing neighboring
     * node of leaf being deleted to become a parent or leaf.
     */

    int is_top_need_update    = 0;
    int is_bottom_need_update = 0;
    /* determine if l_o_t root's bottom/top point to this node*/
    if( ord->lot_root->top == a_lot_delete_node  )
        is_top_need_update = 1;
    else if( ord->lot_root->bottom == a_lot_delete_node)
        is_bottom_need_update = 1;

    if( a_lot_delete_node->parent == NULL )
    /* single associated l_o_t node */
        _l_o_t_return_node(a_lot_delete_node);
    else{
    /* more than one node in the l_o_t tree*/
        
        l_o_t *lot_other_node        = NULL;
        l_o_t *lot_parent_node       = NULL;
        l_o_t *lot_grand_parent_node = NULL;

        /* leaf's parent */
        lot_parent_node = a_lot_delete_node->parent;
        if( lot_parent_node->left != a_lot_delete_node)
        /* which leaf is neighbor leaf on left or and right */
            lot_other_node = lot_parent_node->left;
        else
            lot_other_node = lot_parent_node->right;

        lot_grand_parent_node = lot_parent_node->parent;
        if( lot_grand_parent_node == NULL ){
        /* two leaf tree */

            if( lot_other_node->right != NULL){
            /* we have a tree underneath */
                  if( is_top_need_update == 1){
                      lot_other_node->top    = lot_other_node->right;
                      lot_other_node->bottom = lot_parent_node->bottom;
                  }
                  else if ( is_bottom_need_update == 1){
                      lot_other_node->bottom = lot_other_node->left;
                      lot_other_node->top    = lot_parent_node->top;
                  }
            }
            else{
                 /* single leaf tree - update top/bottom to self*/
                lot_other_node->top    = lot_other_node;
                lot_other_node->bottom = lot_other_node;

            }

            lot_other_node->parent = lot_parent_node->parent;
            
            /* relink o_t tree with new l_o_t root*/
            ord->lot_root = lot_other_node;

            _l_o_t_return_node( a_lot_delete_node);
            _l_o_t_return_node( lot_parent_node  );
        }
        else{

            if( lot_grand_parent_node->left != lot_parent_node )
            /* grandparent new child*/
                lot_grand_parent_node->right = lot_other_node;
            else
                lot_grand_parent_node->left = lot_other_node;

            /* preserve the tree, like against new parent node*/
            lot_other_node->parent = lot_grand_parent_node;
            
            if( is_top_need_update == 1){
            /* node to be deleted was root's top*/
                if( lot_other_node->right != NULL)
                    /* take interiors leaf */
                    ord->lot_root->top = lot_other_node->right;
                else
                    /* its already the leaf*/
                    ord->lot_root->top = lot_other_node;
            }
            else if( is_bottom_need_update == 1){
            /* node to be deleted was root's bottom*/
                if( lot_other_node->right != NULL)
                    /* take interiors leaf */
                    ord->lot_root->bottom = lot_other_node->left;
                else
                    /* its already the leaf*/
                    ord->lot_root->bottom = lot_other_node;
            }

            /* dispose of these nodes*/
            _l_o_t_return_node(a_lot_delete_node);
            _l_o_t_return_node(lot_parent_node);
        }
        
        /* rebalance l_o_t tree*/
        _l_o_t_rebalance_tree(lot_other_node);
    }

    /* begin to delete the o_t node */
    if( ord->right == NULL && ord->height == 0 ){
    /* single o_t node */
        _o_t_return_node(ord);
        return;
    }

    /* down the stack find parent of leaf */
    int tmp = stack_p - 1;
    tmp = ( tmp < 0 ) ? 0 : tmp;
    
    o_t *a_ot_parent = path_st_p [tmp];

    a_ot_parent->key      = a_ot_other->key;
    a_ot_parent->height   = a_ot_other->height;
    a_ot_parent->left     = a_ot_other->left;
    a_ot_parent->right    = a_ot_other->right;

    _o_t_get_node(a_ot_delete);
    _o_t_get_node(a_ot_other);

    _o_t_rebalance_tree(path_st_p, tmp );
}

/* returns 1 if key a occurs before key b in the ordered set, 0 else*/
int compare(o_t *ord, key_t a, key_t b){
    if( ord == NULL){
        printf("\nERROR: in compare(), NULL object found in ord\n");
        exit(1);
    }
    
    /* we fist compare with top*/
    key_t result = ord->lot_root->top->key;

    /* a or b could be top */
    if( result == a)
        return 0;
    else if( result == b)
        return 1;
    
    /* a or b could be bottom*/
    result = ord->lot_root->bottom->key;
    if( result == a)
        return 1;
    else if( result == b)
        return 0;

    /* find keys: a,b on o_t tree, return associated l_o_t nodes*/
    l_o_t *a_lot_node = NULL, *b_lot_node = NULL;
    a_lot_node = _o_t_find_l_o_t_node(ord,a);
    b_lot_node = _o_t_find_l_o_t_node(ord ,b);

    if( a_lot_node == NULL ){
        printf("\nERROR: in compare(), %ld not found o_t tree\n", a);
        exit(1);
    }
    else if( b_lot_node == NULL ){
        printf("\nERROR: in compare(), %ld not found o_t tree\n", b);
        exit(1);
    }
    else if( a_lot_node == b_lot_node  ){
       printf("\nERROR: in compare(), %ld and %ld have same address\n",a,b);
       exit(1);
    }

    l_o_t *lot_child = NULL;
    while( a_lot_node !=  b_lot_node){
     /* find LCA: least common ancestor, walking up the tree */
 
        int diff = a_lot_node->height - b_lot_node->height;
        switch( diff ){
            case -1:
                lot_child = a_lot_node;
                a_lot_node = a_lot_node->parent;
                break;
            case 0:
                lot_child = a_lot_node;
                a_lot_node = a_lot_node->parent;
                b_lot_node = b_lot_node->parent;
                break;
            case 1:
                 b_lot_node = b_lot_node->parent;
                break;
            default:
                printf("\nERROR: in compare(), unexpected height %d",diff);
                exit(1);
        }

        if( a_lot_node == NULL && b_lot_node == NULL){
            printf("\nERROR: in compare(), finding LCA of %ld and %ld\n", a,b);
            exit(1);
        }
        
    }

    int cmp = ( a_lot_node->left == lot_child) ? 1 : 0;

    return cmp;

}

void _l_o_t_rebalance_tree(l_o_t *tr){

   int finished = 0;
   while( tr->parent != NULL && !finished ){
   /* rebalance */
       int tmp_height, old_height;
       tr = tr->parent;
       old_height = tr->height;

       if( tr->left->height - tr->right->height == 2 ){
           if( tr->left->left->height - tr->right->height == 1 ){
                _l_o_t_right_rotation( tr );
                tr->right->height =
                tr->right->left->height + 1;
                tr->height = tr->right->height + 1;
           }
           else{
               _l_o_t_left_rotation( tr->left );
               _l_o_t_right_rotation( tr );
               tmp_height = tr->left->left->height;
               tr->left->height  = tmp_height + 1;
               tr->right->height = tmp_height + 1;
               tr->height = tmp_height + 2;
          }
       }
       else if ( tr->left->height - tr->right->height == -2 ){
           if( tr->right->right->height -
                   tr->left->height == 1 )
           {
               _l_o_t_left_rotation( tr );
               tr->left->height
                = tr->left->right->height + 1;
                tr->height = tr->left->height + 1;
           }
           else
           {
               _l_o_t_right_rotation( tr->right );
               _l_o_t_left_rotation( tr );
               tmp_height = tr->right->right->height;
               tr->left->height  = tmp_height + 1;
               tr->right->height = tmp_height + 1;
               tr->height = tmp_height + 2;
           }
      }
      else{
      /* update height even when no rotation */

          if( tr->left->height > tr->right->height )
              tr->height = tr->left->height + 1;
          else
              tr->height = tr->right->height + 1;
      }


      if( tr->height == old_height ) finished = 1;
   }

}

void _o_t_rebalance_tree(o_t **path_stack, int path_st_p ){


   o_t *tmp_node = NULL;
   int finished = 0;
   while( path_st_p > 0 && !finished ){
   /* rebalance */
       int tmp_height, old_height;

       --path_st_p;
       tmp_node = *(path_stack + path_st_p);
       old_height= tmp_node->height;

       if( tmp_node->left->height - tmp_node->right->height == 2 ){
           if( tmp_node->left->left->height - tmp_node->right->height == 1 ){
                _o_t_right_rotation( tmp_node );
                tmp_node->right->height =
                tmp_node->right->left->height + 1;
                tmp_node->height = tmp_node->right->height + 1;
           }
           else{
               _o_t_left_rotation( tmp_node->left );
               _o_t_right_rotation( tmp_node );
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
               _o_t_left_rotation( tmp_node );
               tmp_node->left->height
                = tmp_node->left->right->height + 1;
                tmp_node->height = tmp_node->left->height + 1;
           }
           else
           {
               _o_t_right_rotation( tmp_node->right );
               _o_t_left_rotation( tmp_node );
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

      if( tmp_node->height == old_height )
          finished = 1;
   }

}

/* returns leaf to node in linear order tree*/
l_o_t *_o_t_find_l_o_t_node(o_t *ord, key_t key){

    l_o_t *result = NULL;
    if( ord->right != NULL ){
    /* find leaf */
        if( key < ord->key)
           result = _o_t_find_l_o_t_node(ord->left, key);
        else
            result = _o_t_find_l_o_t_node(ord->right, key);
    }
    else
        result = (ord->key == key )? (l_o_t *) ord->left : NULL;

    return result;
}


int l_o_t_size_left =0;
l_o_t *l_o_t_current_block = NULL;
l_o_t *l_o_t_free_list     = NULL;


l_o_t* _l_o_t_get_node(){

  l_o_t *tmp = NULL;
  if( l_o_t_free_list != NULL ) {
      tmp = l_o_t_free_list;
      l_o_t_free_list = l_o_t_free_list->left;
  }
  else
  {  if( l_o_t_current_block == NULL || l_o_t_size_left == 0){
         l_o_t_current_block  = (l_o_t *) malloc(BLOCKSIZE * sizeof(l_o_t));
         l_o_t_size_left = BLOCKSIZE;
      }
      tmp = l_o_t_current_block++;
      l_o_t_size_left -= 1;
  }

  tmp->key =
  tmp->height = UNINITIALIZED;

  tmp->parent=
  tmp->left  =
  tmp->right =
  tmp->top   =
  tmp->bottom=NULL;

  return tmp;
}

void _l_o_t_return_node(l_o_t *l_o_t_node){
    if( l_o_t_node == NULL) return;
    l_o_t_node->key   =
    l_o_t_node->height=  UNINITIALIZED;

    l_o_t_node->top   =
    l_o_t_node->bottom=
    l_o_t_node->parent=
    l_o_t_node->right =
    l_o_t_node->left  = NULL;

    l_o_t_node->left  = l_o_t_free_list;
    l_o_t_free_list = l_o_t_node;
}


int o_t_size_left =0;
o_t *o_t_current_block  = NULL;
o_t *o_t_node_free_list = NULL;
l_o_t *lot_NULL = NULL;


o_t *_o_t_get_node(){

  o_t *tmp = NULL;
  if( o_t_node_free_list != NULL ) {
      tmp = o_t_node_free_list;
      o_t_node_free_list = o_t_node_free_list->left;
  }
  else
  {  if( o_t_current_block == NULL || o_t_size_left == 0){
         o_t_current_block = (o_t *) malloc(BLOCKSIZE * sizeof(o_t));
         o_t_size_left = BLOCKSIZE;
      }
      tmp = o_t_current_block++;
      o_t_size_left -= 1;
  }
  tmp->key        =
  tmp->height     = UNINITIALIZED;
  tmp->left       =
  tmp->right      =NULL;
  tmp->lot_root = lot_NULL;
  
  return tmp;
}

void _o_t_return_node(o_t *o_t_node){
    if( o_t_node == NULL) return;
    
    o_t_node->key        =
    o_t_node->height     = UNINITIALIZED;
    o_t_node->right      =
    o_t_node->left       = NULL;

    /* gets rid of warnings: asignment to incompatible type*/
     o_t_node->lot_root = lot_NULL;
  
    
    o_t_node->left  = o_t_node_free_list;
    o_t_node_free_list = o_t_node;
}

void _l_o_t_left_rotation(l_o_t *n){
   l_o_t *tmp_node;
   key_t  tmp_key;
   tmp_node = n->left;
   tmp_key  = n->key;

   n->left  = n->right;
   n->key   = n->right->key;
   n->right = n->left->right;
   n->right->parent = n;
   n->left->right = n->left->left;
   n->left->left  = tmp_node;
   n->left->left->parent = n->left;
   n->left->key   = tmp_key;
}

void _l_o_t_right_rotation(l_o_t *n){
   l_o_t *tmp_node;
   key_t        tmp_key;
   tmp_node = n->right;
   tmp_key  = n->key;

   n->right = n->left;
   n->key   = n->left->key;
   n->left  = n->right->left;
   n->left->parent = n;
   n->right->left = n->right->right;
   n->right->right  = tmp_node;
   n->right->right->parent = n->right;
   n->right->key   = tmp_key;

}

void _o_t_left_rotation(o_t *n){
   o_t *tmp_node;
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

void _o_t_right_rotation(o_t *n){
   o_t *tmp_node;
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
