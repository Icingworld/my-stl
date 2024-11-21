#include <iostream>
#include <cassert>
#include "../src/list.h"

void print(stl::list<int> & my_list)
{
    std::cout << "List elements: ";
    for (auto it = my_list.begin(); it != my_list.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

int main() { 
    stl::list<int> my_list;
    my_list.push_back(1);
    my_list.push_back(2);
    my_list.push_back(3);
    my_list.push_back(4);
    my_list.push_back(5);
    my_list.push_back(6);

    print(my_list);
    
    return 0;
}
