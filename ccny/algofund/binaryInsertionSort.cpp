using <vector>

void binaryInsertionSort( std::vector<int> &vec )
{

    int tmp1,tmp2;
    unsigned int left, right, middle;

    for(unsigned int i=1; i != vec.size(); ++i)
    {
        //const int tmp = vec[i];
        tmp1 = vec[i];
        left = 0;
        right = i;

        while( left < right ) {
            middle = (left + right) / 2;

            if( vec[middle] <= tmp1 )
                left = middle + 1;
            else
                right = middle;
        }

        // exchanges will force number to fall in its correct place
        for(unsigned int j=i; j > left; --j){
           tmp2 = vec[j-1];
           vec[j-1] = vec[j];
           vec[j] = tmp2;
        }
   }
}
