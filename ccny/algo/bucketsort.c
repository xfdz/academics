// Author: Javier Fernandez
/// CCNY-CSC 220 Algorithms, Fall Semester Homework Assignment 2

#include "bucketsort_linked_list.h"

struct listnode *
stringsort (struct listnode *data)
{
  if (data == NULL)
    return data;

  struct listnode *iter_tail = data;
  struct front_back *data_wrapper
    = (struct front_back *) malloc (sizeof (struct front_back));

  while (iter_tail->next != NULL)
    iter_tail = iter_tail->next;

  /* wrapper struct for data list */
  data_wrapper->head = data;
  data_wrapper->tail = iter_tail;
  data_wrapper = ssort (data_wrapper, 0);

  return data_wrapper->head;
}

struct front_back *
ssort (struct front_back *data, int next)
{
  if (data->head == data->tail)
    return data;

  struct bucket *buckets = NULL, *buckets_iter = NULL;

  buckets = create_buckets ();

  /* place each string word in  appropriate bucket */
  while (data->head != NULL) {

    /* get character part of string in position next */
    int character = (int) *(data->head->word + next);

    buckets_iter = buckets;	/* begin from the first bucket */
    while (buckets_iter->ascii_value != character)
      buckets_iter = buckets_iter->next;

    if (buckets_iter->data != NULL) {
      buckets_iter->data->tail->next = data->head;
      buckets_iter->data->tail = buckets_iter->data->tail->next;
    }
    else { 
      /* bucket missing front_back struct, lets create one */

      buckets_iter->data
	= (struct front_back *) malloc (sizeof (struct front_back));
      buckets_iter->data->head = buckets_iter->data->tail = data->head;
    }

    data->head = data->head->next;
    buckets_iter->data->tail->next = NULL;
  }

  ++next;
  buckets_iter = buckets;

  for (; buckets_iter != NULL; buckets_iter = buckets_iter->next) {
    if (buckets_iter->data == NULL)
      continue;
    buckets_iter->data = ssort (buckets_iter->data, next);
  }

  /* 
   * collapse all buckets into a front_back struct 
   */
  data = NULL;
  struct bucket *current_bucket = buckets;
  while (current_bucket != NULL) {
    if (current_bucket->data == NULL) {
      buckets = current_bucket->next;
      free (current_bucket);
      current_bucket = buckets;
      continue;
    }
    if (data == NULL) {
      data = current_bucket->data;
      buckets = current_bucket->next;
      free (current_bucket);
      current_bucket = buckets;
      continue;
    }

    /* link tail to head */
    /* previous front_back tail points to next front_back head */
    data->tail->next = current_bucket->data->head;

    /* next's front_back is new previous tail */
    data->tail = current_bucket->data->tail;
    data->tail->next = NULL;
    buckets = current_bucket->next;
    free (current_bucket->data);
    free (current_bucket);
    current_bucket = buckets;
  }

  return data;
}

struct bucket *
create_buckets ()
{
  struct bucket *list = NULL;
  int i;
  for (i = 256; i != -1; --i) {
    struct bucket *tmp = (struct bucket *) malloc (sizeof (struct bucket));
    if (tmp == NULL)
      perror ("ERROR: allocating dynamic memory with malloc() ");
    tmp->next = list;
    list = tmp;
    list->ascii_value = i;
    list->data = NULL;
  }

  return list;
}
