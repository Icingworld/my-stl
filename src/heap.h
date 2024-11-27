#ifndef __HEAP_H__
#define __HEAP_H__

namespace stl
{

/**
 * @brief 构造堆
 * @link https://zh.cppreference.com/w/cpp/container/priority_queue
 * @details RandomIt必须是一个随机访问迭代器
 */
template <class RandomIt>
void make_heap(RandomIt first, RandomIt last)
{
    make_heap(first, last, std::less<typename std::iterator_traits<RandomIt>::value_type>());
}

/**
 * @brief 构造带有比较函数的堆
 * @details RandomIt必须是一个随机访问迭代器，Compare必须满足比较
 */
template <class RandomIt, class Compare>
void make_heap(RandomIt first, RandomIt last, Compare comp)
{

}

}

#endif