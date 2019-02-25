#include <iostream>
#include "CVector.h"



int main( int argc, char* argv[] )
{
    CVector<int> int_vec;
    int_vec.assign( 3, 10 );

    for (size_t i = 0; i < 3; i++)
    {
        std::cout << int_vec.at(i) << std::endl;
    }
    int_vec.insert( 1, {1, 2, 3, 4} );

    std::cout << "\n\n\n";

    for (size_t i = 0; i < 6; i++)
    {
        std::cout << int_vec.at(i) << std::endl;
    }

    std::cout << "\n\n\n";
    std::cout << int_vec.size() << "\n";
    std::cout << int_vec.capacity() << "\n";

    system("pause");
    return 0;
}
