#include <iostream>
#include <vector>
#include "CVector.h"

struct Foo
{
    Foo()
    {
        digit_a = 0;
        digit_b = 0;
    }
    Foo( int val_a, int val_b )
    {
        digit_a = val_a;
        digit_b = val_b;
    }
    ~Foo()
    {
        std::cout << "Destruct" << std::endl;
    }

    void PrintDigits()
    {
        std::cout << "digit_a: " << digit_a << std::endl;
        std::cout << "digit_b: " << digit_b << std::endl;
        std::cout << std::endl;
    }

    int digit_a;
    int digit_b;
};

int main( int argc, char* argv[] )
{
    CVector<Foo> foo_vec;
    foo_vec.assign( 3, Foo(10, 10) );

    for (size_t i = 0; i < 3; i++)
    {
        foo_vec.at(i).PrintDigits();
    }
    //foo_vec.emplace( 1, 25, 25 );

    std::cout << "\n\n\n";

    /*try
    {
        for (size_t i = 0; i < 4; i++)
        {
            foo_vec.at(i).PrintDigits();
        }
    }
    catch (...)
    {
        std::cout << "EXCEPTION\n\n";
    }*/

    std::cout << "\n\n\n";
    std::cout << foo_vec.size() << "\n";
    std::cout << foo_vec.capacity() << "\n";

    //system("pause");
    return 0;
}
