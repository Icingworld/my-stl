#ifndef __DEQUE_H__
#define __DEQUE_H__

#include <stdexcept>
#include <limits>
#include "memory.h"

namespace stl
{

template <class T, class Alloc = allocator<T>>
class deque
{
public:
    class __deque_iterator
    {
    public:
        using value_type = T;
        using reference = value_type&;
        using const_reference = const value_type&;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::random_access_iterator_tag;
        using map_pointer = value_type**;
    
    protected:
        pointer _cur;       // 当前指向的元素
        pointer _first;     // 当前node的第一个元素
        pointer _last;      // 当前node的最后一个元素
        map_pointer _node;  // 当前node

        using self = __deque_iterator;
        friend class deque;

    public:
        __deque_iterator()
            : _cur(nullptr), _first(nullptr), _last(nullptr), _node(nullptr)
        {}

        bool operator==(const self & other) const
        {
            return _cur == other._cur;
        }

        bool operator!=(const self & other) const
        {
            return !(*this == other);
        }

        bool operator<(const self & other) const
        {
            return _node == other._node ? _cur < other._cur : _node < other._node;
        }

        bool operator>(const self & other) const
        {
            return other < *this;
        }

        bool operator<=(const self & other) const
        {
            return !(*this > other);
        }

        bool operator>=(const self & other) const
        {
            return !(*this < other);
        }

        reference operator*() const
        {
            return *_cur;
        }

        const_reference operator*() const
        {
            return *_cur;
        }

        pointer operator->() const
        {
            return &(operator*());
        }

        const_pointer operator->() const
        {
            return &(operator*());
        }

        self & operator++()
        {
            ++_cur;
            if (_cur == _last) {
                // 已经到达尾部，需要移动到下一个node
                set_node(_node + 1);
                _cur = _first;
            }
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
            if (_cur == _first) {
                // 已经位于头部，需要移动到上一个node
                set_node(_node - 1);
                _cur = _last;
            }
            --_cur;
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
            difference_type offset = n + (_cur - _first);
            difference_type buffer_size = static_cast<difference_type>(buffer_size());
            if (offset >= 0 && offset < buffer_size) {
                // 在当前的node中
                _cur += n;
            } else {
                // 不在当前的node中
                // 计算需要移动的node数
                difference_type node_offset = offset > 0 ? offset / buffer_size : -((-offset - 1) / buffer_size) - 1;
                set_node(_node + node_offset);
                _cur = _first + (offset - node_offset * buffer_size);
            }
            return *this;
        }

        self & operator-=(difference_type n)
        {
            return *this += -n;
        }

        self operator+(difference_type n) const
        {
            self temp = *this;
            return temp += n;
        }

        self operator-(difference_type n) const
        {
            self temp = *this;
            return temp -= n;
        }

        difference_type operator-(const self & other) const
        {
            return ;
        }

        reference operator[](difference_type n) const
        {
            return *(*this + n);
        }
    
    protected:
        /**
         * @brief 设置当前迭代器指向的节点
         */
        void set_node(map_pointer new_node)
        {
            _node = new_node;
            _first = *_node;
            _last = _first + __deque_buffer_size();
        }

        const size_type buffer_size() const noexcept
        {
            return __deque_buffer_size();
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
    using iterator = __deque_iterator;
    using const_iterator = __deque_iterator;

protected:
    const size_type __deque_buffer_size() const noexcept
    {
        return 2;
    }
};


}

#endif