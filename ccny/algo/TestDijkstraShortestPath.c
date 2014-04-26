#include <stdio.h>
#include <stdlib.h>

#define INFNTY 9999
#define TRUE   1
#define FALSE  0
#define ROW    6 
#define COLUMN 6 

struct listnode{ struct listnode *next; int vertexnumber; };

struct listnode shortest_path( int v, int s, int t, int *dist);

int get_next_vertex( int vlength, int accepted_vertice[], int distance[] );

void get_adjacent_vertice( int next_vrtex, int vlength, int accepted_vertice[], int *dist, int distance[], int predecessor[] );

/* 
 * Local function definition 
 */
int *get_adjmatrix();
void test_next_vertex();
void test_shortest_path();
void test_get_adjacent_vertice();

int main(){
      
    //test_get_adjacent_vertice();
    //test_next_vertex();
    test_shortest_path();

    return 0;
}

void test_shortest_path(){

    int *adj_matrix = get_adjmatrix(); 

    struct listnode short_path = shortest_path( ROW,4,5,adj_matrix );

    struct listnode *t = &short_path;
    for(; t != NULL; t = t->next )
        printf("\t %d", t->vertexnumber );
    printf(" \n" );

}

void test_next_vertex(){
    int i, next=2; 
    int distance[ROW], accepted[ROW];
    for(i=0; i < ROW; ++i){
         distance[i]=INFNTY; accepted[i] = FALSE; 
    }
    accepted[next] = TRUE;
    distance[next] = 0;
    distance[0] = 4;
    distance[3] = 22;

    printf("next was: %d",next);

    next = get_next_vertex(ROW, accepted, distance);

    printf("\nnow next is: %d\n",next);

}

/*
 * This function calls function get_adjacent vertice(), and it does
 * what is says, finding vertices that are adjacent to the start vertex.  
 * I chose the start vertex arbitrarely as 2. According to the adjacency
 * matrix defined in dist its adjcent vertices are 0 and 3  
 * weight = u + distance_between(u,v). But if you change variable s to a 
 * number within the range of 0 to COLUMN then you should get a corresponding
 * result.
 *
 */
void test_get_adjacent_vertice(){

    int i, start =2; 
    int distance[ROW], accepted[ROW], predecessor[ROW];
    for(i=0; i < ROW; ++i){
         distance[i]=INFNTY; accepted[i] = FALSE; 
    }
    int next= start;
    distance[next]=0;
    accepted[next]=TRUE; 
    int *dist = get_adjmatrix();
   
    get_adjacent_vertice(next, ROW, accepted, dist, distance, predecessor);

    int k;
    /* print content of distance for visual inspection */
    printf("distance[]:\t");
    for(k=0; k<ROW; ++k) 
        printf("%d\t",distance[k]);
    printf("\n");

    /* print content of accepted for visual inspection */
    printf("accepted[]:\t");
    for(k=0; k<ROW; ++k) 
        printf("%d\t",accepted[k]);
    printf("\n");

    printf("predecessor[]:\t");
    for(k=0; k<ROW; ++k)
        printf("%d\t",predecessor[k]);
    printf("\n");

}


// create adjacency matrix
int *get_adjmatrix(){ 

    // no easy way to generate a digraph dynamically, best way
    // is to copy digraph in to an array and copy it over to dynamic
    // array
    int adest[][COLUMN] = {
        {0, INFNTY, INFNTY, 1, 8, 2           },
        {INFNTY, 0, 2, INFNTY, INFNTY , INFNTY},
        {4, INFNTY, 0, 22, INFNTY, INFNTY     },
        {INFNTY, 2, INFNTY, 0, INFNTY, INFNTY },
        {INFNTY, INFNTY, INFNTY, 9, 0, INFNTY },
        {INFNTY, INFNTY, INFNTY, 9, INFNTY, 0 },
    };
    
    int *vertex_matrix = (int *) malloc( ROW * COLUMN * sizeof(int *));

    // copy automatic memory matrix  to dynamic matrix
    int i,j;
    for( i = 0; i != ROW; ++i){
        for( j = 0; j != COLUMN; ++j)
            *(vertex_matrix + i * ROW+ j ) = adest[i][j];
    }

    return vertex_matrix;
}
