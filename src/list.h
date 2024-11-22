#ifndef __LIST_H__
#define __LIST_H__

#include <limits>
#include "memory.h"

namespace stl
{

/**
 * @brief 双向链表
 * @link https://zh.cppreference.com/w/cpp/container/list
 */
template <typename T, class Alloc = allocator<T>>
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
        __list_node()
            : prev(nullptr), next(nullptr)
        {}

        __list_node(const value_type & value)
            : prev(nullptr), next(nullptr), data(value)
        {}

        __list_node(value_type && value)
            : prev(nullptr), next(nullptr), data(std::move(value))
        {}

        __list_node(const __list_node & other)
            : prev(other.prev), next(other.next), data(other.data)
        {}

        __list_node(__list_node && other)
            : prev(other.prev), next(other.next), data(std::move(other.data))
        {}

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
        using const_reference = const value_type&;
        using pointer = value_type*;
        using const_pointer = const value_type*;
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

        ~__list_iterator() = default;

        __list_iterator & operator=(const __list_iterator & other)
        {
            _node = other._node;
            return *this;
        }

        bool operator==(const __list_iterator & other) const
        {
            return _node == other._node;
        }

        bool operator!=(const __list_iterator & other) const
        {
            return _node != other._node;
        }

        reference operator*()
        {
            return _node->data;
        }

