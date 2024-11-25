#include <iostream>
#include <deque>
#include "../src/deque.h"

int main()
{
    stl::deque<int> deque;
    // std::deque<int> std_deque;
    
    for (int i = 0; i < 100; ++i) {
        deque.push_back(i);
    }

    std::cout << "deque size: " << deque.size() << std::endl;
    std::cout << "deque map size: " << deque._map_size << std::endl;

    for (int i = 0; i < 100; ++i) {
        deque.push_front(i);
    }

    std::cout << "deque size: " << deque.size() << std::endl;
    std::cout << "deque map size: " << deque._map_size << std::endl;

    std::cout << "deque elements: ";
    for (auto it = deque.begin(); it != deque.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    return 0;
}