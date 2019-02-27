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
        std::cout << "digit_a: " << digit_a << ";;; digit_b: " << digit_b << " ";
    }

    int digit_a;
    int digit_b;
};

int main( int argc, char* argv[] )
{
    CVector<Foo> foo_vec{Foo(1, 1), Foo(2, 2), Foo(3, 3), Foo(4, 4), Foo(5, 5), Foo(6, 6), Foo(7, 7), Foo(8, 8), Foo(9, 9)};
    for (size_t i = 0; i < 9; i++)
    {
        foo_vec.at(i).PrintDigits();
    }
    std::cout << "\n\n\n";

    Foo ref = Foo(0, 0);
    try
    {
        ref = foo_vec.erase(5, 7);
    }
    catch (...)
    {
        std::cout << "EXCEPTION RANGE" << std::endl;
        system("pause");
        return 1;
    }

    try
    {
        for (size_t i = 0; i < 7; i++)
        {
            foo_vec.at(i).PrintDigits();
        }
    }
    catch(...)
    {
        std::cout << "EXCEPTION" << std::endl;
    }

    std::cout << "\n\n\n";
    std::cout << foo_vec.size() << "\n";
    std::cout << foo_vec.capacity() << "\n";

    std::cout << "Returned ref: ";
    ref.PrintDigits();

    system("pause");
    return 0;
}
