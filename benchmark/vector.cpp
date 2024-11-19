#include "benchmark.h"
#include "../src/vector.h"

template <typename Func>
void measure(const std::string & label, Func && func)
{
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << label << ": " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() 
              << " ms" << std::endl;
}

int main()
{
    stl::vector<int> stl_vec;
    std::vector<int> std_vec;
    
    int times = 10000;
    // push_back
    std::cout << "times: " << times << std::endl;
    measure("stl push_back", [&]() {
        for (int i = 0; i < times; ++i) {
            stl_vec.push_back(i);
        }
    });
    measure("std push_back", [&]() {
        for (int i = 0; i < times; ++i) {
            std_vec.push_back(i);
        }
    });
    // erase
    measure("stl erase", [&]() {
        for (int i = 0; i < times; ++i) {
            stl_vec.erase(stl_vec.begin());
        }
    });
    measure("std erase", [&]() {
        for (int i = 0; i < times; ++i) {
            std_vec.erase(std_vec.begin());
        }
    });
    // insert
    measure("stl insert front", [&]() {
        for (int i = 0; i < times; ++i) {
            stl_vec.insert(stl_vec.begin(), i);
        }
    });
    measure("std insert front", [&]() {
        for (int i = 0; i < times; ++i) {
            std_vec.insert(std_vec.begin(), i);
        }
    });

    times = 100000;
    std::cout << "times: " << times << std::endl;
    measure("stl push_back", [&]() {
        for (int i = 0; i < times; ++i) {
            stl_vec.push_back(i);
        }
    });
    measure("std push_back", [&]() {
        for (int i = 0; i < times; ++i) {
            std_vec.push_back(i);
        }
    });
    // erase
    measure("stl erase", [&]() {
        for (int i = 0; i < times; ++i) {
            stl_vec.erase(stl_vec.begin());
        }
    });
    measure("std erase", [&]() {
        for (int i = 0; i < times; ++i) {
            std_vec.erase(std_vec.begin());
        }
    });
    measure("stl insert front", [&]() {
        for (int i = 0; i < times; ++i) {
            stl_vec.insert(stl_vec.begin(), i);
        }
    });
    measure("std insert front", [&]() {
        for (int i = 0; i < times; ++i) {
            std_vec.insert(std_vec.begin(), i);
        }
    });

    return 0;
}
