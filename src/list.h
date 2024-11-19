#ifndef __LIST_H__
#define __LIST_H__

namespace stl
{

template <typename T>
class list
{
public:
    using value_type = T;
    using pointer = value_type*;
    using reference = value_type&;
    using const_pointer = const value_type*;
    using const_reference = const value_type&;

private:
    /**
     * @brief 节点
     */
    class node
    {
    public:
        using node_pointer = node*;
    
    public:
        value_type data;
        node_pointer prev;
        node_pointer next;
    
    public:
        node()
            : prev(nullptr), next(nullptr) 
        {}
        node(const value_type & data)
            : data(data), prev(nullptr), next(nullptr)
        {}
        node(value_type && data)
            : data(std::move(data)), prev(nullptr), next(nullptr)
        {}
    };

    using node_pointer = typename node::node_pointer;

    node_pointer head;  // 哨兵节点，不存储数据
    node_pointer tail;  // 哨兵节点
    int _size;

public:
    /**
     * @brief 迭代器
     */
    class iterator
    {
    private:
        friend class list;
        node_pointer _node; // 当前节点
    public:
        iterator()
            : _node(nullptr)
        {}
        iterator(node_pointer node)
            : _node(node)
        {}
        iterator(const iterator & other)
            : _node(other._node)
        {}
        iterator(iterator && other)
            : _node(std::move(other._node))
        {}
        ~iterator()
        {}
    
    public:
        iterator & operator=(const iterator & other)
        {
            if (this != &other)
                _node = other._node;
            return *this;
        }
        iterator & operator=(iterator && other)
        {
            if (this != &other)
                _node = std::move(other._node);
            return *this;
        }

        /**
         * @brief 解引用
         */
        reference operator*()
        {
            return _node->data;
        }
        /**
         * @brief 解引用
         */
        pointer operator->()
        {
            return &(_node->data);
        }

        /**
         * @brief 等于运算符
         */
        bool operator==(const iterator & other) const
        {
            return _node == other._node;
        }
        /**
         * @brief 不等于运算符
         */
        bool operator!=(const iterator & other) const
        {
            return _node != other._node;
        }

        /**
         * @brief 前置递增
         */
        iterator & operator++()
        {
            _node = _node->next;
            return *this;
        }
        /**
         * @brief 后置递增
         */
        iterator operator++(int)
        {
            iterator tmp = *this;
            ++(*this);              // 改变当前迭代器指向的节点
            return tmp;             // 返回前置递增前的迭代器
        }

        /**
         * @brief 前置递减
         */
        iterator & operator--()
        {
            _node = _node->prev;
            return *this;
        }

        /**
         * @brief 后置递减
         */
        iterator operator--(int)
        {
            iterator tmp = *this;
            --(*this);
            return tmp;
        }
    };

public:
    list()
        : head(new node), tail(new node), _size(0)
    {
        head->next = tail;
        tail->prev = head;
    }

    ~list()
    {
        clear();
        delete head;
        delete tail;
    }

    /**
     * @brief 返回第一个元素
     */
    reference front()
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
     * @brief 返回指向第一个元素的迭代器
     */
    iterator begin()
    {
        return iterator(head->next);
    }

    /**
     * @brief 返回指向最后一个元素后一个节点的迭代器
     */
    iterator end()
    {
        return iterator(tail);
    }

    bool empty() const
    {
        return _size == 0;
    }

    size_t size() const
    {
        return _size;
    }

    /**
     * @brief 清空链表
     */
    void clear()
    {
        while (!empty()) {
            pop_front();
        }
    }

    /**
     * @brief 删除指定位置的节点
     */
    iterator erase(iterator pos)
    {
        iterator tmp = pos;
        ++tmp;
        pos._node->prev->next = pos._node->next;
        pos._node->next->prev = pos._node->prev;
        delete pos._node;
        --_size;
        return tmp;
    }

    /**
     * @brief 在指定位置插入节点
     */
    void insert(iterator position, const_reference data)
    {
        insert_after(position, data);
    }
    void insert(iterator position, value_type && data)
    {
        insert_after(position, std::move(data));
    }

    /**
     * @brief 在链表头部插入节点
     */
    template <typename U>
    void push_front(U && data)
    {
        insert_after(--begin(), std::forward<U>(data));
    }

    /**
     * @brief 在链表尾部插入节点
     */
    template <typename U>
    void push_back(U && data)
    {
        insert_after(--end(), std::forward<U>(data));
    }

    /**
     * @brief 删除链表头部节点
     */
    void pop_front()
    {
        // 判断是否为空
        if (_size == 0) {
            return;
        }
        erase(begin());
    }

    /**
     * @brief 删除链表尾部节点
     */
    void pop_back()
    {
        // 判断是否为空
        if (_size == 0) {
            return;
        }
        erase(--end());
    }

    void swap(list & other)
    {
        std::swap(head, other.head);
        std::swap(tail, other.tail);
        std::swap(_size, other._size);
    }

private:
    /**
     * @brief  在pos之后插入节点
     */
    template <typename U>
    void insert_after(iterator pos, U && data)
    {
        node_pointer new_node = new node(std::forward<U>(data));
        new_node->next = pos._node->next;
        new_node->prev = pos._node;
        pos._node->next->prev = new_node;
        pos._node->next = new_node;
        ++_size;
    }

};

}

#endif