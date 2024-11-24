#ifndef __ARRAY_H__
#define __ARRAY_H__

#include "memory.h"

namespace stl
{

/**
 * @brief 数组
 * @details 固定长度数组
 * @link https://zh.cppreference.com/w/cpp/container/array
 */
template <typename T, std::size_t N>
class array
{
public:
    // 迭代器
    class __array_iterator
    {
    public:
        using value_type = T;
        using reference = value_type&;
        using const_reference = const value_type&;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::random_access_iterator_tag;

    protected:
        using self = __array_iterator;
    
    protected:
        pointer _ptr;

    public:
        __array_iterator()
            : _ptr(nullptr)
        {}
        __array_iterator(pointer ptr)
            : _ptr(ptr)
        {}
        __array_iterator(const_pointer ptr)
            : _ptr(const_cast<pointer>(ptr))
        {}
        __array_iterator(const self & other)
            : _ptr(other._ptr)
        {}
        
        self & operator=(const self & other)
        {
            _ptr = other._ptr;
            return *this;
        }

        bool operator==(const self & other) const
        {
            return _ptr == other._ptr;
        }

        bool operator!=(const self & other) const
        {
            return _ptr != other._ptr;
        }

        bool operator<(const self & other) const
        {
            return _ptr < other._ptr;
        }

        bool operator>(const self & other) const
        {
            return _ptr > other._ptr;
        }

        bool operator<=(const self & other) const
        {
            return _ptr <= other._ptr;
        }

        bool operator>=(const self & other) const
        {
            return _ptr >= other._ptr;
        }

        reference operator*()
        {
            return *_ptr;
        }

        const_reference operator*() const
        {
            return *_ptr;
        }

        pointer operator->()
        {
            return &(operator*());
        }

        const_pointer operator->() const
        {
            return &(operator*());
        }

        self & operator++()
        {
            ++_ptr;
            return *this;
        }

        self operator++(int)
        {
            self temp = *this;
            ++*this;
            return temp;
        }

        self & operator--()
        {
            --_ptr;
            return *this;
        }

        self operator--(int)
        {
            self temp = *this;
            --*this;
            return temp;
        }

        self & operator+=(difference_type n)
        {
            _ptr += n;
            return *this;
        }

        self & operator-=(difference_type n)
        {
            _ptr -= n;
            return *this;
        }

        self operator+(difference_type n) const
        {
            self temp = *this;
            temp += n;
            return temp;
        }

        self operator-(difference_type n) const
        {
            self temp = *this;
            temp -= n;
            return temp;
        }

        difference_type operator-(const self & other) const
        {
            return _ptr - other._ptr;
        }

        reference operator[](difference_type n) const
        {
            return *(_ptr + n);
        }
    };

public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = __array_iterator;
    using const_iterator = const __array_iterator;

public:
    value_type _data[N];        // 数据

public:
    // 元素访问

    reference operator[](size_type pos)
    {
        return _data[pos];
    }

    const_reference operator[](size_type pos) const
    {
        return _data[pos];
    }

    reference at(size_type pos)
    {
        if (pos >= size()) {
            throw std::out_of_range("array out of range");
        }
        return _data[pos];
    }

    const_reference at(size_type pos) const
    {
        if (pos >= N) {
            throw std::out_of_range("array out of range");
        }
        return _data[pos];
    }

    reference front()
    {
        return *begin();
    }

    const_reference front() const
    {
        return *begin();
    }

    reference back()
    {
        return *--end();
    }

    const_reference back() const
    {
        return *--end();
    }

    pointer data()
    {
        return _data;
    }

    const_pointer data() const
    {
        return _data;
    }

    // 迭代器

    iterator begin()
    {
        return iterator(_data);
    }

    const_iterator begin() const
    {
        return const_iterator(_data);
    }

    const_iterator cbegin() const
    {
        return const_iterator(_data);
    }

    iterator end()
    {
        return iterator(_data + N);
    }

    const_iterator end() const
    {
        return const_iterator(_data + N);
    }

    const_iterator cend() const
    {
        return const_iterator(_data + N);
    }

    // 容量

    constexpr size_type size() const noexcept
    {
        return N;
    }

    constexpr bool empty() const noexcept
    {
        return begin() == end();
    }

    constexpr size_type max_size() const noexcept
    {
        return size();
    }

    // 操作

    void fill(const value_type & value)
    {
        iterator it = begin();
        while (it != end()) {
            *it = value;
            ++it;
        }
    }

    void swap(array<T, N> & other) noexcept
    {
        std::swap(_data, other._data);
    }
};

// 非成员函数

template <typename T, std::size_t N>
bool operator==(const array<T, N> & a, const array<T, N> & b)
{
    return std::equal(a.begin(), a.end(), b.begin(), b.end());
}

template <typename T, std::size_t N>
bool operator!=(const array<T, N> & a, const array<T, N> & b)
{
    return !(a == b);
}

template <typename T, std::size_t N>
bool operator<(const array<T, N> & a, const array<T, N> & b)
{
    return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
}

template <typename T, std::size_t N>
bool operator<=(const array<T, N> & a, const array<T, N> & b)
{
    return !(b < a);
}

template <typename T, std::size_t N>
bool operator>(const array<T, N> & a, const array<T, N> & b)
{
    return b < a;
}

template <typename T, std::size_t N>
bool operator>=(const array<T, N> & a, const array<T, N> & b)
{
    return !(a < b);
}

/**
 * @brief 交换两个array
 */
template <typename T, std::size_t N>
void swap(array<T, N> & a, array<T, N> & b)
{
    a.swap(b);
}

/**
 * @brief 获取array中第I个元素
 */
template <std::size_t I, class T, std::size_t N>
T & get(array<T, N> & a)
{
    return a[I];
}

/**
 * @brief 获取array中第I个元素
 */
template <std::size_t I, class T, std::size_t N>
T && get(array<T, N> && a)
{
    return std::move(a[I]);
}

/**
 * @brief 获取array中第I个元素
 */
template <std::size_t I, class T, std::size_t N>
const T & get(const array<T, N> & a)
{
    return a[I];
}

/**
 * @brief 获取array中第I个元素
 */
template <std::size_t I, class T, std::size_t N>
const T && get(const array<T, N> && a)
{
    return std::move(a[I]);
}

// 辅助类

template <std::size_t I, class T>
class tuple_element;

template <std::size_t I, class T, std::size_t N>
class tuple_element<I, array<T, N>>
{
public:
    using type = T;
};

template <class T>
class tuple_size;

template<class T, std::size_t N>
class tuple_size<array<T, N>> : std::integral_constant<std::size_t, N>
{

};

} // namespace stl

#endif