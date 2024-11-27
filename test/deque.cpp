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

    for (int i = 0; i < 100; ++i) {
        deque.push_front(i);
    }

    std::cout << "deque size: " << deque.size() << std::endl;
    // std::cout << "deque map size: " << deque._map_size << std::endl;

    deque.pop_back();
    deque.pop_front();

    std::cout << "deque elements: ";
    for (auto it = deque.begin(); it != deque.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    deque.clear();

    std::cout << "deque size: " << deque.size() << std::endl;
    // std::cout << "deque map size: " << deque._map_size << std::endl;

    for (int i = 0; i < 50; ++i) {
        deque.push_back(i);
    }

    deque.erase(deque.begin() + 2, deque.begin() + 5);

    std::cout << "deque elements: ";
    for (auto it = deque.begin(); it != deque.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    deque.clear();

    for (int i = 0; i < 20; ++i) {
        deque.push_back(i);
    }

    std::cout << "deque elements: ";
    for (auto it = deque.begin(); it != deque.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    int temp_value = 666;
    deque.insert(deque.begin() + 2, 3, temp_value);

    // deque.insert(deque.begin(), 3, temp_value);
    // deque.insert(deque.end(), 3, temp_value);

    deque.insert(deque.end() - 3, 5, temp_value);

    deque.emplace(deque.begin() + 10, 777);

    std::cout << "deque elements: ";
    for (auto it = deque.begin(); it != deque.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    return 0;
}