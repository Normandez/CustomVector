#include <iostream>
#include <vector>
#include <string>
#include "CVector.h"

int main( int argc, char* argv[] )
{
    CVector<std::string> numbers;

    numbers.push_back("abc");
    std::string s = "def";
    std::string z;
    numbers.push_back( std::move(s) );

    for( size_t it = 0; it < 2; it++ )
    {
        std::cout << numbers.at(it) << " ";
    }
    std::cout << std::endl << std::endl;

    std::cout << "s = " << s << std::endl;
    
    std::cout << "SIZE: " << numbers.size() << std::endl;
    std::cout << "CAPACITY: " << numbers.capacity() << std::endl;

    system("pause");
    return 0;
}
