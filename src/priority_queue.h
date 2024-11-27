#ifndef __PRIORITY_QUEUE_H__
#define __PRIORITY_QUEUE_H__

#include <functional>
#include <algorithm>
#include "vector.h"

namespace stl
{

/**
 * @brief 优先队列
 * @link https://zh.cppreference.com/w/cpp/container/priority_queue
 * @details 优先队列默认以vector作为底层容器
 */
template <class T, class Container = vector<T>, class Compare = std::less<typename Container::value_type>>
class priority_queue
{
    public: 
};

}

#endif