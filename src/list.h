#ifndef __LIST_H__
#define __LIST_H__

#include "memory.h"

namespace stl
{

template <typename T, class Alloc = simple_alloc<T>>
class list
{
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
    
    public:
        __list_node() : prev(nullptr), next(nullptr) {}
        __list_node(const value_type & value) : prev(nullptr), next(nullptr), data(value) {}
        __list_node(value_type && value) : prev(nullptr), next(nullptr), data(std::move(value)) {}
        __list_node(const __list_node & other) : prev(other.prev), next(other.next), data(other.data) {}
        __list_node(__list_node && other) : prev(other.prev), next(other.next), data(std::move(other.data)) {}

        ~__list_node() = default;
    };

public:
    // 链表迭代器
    class __list_iterator
    {
    public:
        // 嵌套类型
        using value_type = T;
        friend class list;
    
    protected:
        using reference = value_type&;
        using pointer = value_type*;
        using node_pointer = __list_node*;
        using self = __list_iterator;
        // using iterator_category = stl::bidirectional_iterator_tag;
    
    protected:
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
    };

public:
    // 嵌套类型
    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = __list_iterator;
    using node = __list_node;
    using node_pointer = node*;
    using size_type = size_t;
    using value_type_allocator = Alloc;
    using list_node_allocator = typename Alloc::template rebind<node>::other;

protected:
    node_pointer dummy;     // 虚拟尾节点
    list_node_allocator __list_node_allocator;

protected:
    // 提供链表内部使用的节点操作
    node_pointer get_node()
    {
        return __list_node_allocator.allocate();
    }

    void put_node(node_pointer p)
    {
        __list_node_allocator.deallocate(p);
    }

    node_pointer create_node(const value_type & value)
    {
        node_pointer p = get_node();
        __list_node_allocator.construct(p, value);
        return p;
    }

    void destroy_node(node_pointer p)
    {
        __list_node_allocator.destroy(p);
    }

public:
    // 构造函数
    list()
    {
        // 一个空节点
        dummy = get_node();
        dummy->prev = dummy;
        dummy->next = dummy;
    }

public:
    // 对外接口
    iterator insert(iterator pos, const T & value)
    {
        node_pointer temp = create_node(value);
        temp->prev = pos._node->prev;
        temp->next = pos._node;
        pos._node->prev->next = temp;
        pos._node->prev = temp;
        return iterator(temp);
    }

    iterator push_front(const T & value)
    {
        return insert(begin(), value);
    }

    iterator push_back(const T & value)
    {
        return insert(end(), value);
    }

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

    reference back()
    {
        return *(--end());
    }
};

} // namespace stl

#endif