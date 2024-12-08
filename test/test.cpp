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

    return 0;
}