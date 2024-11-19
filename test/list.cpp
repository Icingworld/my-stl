#include <iostream>
#include <cassert>
#include "../src/list.h"

stl::list<int> my_list;

void print()
{
    std::cout << "List elements: ";
    for (auto it = my_list.begin(); it != my_list.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

void test_list() {
    // 测试构造函数和基本操作
    assert(my_list.size() == 0);
    assert(my_list.empty());

    // 测试 push_back
    my_list.push_back(10);
    my_list.push_back(20);
    my_list.push_back(30);
    assert(my_list.size() == 3);
    assert(my_list.front() == 10);
    assert(my_list.back() == 30);

    // 测试 push_front
    my_list.push_front(5);
    assert(my_list.size() == 4);
    assert(my_list.front() == 5);

    // 测试 pop_back
    my_list.pop_back();
    assert(my_list.size() == 3);
    assert(my_list.back() == 20);

    // 测试 pop_front
    my_list.pop_front();
    assert(my_list.size() == 2);
    assert(my_list.front() == 10);

    // 测试迭代器遍历
    my_list.push_back(40);
    my_list.push_back(50);
    std::cout << "List elements: ";
    print();

    // 测试 insert_after
    auto it = my_list.begin();
    ++it; // 指向第二个元素
    my_list.insert(it, 25); // 在 20 后插入 25
    assert(my_list.size() == 5);

    std::cout << "List elements after insert: ";
    print();

    // 测试 erase
    it = my_list.begin();
    ++it; // 指向第二个元素
    my_list.erase(it); // 删除 20
    assert(my_list.size() == 4);

    std::cout << "List elements after erase: ";
    print();

    // 测试 clear
    while (!my_list.empty()) {
        my_list.pop_front();
    }
    assert(my_list.size() == 0);
    assert(my_list.empty());

    std::cout << "All tests passed!" << std::endl;
}

int main() {
    test_list();
    return 0;
}
