#include <stdio.h>
#include <stdlib.h>

#define MAX         8
#define ADJVERTICES 8
#define MINE        1

#define UNDEF    -1
#define UNVISITED 0
#define VISITED   1 

/* linked list, with vertex coord*/
struct Adjlist{
   int x,y;
   struct Adjlist *next;
};

/* vertex coord, and neighbor vertices*/
struct Vertex {
    int x,y;   
    struct Adjlist *adj;
    struct Adjlist *virt_adj;
};

/* linkelist to hold single value used to map
 * back to  
 */
struct Node{ 
    int value;
    struct Node *next;
};

typedef struct {
    int size, *base, *top;
}stck_t;

typedef struct sp_t { 
    int value;
    struct sp_t *next;
}span_tree;


int is_valid_coord(int x, int y , int maxrow, int maxcol );
void add_adj_vertex(struct Adjlist **v, int vx, int vy );

stck_t *create_stack(int size);
int stack_empty(stck_t *st);
int push(int vertex, stck_t *st);
int pop(stck_t *st);

int main()
{

    int maxcnt = MAX*MAX;

    /* close, but invalid representation of 
     * our board game. Missing 2 Rows, and 2
     * Columns implicitly owned by each user.
     * Those implicit Rows,Columns are used to
     * determine if the game has been won by 
     * either opponent, describing connected
     * component.
     */
    int GameBoard[MAX][MAX] = {
       {0,0,1,0,0,0,0,0},
       {0,1,1,0,0,1,0,0},
       {1,1,0,0,0,1,0,0},
       {1,0,1,1,0,0,0,0},
       {0,1,0,1,1,0,0,0},
       {0,1,0,1,0,0,0,0},
       {0,0,1,0,0,0,0,0},
       {0,0,1,0,0,0,0,0},
    };
 
    struct Vertex vertice_list[maxcnt];

    /* gives index into vertice_list to find vertex*/
    int vertice_lookup[MAX][MAX];
    for(int i=0; i !=MAX; ++i)
        for(int j=0; j !=MAX; ++j)
            vertice_lookup[i][j] = UNDEF; 
    
    int cnt=0;
    for(int row=0; row != MAX; ++row){
        for(int col=0; col != MAX; ++col, ++cnt){

           /* initialization */
           vertice_list[cnt].x   = row;
           vertice_list[cnt].y   = col;
           vertice_list[cnt].adj = 
           vertice_list[cnt].virt_adj = NULL;
     
	   if( GameBoard[row][col] != MINE ) continue;

           /* maps Gameboard to vertice_list */
           vertice_lookup[row][col] = cnt;

           int tmp_row, tmp_col, result =0;
           /* ClockWise - north vertex */ 
           tmp_row = row-1; tmp_col = col;
           result = is_valid_coord(tmp_row,tmp_col,MAX,MAX);
           if( result && GameBoard[tmp_row][tmp_col]== MINE)
               add_adj_vertex(&vertice_list[cnt].adj,tmp_row,tmp_col); 
 
           /* virtual: north-east vertex  - add to both lists*/
           tmp_row = row-1; tmp_col = col+1;
           result = is_valid_coord(tmp_row,tmp_col,MAX,MAX);
           if( result && GameBoard[tmp_row][tmp_col] == MINE)
               add_adj_vertex(&vertice_list[cnt].virt_adj,tmp_row,tmp_col); 
 
           /* east vertex */
           tmp_row = row; tmp_col = col+1;
           result = is_valid_coord(tmp_row,tmp_col,MAX,MAX);
           if( result && GameBoard[tmp_row][tmp_col] == MINE) 
               add_adj_vertex(&vertice_list[cnt].adj,tmp_row,tmp_col); 

           /* sourth-east vertex */
           tmp_row = row+1; tmp_col = col+1;
           result = is_valid_coord(tmp_row,tmp_col,MAX,MAX);
           if( result && GameBoard[tmp_row][tmp_col] == MINE) 
               add_adj_vertex(&vertice_list[cnt].adj,tmp_row,tmp_col); 
 
           /* south vertex */
           tmp_row = row+1; tmp_col = col;
           result = is_valid_coord(tmp_row,tmp_col,MAX,MAX);
           if( result && GameBoard[tmp_row][tmp_col] == MINE) 
               add_adj_vertex(&vertice_list[cnt].adj,tmp_row,tmp_col); 
          
           /* virtual: sourth west vertex - add to both list*/ 
           tmp_row = row+1; tmp_col = col-1;
           result = is_valid_coord(tmp_row,tmp_col,MAX,MAX);
           if( result && GameBoard[tmp_row][tmp_col] == MINE)
               add_adj_vertex(&vertice_list[cnt].virt_adj,tmp_row,tmp_col); 
         
           /* west vertex */ 
           tmp_row = row; tmp_col = col-1;
           result = is_valid_coord(tmp_row,tmp_col,MAX,MAX);
           if( result && GameBoard[tmp_row][tmp_col] == MINE) 
               add_adj_vertex(&vertice_list[cnt].adj,tmp_row,tmp_col); 

           /* north west vertex */
           tmp_row = row-1; tmp_col = col-1;
           result = is_valid_coord(tmp_row,tmp_col,MAX,MAX);
           if( result && GameBoard[tmp_row][tmp_col] == MINE) 
               add_adj_vertex(&vertice_list[cnt].adj,tmp_row,tmp_col); 
    
       }
    }


    struct Node *neighbor_vert[maxcnt];
    for(int i=0; i != maxcnt; ++i)
        neighbor_vert[i] = NULL;

    /*
     * neighbor_vert is shadow of vertice_list, holding integers serving
     * as indices into vertice_list. With the index value you can map back 
     * into vertice_list, and extract the original struct object. 
     */

    for(int i=0; i!=maxcnt; ++i){
    /* copy index value from vertice_list */
       if( vertice_list[i].adj == NULL ) continue;

       struct Adjlist *adjptr = vertice_list[i].adj;
       struct Adjlist *iter   = NULL;
       struct Node *tmp_node  = NULL, *indexlist = NULL;
       for(iter = adjptr; iter!=NULL; iter=iter->next){
       /* build linked list of neighbors */

           /* lookup emits index into vertice_list */
           int index= vertice_lookup[iter->x][iter->y];
           if( index == UNDEF ){
           /* discovered illegal index value  */
               printf("\nERROR: undefined index value\n"); exit(1);
           }
     
           tmp_node = (struct Node*)malloc(sizeof(struct Node));
           if( tmp_node == NULL ){
               printf("\nERROR: malloc() failed\n"); exit(1);
           }
           tmp_node->value = index; 
           tmp_node->next  = indexlist; 
           indexlist  = tmp_node;
       }
       /* store neighbors in matching index position of vertice_list*/
       neighbor_vert[i] = indexlist;
    }


    int xrow,ycol;   
    struct Adjlist *adjptr = NULL, *virt_adjptr = NULL;
    for(cnt=0; cnt != maxcnt; ++cnt){
    /* print vertices that I own */

        if( vertice_list[cnt].adj == NULL ) continue; 

        xrow   = vertice_list[cnt].x;
        ycol   = vertice_list[cnt].y;
 
        printf("\nvertex(%d,%d)" ,xrow,ycol );
        printf("\tlives in index: %d of vertice_lookup[%d][%d]", 
          vertice_lookup[xrow][ycol],
               xrow,ycol);

        printf("\n\t\tadjacent: \n\t\t\t");
        /* adjacent vertices */ 
        adjptr = vertice_list[cnt].adj;
        while( adjptr != NULL ){
            printf("vertex(%d,%d)  ", adjptr->x, adjptr->y); 
            adjptr = adjptr->next;
        }

        /* virtual adjacent vertices */
        virt_adjptr = vertice_list[cnt].virt_adj; 
        printf("\n\t\tvirt_adjacent:\n\t\t\t");
        while( virt_adjptr != NULL ){
            printf("vertex(%d,%d)   ", virt_adjptr->x, virt_adjptr->y);
            virt_adjptr = virt_adjptr->next;
        }
    }

    /*
     * Depth First Search algorithm employed to traverse the graph. 
     * The search also produces forrest of spaning trees.
     */


    span_tree *span_tree_list[maxcnt];
    for(int i=0; i!=maxcnt; ++i)
        span_tree_list[i] = NULL;
    
    int color[maxcnt]; 
    for(int i=0; i !=maxcnt; ++i) 
        color[i] = UNVISITED; 

    printf("\n");
    int result, vindex;
    stck_t *st_p = create_stack(maxcnt);    
    for(int i=0; i != maxcnt; ++i){
    /* visit each vertex in vertice_list */
        if(color[i] == VISITED || neighbor_vert[i] == NULL) continue;

        push(i,st_p);

        span_tree *tmp_sp1 = NULL, *tmp_sp2 = NULL;
        while( (result = stack_empty(st_p)) != 1 ){
        /* visit its neighbor vertices */
            vindex = pop(st_p);
            if(neighbor_vert[vindex]==NULL || color[vindex]==VISITED)
                continue;

            color[vindex] = VISITED; 
            tmp_sp1 = (span_tree*)malloc(sizeof(span_tree));
            if( tmp_sp1 == NULL){
               printf("\nERROR: malloc() failed\n"); exit(1);
            }
            tmp_sp1->value = vindex;
            tmp_sp1->next  = tmp_sp2;
            tmp_sp2 = tmp_sp1;
       
            struct Node *iter_vert = neighbor_vert[vindex];
            for(; iter_vert != NULL; iter_vert = iter_vert->next ){
                int val = iter_vert->value;
                if( color[val] == VISITED ||neighbor_vert[val] ==NULL)
                    continue;
                push(iter_vert->value,st_p);
            }
        }
        span_tree_list[i] = tmp_sp2;

        printf("\n");

    }   

    int sum=0;
    int connected[2] = { 0, 0 };
    for(int i=0; i !=maxcnt; ++i){
        if( span_tree_list[i] == NULL ) continue; 
        printf("\n Spanning Tree: %d \n", i);

        span_tree *iter = span_tree_list[i];
        for(; iter != NULL; iter = iter->next){
            int row = vertice_list[iter->value].x;
            int col = vertice_list[iter->value].y;
            printf(" index: %d - vertice_list[%d] => (%d,%d)\n",
              iter->value, iter->value,row,col);
            
            if( row == 0 )
                connected[0] = 1;
            else if( row == MAX-1)
                connected[1] = 1; 

            sum = connected[0] + connected[1];
            if( sum == 2 )
            printf("\n Player A has won!!\n");
        }

        printf("\n\n");
    }

}

