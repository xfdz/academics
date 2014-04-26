/*
 * File: ordered_tree.h
 * Author: Javier Fernandez
 * CCNY-CSC-I9604 adv data struc
 * Fall-2010 Homework Assignment 3
 * Structure maintains linear Ordered
 */

#ifndef LINEAR_ORDER_TREE_H
#define	LINEAR_ORDER_TREE_H

typedef long key_t;

/* only the root of o_t has top,bottom initialized*/
typedef struct linear_ordered_tree{
    key_t key;
    int height;
    struct linear_ordered_tree *parent;
    struct linear_ordered_tree *top;
    struct linear_ordered_tree *bottom;
    struct linear_ordered_tree *left;
    struct linear_ordered_tree *right;

}l_o_t;

typedef struct ordered_tree{
    key_t key;
    int height;
    l_o_t  *lot_root;
    struct ordered_tree  *left;
    struct ordered_tree  *right;
}o_t;

/* public functions */
o_t* create_order();
void insert_before(o_t *ord, key_t a, key_t b);
void insert_after(o_t *ord, key_t a, key_t b);
void insert_top(o_t *ord, key_t a);
void insert_bottom(o_t *ord, key_t a);
void delete_o(o_t *ord, key_t a);
int compare(o_t *ord, key_t a, key_t b);

/* private functions */
o_t* _o_t_get_node();
void _o_t_return_node(o_t *o_t_node);
l_o_t *_o_t_get_l_o_t_root(o_t *tmp_node);
l_o_t *_o_t_get_l_o_t_bottom_node(o_t *tmp_node);
l_o_t *_o_t_get_l_o_t_top_node(o_t *tmp_node);
o_t *_o_t_get_node();
void _o_t_return_node(o_t* );
l_o_t* _l_o_t_get_node();
void   _l_o_t_return_node(l_o_t *l_o_t_node);
void _l_o_t_rebalance_tree(l_o_t *tr);
void _o_t_rebalance_tree(o_t **path_stack, int path_st_p );
void _l_o_t_left_rotation(l_o_t *n);
void _l_o_t_right_rotation(l_o_t *n);
void _o_t_left_rotation(o_t *n);
void _o_t_right_rotation(o_t *n);
l_o_t *_o_t_find_l_o_t_node(o_t *ord, key_t key);


#endif	/* LINEAR_ORDER_TREE_H */

