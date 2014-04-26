#include <iostream>
#include <cstdio>
#include <vector>
#include <cstdlib>
#include <ctime>
#define  MAXNUM_CNT 100000
#define  NUM_RANGE 10000

using std::cout;
using std::endl;
using std::vector;

//CCNY-CSCI600-Fund_algo Spring 2010 Semester
//Contrast insertionSort and binaryInsertionSort


void binaryInsertionSort( vector<int>&);
void insertionSort(vector<int>&);

int main()
{
   vector<int> vec1,vec2;

   srand( time(NULL) );

   for(unsigned int i=0; i != MAXNUM_CNT; ++i){
       int r = rand() % MAXNUM_CNT;
       vec1.push_back(r );  
   }
   vec2 = vec1;

   cout << "Creating " << MAXNUM_CNT << " random numbers"<<endl;
   cout << "Calling binaryInsertionSort()" << endl;

   time_t curr_time1, curr_time2;
   curr_time1 = time(NULL);

   if( curr_time1 == -1 ) 
       perror("ERROR: getting current time with time()\n");

   binaryInsertionSort(vec1);

   curr_time2 = time(NULL);
   if( curr_time2 == -1 ) 
       perror("ERROR: getting current time with time()\n");

   double elapsed = difftime(curr_time2,curr_time1);
   cout << "Time elapsed in binaryInsertionSort(): " 
        << elapsed << " sec" << endl;


   cout << "Calling insertionSort()" << endl;
   curr_time1 = time(NULL);
   if( curr_time1 == -1 ) 
       perror("ERROR: getting current time with time()\n");

   insertionSort(vec2);

   curr_time2 = time(NULL);
   if( curr_time2 == -1 ) 
       perror("ERROR: getting current time with time()\n");

   elapsed = difftime(curr_time2,curr_time1);
   cout << "Time elapsed in binaryInsertionSort(): " 
        << elapsed << " sec" << endl;
  
/* 
   vector<int>::iterator iter = vec.begin();
   for(; iter!= vec.end(); ++iter)
       cout << *iter << " ";
*/

   cout << endl;
   return 0;
}
