#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__

#include <stdexcept>
#include <cmath>
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
        using const_self = const __hashtable_iterator;
        using node = __hashtable_node;
    
    public:
        node* _node;            // 指向当前节点
        hashtable* _hashtable;  // 指向所在的哈希表，用于向其他桶跳跃

    public:

        __hashtable_iterator()
            : _node(nullptr), _hashtable(nullptr)
        {}

        __hashtable_iterator(node* nd, hashtable* hst)
            : _node(nd), _hashtable(hst)
        {}

        virtual ~__hashtable_iterator() = default;
    
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
                size_type index = _hashtable->_hash_key(old->_value) + 1;  // 下一个桶的索引，这是怎么算的，O(1)?
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

        const_self& operator++() const
        {
            ++(*const_cast<self*>(this));   // 这。。。
            return *this;
        }

        const_self operator++(int) const
        {
            const_self tmp = *this;
            ++(*this);
            return tmp;
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
        : public __hashtable_iterator
    {
    public:
        using self = __hashtable_local_iterator;
        using const_self = const __hashtable_local_iterator;
        using node = __hashtable_node;

    public:
        __hashtable_local_iterator()
            : __hashtable_iterator()
        {}

        __hashtable_local_iterator(node* nd, hashtable* hst)
            : __hashtable_iterator(nd, hst)
        {}

        ~__hashtable_local_iterator() = default;

    public:
        self& operator++()
        {
            __hashtable_iterator::_node = __hashtable_iterator::_node->_next;   // 尝试指向下一个节点
            return *this;   // 迭代器是否相等只取决于node，所以如果_node == nullptr，那么一定和end()相等
        }

        const_self& operator++() const
        {
            ++(*const_cast<self*>(this));
            return *this;
        }

        self operator++(int)
        {
            self tmp = *this;
            ++(*this);
            return tmp;
        }

        const_self operator++(int) const
        {
            const_self tmp = *this;
            ++(*this);
            return tmp;
        }
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
    float _max_load_factor;             // 最大负载因子

public:
    hashtable(size_type n, const hasher & hash, const key_equal & equal)
        : _hashtable_elements(0), _hash(hash), _equal(equal), _extract_key(extract_key()), _max_load_factor(1.0)   // key提取函数和键值对类型绑定，无法更改，所以不能传入
    {
        _initialize_buckets(n);
    }

    ~hashtable()
    {}

public:
    // 迭代器

    /**
     * @brief 返回第一个有效元素的迭代器
     */
    iterator begin()
    {
        for (size_type index = 0; index < _buckets.size(); ++index) {
            if (_buckets[index] != nullptr) {
                return iterator(_buckets[index], this);
            }
        }
        return end();
    }

    const_iterator begin() const
    {
        for (size_type index = 0; index < _buckets.size(); ++index) {
            if (_buckets[index] != nullptr) {
                return const_iterator(_buckets[index], this);
            }
        }
        return end();
    }

    const_iterator cbegin() const
    {
        return begin();
    }

    /**
     * @brief 返回一个空的迭代器
     */
    iterator end()
    {
        return iterator();
    }

    const_iterator end() const
    {
        return const_iterator();
    }

    const_iterator cend() const
    {
        return end();
    }

    // 容量

    bool empty() const
    {
        return _hashtable_elements == 0;
    }

    size_type size() const
    {
        return _hashtable_elements;
    }

    size_type max_size() const
    {
        return std::numeric_limits<size_type>::max();
    }

    // 修改器

    /**
     * @brief 不允许重复的插入
     */
    stl::pair<iterator, bool> insert_unique(const value_type & value)
    {
        _resize(_hashtable_elements + 1);        // 判断是否需要重建哈希表
        return _insert_unique_noresize(value);
    }

    /**
     * @brief 允许重复的插入
     */
    iterator insert_equal(const value_type & value)
    {
        _resize(_hashtable_elements + 1);
        return _insert_equal_noresize(value);
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
     * @brief 删除元素
     */
    iterator erase(const_iterator pos)
    {
        if (pos == end()) {
            return end();
        }
        return erase(pos, std::next(pos));
    }

    /**
     * @brief 删除指定范围的元素
     * @details 相当丑陋
     */
    iterator erase(const_iterator first, const_iterator last)
    {
        if (first == last) {
            return end();
        }

        size_type first_index = _hash_key(*first);
        size_type last_index = -1;
        if (last != cend())
            last_index = _hash_key(*last);
        node* cur = first._node;

        // 同一个桶
        if (first_index == last_index || last == cend()) {
            node* pre = nullptr;
            while (cur != last._node) {
                node* next = cur->_next;
                if (pre == nullptr) {
                    _buckets[first_index] = next; // 更新桶头指针
                } else {
                    pre->_next = next; // 更新链表指针
                }
                _destroy_node(cur);
                --_hashtable_elements;
                cur = next;
            }
            return iterator(last._node, this);
        }

        // 跨桶删除
        for (size_type i = first_index; i <= last_index; ++i) {
            node* pre = nullptr;
            node* current = _buckets[i];

            if (i == first_index) {
                // 第一个桶：删除从 first 到桶尾
                while (current && current != last._node) {
                    node* next = current->_next;
                    if (current == cur) {
                        _buckets[i] = next; // 更新桶头
                    } else {
                        pre->_next = next; // 更新链表指针
                    }
                    _destroy_node(current);
                    --_hashtable_elements;
                    current = next;
                }
                cur = nullptr; // 标记后续不需要处理起始节点
            } else if (i == last_index) {
                // 最后一个桶：删除到 last 节点之前
                while (current && current != last._node) {
                    node* next = current->_next;
                    if (pre == nullptr) {
                        _buckets[i] = next;
                    } else {
                        pre->_next = next;
                    }
                    _destroy_node(current);
                    --_hashtable_elements;
                    current = next;
                }
            } else {
                // 中间桶：清空整个桶
                while (current) {
                    node* next = current->_next;
                    _destroy_node(current);
                    --_hashtable_elements;
                    current = next;
                }
                _buckets[i] = nullptr; // 清空桶头指针
            }
        }

        return iterator(last._node, this);
    }


    /**
     * @brief 删除指定key的元素
     */
    size_type erase(const key_type & key)
    {
        size_type n = count(key);
        auto pair = equal_range(key);
        erase(pair.first, pair.second);
        return n;
    }

    /**
     * @brief 交换内容
     */
    void swap(hashtable & other)
    {
        std::swap(_buckets, other._buckets);
        std::swap(_node_allocator, other._node_allocator);
        std::swap(_hashtable_elements, other._hashtable_elements);
        std::swap(_hash, other._hash);
        std::swap(_equal, other._equal);
        std::swap(_extract_key, other._extract_key);
        std::swap(_max_load_factor, other._max_load_factor);
    }

    // 查找

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
     * @brief 返回匹配特定键的元素数量
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
                return const_iterator(first, const_cast<hashtable *>(this));
            }
            first = first->_next;
        }
        return cend();
    }

    stl::pair<iterator, iterator> equal_range(const key_type & key)
    {
        iterator it = find(key);
        iterator it2 = it;
        if (it != end()) {
            while (it2 != end() && _equal(_extract_key(*it2), key)) {
                ++it2;
            }
        }
        return stl::pair<iterator, iterator>(it, it2);
    }

    stl::pair<const_iterator, const_iterator> equal_range(const key_type & key) const
    {
        std::cout << "equal_range" << std::endl;
        const_iterator it = find(key);
        std::cout << "getval: " << it->second << std::endl;
        const_iterator it2 = it;
        std::cout << "getval2: " << it2->second << std::endl;
        if (it != cend()) {
            while (it2 != cend() && _equal(_extract_key(*it2), key)) {
                ++it2;
            }
        }
        return stl::pair<const_iterator, const_iterator>(it, it2);
    }

    // 桶接口

    local_iterator begin(size_type n)
    {
        node * first = _buckets[n];
        if (first != nullptr) {
            return local_iterator(first, this);
        }
        return end(n);
    }

    const_local_iterator begin(size_type n) const
    {
        node * first = _buckets[n];
        if (first != nullptr) {
            return const_local_iterator(first, const_cast<hashtable *>(this));
        }
        return end(n);
    }

    const_local_iterator cbegin(size_type n) const
    {
        return begin(n);
    }

    local_iterator end(size_type)
    {
        return local_iterator();
    }

    const_local_iterator end(size_type) const
    {
        return const_local_iterator();
    }

    const_local_iterator cend(size_type) const
    {
        return end();
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

    // 散列策略

    /**
     * @brief 返回每个桶的平均元素数量
     */
    float load_factor() const
    {
        return size() / bucket_count();
    }

    /**
     * @brief 返回最大加载因子
     */
    float max_load_factor() const
    {
        return _max_load_factor;
    }

    /**
     * @brief 设置最大加载因子
     */
    void max_load_factor(float ml)
    {
        _max_load_factor = ml;
    }

    /**
     * @brief 预留至少指定数量的桶并重新生成散列表
     * @details 生成一个有n个桶的散列表
     */
    void rehash(size_type n)
    {
        // TODO 等vector功能完善再修改
        // stl::vector<node *> new_buckets(n, nullptr);
        stl::vector<node *> new_buckets;
        new_buckets.reserve(n);
        new_buckets.insert(new_buckets.end(), n, nullptr);
        // 遍历所有桶，拿出节点存放到新的桶中
        size_type old_elements = bucket_count();
        for (size_type i = 0; i < old_elements; ++i) {
            node * old_first = _buckets[i];
            while (old_first != nullptr) {
                size_type new_index = _hash_key(old_first->_value, n);
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

    /**
     * @brief 为至少指定数量的元素预留空间并重新生成散列表
     * @details 计算插入count个元素需要多少桶，使得在插入过程中不会resize，并重新生成散列表
     */
    void reserve(size_type count)
    {
        rehash(std::ceil(count / max_load_factor()));
    }

    // 观察器

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
     * @brief 根据指定的元素个数判断是否需要重建哈希表
     */
    void _resize(size_type hashtable_elements)
    {
        // 根据《STL源码剖析》，判断方法是当哈希表中的元素个数大于桶的大小，就重建，体现在_max_load_factor = 1.0
        if (load_factor() > max_load_factor()) {
            rehash(hashtable_elements * 2);
        }
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

    template <typename T>
    iterator _erase(T && pos)
    {
        value_type to_del = *pos;
        size_type index = _hash_key(to_del);
        node * cur = _buckets[index];
        node * prev = nullptr;

        while (cur && cur != pos._node) {
            prev = cur;
            cur = cur->_next;
        }

        if (cur == nullptr) {
            throw std::logic_error("Invalid iterator: node not found");
        }

        if (prev == nullptr) {
            // 当前节点是桶头，直接修改桶的头指针
            _buckets[index] = cur->_next;
        } else {
            // 当前节点是链表中的非头部节点
            prev->_next = cur->_next;
        }

        iterator next_pos(cur->_next, this);
        _destroy_node(cur);
        --_hashtable_elements;
        return next_pos;
    }
};

// 非成员函数

template <class Key, class Value, class Hash, class KeyEqual, class ExtractKey, class Allocator>
void swap(stl::hashtable<Key, Value, Hash, KeyEqual, ExtractKey, Allocator> & lhs,
          stl::hashtable<Key, Value, Hash, KeyEqual, ExtractKey, Allocator> & rhs)
{
    lhs.swap(rhs);
}

} // namespace stl

#endif