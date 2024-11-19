#include <vector>
#include <iostream>

#include "../src/vector.h"

int main() {
    stl::vector<int> vec;
    std::cout << "vector address: " << &vec << std::endl;
    // std::cout << "vector first: " << vec.first << std::endl;
    std::cout << "vector capacity: " << vec.capacity() << std::endl;
    vec.push_back(1);
    // std::cout << "vector first: " << vec.first << std::endl;
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);
    vec.push_back(5);
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    std::cout << "vector address: " << &vec << std::endl;
    // std::cout << "vector first: " << vec.first << std::endl;
    std::cout << "vector size: " << vec.size() << std::endl;
    std::cout << "vector capacity: " << vec.capacity() << std::endl;
    std::cout << "vector[2]: " << vec[2] << std::endl;
    vec[2] = 10;
    std::cout << "vector[2]: " << vec[2] << std::endl;
    vec.pop_back();
    std::cout << "vector size: " << vec.size() << std::endl;
    std::cout << "vector capacity: " << vec.capacity() << std::endl;
    vec.insert(vec.begin() + 3, 999);
    std::cout << "vector[3]: " << vec[3] << std::endl;
    std::cout << "vector size: " << vec.size() << std::endl;
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    vec.erase(vec.begin() + 2);
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    vec.erase(vec.begin() + 1, vec.begin() + 2);
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    return 0;
}