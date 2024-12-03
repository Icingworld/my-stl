#include <iostream>
#include <vector>

int main()
{
    std::vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);
    vec.push_back(5);
    vec.push_back(6);
    vec.push_back(7);
    vec.push_back(8);
    vec.push_back(9);
    vec.push_back(10);
    vec.push_back(11);

    std::vector<int>::const_iterator it = vec.cbegin();

    while (it != vec.cend()) {
        std::cout << *it << " ";
        ++it;
    }
    std::cout << std::endl;

    return 0;
}