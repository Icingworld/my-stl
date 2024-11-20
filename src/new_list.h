#ifndef __LIST_H__
#define __LIST_H__

#include "utility.h"

namespace stl
{

template <typename T>
class list
{
public:
    // 嵌套类型
    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = __list_iterator;
    using node_pointer = __list_node*;
    using size_type = size_t;

protected:
    // 链表节点类
    class __list_node
    {
    public:
        // 嵌套类型
        using value_type = T;
        using node_pointer = __list_node*;
    
    public:
        node_pointer prev;
        node_pointer next;
        value_type data;
    }

public:
    // 链表迭代器
    class __list_iterator
    {
    public:
        // 嵌套类型
        using value_type = T;
        using reference = value_type&;
        using pointer = value_type*;
        using node_pointer = __list_node*;
        using self = __list_iterator;
        using iterator_category = stl::bidirectional_iterator_tag;
    
    private:
        node_pointer _node;
    
    public:
        __list_iterator()
            : _node(nullptr)
        {}
        __list_iterator(node_pointer node)
            : _node(node)
        {}
        __list_iterator(const __list_iterator & other)
            : _node(other._node)
        {}

        bool operator==(const __list_iterator & other) const
        {
            return _node == other._node;
        }

        bool operator!=(const __list_iterator & other) const
        {
            return _node != other._node;
        }

        reference operator*() const
        {
            return _node->data;
        }

        pointer operator->() const
        {
            return &(operator*());
        }

        self & operator++()
        {
            _node = _node->next;
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
            _node = _node->prev;
            return *this;
        }

        self operator--(int)
        {
            self temp = *this;
            --*this;
            return temp;
        }
    }

protected:
    node_pointer dummy;

public:
    list()
        : dummy(new __list_node)
    {}

    ~list()
    {}

    iterator begin()
    {
        return ++iterator(dummy);
    }

    iterator end()
    {
        return iterator(dummy);
    }

    bool empty()
    {
        return begin() == end();
    }

    size_type size()
    {
        size_type _size = 0;
        // 计算迭代器距离
        return _size;
    }

    reference front()
    {
        return *begin();
    }

    reference end()
    {
        return *(--end());
    }
};

} // namespace stl

#endif