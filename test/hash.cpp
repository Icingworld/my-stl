#include <iostream>
#include "../src/hash.h"


int main()
{
    stl::hash<int> h1;
    std::cout << h1(1) << std::endl;
    std::cout << h1(2) << std::endl;

    stl::hash<bool> h2;
    std::cout << h2(true) << std::endl;
    std::cout << h2(false) << std::endl;

    return 0;
}