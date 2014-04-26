/* Author: Javier Fernandez
 * CCNY-CSC 220 Algorithms, Fall Semester Homework Assignment 3 
 */
#include <stdlib.h>
#include <stdio.h>
#define INFNTY 9999
#define TRUE  1
#define FALSE 0
#define NOVERTEX -1

struct listnode
{
  struct listnode *next;
  int vertexnumber;
};
/* 
 * - v: the number of vertices of the graph - s: the start vertext
 * - t: the target vertex- dist: the matrix of edgelengths
 */
struct listnode shortest_path (int v, int s, int t, int *dist);

/*Discovers next vertex with lowest length and in unallowed set*/
int get_next_vertex (int vlength, int accepted_vertice[], int distance[]);

/*
 * Discovers vertices adjacent to next in unallowed set. When adjacent vertex is * discovered its length is updated, and as a by product next is recoreded 
 * in predecessor to generate the trace back of shortest path from s to t
 */
void get_adjacent_vertice (int next_vrtex, int vlength, int accepted_vertice[],
			   int *dist, int distance[], int predecessor[]);

struct listnode
shortest_path (int v, int s, int t, int *dist)
{

  int distance[v], accepted_vertice[v], predecessor[v], i;

  for (i = 0; i != v; ++i) {
    distance[i] = INFNTY;
    predecessor[i] = NOVERTEX;
    accepted_vertice[i] = FALSE;
  }
  distance[s] = 0;
  int allowed_size;

  /* discover start vertex, and its adjacent vertices */
  for (allowed_size = 0; allowed_size != v; ++allowed_size) {

    int next = get_next_vertex (v, accepted_vertice, distance);
    if (next == INFNTY)
      break;
    accepted_vertice[next] = TRUE;
    get_adjacent_vertice (next, v, accepted_vertice, dist, distance,
			  predecessor);
  }

  int is_false = 0;
  int vertex_on_path = t;
  struct listnode short_path = { NULL, s };	/* got start vertex */

  /* get path to destination vertex */
  while (predecessor[vertex_on_path] != s) {

    struct listnode *node
      = (struct listnode *) malloc (sizeof (struct listnode));

    if (shortest_path == NULL)
      perror ("ERROR: allocating dynamic memory with malloc() ");

    if (is_false++ == 0)
      node->vertexnumber = vertex_on_path;
    else {
      node->vertexnumber = predecessor[vertex_on_path];
      vertex_on_path = predecessor[vertex_on_path];
    }
    node->next = short_path.next;
    short_path.next = node;
  }
  return short_path;
}

int
get_next_vertex (int vlength, int accepted_vertice[], int distance[])
{

  int min_vertex, min_edge_length, i;
  min_vertex = min_edge_length = INFNTY;
  for (i = 0; i != vlength; ++i) {
    if (accepted_vertice[i] == TRUE)
      continue;

    if (distance[i] < min_edge_length) {
      min_edge_length = distance[i];
      min_vertex = i;
    }
  }
  return min_vertex;
}

void
get_adjacent_vertice (int next, int vlength, int accepted_vertice[], int *dist,
		      int distance[], int predecessor[])
{
  int v;
  for (v = 0; v != vlength; ++v) {
    if (accepted_vertice[v] == TRUE)
      continue;

    int sum = distance[next] + *(dist + next * vlength + v);

    if (sum < distance[v]) {
      distance[v] = sum;
      predecessor[v] = next;
    }
  }
}
