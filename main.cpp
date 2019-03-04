#include <iostream>
#include <vector>
#include <string>
#include "CVector.h"

int main( int argc, char* argv[] )
{
	CVector<int> c = {1, 2, 3};
    std::cout << "The vector holds: ";
    for( size_t it = 0; it < c.size(); it++ )
    {
        std::cout << c.at(it) << ' ';
    }
    std::cout << "\nSIZE: " << c.size() << "\nCAPACITY: " << c.capacity() << std::endl << std::endl;

    c.resize( 5, 8 );
    std::cout << "After resize up to 5: ";
    for( size_t it = 0; it < c.size(); it++ )
    {
        std::cout << c.at(it) << ' ';
    }
    std::cout << "\nSIZE: " << c.size() << "\nCAPACITY: " << c.capacity() << std::endl << std::endl;

    c.resize(2);
    std::cout << "After resize down to 2: ";
    for( size_t it = 0; it < c.size(); it++ )
    {
        std::cout << c.at(it) << ' ';
    }
    std::cout << "\nSIZE: " << c.size() << "\nCAPACITY: " << c.capacity() << std::endl << std::endl;
    std::cout << '\n';

    system("pause");
    return 0;
}