        const_reference operator*() const
        {
            return _node->data;
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
    using allocator_type = Alloc;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename Alloc::pointer;
    using const_pointer = typename Alloc::const_pointer;
    using iterator = __list_iterator;
    using const_iterator = const __list_iterator;

protected:
    using node = __list_node;
    using node_pointer = node*;
    using list_node_allocator = typename Alloc::template rebind<node>::other;

protected:
    node_pointer dummy;                             // 虚拟尾节点
    size_type _size;                                // 元素个数
    allocator_type __allocator;                     // 元素分配器
    list_node_allocator __list_node_allocator;      // 节点分配器

public:
    // 构造函数
    list()
        : dummy(nullptr), _size(0)
    {
        // 一个空节点
        dummy = get_node();
        dummy->prev = dummy;
        dummy->next = dummy;
    }

    ~list()
    {
        clear();
        put_node(dummy);
    }

public:
    // 小工具

    /**
     * @brief 获取分配器
     */
    allocator_type get_allocator() const noexcept
    {
        return __allocator;
    }

    // 元素访问

    /**
     * @brief 返回第一个元素
     */
    reference front()
    {
        return *begin();
    }

    /**
     * @brief 返回第一个元素的常量引用
     */
    const_reference front() const
    {
        return *begin();
    }

    /**
     * @brief 返回最后一个元素
     */
    reference back()
    {
        return *(--end());
    }

    /**
     * @brief 返回最后一个元素的常量引用
     */
    const_reference back() const
    {
        return *(--end());
    }

    // 迭代器

    /**
     * @brief 返回指向起始的迭代器
     */
    iterator begin() noexcept
    {
        return ++iterator(dummy);
    }

    /**
     * @brief 返回指向起始的常量迭代器
     */
    const_iterator begin() const noexcept
    {
        return ++const_iterator(dummy);
    }

    /**
     * @brief 返回指向起始的常量迭代器
     */
    const_iterator cbegin() const noexcept
    {
        return ++const_iterator(dummy);
    }

    /**
     * @brief 返回指向末尾的迭代器
     */
    iterator end() noexcept
    {
        return iterator(dummy);
    }

    /**
     * @brief 返回指向末尾的常量迭代器
     */
    const_iterator end() const noexcept
    {
        return const_iterator(dummy);
    }

    /**
     * @brief 返回指向末尾的常量迭代器
     */
    const_iterator cend() const noexcept
    {
        return const_iterator(dummy);
    }

    // 容量

    /**
     * @brief 判断是否为空
     */
    bool empty()
    {
        return begin() == end();
    }

    /**
     * @brief 返回元素个数
     */
    size_type size()
    {
        return _size;
    }

    /**
     * @brief 返回最大元素个数
     */
    size_type max_size()
    {
        // https://zh.cppreference.com/w/cpp/container/list/max_size
        return std::numeric_limits<difference_type>::max();
    }

    // 修改器

    /**
     * @brief 清空链表
     */
    void clear()
    {
        while (!empty())
        {
            pop_front();
        }
        _size = 0;
    }

    iterator insert(iterator pos, const T & value)
    {
        node_pointer temp = create_node(value);
        temp->prev = pos._node->prev;
        temp->next = pos._node;
        pos._node->prev->next = temp;
        pos._node->prev = temp;
        ++_size;
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

    iterator erase(iterator pos)
    {
        if (pos._node == dummy) // 删除end()节点或者是空链表时
            return pos;
        pos._node->prev->next = pos._node->next;
        pos._node->next->prev = pos._node->prev;
        node_pointer temp = pos._node;
        pos._node = pos._node->next;
        destroy_node(temp);
        --_size;
        return iterator(pos._node);
    }

    void pop_front()
    {
        erase(begin());
    }

    void pop_back()
    {
        erase(--end());
    }

    /**
     * @brief 修改链表大小
     */
    void resize(size_type n)
    {
        // value_type需要提供默认构造
        resize(n, value_type());
    }

    /**
     * @brief 修改链表大小
     */
    void resize(size_type n, const value_type & value)
    {
        size_type cur = size();
        if (n == cur)
            return;
        if (n > cur) {
            for (size_type i = cur; i < n; ++i) {
                push_back(value);
            }
        } else {
            for (size_type i = cur; i > n; --i) {
                pop_back();
            }
        }
    }

    void swap(list & other)
    {
        std::swap(dummy, other.dummy);
        std::swap(_size, other._size);
    }

    // 链表专属操作

    /**
     * @brief 合并升序链表
     */
    void merge(list & other)
    {
        merge(other, [](const T & a, const T & b){
            return a < b;
        });
    }

    /**
     * @brief 合并升序链表
     */
    void merge(list && other)
    {
        merge(std::move(other), [](const T & a, const T & b){
            return a < b;
        });
    }

    /**
     * @brief 合并升序链表
     */
    template <class Compare>
    void merge(list & other, Compare comp)
    {
        
    }

    /**
     * @brief 合并升序链表
     */
    template <class Compare>
    void merge(list && other, Compare comp)
    {
        
    }

    /**
     * @brief 反转链表
     */
    void reverse()
    {
        node_pointer cur = dummy->next;
        node_pointer temp = nullptr;

        while (cur != dummy) {
            temp = cur->next;
            std::swap(cur->next, cur->prev);
            cur = temp;
        }
        std::swap(dummy->next, dummy->prev);
    }

    /**
     * @brief 删除相邻的重复元素
     * @details operator== 为默认的比较函数
     */
    void unique()
    {
        unique([](const T & a, const T & b){
            return a == b;  // 默认的比较函数
        });
    }

    /**
     * @brief 删除相邻的满足比较函数的元素
     * @param p 比较函数
     */
    template <class BinaryPredicate>
    void unique(BinaryPredicate p)
    {
        iterator cur = begin();
        iterator next = cur;
        ++next;

        while (next != end()) {
            if (p(*cur, *next)) {
                // next迭代器在删除的过程中移动了
                next = erase(next);
            } else {
                cur = next;
                ++next;
            }
        }
    }

    /**
     * @brief 对链表进行排序
     * @details operator< 为默认的比较函数
     */
    void sort()
    {
        sort([](const T & a, const T & b){
            return a < b;   // 默认的比较函数
        });
    }

    /**
     * @brief 使用比较函数对链表进行排序
     * @param comp 比较函数
     */
    template <class Compare>
    void sort(Compare comp)
    {
        // 使用归并排序
    }

protected:
    // 提供链表内部使用的节点操作
    
    node_pointer get_node()
    {
        return __list_node_allocator.allocate(1);
    }

    void put_node(node_pointer ptr)
    {
        __list_node_allocator.deallocate(ptr);
    }

    node_pointer create_node(const value_type & value)
    {
        node_pointer p = get_node();
        __list_node_allocator.construct(p, value);
        return p;
    }

    void destroy_node(node_pointer ptr)
    {
        __list_node_allocator.destroy(ptr);
        put_node(ptr);
    }
};

} // namespace stl

#endif