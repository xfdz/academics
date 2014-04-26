/// Author: Javier Fernandez 
/// CCNY-CSC 220 Algorithms, Fall Semester- Homework Assignment 1
#include "qsort_linked_list.h"
struct listnode *
quicksort (struct listnode *data, int len)
{

  if (len < 2 || data == NULL)
    return data;

  struct listnode *too_small, *too_big, *pivot;

  pivot = divide_list (&data, &too_small, &too_big);

  int too_small_cnt = count_list (too_small);
  int too_big_cnt = count_list (too_big);

  concatinate_list (&data, quicksort (too_small, too_small_cnt), pivot,
		    quicksort (too_big, too_big_cnt));
  return data;
}

int
count_list (struct listnode *list)
{
  struct listnode *ptr = list;
  int len = 0;
  for (; ptr != NULL; ++len)
    ptr = ptr->next;
  return len;
}

struct listnode *
get_pivot (struct listnode **data, int len)
{
  static int is_random_set = 0;
  if (len < 2 || *data == NULL)
    return NULL;
  else if (is_random_set == 0) {
    is_random_set = 1;
    srand (time (NULL));
  }
  struct listnode *pivot = NULL, *current = *data;
  int rndm = rand () % len + 1;
  if (rndm == 1) {
    pivot = current;		// first element pointed to by pivot 
    *data = current->next;	// makes sure head of list points to next node
    pivot->next = NULL;		// move pivot away from list
  }
  else {
    int i = 1;
    for (; i != rndm - 1; ++i)
      current = current->next;
    pivot = current->next;	// pivot was found here 
    current->next = pivot->next;	// move list away from pivot
    pivot->next = NULL;		// move pivot away from list
  }
  return pivot;
}

struct listnode *
divide_list (struct listnode **data, struct listnode **too_small,
	     struct listnode **too_big)
{
  struct listnode *too_small_tail, *too_big_tail, *pivot;
  *too_small = *too_big = too_small_tail = too_big_tail = NULL;

  pivot = get_pivot (data, count_list (*data));

  // iterate over list to divide
  while (*data != NULL) {
    if ((*data)->value <= pivot->value) {
      node_classification (too_small, &too_small_tail, *data);
      *data = (*data)->next;
      too_small_tail->next = NULL;
    }
    else {
      node_classification (too_big, &too_big_tail, *data);
      *data = (*data)->next;
      too_big_tail->next = NULL;
    }
  }
  return pivot;
}

void
node_classification (struct listnode **newlist, struct listnode **newlist_tail,
		     struct listnode *data)
{
  // first time around
  if ((*newlist) == NULL) {
    *newlist = *newlist_tail = data;
  }
  else {
    (*newlist_tail)->next = data;
    (*newlist_tail) = (*newlist_tail)->next;
  }
}

void
concatinate_list (struct listnode **data, struct listnode *too_small,
		  struct listnode *pivot, struct listnode *too_big)
{

  struct listnode *too_small_tail = too_small;

  // interate through list to find its tail 
  if (too_small_tail != NULL) {

    while (too_small_tail->next != NULL)
      too_small_tail = too_small_tail->next;

    too_small_tail->next = pivot;
  }

  if (too_big != NULL)
    pivot->next = too_big;

  *data = (too_small != NULL) ? too_small : pivot;
}