void add_adj_vertex(struct Adjlist **v, int vx, int vy ){

    struct Adjlist *vtmp = (struct Adjlist*) malloc(sizeof(struct Adjlist));

    if( vtmp == NULL ){
        printf("\nERROR: in add_adj_vertex() calling malloc()\n"); exit(1);
    }
    vtmp->next = NULL;
    vtmp->x = vx; 
    vtmp->y = vy;
    vtmp->next = *v;
    *v = vtmp; 
}

/* 
 * Checks to see if the coordinate 
 * are off the matrix dimension
 */
int is_valid_coord(int x, int y , int maxrow, int maxcol ){
    return ( (x >= 0 && y >= 0) && (x < maxrow && y < maxcol) ) ? 1 : 0;
}


/* stack operation helper functions */
stck_t *create_stack(int size){
    stck_t *st = (stck_t*) malloc( sizeof(stck_t) );
    st->base = (int*) malloc(size * sizeof(int));
    if( st == NULL ){
        printf("\nERROR: in create_stack() calling malloc()\n"); exit(1);
    }
    st->size =size;
    st->top  = st->base;
    return st;
}

int stack_empty(stck_t *st){
    return ( st->base == st->top);
}

int push(int vertex, stck_t *st){
    if( st->top > st->base + st->size ) return -1;

    *(st->top) = vertex;
    st->top +=1;
    return 0;
}

int pop(stck_t *st){
    st->top -= 1;
    return (*(st->top));
}



