#include <iostream>
#include <vector>
#include <queue>
#include "kruskal.h"
#include "disjointSet.h"

//CCNY-CSCI1500-Intro_to_AI Spring Semester
//Chapter-2 Programming Assignment 1 : Minimum Spanning Tree

using std::vector;
using std::queue;

vector<int> kruskal_mst( Edge *edge_list[], int num_edges )
{
    std::priority_queue<Edge, vector<Edge>, CompareEdge> pq;

    for(int i=0; i != num_edges; i++)
        pq.push( *edge_list[i] );

    std::cout << "pq.size() "<< pq.size() << std::endl;

    DisjSets disjset(num_edges);
    vector <int>  minSpanTree;

    int numEdgeCount = 0; 
    while( !pq.empty() ){
        const Edge edger =  pq.top();

        // where edge (A, B) belong to their respective set
        int uset = disjset.find( edger.getVertexA() );
        int vset = disjset.find( edger.getVertexB() );

        // neither u or v are in the same set, then union them
        if( uset != vset ){
           ++numEdgeCount;
           disjset.unionSets( uset, vset );

           minSpanTree.push_back( edger.getVertexA() );
           minSpanTree.push_back( edger.getVertexB() );
        }

         pq.pop();
     }

    std::cout << "minSpanTree.size() " << minSpanTree.size() << std::endl;

    return minSpanTree; 
}
