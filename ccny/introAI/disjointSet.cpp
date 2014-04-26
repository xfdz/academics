#include "disjointSet.h"

//CCNY-CSCI1500-Intro_to_AI Spring Semester
//Chapter-2 Programming Assignment 1 : Minimum Spanning Tree

// union two disjoint sets
void DisjSets::unionSets( int root1, int root2){
    if( s[root2] < s[root1] )
        s[root1] = root2; 
    else
    {
        // update height if the same
        if( s[root1] == s[root2] )
            s[root1]--;
        s[root2] = root1;
    }
}


// Peform a find with path compression.
int DisjSets::find( int x ){
    if( s[x] < 0 )
        return x;
    else
        return s[x] = find( s[x] );
}

