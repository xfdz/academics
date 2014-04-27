// Test input number if its power of two

#include <iostream>
#include <cstdlib>
using std::cout;
using std::endl;

int main(int argc, char *argv[])
{
    int input = atoi( argv[ argc - 1 ] );

    if( ( input != 0  && (input & (input-1)) == 0 ) ) 
        cout <<  input <<" is power of two" << endl; 
    else
        cout <<  input << " is not power of two"<<endl;

    return 0;
}
