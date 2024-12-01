#include <iostream>
#include <string>
#include "../src/unordered_map.h"

int main()
{
    stl::unordered_map<int, std::string> map;
    for (int i = 0; i < 10; ++i)
    {
        map[i] = std::string("test") + std::to_string(i);
    }
    for (int i = 0; i < 10; ++i)
    {
        std::cout << map[i] << std::endl;
    }
}