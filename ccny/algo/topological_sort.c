/* Author: Javier Fernandez
 * CCNY-CSC 220 Algorithms, Fall Semester Homework Assignment 4 
 */
#include <stdio.h>
#include <stdlib.h>
#define TRUE      1
#define ZERODEGRE 0
#define NOVERTEX -1

struct listnode
{
  struct listnode *next;
  int vertexnumber;
};
struct degree
{
  int vertex;
  struct degree *next, *prev;
};
typedef struct degree degree_bucket;

struct listnode *
topological_sort (int n, int *precede)
{
  degree_bucket *ptr_into_bucket[n];

  int vertice_valence[n], row, column, highest_degree = 0;

  /* Preprocessing: 
   * Discovers vertices out-degree from precede, and vertex of highest degree.
   * Also initializes array of pointers for valence table creation/lookup.
   */
  for (row = 0; row != n; ++row) {

    int sum = 0;
    for (column = 0; column != n; ++column) {
      if (*(precede + row * n + column) == TRUE)
	++sum;
    }
    vertice_valence[row] = sum;
    if (highest_degree < sum)
      highest_degree = sum;

    /* Each slot points to a unique vertex, whose position mirrors precede */
    ptr_into_bucket[row] = (struct degree *) malloc (sizeof (struct degree));
    ptr_into_bucket[row]->vertex = row;
    ptr_into_bucket[row]->prev = ptr_into_bucket[row]->next = NULL;
  }

  int i;
  degree_bucket *degree_list[highest_degree];
  for (i = 0; i != highest_degree + 1; ++i)
    degree_list[i] = NULL;

  /* iterate over vertex array, placing into bucket each vertex by degree */
  for (i = 0; i != n; ++i) {
    int degree = vertice_valence[i];

    if (degree_list[degree] == NULL)
      degree_list[degree] = ptr_into_bucket[i];
    else {

      degree_bucket *tmp_iter = degree_list[degree];

      /* find end of list to add vertex to it */
      for (; tmp_iter->next != NULL; tmp_iter = tmp_iter->next);

      tmp_iter->next = ptr_into_bucket[i];
      ptr_into_bucket[i]->prev = tmp_iter;
    }
  }

  struct listnode *sorted_list = NULL;
  while (degree_list[ZERODEGRE] != NULL) {
    degree_bucket *remove_this_vrtx = NULL, *move_this_vrtx = NULL;

    remove_this_vrtx = degree_list[ZERODEGRE];
    int j = remove_this_vrtx->vertex;

    --vertice_valence[j];
    column = remove_this_vrtx->vertex;

    /* be a good citizen and re-link list before removing */
    degree_list[ZERODEGRE] = degree_list[ZERODEGRE]->next;

    for (row = 0; row != n; ++row) {
      if (*(precede + row * n + column) != 1)
	continue;

      int prev_degree = vertice_valence[row]--;
      move_this_vrtx = ptr_into_bucket[row];

      /* be a good citizen and re-link list before moving */
      if (move_this_vrtx->prev == NULL)
	degree_list[prev_degree] = move_this_vrtx->next;
      else {
	move_this_vrtx->prev->next = move_this_vrtx->next;

	if (move_this_vrtx->next != NULL)
	  move_this_vrtx->next->prev = move_this_vrtx->prev;
      }
      move_this_vrtx->prev = move_this_vrtx->next = NULL;

      /* move vertex into lower degree */
      if (degree_list[vertice_valence[row]] == NULL)
	degree_list[vertice_valence[row]] = move_this_vrtx;
      else {
	degree_list[vertice_valence[row]]->prev = move_this_vrtx;
	move_this_vrtx->next = degree_list[vertice_valence[row]];
	degree_list[vertice_valence[row]] = move_this_vrtx;
      }
    }

    struct listnode *tmp =
      (struct listnode *) malloc (sizeof (struct listnode));
    if (tmp == NULL)
      perror ("ERROR: allocating dynamic memory with mallo()");

    tmp->vertexnumber = column;
    tmp->next = sorted_list;
    sorted_list = tmp;

    /*free( remove_this_vrtx ); remove_this_vrtx = NULL; */
  }
  return sorted_list;
}
