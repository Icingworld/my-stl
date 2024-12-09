#include <iostream>
#include <vector>

class Test
{
public:


    int test(const int a)
    {
        return a;
    }

};

int main()
{
    Test test;
    std::cout << test.test(1) << std::endl;

    std::vector<int> vec = {1, 2, 3, 4};
    std::vector<int>::const_iterator it = vec.cbegin();
    ++it;
    std::cout << *it << std::endl;

    return 0;
}