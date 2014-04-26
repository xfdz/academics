#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "qsort_linked_list.h"

using std::cout;
using std::endl;
#define MAX 500 

struct listnode *create_list();
void print_linked_list( struct listnode *linked_list);
void integrated_test(struct listnode *linked_list);
void test_divide_list(struct listnode *linked_list);
void test_copy_list( struct listnode *linkedlist );
void test_concatinate_list();

int main()
{
    srand(time(0));
    struct listnode *list = NULL;
    list = create_list();
    print_linked_list( list );

    // TEST functions
    integrated_test( list );
    //test_divide_list( list );
    //test_copy_list( list );
    //test_concatinate_list();

    cout << "Finished tests execution" << endl;
    return 0;
}

struct listnode *create_list()
{
    struct listnode *linked_list = NULL;

    //count from 1 - this is not an array
    for(int i=0; i < MAX; ++i ){
        struct listnode *list
         = (struct listnode *) malloc( sizeof( struct listnode *));
      
        if( list == NULL )
            perror("ERROR: allocating dynamic memory with malloc()");
      
        list->value = rand() % MAX;
      
        list->next = linked_list;
      
        linked_list = list;
     }

    return linked_list;

}

void print_linked_list( struct listnode *linked_list)
{
    if( linked_list != NULL ){
        for(struct listnode *tmp = linked_list; 
            tmp != NULL; tmp = tmp->next){
            cout << "Value: " << tmp->value << " Address pointed to " << 
            tmp << " Address of pointer " <<  &linked_list << endl;
        }
    }

}

void integrated_test(struct listnode *linked_list) {

    cout << "\nExecuting: integrated_test()" << endl;

    struct listnode *result = quicksort(linked_list, MAX);

    cout << "\n List after quicksort()" << endl;

    print_linked_list( result );

    cout <<"\n"<< endl;
}

void test_divide_list(struct listnode *linked_list) {

    cout << "\nExecuting: test_divide_list()" << endl;

    struct listnode *too_small, *too_big, *pivot; 
 
    //pivot = divide_list(linked_list, &too_small, &too_big);

    ///////POST divide_list() 
    cout << "\nPost divide_list()" << endl;

    cout << "pivot value: "; 
    print_linked_list( pivot );

    cout << "\ntoo_small values: "<<endl;
    print_linked_list(too_small);

    cout << "\ntoo_big values: "<<endl;
    print_linked_list(too_big);

}

void test_copy_list( struct listnode *linked_list )
{
/*
    cout << "\nExecuting: test_copy_list() " << endl;

    struct listnode *too_small, *too_big, *current;
    too_small = too_big = current = NULL;

    int pivot = 5;    

    current = linked_list;

    while( current != NULL ){
        if( current->value < pivot )
            copy_list( &too_small, current); 
        else
            copy_list( &too_big,current);

        struct listnode *erase = current;
        current = current->next;
        free( erase ); 
        erase = NULL;
    }

    cout <<"\n Printing too_small " << endl;
    print_linked_list( too_small );

    cout <<"\n Printing too_big " << endl;
    print_linked_list( too_big );
*/
}


void test_concatinate_list()
{
/*
    cout <<"\n Running: test_concatinate_list()" <<endl;
 
    // first list
    cout <<"\n First list " <<endl;
    struct listnode *first = create_list();
    print_linked_list( first );

    // second list
    cout <<"\n Second list " <<endl;
    struct listnode *second = create_list();
    print_linked_list( second);

    // Pivot
    struct listnode *pivot 
     = (struct listnode *) malloc( sizeof(struct listnode ));
    pivot->next = NULL;
    pivot->value = rand() % MAX;
    cout << "\n Pivot: " << endl;
    print_linked_list( pivot );

    struct listnode *data = NULL;
    concatinate_list(&data, first,pivot, second );

    cout << "\n List after concatinate_list() "<<endl;
    print_linked_list( data );
    cout << endl;
*/
}
