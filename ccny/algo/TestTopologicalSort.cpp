#include <stdio.h>
#include <stdlib.h>
#define ROW 6 
#define COLUMN 6 

int *get_adjacency_matrix();
struct listnode *topological_sort(int n, int *precede );

struct listnode{ struct listnode *next; int vertexnumber; };


int main(){

    int *matrix = get_adjacency_matrix();
    struct listnode *list = topological_sort( ROW, matrix);

    for(; list != NULL; list = list->next) 
        printf("\n Vertex: %d", list->vertexnumber ); 
    printf("\n");

    return 0;
}

int *get_adjacency_matrix(){

   /* i equal rows, and j equals columns. If i goes before
    * j then equals 1, else equals 0 
    */
    
    /* vertex 1 outdegree 2 
     * vertex 2 outdegree 0 
     * vertex 3 outdegree 1 
    */
     
    int adest[][COLUMN] = {
        { 0,1,1,0,0,0 },
        { 0,0,0,1,0,0 },
        { 0,1,0,1,1,0 },
        { 0,0,0,0,1,1 },
        { 0,0,0,0,0,1 },
        { 0,0,0,0,0,0 }
    };

    // array is ROWxCOLUMN - created flat
    int *vertex_matrix = (int *) malloc( ROW * COLUMN * sizeof(int *));

    // copy automatic memory matrix  to dynamic matrix
    int i,j;
    for( i = 0; i != ROW; ++i){
        for( j = 0; j != COLUMN; ++j)
            *(vertex_matrix + i * ROW + j ) = adest[i][j];
    }

    return vertex_matrix;
}
