#include <iostream>
#include <cassert>
#include <list>
#include "../src/list.h"

void print(stl::list<int> & my_list)
{
    std::cout << "List elements: ";
    for (auto it = my_list.begin(); it != my_list.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

class test
{
private:
    int a;
public:
    test() : a(0) {}
    test(int a) : a(a) {}

    ~test() {
        std::cout << "destructor called" << std::endl;
    }
};

bool cmp(const int & a, const int & b) {
    return a == b;
}

int main() {
    stl::list<int> my_list;
    stl::list<int> my_list2;
    my_list2.push_back(777);
    
    std::cout << "my_list size: " << my_list.size() << std::endl;
    std::cout << "max size: " << my_list.max_size() << std::endl;
    my_list.push_back(1);
    my_list.push_back(2);
    my_list.push_back(2);
    my_list.push_back(3);
    my_list.push_back(4);
    my_list.push_back(5);
    my_list.push_back(6);
    std::cout << "my_list size: " << my_list.size() << std::endl;
    // 测试常量迭代器
    stl::list<int>::const_iterator it = my_list.begin();
    std::cout << "my_list first: " << *it << std::endl;
    // *it = 12;            // 编译错误，不能修改的左值

    my_list.resize(9, 666);
    print(my_list);
    my_list.reverse();
    print(my_list);
    my_list.unique();
    // my_list.unique(cmp);
    print(my_list);
    // swap
    my_list.swap(my_list2);
    print(my_list);
    print(my_list2);
    // sort
    my_list2.sort();
    print(my_list2);
    my_list.clear();
    std::cout << "my_list size: " << my_list.size() << std::endl;
    print(my_list);
    std::cout << "my_list is empty: " << my_list.empty() << std::endl;
    
    return 0;
}
