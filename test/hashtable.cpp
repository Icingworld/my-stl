#include <iostream>
#include "../src/functional.h"
#include "../src/hashtable.h"

template <class T>
class keyExtractor;

template <typename T, typename U>
class keyExtractor<stl::pair<const T, U>>
{
public:
    const T & operator()(const stl::pair<const T, U> & p) const
    {
        return p.first;
    }
};

int main()
{
    stl::hashtable<int, int, stl::hash<int>, stl::equal_to<int>, keyExtractor<stl::pair<const int, int>>, stl::allocator<int>> h1(50, stl::hash<int>(), stl::equal_to<int>());
    std::cout << h1.bucket_count() << std::endl;

    // stl::pair<stl::hashtable<int, int, stl::hash<int>, stl::equal_to<int>, keyExtractor<stl::pair<const int, int>>, stl::allocator<int>>::iterator, bool> it = h1.insert_unique(stl::pair<const int, int>(1, 6));
    auto it = h1.insert_unique(stl::pair<const int, int>(1, 6));
    auto it1 = h1.insert_equal(stl::pair<const int, int>(1, 9));
    auto it2 = h1.insert_unique(stl::pair<const int, int>(2, 7));
    std::cout << it2.first->second << " " << it2.second << std::endl;
    auto it3 = h1.find(1);
    std::cout << it3->second << std::endl;
    auto count = h1.count(1);
    std::cout << count << std::endl;

    for (int i = 0; i < 20; ++i) {
        h1.insert_equal(stl::pair<const int, int>(1, i + 1));
    }

    count = h1.count(1);
    std::cout << count << std::endl;

    auto pair = h1.equal_range(1);
    for (auto it = pair.first; it != pair.second; ++it) {
        std::cout << it->second << " ";
    }
    std::cout << std::endl;

    h1.clear();
}