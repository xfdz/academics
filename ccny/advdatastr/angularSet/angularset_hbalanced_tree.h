/* 
 * File:   angularset_hbalanced_tree.h
 * Created on September 24, 2010, 6:56 PM
 * 
 * Author: Javier Fernandez
 * CCNY-CSCI-I9604 Advanced data structure
 * Fall 2010 HW# 1
 *
 */

#ifndef ANGULARSET_HBALANCED_TREE_H
#define	ANGULARSET_HBALANCED_TREE_H

typedef float object_t;
typedef float key_t;

typedef struct agl_s_t{
    key_t  key;
    int    height;
    struct agl_s_t *right;
    struct agl_s_t *left;
}angle_set_t;

/* 
 * Function declarations
 */
angle_set_t *create_aset();
void insert_aset(angle_set_t *s, float a);
void delete_aset(angle_set_t *s, float a );
int count_aset(angle_set_t *tree, float a, float b);
int count_aset_range(angle_set_t *tree, float a, float b);
float first_aset(angle_set_t *tree, float a, float b );
float last_aset(angle_set_t *tree, float a, float b);


/*
 * Prof.Brass's functions
*/
angle_set_t *_get_node();
void _return_node(angle_set_t *node);
int _insert(angle_set_t *tree, key_t new_key, object_t *new_object);
object_t *_delete(angle_set_t *tree, key_t delete_key);
void _left_rotation(angle_set_t *n);
void _right_rotation(angle_set_t *n);


#endif	/* ANGULARSET_HBALANCED_TREE_H */

