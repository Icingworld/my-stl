#include "../src/utility.h"
#include <assert.h>

int main()
{
    // 测试pair
    stl::pair<int, double> pair = stl::make_pair<int, double>(1, 2.0);
    assert(pair.first == 1 && pair.second == 2.0);
    return 0;
}