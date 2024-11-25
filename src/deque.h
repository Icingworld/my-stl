#ifndef __DEQUE_H__
#define __DEQUE_H__

#include <stdexcept>
#include <limits>
#include "memory.h"

namespace stl
{

/**
 * @brief 双端队列
 * @link https://zh.cppreference.com/w/cpp/container/deque
 */
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
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::random_access_iterator_tag;
        using map_pointer = value_type**;
    
    protected:
        pointer _cur;       // 当前指向的元素
        pointer _first;     // 当前node的第一个元素
        pointer _last;      // 当前node的最后一个元素
        map_pointer _node;  // 当前node指针

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

        reference operator*()
        {
            return *_cur;
        }

        const_reference operator*() const
        {
            return *_cur;
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
            difference_type _buffer_size = static_cast<difference_type>(buffer_size());
            if (offset >= 0 && offset < _buffer_size) {
                // 在当前的node中
                _cur += n;
            } else {
                // 不在当前的node中
                // 计算需要移动的node数
                difference_type node_offset = offset > 0 ? offset / _buffer_size : -((-offset - 1) / _buffer_size) - 1;
                set_node(_node + node_offset);
                _cur = _first + (offset - node_offset * _buffer_size);
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
            return static_cast<difference_type>((_node - other._node) * buffer_size() + (_cur - _first) - (other._cur - other._first));
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
            _last = _first + buffer_size();
        }

        size_type buffer_size() const noexcept
        {
            return deque::__deque_buffer_size();
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
    using map_pointer = value_type**;
    using iterator = __deque_iterator;
    using const_iterator = __deque_iterator;
    using map_allocator_type = typename allocator_type::template rebind<pointer>::other;

// protected:
    iterator _start;                    // 起始位置迭代器，指向头部
    iterator _finish;                   // 结束位置迭代器，指向尾部的下一个位置
    map_pointer _map;                   // 中控器指针
    size_type _map_size;                // 中控器大小

    allocator_type allocator;           // 元素分配器
    map_allocator_type  map_allocator;  // 中控器分配器

public:
    // 构造函数
    deque()
        : _start(), _finish(), _map(nullptr), _map_size(0)
    {
        __initialize_map(0);
    }

public:
    // 小工具

    allocator_type get_allocator() const
    {
        return allocator;
    }

    // 元素访问

    reference at(size_type n)
    {
        if (n >= size()) {
            throw std::out_of_range("deque out of range");
        }
        return (*this)[static_cast<difference_type>(n)];
    }

    const_reference at(size_type n) const
    {
        if (n >= size()) {
            throw std::out_of_range("deque out of range");
        }
        return (*this)[static_cast<difference_type>(n)];
    }

    reference operator[](size_type n)
    {
        return begin()[static_cast<difference_type>(n)];
    }

    const_reference operator[](size_type n) const
    {
        return begin()[static_cast<difference_type>(n)];
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
        return *(--end());
    }

    const_reference back() const
    {
        return *(--end());
    }

    // 迭代器

    iterator begin() noexcept
    {
        return _start;
    }

    const_iterator begin() const noexcept
    {
        return _start;
    }

    const_iterator cbegin() const noexcept
    {
        return _start;
    }

    iterator end() noexcept
    {
        return _finish;
    }

    const_iterator end() const noexcept
    {
        return _finish;
    }

    const_iterator cend() const noexcept
    {
        return _finish;
    }

    // 容量

    /**
     * @brief 容器是否为空
     */
    bool empty() const noexcept
    {
        return _start == _finish;
    }

    /**
     * @brief 元素个数
     */
    size_type size() const noexcept
    {
        return _finish - _start;
    }

    /**
     * @brief 最大容量
     */
    size_type max_size() const noexcept
    {
        return std::numeric_limits<size_type>::max() / sizeof(value_type);
    }

    /**
     * @brief 释放不使用的空间
     */
    void shrink_to_fit()
    {
        // TODO
    }

    // 修改器

    /**
     * @brief 尾部插入元素
     */
    void push_back(const value_type & value)
    {
        if (_finish._cur != _finish._last - 1) {
            // 缓冲区还有空间，直接构造
            allocator.construct(_finish._cur, value);
            ++_finish._cur;
        } else {
            // 缓冲区已满，需要扩展，这里调用辅助函数实现
            __push_back_aux(value);
        }
    }

    /**
     * @brief 尾部插入元素
     */
    void push_back(value_type && value)
    {
        if (_finish._cur != _finish._last - 1) {
            // 缓冲区还有空间，直接构造
            allocator.construct(_finish._cur, std::move(value));
            ++_finish._cur;
        } else {
            // 缓冲区已满，需要扩展，这里调用辅助函数实现
            __push_back_aux(std::move(value));
        }
    }

    /**
     * @brief 头部插入元素
     */
    void push_front(const value_type & value)
    {
        if (_start._cur != _start._first) {
            // 缓冲区还有空间，直接构造
            allocator.construct(_start._cur - 1, value);
            --_start._cur;
        } else {
            // 缓冲区已满，需要扩展，这里调用辅助函数实现
            __push_front_aux(value);
        }
    }

    /**
     * @brief 头部插入元素
     */
    void push_front(value_type && value)
    {
        if (_start._cur != _start._first) {
            // 缓冲区还有空间，直接构造
            allocator.construct(_start._cur - 1, std::move(value));
            --_start._cur;
        } else {
            // 缓冲区已满，需要扩展，这里调用辅助函数实现
            __push_front_aux(std::move(value));
        }
    }

    void swap(deque & other)
    {
        std::swap(_start, other._start);
        std::swap(_finish, other._finish);
        std::swap(_map, other._map);
        std::swap(_map_size, other._map_size);
        // TODO，等写完allocator_traits再实现
        // if (allocator_type::propagate_on_container_swap::value) {
        //     std::swap(_alloc, other._alloc);
        // }
    }

protected:
    // 内部函数

    /**
     * @brief 计算节点大小
     */
    static size_type __deque_buffer_size()
    {
        // 设计为使用对象大小的8倍
        return 8 * sizeof(value_type);
    }

    /**
     * @brief 申请一个节点内存
     */
    pointer __allocate_node()
    {
        // 申请一个buffer_size()大小的内存
        return allocator.allocate(__deque_buffer_size());
    }

    /**
     * @brief 释放一个节点内存
     */
    void __deallocate_node(pointer p)
    {
        allocator.deallocate(p, __deque_buffer_size());
    }

    /**
     * @brief 在中控器的某个范围内申请节点内存
     */
    void __create_nodes(map_pointer first, map_pointer last)
    {
        for (map_pointer cur = first; cur != last; ++cur) {
            *cur = __allocate_node();
        }
    }

    /**
     * @brief 在中控器的某个范围内释放节点内存
     */
    void __destroy_nodes(map_pointer first, map_pointer last)
    {
        for (map_pointer cur = first; cur != last; ++cur) {
            __deallocate_node(*cur);
        }
    }

    /**
     * @brief 申请中控器所需内存
     * @param n 节点个数
     */
    void __allocate_map(size_type n)
    {
        _map = map_allocator.allocate(n);
    }

    /**
     * @brief 释放中控器内存
     */
    void __deallocate_map()
    {
        map_allocator.deallocate(_map, _map_size);
    }

    /**
     * @brief 初始化中控器
     * @param n 元素个数
     */
    void __initialize_map(size_type n)
    {
        // 计算需要的节点数量
        size_type node_count = n / __deque_buffer_size() + 1;
        // 额外申请2个节点，用于冗余，如果小于最小大小则设置为最小(8)
        _map_size = std::max(static_cast<size_type>(8), static_cast<size_type>(node_count + 2));
        __allocate_map(_map_size);
        // 初始化两个迭代器，选择中间是因为双端队列需要在两端操作
        // 经过这样设置，前端有一半的节点，后端至少有冗余的2个节点
        map_pointer start_node = _map + (_map_size - node_count) / 2;
        map_pointer finish_node = start_node + node_count;
        // 初始化范围内所有节点
        // 除了范围内的节点，其他冗余节点都没有申请内存
        __create_nodes(start_node, finish_node);
        // 设置中控器的起始和结束迭代器
        _start.set_node(start_node);
        _start._cur = _start._first;
        _finish.set_node(finish_node - 1);
        _finish._cur = _finish._first + (n % __deque_buffer_size());
    }

    /**
     * @brief 在尾部插入元素的辅助函数
     * @details 用于需要扩展的情况
     */
    template <typename... Args>
    void __push_back_aux(Args&&... args)
    {
        // 这里需要判断是否需要重新扩展中控器，如果还有剩余节点则直接构造并移动到下一个节点
        __reserve_map_at_back();
        // 不管是否扩展，下一个节点都是未申请内存的
        *(_finish._node + 1) = __allocate_node();
        // 在当前节点的最后一个位置构造元素
        allocator.construct(_finish._cur, std::forward<Args>(args)...);
        // 移动到下一个位置
        _finish.set_node(_finish._node + 1);
        _finish._cur = _finish._first;
    }

    /**
     * @brief 在头部插入元素的辅助函数
     * @details 用于需要扩展的情况
     */
    template <typename... Args>
    void __push_front_aux(Args&&... args)
    {
        __reserve_map_at_front();
        // 申请内存
        *(_start._node - 1) = __allocate_node();
        // 需要先移动到上一个节点的最后一个位置，再构造元素
        _start.set_node(_start._node - 1);
        _start._cur = _start._last - 1;
        allocator.construct(_start._cur, std::forward<Args>(args)...);
    }
    
    /**
     * @brief 在需要时于尾部扩展中控器
     */
    void __reserve_map_at_back(size_type n = 1)
    {
        // 如果后备的节点不够，则需要重新分配中控器
        if (n + 1 > _map_size - (_finish._node - _map)) {
            __reallocate_map(n, false);
        }
    }

    /**
     * @brief 在需要时于头部扩展中控器
     */
    void __reserve_map_at_front(size_type n = 1)
    {
        if (n > static_cast<size_type>(_start._node - _map)) {
            __reallocate_map(n, true);
        }
    }

    /**
     * @brief 重新分配中控器
     * @details 如果预留的节点足够，则只移动中控器中节点的位置，否则需要重新分配中控器内存
     */
    void __reallocate_map(size_type n, bool is_front)
    {
        const size_type old_num_nodes = _finish._node - _start._node + 1;
        const size_type new_num_nodes = old_num_nodes + n;
        map_pointer new_start;

        if (_map_size > 2 * new_num_nodes) {
            // 预留的节点足够，计算新的起始位置
            new_start = _map + (_map_size - new_num_nodes) / 2 + (is_front ? n : 0);
            if (new_start < _start._node) {
                // 需要向前移动
                std::copy(_start._node, _finish._node + 1, new_start);
            } else {
                // 向后移动
                std::copy_backward(_start._node, _finish._node + 1, new_start + old_num_nodes); // 注意第三个参数是结尾，因为要从后向前拷贝
            }
        } else {
            // 预留的节点不够，需要重新申请内存，并拷贝数据到新的中控器
            size_type new_map_size = _map_size + std::max(_map_size, n) + 2;    // 新空间至少是原来的两倍 + 2
            // 申请一块新内存供新中控器使用
            map_pointer new_map = map_allocator.allocate(new_map_size);
            new_start = new_map + (new_map_size - new_num_nodes) / 2 + (is_front ? n : 0);
            // 拷贝数据
            std::copy(_start._node, _finish._node + 1, new_start);
            // 释放旧中控器内存
            map_allocator.deallocate(_map, _map_size);
            // 设置新的中控器
            _map = new_map;
            _map_size = new_map_size;
        }

        // 设置新的起始和末尾迭代器
        _start.set_node(new_start);
        _finish.set_node(new_start + old_num_nodes - 1);
    }
};

// 非成员函数

template <class T, class Alloc>
bool operator==(const deque<T, Alloc> & lhs, const deque<T, Alloc> & rhs)
{
    return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <class T, class Alloc>
bool operator!=(const deque<T, Alloc> & lhs, const deque<T, Alloc> & rhs)
{
    return !(lhs == rhs);
}

template <class T, class Alloc>
bool operator<(const deque<T, Alloc> & lhs, const deque<T, Alloc> & rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <class T, class Alloc>
bool operator>(const deque<T, Alloc> & lhs, const deque<T, Alloc> & rhs)
{
    return rhs < lhs;
}

template <class T, class Alloc>
bool operator<=(const deque<T, Alloc> & lhs, const deque<T, Alloc> & rhs)
{
    return !(rhs < lhs);
}

template <class T, class Alloc>
bool operator>=(const deque<T, Alloc> & lhs, const deque<T, Alloc> & rhs)
{
    return !(lhs < rhs);
}

template <class T, class Alloc>
void swap(deque<T, Alloc> & lhs, deque<T, Alloc> & rhs)
{
    lhs.swap(rhs);
}

}

#endif