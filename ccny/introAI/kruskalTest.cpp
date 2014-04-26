#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include "kruskal.h"

//CCNY-CSCI1500-Intro_to_AI Spring Semester
//Chapter-2 Programming Assignment 1 : Minimum Spanning Tree

#define NUM_EDGES 18

using std::cout;
using std::endl;
using std::string;
using std::vector;

int main(){

    // Edge = (vertex,vertex,weight)
    // undirected graph - notice edge repetition
    Edge *edge_list[NUM_EDGES] = {
        new Edge(1,2,3),
        new Edge(1,5,2),
        new Edge(1,6,8),
        new Edge(2,1,3),
        new Edge(2,3,1),
        new Edge(2,5,4),
        new Edge(2,6,7),
        new Edge(3,2,1),
        new Edge(3,4,3),
        new Edge(3,5,5),
        new Edge(4,3,3),
        new Edge(4,5,4),
        new Edge(5,2,4),
        new Edge(5,3,5),
        new Edge(5,4,4),
        new Edge(6,1,8),
        new Edge(6,2,7),
        new Edge(6,5,6),
    };

    vector<int> minSpanTree = kruskal_mst( edge_list, NUM_EDGES );

    // Print MST
    vector<int>::const_iterator iter = minSpanTree.begin();
    while ( iter != minSpanTree.end() ){
        cout << "( " << *iter++ << " , " << *iter++ << " )" << endl;
    }


    return 0;
}
