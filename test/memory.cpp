#include <iostream>
#include "../src/memory.h"
#include <string>
#include <vector>

void test_simple_alloc() {
    // 定义分配器
    stl::allocator<int> alloc;

    // 分配内存
    int* int_ptr = alloc.allocate(1);
    std::cout << "Memory allocated for int." << std::endl;

    // 构造对象
    alloc.construct(int_ptr, 42);
    std::cout << "Constructed int with value: " << *int_ptr << std::endl;

    // 销毁对象
    alloc.destroy(int_ptr);
    std::cout << "Destroyed int object." << std::endl;

    // 释放内存
    alloc.deallocate(int_ptr);
    std::cout << "Deallocated memory for int." << std::endl;

    // 测试rebind功能
    using string_Alloc = typename stl::allocator<int>::template rebind<std::string>::other;
    string_Alloc string_alloc;

    // 分配内存
    std::string* str_ptr = string_alloc.allocate(1);
    std::cout << "Memory allocated for std::string." << std::endl;

    // 构造对象
    string_alloc.construct(str_ptr, "Hello, Allocator!");
    std::cout << "Constructed std::string with value: " << *str_ptr << std::endl;

    // 销毁对象
    string_alloc.destroy(str_ptr);
    std::cout << "Destroyed std::string object." << std::endl;

    // 释放内存
    string_alloc.deallocate(str_ptr);
    std::cout << "Deallocated memory for std::string." << std::endl;

    // 测试STL模板vector
    std::vector<int, stl::allocator<int>> int_vector;
    std::cout << "Construct int_vector with simple_alloc." << std::endl;
    int_vector.push_back(1);
    int_vector.push_back(2);
    int_vector.push_back(3);
    std::cout << "Size of int_vector: " << int_vector.size() << std::endl;

    std::cout << "Print int_vector: ";
    for (auto it = int_vector.begin(); it != int_vector.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    int_vector.clear();
    std::cout << "Size of int_vector after clear: " << int_vector.size() << std::endl;
}

int main() {
    test_simple_alloc();
    return 0;
}