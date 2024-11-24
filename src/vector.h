#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <iostream>
#include <limits>
#include <initializer_list>
#include <stdexcept>
#include "memory.h"

namespace stl
{

template <typename T, typename Alloc = stl::allocator<T>>
class vector
{
public:
    class __vector_iterator
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
        using self = __vector_iterator;
    
    protected:
        pointer _ptr;
        friend class vector;

    public:
        __vector_iterator()
            : _ptr(nullptr)
        {}
        __vector_iterator(pointer ptr)
            : _ptr(ptr)
        {}
        __vector_iterator(const_pointer ptr)
            : _ptr(const_cast<pointer>(ptr))
        {}
        __vector_iterator(const self & other)
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
    using allocator_type = Alloc;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = __vector_iterator;
    using const_iterator = const iterator;

protected:
    pointer start;              // 起始元素指针
    pointer finish;             // 结束元素的下一位指针
    pointer end_of_storage;     // 空间末尾指针

    Alloc allocator;            // 分配器

public:
    vector()
        : start(nullptr), finish(nullptr), end_of_storage(nullptr)
    {}

    ~vector()
    {
        clear();
        // 销毁空间
        allocator.deallocate(start, end_of_storage - start);
    }

public:
    // 小工具

    /**
     * @brief 获取分配器
     */
    Alloc get_allocator() const noexcept
    {
        return allocator;
    }

    // 元素访问

    /**
     * @brief 获取指定位置的元素
     */
    reference at(size_type pos)
    {
        if (pos >= size()) {
            throw std::out_of_range("vector out of range");
        }
        return *(begin() + pos);
    }

    /**
     * @brief 获取指定位置的元素
     */
    const_reference at(size_type pos) const
    {
        if (pos >= size()) {
            throw std::out_of_range("vector out of range");
        }
        return *(begin() + pos);
    }

    /**
     * @brief 获取指定位置的元素
     */
    reference operator[](size_type pos)
    {
        return *(begin() + pos);
    }

    /**
     * @brief 获取指定位置的元素
     */
    const_reference operator[](size_type pos) const
    {
        return *(begin() + pos);
    }

    /**
     * @brief 获取第一个元素
     */
    reference front()
    {
        return *begin();
    }

    /**
     * @brief 获取第一个元素
     */
    const_reference front() const
    {
        return *begin();
    }

    /**
     * @brief 获取最后一个元素
     */
    reference back()
    {
        return *(--end());
    }

    /**
     * @brief 获取最后一个元素
     */
    const_reference back() const
    {
        return *(--end());
    }

    /**
     * @brief 提供访问底层数据的指针
     */
    pointer data()
    {
        return start;
    }

    /**
     * @brief 提供访问底层数据的常量指针
     */
    const_pointer data() const
    {
        return start;
    }

    // 迭代器

    iterator begin()
    {
        return iterator(start);
    }

    const_iterator begin() const
    {
        return const_iterator(start);
    }

    const_iterator cbegin() const
    {
        return const_iterator(start);
    }

    iterator end()
    {
        return iterator(finish);
    }

    const_iterator end() const
    {
        return const_iterator(finish);
    }

    const_iterator cend() const
    {
        return const_iterator(finish);
    }

    // 容量

    /**
     * @brief 容器是否为空
     */
    bool empty() const noexcept
    {
        return begin() == end();
    }

    /**
     * @brief 元素个数
     */
    size_type size() const
    {
        return std::distance(begin(), end());
    }

    /**
     * @brief 最大容量
     */
    size_type max_size() const noexcept
    {
        return std::numeric_limits<difference_type>::max();
    }

    /**
     * @brief 重新分配空间
     */
    void reserve(size_type new_cap)
    {
        if (new_cap <= capacity())
            return;
        
        _reallocate(new_cap);
    }

    /**
     * @brief 容量
     */
    size_type capacity() const
    {
        return std::distance(begin(), iterator(end_of_storage));
    }

    /**
     * @brief 释放不使用的空间
     * @details 本实现重新分配空间
     */
    void shrink_to_fit()
    {
        _reallocate(size());
    }

    // 修改器

    /**
     * @brief 清空vector
     * @details 清空vector，但不归还空间
     */
    void clear()
    {
        for (pointer it = start; it != finish; ++it) {
            it->~value_type();
        }
        finish = start;
    }

    /**
     * @brief 在指定位置插入元素
     */
    iterator insert(iterator pos, const value_type & value)
    {
        if (size() == capacity()) {
            reserve(capacity() == 0 ? 1 : capacity() * 2);
        }

        // std::memmove(pos._ptr + 1, pos._ptr, (finish - pos._ptr) * sizeof(value_type));
        std::move(pos._ptr, finish, pos._ptr + 1);
        *pos._ptr = value;
        ++finish;
        return pos;
    }

    /**
     * @brief 在指定位置插入元素
     */
    iterator insert(iterator pos, value_type && value)
    {
        if (size() == capacity()) {
            reserve(capacity() == 0 ? 1 : capacity() * 2);
        }

        // std::memmove(pos._ptr + 1, pos._ptr, (finish - pos._ptr) * sizeof(value_type));
        std::move(pos._ptr, finish, pos._ptr + 1);
        *pos._ptr = std::move(value);
        ++finish;
        return pos;
    }

