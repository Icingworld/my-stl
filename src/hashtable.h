#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__

#include "vector.h"
#include "hash.h"
#include "utility.h"

namespace stl
{

/**
 * 
 */
template <
    class Key,
    class Value,
    class Hash,
    class Equal,
    class ExtrctKey,
    class Allocator
>
class hashtable
{
protected:
    /**
     * @brief 哈希表节点类
     */
    class __hashtable_node
    {
    public:
        using key_type = Key;
        using value_type = Value;
        using self = __hashtable_node;
    
    public:
        key_type _key;
        value_type _value;
        self* _next;
    };

public:
    /**
     * @brief 哈希表迭代器
     */
    class __hashtable_iterator
    {
    public:
        using key_type = Key;
        using value_type = Value;
        using reference = value_type&;
        using const_reference = const value_type&;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;
        using iterator_category = std::forward_iterator_tag;

        using self = __hashtable_iterator;
        using node = __hashtable_node;
    
    public:
        node* _node;            // 指向当前节点
        hashtable* _hashtable;  // 指向所在的哈希表，用于向其他桶跳跃

    public:
        __hashtable_iterator()
            : _node(nullptr), _hashtable(nullptr)
        {}

        __hashtable_iterator(node* node, hashtable* hst)
            : _node(node), _hashtable(hst)
        {}

        ~__hashtable_iterator() = default;
    
        self& operator=(const self& other)
        {
            _node = other._node;
            _hashtable = other._hashtable;
            return *this;
        }

    public:
        reference operator*()
        {
            return _node->_value;
        }

        const_reference operator*() const
        {
            return _node->_value;
        }

        pointer operator->()
        {
            return &(operator*());
        }

        const_pointer operator->() const
        {
            return &(operator*());
        }

        self& operator++()
        {
            node * old = _node;
            _node = _node->_next;   // 尝试指向下一个节点
            if (_node == nullptr) {
                // 不存在下一个节点，需要跳转到下一个桶
                size_type index = _hashtable->bucket(old->_key) + 1;  // 下一个桶的索引，这是怎么算的，O(1)?
                while (index < _hashtable->bucket_count() && _hashtable->_bucket[index] == nullptr) {
                    ++index;
                }
                // 设置新的节点
                if (index < _hashtable->bucket_count()) {
                    _node = _hashtable->_bucket[index];
                }
                // 否则已经到达桶的末尾，保持为nullptr
            }
            return *this;
        }

        self operator++(int)
        {
            self tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const self& other) const
        {
            return _node == other._node;
        }

        bool operator!=(const self& other) const
        {
            return !(*this == other);
        }
    };

public:
    using key_type = Key;
    using value_type = Value;
    using size_type = std::size_t;
    using hasher = Hash;
    using node = __hashtable_node;
    using Alloc = Allocator;
    using Node_Alloc = typename Alloc::template rebind<__hashtable_node>::other;

protected:
    stl::vector<node *> _bucket;        // 哈希表桶
    node_allocator _node_allocator;     // 节点分配器
    size_type _hashtable_size;          // 哈希表的大小

public:
    hashtable()
    {}

    ~hashtable()
    {}

public:
    // 提供给高级数据结构的接口

    /**
     * @brief 桶的数量
     * @details 返回的是vector的大小
     */
    size_type bucket_count() const
    {
        return _bucket.size();
    }

    /**
     * @brief 桶的最大数量
     */
    size_type max_bucket_count() const
    {
        return _bucket.max_size();
    }

    /**
     * @brief 返回索引为n的桶的元素个数
     */
    size_type bucket_size(size_type n) const
    {
        return 0;
    }

    /**
     * @brief 返回key所在的桶的索引
     */
    size_type bucket(const key_type& key) const
    {
        return 0;
    }
};

} // namespace stl

#endif