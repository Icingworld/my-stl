#ifndef __STACK_H__
#define __STACK_H__

#include "deque.h"

namespace stl
{

/**
 * @brief 栈
 * @link https://zh.cppreference.com/w/cpp/container/stack
 * @details 栈默认以deque作为底层容器
 */
template <class T, class Container = deque<T>>
class stack
{
public:
    using container_type = Container;
    using value_type = typename Container::value_type;
    using size_type = typename Container::size_type;
    using reference = typename Container::reference;
    using const_reference = typename Container::const_reference;

protected:
    container_type c;            // 基础容器

public:
    stack() = default;

    ~stack() = default;

public:
    // 元素访问

    reference top()
    {
        return c.back();
    }

    const_reference top() const
    {
        return c.back();
    }

    // 容量

    bool empty() const
    {
        return c.empty();
    }

    size_type size() const
    {
        return c.size();
    }

    // 修改器

    void push(const value_type& x)
    {
        c.push_back(x);
    }

    void push(value_type&& x)
    {
        c.push_back(std::move(x));
    }

    template <class... Args>
    void emplace(Args&&... args)
    {
        c.emplace_back(std::forward<Args>(args)...);
    }

    void pop()
    {
        c.pop_back();
    }

    void swap(stack& other)
    {
        c.swap(other.c);
    }
};

// 非成员函数

template <class T, class Container>
bool operator==(const stack<T, Container>& x, const stack<T, Container>& y)
{
    return x.c == y.c;
}

template <class T, class Container>
bool operator!=(const stack<T, Container>& x, const stack<T, Container>& y)
{
    return !(x == y);
}

template <class T, class Container>
bool operator<(const stack<T, Container>& x, const stack<T, Container>& y)
{
    return x.c < y.c;
}

template <class T, class Container>
bool operator>(const stack<T, Container>& x, const stack<T, Container>& y)
{
    return y < x;
}

template <class T, class Container>
bool operator<=(const stack<T, Container>& x, const stack<T, Container>& y)
{
    return !(y < x);
}

template <class T, class Container>
bool operator>=(const stack<T, Container>& x, const stack<T, Container>& y)
{
    return !(x < y);
}

template <class T, class Container>
void swap(stack<T, Container>& x, stack<T, Container>& y)
{
    x.swap(y);
}

}

#endif