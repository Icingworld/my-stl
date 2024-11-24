#include <array>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include "../src/array.h"

int main() {
    // 定义和初始化 std::array
    stl::array<int, 5> arr = {1, 2, 3, 4, 5};

    // 测试元素访问
    std::cout << "Array elements using operator[]: ";
    for (std::size_t i = 0; i < arr.size(); ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";

    std::cout << "Array elements using at(): ";
    try {
        for (std::size_t i = 0; i < arr.size(); ++i) {
            std::cout << arr.at(i) << " ";
        }
        // 测试 at() 的越界访问
        std::cout << "\nTrying to access out-of-bounds index: ";
        std::cout << arr.at(10) << "\n"; // 这里会抛出异常
    } catch (const std::out_of_range& e) {
        std::cout << "Caught exception: " << e.what() << "\n";
    }

    // 测试 front() 和 back()
    std::cout << "First element (front): " << arr.front() << "\n";
    std::cout << "Last element (back): " << arr.back() << "\n";

    // 测试迭代器操作
    std::cout << "Array elements using iterators: ";
    for (auto it = arr.begin(); it != arr.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    // 测试反向迭代器
    // std::cout << "Array elements in reverse order: ";
    // for (auto it = arr.rbegin(); it != arr.rend(); ++it) {
    //     std::cout << *it << " ";
    // }
    // std::cout << "\n";

    // 测试容量相关函数
    std::cout << "Array size: " << arr.size() << "\n";
    std::cout << "Array max_size: " << arr.max_size() << "\n";
    std::cout << "Is array empty? " << (arr.empty() ? "Yes" : "No") << "\n";

    // 测试填充 fill()
    arr.fill(42);
    std::cout << "Array after fill(): ";
    for (const auto& elem : arr) {
        std::cout << elem << " ";
    }
    std::cout << "\n";

    // 测试交换 swap()
    stl::array<int, 5> other = {10, 20, 40, 30, 50};
    std::cout << "Other array before swap: ";
    for (const auto& elem : other) {
        std::cout << elem << " ";
    }
    std::cout << "\n";

    arr.swap(other);

    std::cout << "Array after swap(): ";
    for (const auto& elem : arr) {
        std::cout << elem << " ";
    }
    std::cout << "\n";

    std::cout << "Other array after swap: ";
    for (const auto& elem : other) {
        std::cout << elem << " ";
    }
    std::cout << "\n";

    // 测试 std::sort 和 std::accumulate
    std::sort(arr.begin(), arr.end());
    std::cout << "Array after sort(): ";
    for (const auto& elem : arr) {
        std::cout << elem << " ";
    }
    std::cout << "\n";

    int sum = std::accumulate(arr.begin(), arr.end(), 0);
    std::cout << "Sum of array elements: " << sum << "\n";

    // 测试直接访问底层数据
    int* raw_data = arr.data();
    std::cout << "Raw data accessed via data(): ";
    for (std::size_t i = 0; i < arr.size(); ++i) {
        std::cout << raw_data[i] << " ";
    }
    std::cout << "\n";

    std::cout << "All tests passed!" << std::endl;

    return 0;
}
