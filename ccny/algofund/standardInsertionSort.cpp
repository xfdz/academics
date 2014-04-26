#include <vector>

void insertionSort( std::vector<int> &vec ){

    for( unsigned int i=1; i != vec.size(); ++i){

        for(unsigned int j=i; j !=0 && vec[j-1] > vec[j]; --j){
                int tmp = vec[j];
                vec[j] = vec[j-1];
                vec[j-1] = tmp;
        }
     }
}
