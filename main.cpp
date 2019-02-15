#include <iostream>
#include <vector>

#include <memory>
#include <new>



int main( int argc, char* argv[] )
{
    std::allocator<int> alloc;

    int* int_arr = alloc.allocate(5);
    alloc.construct( int_arr, 1 );
    alloc.construct( int_arr + 1, 2 );
    alloc.construct( int_arr + 2, 3 );

    std::cout << int_arr[0] << " " << int_arr[1] << " " << int_arr[2] << std::endl;

    alloc.deallocate( int_arr, 5 );
    std::vector<int> vec;
    system("pause");
    return 0;
}
