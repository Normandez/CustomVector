#include <iostream>
#include <vector>
#include <string>
#include "CVector.h"

void printVector( const CVector<int>& vec )
{
    for( size_t it = 0; it < vec.size(); it++ )
    {
        std::cout << vec.at(it) << " ";
    }
    std::cout << "\nSIZE: " << vec.size() << "\nCAPACITY: " << vec.capacity() << std::endl << std::endl;
}

int main( int argc, char* argv[] )
{
	CVector<int> v1{1, 2, 3};
    CVector<int> v2{1, 2, 3};
 
    std::cout << ( v1 <= v2 ) << std::endl;

    system("pause");
    return 0;
}
