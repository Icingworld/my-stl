#include <iostream>
#include "../src/alloc.h"
#include <string>

void test_simple_alloc() {
    // 定义分配器
    stl::simple_alloc<int> alloc;

    // 分配内存
    int* int_ptr = alloc.allocate();
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

    // 测试复杂类型
    stl::simple_alloc<std::string> string_alloc;
    // using string_Alloc = typename stl::simple_alloc<int>::rebind<std::string>::other;
    // string_Alloc string_alloc;

    // 分配内存
    std::string* str_ptr = string_alloc.allocate();
    std::cout << "Memory allocated for std::string." << std::endl;

    // 构造对象
    string_alloc.construct(str_ptr, "Hello, Allocator!");
    std::cout << "Constructed std::string with value: " << *str_ptr << std::endl;

    // 销毁对象
    string_alloc.destroy(str_ptr);
    std::cout << "Destroyed std::string object." << std::endl;
    std::cout << "Constructed std::string with value: " << *str_ptr << std::endl;

    // 释放内存
    string_alloc.deallocate(str_ptr);
    std::cout << "Deallocated memory for std::string." << std::endl;
}

int main() {
    test_simple_alloc();
    return 0;
}