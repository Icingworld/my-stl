#ifndef __UTILITY_H__
#define __UTILITY_H__

#include <utility>

namespace stl
{

template <typename T, typename U>
class pair
{
public:
    using first_type = T;
    using second_type = U;

public:
    first_type first;
    second_type second;

    pair()
        : first(), second()
    {}

    pair(const first_type & t, const second_type & u)
        : first(t), second(u)
    {}

    pair(const pair & p)
        : first(p.first), second(p.second)
    {}

    pair(first_type && t, second_type && u)
        : first(std::move(t)), second(std::move(u))
    {}

    pair(pair && p)
        : first(std::move(p.first)), second(std::move(p.second))
    {}

    ~pair() = default;

    pair & operator=(const pair & p)
    {
        if (this != &p) {
            first = p.first;
            second = p.second;
        }
        return *this;
    }
};

/**
 * @brief 创建pair对象的模板函数
 */
template <typename T, typename U>
pair<T, U> make_pair(T && first, U && second)
{
    return pair<T, U>(std::forward<T>(first), std::forward<U>(second));
}

}

#endif