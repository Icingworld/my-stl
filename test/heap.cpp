#include <iostream>
#include <vector>
// #include <algorithm>
#include "../src/vector.h"
#include "../src/heap.h"


int main()
{
    // stl::vector<int> vec;
    // vec.push_back(40);
    // vec.push_back(30);
    // vec.push_back(15);
    // vec.push_back(10);
    // vec.push_back(20);

    // stl::sort_heap(vec.begin(), vec.end());

    // for (auto it = vec.begin(); it != vec.end(); ++it) {
    //     std::cout << *it << " ";
    // }
    // std::cout << std::endl;

    std::cout << "vec2: " << std::endl;  

    std::vector<int> vec2 = {9, 5, 7, 1, 3, 4, 1};
    stl::sort_heap(vec2.begin(), vec2.end());

    stl::make_heap(vec2.begin(), vec2.end());

    for (auto it = vec2.begin(); it != vec2.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

}