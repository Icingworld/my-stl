#include <iostream>
#include <deque>
#include "../src/deque.h"

int main()
{
    stl::deque<int> deque;
    std::deque<int> std_deque;
    std::cout << "deque max_size: " << deque._map_size << std::endl;
    std::cout << "deque size: " << deque.size() << std::endl;
}