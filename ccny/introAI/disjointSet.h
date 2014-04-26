#ifndef GUARD_disjointSet
#define GUARD_disjointSet

//CCNY-CSCI1500-Intro_to_AI Spring Semester
//Chapter-2 Programming Assignment 1 : Minimum Spanning Tree

#include<vector>

class DisjSets
{
    public:
        explicit DisjSets( int numElements ): s( numElements){ 
            for( unsigned int i=0; i < s.size(); i++) s[i] = -1;
        }

        const std::vector<int> getSetHeirarchy(){
           return s;
        }

        int find( int x );
        void unionSets( int root1, int root2);

    private:
        std::vector<int> s; 
};

void unionSets( int root1, int root2);
//void DisjSets::unionSets( int root1, int root2);

int find( int x );
//int DisjSets::find( int x );

#endif
