#ifndef __DEQUE_H__
#define __DEQUE_H__

#include <stdexcept>
#include <limits>
#include <initializer_list>
#include "memory.h"

#include <iostream>

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

        __deque_iterator(const self & other)
            : _cur(other._cur), _first(other._first), _last(other._last), _node(other._node)
        {}

        self & operator=(const self & other)
        {
            _cur = other._cur;
            _first = other._first;
            _last = other._last;
            _node = other._node;
            return *this;
        }

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
         * @brief 设置当前迭代器指向的缓冲区
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

protected:
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
     * @brief 清空双端队列
     * @details 和标准库中的实现不太一样
     */
    void clear()
    {
        // 暂存迭代器位置
        map_pointer old_start = _start._node;
        map_pointer old_finish = _finish._node;
        // 删除所有元素
        while (!empty())
        {
            pop_back();
        }
        // 释放所有的缓冲区
        __destroy_nodes(old_start, old_finish + 1);
        // 释放中控器
        map_allocator.deallocate(_map, _map_size);
        // 重新初始化中控器
        __initialize_map(0);
    }

    /**
     * @brief 在指定位置插入元素
     */
    iterator insert(const_iterator pos, const value_type& value)
    {
        if (pos == begin()) {
            push_front(value);
            return begin();
        } else if (pos == end()) {
            push_back(value);
            return end() - 1;
        } else {
            return __insert_aux(pos, value);
        }
    }

    /**
     * @brief 在指定位置插入元素
     */
    iterator insert(const_iterator pos, value_type&& value)
    {
        emplace(pos, std::move(value));
    }

    /**
     * @brief 在指定位置插入多个元素
     */
    iterator insert(const_iterator pos, size_type count, const value_type& value)
    {
        difference_type offset = pos - cbegin();
        __fill_insert(pos, count, value);
        return begin() + offset;
    }

    /**
     * @brief 在指定位置插入迭代器范围内的元素
     */
    // template <class InputIterator>
    // iterator insert(const_iterator pos, InputIterator first, InputIterator last)
    // {

    // }

    /**
     * @brief 在指定位置插入initializer_list
     */
    // iterator insert(const_iterator pos, std::initializer_list<value_type> ilist)
    // {
    //     return insert(pos, ilist.begin(), ilist.end());
    // }

    /**
     * @brief 在指定位置构造元素
     */
    template <class... Args>
    iterator emplace(const_iterator pos, Args&&... args)
    {
        if (pos == begin()) {
            // 这段代码和push_front的实现是一样的
            if (_start._cur != _start._first) {
                allocator.construct(_start._cur - 1, std::forward<Args>(args)...);
                --_start._cur;
            } else {
                __push_front_aux(std::forward<Args>(args)...);
            }
            return begin();
        } else if (pos == end()) {
            if (_finish._cur != _finish._last - 1) {
                allocator.construct(_finish._cur, std::forward<Args>(args)...);
                ++_finish._cur;
            } else {
                __push_back_aux(std::forward<Args>(args)...);
            }
            return end() - 1;
        } else {
            return __insert_aux(pos, std::forward<Args>(args)...);
        }
    }

    /**
     * @brief 删除指定位置的元素
     */
    iterator erase(const_iterator pos)
    {
        iterator next = pos;
        ++next;
        // 判断移动前面的元素还是后面的元素
        difference_type nums_before = pos - _start;
        if (nums_before < static_cast<difference_type>(size() / 2)) {
            // 移动前面的元素
            std::copy_backward(_start, pos, next);
            pop_front();
        } else {
            // 移动后面的元素
            std::copy(next, _finish, pos);
            pop_back();
        }
        // 所有迭代器都会失效，需要重新计算
        return _start + nums_before;
    }

    /**
     * @brief 删除指定范围的元素
     */
    iterator erase(const_iterator first, const_iterator last)
    {
        // 判断移动前面的元素还是后面的元素
        difference_type n = last - first;
        difference_type nums_before = first - _start;       // 前面的元素个数
        if (nums_before < static_cast<difference_type>(size() - n / 2)) {
            // 移动前面的元素
            std::copy_backward(_start, first, last);
            // 销毁前面的元素
            for (size_type i = 0; i < n; ++i) {
                allocator.destroy((_start + i)._cur);
            }
            // 释放前面的缓冲区
            __destroy_nodes(_start._node, (_start + n)._node);  // 注意_start + n是新的起点缓冲区，不能被释放
            // 设置新的起点
            _start = _start + n;
        } else {
            // 移动后面的元素
            std::copy(last, _finish, first);
            // 销毁后面的元素
            for (size_type i = 0; i < n; ++i) {
                allocator.destroy((_finish - i - 1)._cur);
            }
            // 释放后面的缓冲区
            __destroy_nodes((_finish - n + 1)._node, (_finish + 1)._node);
            // 设置新的终点
            _finish = _finish - n;
        }
        return _start + nums_before;
    }

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
     * @brief 尾部构造元素
     */
    template <class... Args>
    void emplace_back(Args&&... args)
    {
        emplace(_finish, std::forward<Args>(args)...);
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

    template <class... Args>
    void emplace_front(Args&&... args)
    {
        emplace(_start, std::forward<Args>(args)...);
    }

    /**
     * @brief 删除尾部元素
     */
    void pop_back()
    {
        if (_finish._cur != _finish._first) {
            // 当前位置不在缓冲区起始位置
            --_finish._cur;
            allocator.destroy(_finish._cur);
        } else {
            // 在起始位置，需要跳跃到前一个缓冲区
            __pop_back_aux();
        }
    }

    /**
     * @brief 删除头部元素
     */
    void pop_front()
    {
        if (_start._cur != _start._last - 1) {
            // 当前位置不在缓冲区末尾位置
            allocator.destroy(_start._cur);
            ++_start._cur;
        } else {
            // 在末尾位置，需要跳跃到后一个缓冲区
            __pop_front_aux();
        }
    }

    /**
     * @brief 交换双端队列
     */
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
     * @brief 计算缓冲区大小
     */
    static size_type __deque_buffer_size()
    {
        // 设计为使用对象大小的8倍
        return 8 * sizeof(value_type);
    }

    /**
     * @brief 申请一个缓冲区内存
     */
    pointer __allocate_node()
    {
        // 申请一个buffer_size()大小的内存
        return allocator.allocate(__deque_buffer_size());
    }

    /**
     * @brief 释放一个缓冲区内存
     */
    void __deallocate_node(pointer p)
    {
        allocator.deallocate(p, __deque_buffer_size());
    }

    /**
     * @brief 在中控器的某个范围内申请缓冲区内存
     */
    void __create_nodes(map_pointer first, map_pointer last)
    {
        for (map_pointer cur = first; cur != last; ++cur) {
            *cur = __allocate_node();
        }
    }

    /**
     * @brief 在中控器的某个范围内释放缓冲区内存
     */
    void __destroy_nodes(map_pointer first, map_pointer last)
    {
        for (map_pointer cur = first; cur != last; ++cur) {
            __deallocate_node(*cur);
        }
    }

    /**
     * @brief 申请中控器所需内存
     * @param n 缓冲区个数
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
        // 计算需要的缓冲区数量
        size_type node_count = n / __deque_buffer_size() + 1;
        // 额外申请2个缓冲区，用于冗余，如果小于最小大小则设置为最小(8)
        _map_size = std::max(static_cast<size_type>(8), static_cast<size_type>(node_count + 2));
        __allocate_map(_map_size);
        // 初始化两个迭代器，选择中间是因为双端队列需要在两端操作
        // 经过这样设置，前端有一半的缓冲区，后端至少有冗余的2个缓冲区
        map_pointer start_node = _map + (_map_size - node_count) / 2;
        map_pointer finish_node = start_node + node_count;
        // 初始化范围内所有缓冲区
        // 除了范围内的缓冲区，其他冗余缓冲区都没有申请内存
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
    template <class... Args>
    void __push_back_aux(Args&&... args)
    {
        // 这里需要判断是否需要重新扩展中控器，如果还有剩余缓冲区则直接构造并移动到下一个缓冲区
        __reserve_map_at_back();
        // 不管是否扩展，下一个缓冲区都是未申请内存的
        *(_finish._node + 1) = __allocate_node();
        // 在当前缓冲区的最后一个位置构造元素
        allocator.construct(_finish._cur, std::forward<Args>(args)...);
        // 移动到下一个位置
        _finish.set_node(_finish._node + 1);
        _finish._cur = _finish._first;
    }

    /**
     * @brief 在头部插入元素的辅助函数
     * @details 用于需要扩展的情况
     */
    template <class... Args>
    void __push_front_aux(Args&&... args)
    {
        __reserve_map_at_front();
        // 申请内存
        *(_start._node - 1) = __allocate_node();
        // 需要先移动到上一个缓冲区的最后一个位置，再构造元素
        _start.set_node(_start._node - 1);
        _start._cur = _start._last - 1;
        allocator.construct(_start._cur, std::forward<Args>(args)...);
    }
    
    /**
     * @brief 在需要时于尾部扩展中控器
     */
    void __reserve_map_at_back(size_type n = 1)
    {
        // 如果后备的缓冲区不够，则需要重新分配中控器
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
     * @details 如果预留的缓冲区足够，则只移动中控器中缓冲区的位置，否则需要重新分配中控器内存
     */
    void __reallocate_map(size_type n, bool is_front)
    {
        const size_type old_num_nodes = _finish._node - _start._node + 1;
        const size_type new_num_nodes = old_num_nodes + n;
        map_pointer new_start;

        if (_map_size > 2 * new_num_nodes) {
            // 预留的缓冲区足够，计算新的起始位置
            new_start = _map + (_map_size - new_num_nodes) / 2 + (is_front ? n : 0);
            if (new_start < _start._node) {
                // 需要向前移动
                std::copy(_start._node, _finish._node + 1, new_start);
            } else {
                // 向后移动
                std::copy_backward(_start._node, _finish._node + 1, new_start + old_num_nodes); // 注意第三个参数是结尾，因为要从后向前拷贝
            }
        } else {
            // 预留的缓冲区不够，需要重新申请内存，并拷贝数据到新的中控器
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

    /**
     * @brief 插入元素的辅助函数
     * @details 先腾出1个元素的内存，再插入元素
     */
    template <class... Args>
    iterator __insert_aux(iterator pos, Args&&... args)
    {
        value_type x(std::forward<Args>(args)...);
        // 判断移动前面的元素还是后面的元素
        difference_type nums_before = pos - _start;
        if (nums_before < static_cast<difference_type>(size() / 2)) {
            // 移动前面的元素，先在头部插入一个和当前头部一样的元素
            push_front(std::move(front()));
            // 设置标号
            iterator front1 = _start;
            ++front1;
            iterator front2 = front1;
            ++front2;
            pos = _start + nums_before;
            iterator pos1 = pos;
            ++pos1;
            // 拷贝内存
            std::copy(front2, pos1, front1);
        } else {
            // 移动后面的元素
            push_back(std::move(back()));
            // 设置标号
            iterator back1 = _finish;
            --back1;
            iterator back2 = back1;
            --back2;
            pos = _start + nums_before;
            // 拷贝内存
            std::copy_backward(pos, back2, back1);
        }
        // 移动元素
        *pos = std::move(x);
        return pos;
    }

    /**
     * @brief 插入n个元素的辅助函数
     * @details 如果在begin处或者end处插入，直接扩展并插入，否则调用__insert_aux
     */
    void __fill_insert(iterator pos, size_type n, const value_type& x)
    {
        if (pos == _start) {
            // 在头部插入
            __reserve_map_at_front(n);
            // STL有更好的实现，需要memory中的辅助工具，还没有实现
            for (size_type i = 0; i < n; ++i) {
                --_start;                               // 先移动起点
                allocator.construct(_start._cur, x);    // 构造新元素
            }
        } else if (pos == _finish) {
            // 在尾部插入
            __reserve_map_at_back(n);
            for (size_type i = 0; i < n; ++i) {
                allocator.construct(_finish._cur, x);   // 构造新元素
                ++_finish;                              // 移动终点
            }
        } else {
            __insert_aux(pos, n, x);
        }
    }

    /**
     * @brief 插入元素的辅助函数
     * @details 先腾出n个元素的内存，再插入n个元素
     */
    template <class... Args>
    iterator __insert_aux(iterator pos, size_type n, Args&&... args)
    {
        value_type x(std::forward<Args>(args)...);
        difference_type nums_before = pos - _start;

        if (nums_before < static_cast<difference_type>(size() / 2)) {
            // 前半部分插入
            __reserve_map_at_front(n / __deque_buffer_size() + 1);  // 扩展中控器，传入缓冲区个数，而不是元素个数
            iterator old_start = _start;
            // 不能在这里计算new_start，因为前面的区域未分配，会发生未定义行为
            pos = old_start + nums_before;
            __create_nodes((old_start - n)._node, old_start._node);  // 初始化缓冲区
            // 移动旧元素，因为向前移动且可能出现重叠，必须使用std::copy
            // 参见https://zh.cppreference.com/w/cpp/algorithm/copy
            iterator new_start = old_start - n;
            std::copy(old_start, pos, new_start);
            // 插入新的元素
            std::fill(pos - n, pos, x);
            _start = new_start; // 更新起始点
        } else {
            // 后半部分插入
            __reserve_map_at_back(n / __deque_buffer_size() + 1);  // 扩展中控器
            iterator old_finish = _finish;
            __create_nodes(old_finish._node + 1, (old_finish + n)._node + 1);  // 初始化缓冲区
            // 移动旧元素，只能用std::copy_backward
            // 参见https://zh.cppreference.com/w/cpp/algorithm/copy_backward
            iterator new_finish = old_finish + n;
            std::copy_backward(pos, old_finish, new_finish);
            // 插入新的元素
            std::fill(pos, pos + n, x);
            _finish = new_finish; // 更新结束点
        }
        return _start + nums_before; // 返回新插入元素的起始位置
    }

    /**
     * @brief 在需要跳转时于尾部删除元素
     */
    void __pop_back_aux()
    {
        // 先移动到上一个缓冲区
        _finish.set_node(_finish._node - 1);
        _finish._cur = _finish._last - 1;
        allocator.destroy(_finish._cur);
    }

    /**
     * @brief 在需要跳转时于头部删除元素
     */
    void __pop_front_aux()
    {
        // 先销毁元素再跳转
        allocator.destroy(_start._cur);
        _start.set_node(_start._node + 1);
        _start._cur = _start._first;
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