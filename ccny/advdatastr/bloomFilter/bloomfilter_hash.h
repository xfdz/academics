/*
 * Author: Javier Fernandez
 * CCNY-CSC-I9604 adv data struc
 * Fall-2010 Homework Assignment 4
 * Bloom filter hash table, 200k hashable strings
 */

#ifndef BLOOMFILTER_HASH_H
#define	BLOOMFILTER_HASH_H


#define BLOCKSIZE        256
#define MAXP             227832
#define BF_TABLES        10
#define MAXNUM_STRINGS   200000
#define MAX_HASH_STRINGS 25000

typedef struct htp_l_node {
    int a;
    struct htp_l_node *next;
} htp_l_node_t;

typedef struct bf_param{
    int b;
    htp_l_node_t *a_list;
} bf_param_t;


typedef struct bf{
    bf_param_t bf_param[BF_TABLES];
    unsigned char *abits[BF_TABLES];
} bf_t;

bf_t *create_bf();
htp_l_node_t *get_node();
void insert_bf(bf_t *b, char *s);
int is_element(bf_t *b, char *q);
int universalhashfunction(char *, bf_param_t);



#endif	/* BLOOMFILTER_HASH_H */

