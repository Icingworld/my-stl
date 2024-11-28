#ifndef __PRIORITY_QUEUE_H__
#define __PRIORITY_QUEUE_H__

#include "vector.h"
#include "heap.h"

namespace stl
{

/**
 * @brief 优先队列
 * @link https://zh.cppreference.com/w/cpp/container/priority_queue
 * @details 优先队列默认以vector作为底层容器
 */
template <class T, class Container = stl::vector<T>, class Compare = std::less<typename Container::value_type>>
class priority_queue
{
public:
    using container_type = Container;
    using value_type = typename Container::value_type;
    using size_type = typename Container::size_type;
    using reference = typename Container::reference;
    using const_reference = typename Container::const_reference;

protected:
    container_type c;   // 基础容器
    Compare comp;       // 比较器

public:
    // 构造函数

    priority_queue()
        : c(), comp()
    {}

    ~priority_queue() = default;

public:
    // 元素访问

    /**
     * @brief 获取优先队列队首元素
     */
    const_reference top() const
    {
        return c.front();
    }

    // 容量

    /**
     * @brief 优先队列是否为空
     */
    bool empty() const
    {
        return c.empty();
    }

    /**
     * @brief 优先队列的元素个数
     */
    size_type size() const
    {
        return c.size();
    }

    // 修改器

    /**
     * @brief 在优先队列中插入元素
     */
    void push(const value_type& x)
    {
        c.push_back(x);
        stl::push_heap(c.begin(), c.end(), comp);
    }

    /**
     * @brief 在优先队列中插入元素
     */
    void push(value_type&& x)
    {
        c.push_back(std::move(x));
        stl::push_heap(c.begin(), c.end(), comp);
    }

    template <class... Args>
    void emplace(Args&&... args)
    {
        c.emplace_back(std::forward<Args>(args)...);
        stl::push_heap(c.begin(), c.end(), comp);
    }

    /**
     * @brief 删除队首元素
     */
    void pop()
    {
        stl::pop_heap(c.begin(), c.end(), comp);
        c.pop_back();
    }

    /**
     * @brief 交换两个优先队列
     */
    void swap(priority_queue& other) noexcept
    {
        std::swap(c, other.c);
        std::swap(comp, other.comp);
    }
};

template <class T, class Container, class Compare>
void swap(priority_queue<T, Container, Compare>& lhs, priority_queue<T, Container, Compare>& rhs)
{
    lhs.swap(rhs);
}

} // namespace stl

#endif