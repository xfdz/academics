/*
/* Author: Javier Fernandez
 * CCNY-CSC-I9604 adv data struc
 * Fall-2010 Homework Assignment 2
 * measure tree algorithm
*/


#ifndef MEASURE_TREE_H
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



#endif	/* MEASURE_TREE_H */

