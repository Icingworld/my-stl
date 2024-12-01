#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__

#include <stdexcept>
#include "vector.h"
#include "hash.h"
#include "utility.h"

namespace stl
{

template <class T>
class keyExtractor;

template <typename T, typename U>
class keyExtractor<stl::pair<const T, U>>
{
public:
    const T & operator()(const stl::pair<const T, U> & p) const
    {
        return p.first;
    }
};

/**
 * 
 */
template <
    class Key,
    class Value,
    class Hash,
    class Equal,
    class ExtractKey,
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
        using value_type = stl::pair<const Key, Value>;
        using self = __hashtable_node;
    
    public:
        value_type _value;
        self* _next;
    
    public:
        __hashtable_node(const value_type & value)
            : _value(value), _next(nullptr)
        {}
    };

public:
    /**
     * @brief 哈希表迭代器
     */
    class __hashtable_iterator
    {
    public:
        using value_type = stl::pair<const Key, Value>;
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
                while (index < _hashtable->bucket_count() && _hashtable->_buckets[index] == nullptr) {
                    ++index;
                }
                // 设置新的节点
                if (index < _hashtable->bucket_count()) {
                    _node = _hashtable->_buckets[index];
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

    class __hashtable_local_iterator
    {

    };

public:
    using key_type = Key;
    using mapped_type = Value;
    using value_type = stl::pair<const Key, Value>;
    using size_type = std::size_t;
    using hasher = Hash;
    using key_equal = Equal;
    using extract_key = ExtractKey;
    using node = __hashtable_node;
    using Alloc = Allocator;
    using iterator = __hashtable_iterator;
    using const_iterator = const __hashtable_iterator;
    using local_iterator = __hashtable_local_iterator;
    using const_local_iterator = const __hashtable_local_iterator;
    using Node_Alloc = typename Alloc::template rebind<__hashtable_node>::other;

protected:
    stl::vector<node *> _buckets;       // 哈希表桶
    Node_Alloc _node_allocator;         // 节点分配器
    size_type _hashtable_elements;      // 哈希表的元素个数
    hasher _hash;                       // 哈希函数
    key_equal _equal;                   // 比较函数
    extract_key _extract_key;           // 提取key的函数

public:
    hashtable(size_type n, const hasher & hash, const key_equal & equal)
        : _hashtable_elements(0), _hash(hash), _equal(equal), _extract_key(extract_key())   // key提取函数和键值对类型绑定，无法更改，所以不能传入
    {
        _initialize_buckets(n);
    }

    ~hashtable()
    {}

public:
    iterator begin()
    {

    }

    const_iterator begin() const
    {

    }

    iterator end()
    {

    }

    const_iterator end() const
    {

    }

    /**
     * @brief 不允许重复的插入
     */
    stl::pair<iterator, bool> insert_unique(const value_type & value)
    {
        resize(_hashtable_elements + 1);        // 判断是否需要重建哈希表
        return _insert_unique_noresize(value);
    }

    /**
     * @brief 允许重复的插入
     */
    iterator insert_equal(const value_type & value)
    {
        resize(_hashtable_elements + 1);
        return _insert_equal_noresize(value);
    }

    /**
     * @brief 根据指定的元素个数判断是否需要重建哈希表
     */
    void resize(size_type hashtable_elements)
    {
        // 根据《STL源码剖析》，判断方法是当哈希表中的元素个数大于桶的大小，就重建
        const size_type old_elements = _buckets.size();
        if (hashtable_elements > old_elements) {
            // 重建哈希表
            const size_type new_size = hashtable_elements * 2;
            // TODO 等vector功能完善再修改
            // stl::vector<node *> new_buckets(new_size, nullptr);
            stl::vector<node *> new_buckets;
            new_buckets.reserve(new_size);
            new_buckets.insert(new_buckets.end(), new_size, nullptr);
            // 遍历所有桶，拿出节点存放到新的桶中
            for (size_type i = 0; i < old_elements; ++i) {
                node * old_first = _buckets[i];
                while (old_first != nullptr) {
                    size_type new_index = _hash_key(old_first->_value, new_size);
                    _buckets[i] = old_first->_next;  // 旧桶指向下一个节点
                    // 插入到新桶
                    old_first->_next = new_buckets[new_index];
                    new_buckets[new_index] = old_first;
                    // 继续处理下一个节点
                    old_first = _buckets[i];
                }
            }
            // 交换桶
            _buckets.swap(new_buckets);
        }
    }

    /**
     * @brief 清空哈希表
     */
    void clear()
    {
        for (size_type i = 0; i < _buckets.size(); ++i) {
            node * first = _buckets[i];
            while (first != nullptr) {
                node * next = first->_next;
                _node_allocator.destroy(first);
                _node_allocator.deallocate(first, 1);
                first = next;
            }
            _buckets[i] = nullptr;
        }
    }

    /**
     * @brief 查找key
     * @details 这里需要更改一下，提高复用性
     */
    iterator find(const key_type & key)
    {
        size_type index = _hash_key(key);
        node * first = _buckets[index];
        while (first != nullptr) {
            if (_equal(_extract_key(first->_value), key)) {
                return iterator(first, this);
            }
            first = first->_next;
        }
        return end();
    }

    const_iterator find(const key_type & key) const
    {
        size_type index = _hash_key(key);
        node * first = _buckets[index];
        while (first != nullptr) {
            if (_equal(_extract_key(first->_value), key)) {
                return iterator(first, this);
            }
            first = first->_next;
        }
        return end();
    }

    mapped_type & at(const key_type & key)
    {
        iterator it = find(key);
        if (it != end()) {
            return it->second;
        }
        throw std::out_of_range("key not found");
    }

    const mapped_type & at(const key_type & key) const
    {
        iterator it = find(key);
        if (it != end()) {
            return it->second;
        }
        throw std::out_of_range("key not found");
    }

    mapped_type & operator[](const key_type & key)
    {
        auto result = insert_unique(value_type(key, mapped_type()));    // 尝试插入，不管是否插入成功，都返回一个迭代器
        return result.first->second;
    }

    const mapped_type & operator[](const key_type & key) const
    {
        auto result = insert_unique(value_type(key, mapped_type()));
        return result.first->second;
    }

    /**
     * @brief 返回键值为key的元素个数
     */
    size_type count(const key_type & key) const
    {
        size_type index = _hash_key(key);
        node * first = _buckets[index];
        size_type _count = 0;
        while (first != nullptr) {
            if (_equal(_extract_key(first->_value), key)) {
                ++_count;
            }
            first = first->_next;
        }
        return _count;
    }

    /**
     * @brief 桶的数量
     * @details 返回的是vector的大小
     */
    size_type bucket_count() const
    {
        return _buckets.size();
    }

    /**
     * @brief 桶的最大数量
     */
    size_type max_bucket_count() const
    {
        return _buckets.max_size();
    }

    /**
     * @brief 返回索引为n的桶的元素个数
     */
    size_type bucket_size(size_type n) const
    {
        node * first = _buckets[n];
        size_type i = 0;
        for (; first != nullptr; ++i, first = first->next) {
            ++n;
        }
        return i;
    }

    /**
     * @brief 返回key所在的桶的索引
     */
    size_type bucket(const key_type& key) const
    {
        return _hash_key(key);
    }

    hasher hash_function() const
    {
        return _hash;
    }

    key_equal key_eq() const
    {
        return _equal;
    }

protected:
    // 内部函数

    /**
     * @brief 创建一个节点
     */
    node * _create_node(const value_type & value)
    {
        node * new_node = _node_allocator.allocate(1);
        _node_allocator.construct(new_node, value);
        return new_node;
    }

    /**
     * @brief 销毁一个节点
     */
    void _destroy_node(node * node)
    {
        _node_allocator.destroy(node);
        _node_allocator.deallocate(node, 1);
    }

    /**
     * @brief 初始化桶
     */
    void _initialize_buckets(size_type buckets_size)
    {
        // 计算需要多大的桶
        // TODO
        _buckets.reserve(buckets_size);
        _buckets.insert(_buckets.end(), buckets_size, nullptr);
    }

    /**
     * @brief 计算key所在的桶
     * @param key 键
     * @param n 桶的数量
     */
    size_type _hash_key(const key_type & key, size_type n) const
    {
        // 计算模
        return _hash(key) % n;
    }

    /**
     * @brief 计算key所在的桶
     * @param key 键
     */
    size_type _hash_key(const key_type & key) const
    {
        return _hash_key(key, _buckets.size());
    }

    /**
     * @brief 计算pair<key, value>所在的桶
     * @param value 键值对
     */
    size_type _hash_key(const value_type & value) const
    {
        return _hash_key(_extract_key(value), _buckets.size());
    }

    /**
     * @brief 计算pair<key, value>所在的桶
     * @param value 键值对
     * @param n 桶的数量
     */
    size_type _hash_key(const value_type & value, size_type n) const
    {
        return _hash_key(_extract_key(value), n);
    }

    /**
     * @brief 无需重建的不重复插入
     */
    stl::pair<iterator, bool> _insert_unique_noresize(const value_type & value)
    {
        // 计算key所在的桶
        size_type index = _hash_key(value);
        // 寻找插入点
        node * first = _buckets[index];
        for (node * cur = first; cur != nullptr; cur = cur->_next) {
            if (_equal(_extract_key(cur->_value), _extract_key(value))) {
                // 键已经存在，插入失败
                return stl::make_pair<iterator, bool>(iterator(cur, this), false);
            }
        }
        // 找到的话first就是插入点
        node * new_node = _create_node(value);  // 创建节点
        new_node->_next = first;
        _buckets[index] = new_node;
        ++_hashtable_elements;
        return stl::make_pair<iterator, bool>(iterator(new_node, this), true);
    }

    /**
     * @brief 无需重建的可重复插入
     */
    iterator _insert_equal_noresize(const value_type & value)
    {
        size_type index = _hash_key(value);
        node * new_node = _create_node(value);
        node * first = _buckets[index];
        for (node * cur = first; cur != nullptr; cur = cur->_next) {
            if (_equal(_extract_key(cur->_value), _extract_key(value))) {
                // 找到键，直接在此插入
                new_node->_next = cur->_next;
                cur->_next = new_node;
                ++_hashtable_elements;
                return iterator(new_node, this);
            }
        }
        // 没有找到，直接在头部插入
        new_node->_next = first;
        _buckets[index] = new_node;
        ++_hashtable_elements;
        return iterator(new_node, this);
    }
};

} // namespace stl

#endif