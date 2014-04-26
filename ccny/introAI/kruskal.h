#ifndef GUARD_kruskal_h
#define GUARD_kruskal_h

//CCNY-CSCI1500-Intro_to_AI Spring Semester
//Chapter-2 Programming Assignment 1 : Minimum Spanning Tree

// Class used to represent an edge in a undirected graph with weight
class Edge
{
    public:
        Edge() : vertexA(-1), vertexB(-1), weight(-1){}

        Edge(unsigned int A, unsigned int B, unsigned int w){ 
            vertexA = A; vertexB = B; weight = w;
        }

        void setVertexA(int v){ vertexA = v;}
        void setVertexB(int v){ vertexB = v;}
        void setWeight( int w){ weight = w; }

        int getWeight()const{ return weight; }
        int getVertexA()const { return vertexA;}
        int getVertexB()const { return vertexB;}
        
        ~Edge(){ /*std::cout << "Calling Destructor" << std::endl;*/ }

    private:
        int vertexA, vertexB, weight;

};

// Comparison function used by STL priority queue
// in oder to determine which Edge has higher precedence
class CompareEdge
{
    public:
        // E2 has higher priority if its less than E1
        bool operator()(Edge& E1, Edge& E2){
            if( E2.getWeight() < E1.getWeight()) return true;
            return false;
        }
};

std::vector<int> kruskal_mst( Edge *edge_list[], int num_edges );


#endif