    /**
     * @brief 在指定位置插入n个元素
     */
    iterator insert(iterator pos, size_type count, const value_type & value)
    {
        if (size() + count > capacity()) {
            reserve(size() + count);
        }

        // std::memmove(pos._ptr + count, pos._ptr, (finish - pos._ptr) * sizeof(value_type));
        std::move(pos._ptr, finish, pos._ptr + count);
        std::uninitialized_fill_n(pos._ptr, count, value);
        finish += count;
        return pos;
    }

    /**
     * @brief 在指定位置插入迭代器范围的元素
     */
    template <class InputIterator>
    iterator insert(iterator pos, InputIterator first, InputIterator last)
    {
        InputIterator it = first;
        while (it != last) {
            pos = insert(pos, *it);
            ++it;
        }
        return pos;
    }

    /**
     * @brief 在指定位置插入initializer_list
     */
    iterator insert(iterator pos, std::initializer_list<value_type> ilist)
    {
        return insert(pos, ilist.begin(), ilist.end());
    }

    /**
     * @brief 在指定位置构造元素
     */
    template <class... Args>
    iterator emplace(iterator pos, Args &&... args)
    {
        if (size() == capacity()) {
            reserve(capacity() == 0 ? 1 : capacity() * 2);
        }

        // std::memmove(pos._ptr + 1, pos._ptr, (finish - pos._ptr) * sizeof(value_type));
        std::move(pos._ptr, finish, pos._ptr + 1);
        allocator.construct(pos._ptr, std::forward<Args>(args)...);
        ++finish;
        return pos;
    }

    /**
     * @brief 删除指定位置的元素
     */
    iterator erase(const_iterator pos)
    {
        pointer target = pos._ptr;
        std::move(target + 1, finish, target);
        allocator.destroy(--finish);    // 销毁最后一个元素，因为它不再有效
        return iterator(target);
    }

    /**
     * @brief 删除指定范围的元素
     */
    iterator erase(const_iterator first, const_iterator last)
    {
        if (first == last)
            return iterator(last._ptr);

        std::move(last._ptr, finish, first._ptr);
        for (pointer it = first._ptr; it != last._ptr; ++it) {
            allocator.destroy(it);
        }
        finish = finish - (last._ptr - first._ptr);
        return iterator(first._ptr);
    }

    /**
     * @brief 在尾部插入元素
     */
    void push_back(const reference value)
    {
        if (size() == capacity()) {
            reserve(capacity() == 0 ? 1 : capacity() * 2);
        }
        allocator.construct(finish, value);
        ++finish;
    }

    /**
     * @brief 在尾部插入元素
     */
    void push_back(value_type && value)
    {
        emplace_back(std::move(value));
    }

    template <typename... Args>
    void emplace_back(Args&&... args)
    {
        if (size() == capacity()) {
            reserve(capacity() == 0 ? 1 : capacity() * 2);
        }
        allocator.construct(finish, std::forward<Args>(args)...);
        ++finish;
    }

    /**
     * @brief 删除尾部元素
     */
    void pop_back()
    {
        --finish;
        allocator.destroy(finish);
    }

    /**
     * @brief 修改vector的大小
     */
    void resize(size_type count)
    {
        if (count < size()) {
            erase(begin() + count, end());
        } else if (count > size()) {
            insert(end(), count - size(), value_type());
        }
    }

    /**
     * @brief 修改vector的大小
     */
    void resize(size_type count, const value_type & value)
    {
        if (count < size()) {
            erase(begin() + count, end());
        } else if (count > size()) {
            insert(end(), count - size(), value);
        }
    }

    /**
     * @brief 交换vector
     * @details 交换两个vector
     */
    void swap(vector & other)
    {
        std::swap(start, other.start);
        std::swap(finish, other.finish);
        std::swap(end_of_storage, other.end_of_storage);
    }

protected:
    // 内部函数

    void _reallocate(size_type new_cap)
    {
        size_type old_size = size();
        // 分配新空间
        pointer new_start = allocator.allocate(new_cap);
        // 将旧数据拷贝到新空间，这里需要重写这个函数
        std::uninitialized_copy(start, finish, new_start);
        // 销毁旧元素
        for (pointer it = start; it != finish; ++it) {
            it->~value_type();
        }
        // 释放旧空间
        allocator.deallocate(start, capacity());

        // 更新指针
        start = new_start;
        finish = new_start + old_size;
        end_of_storage = new_start + new_cap;
    }
};

// 非成员函数

template <class T, class Alloc>
bool operator==(const vector<T, Alloc> & lhs, const vector<T, Alloc> & rhs)
{
    return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <class T, class Alloc>
bool operator!=(const vector<T, Alloc> & lhs, const vector<T, Alloc> & rhs)
{
    return !(lhs == rhs);
}

template <class T, class Alloc>
bool operator<(const vector<T, Alloc> & lhs, const vector<T, Alloc> & rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <class T, class Alloc>
bool operator>(const vector<T, Alloc> & lhs, const vector<T, Alloc> & rhs)
{
    return rhs < lhs;
}

template <class T, class Alloc>
bool operator<=(const vector<T, Alloc> & lhs, const vector<T, Alloc> & rhs)
{
    return !(rhs < lhs);
}

template <class T, class Alloc>
bool operator>=(const vector<T, Alloc> & lhs, const vector<T, Alloc> & rhs)
{
    return !(lhs < rhs);
}

template <class T, class Alloc>
void swap(vector<T, Alloc> & lhs, vector<T, Alloc> & rhs)
{
    lhs.swap(rhs);
}

}

#endif