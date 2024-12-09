#ifndef __UNORDERED_MULTIMAP_H__
#define __UNORDERED_MULTIMAP_H__

#include "hashtable.h"
#include "functional.h"

namespace stl
{

template <
    class Key,
    class T,
    class Hash = stl::hash<Key>,
    class KeyEqual = stl::equal_to<Key>,
    class Allocator = stl::allocator<stl::pair<const Key, T>>
> class unordered_multimap
{
public:
    using key_type = Key;
    using mapped_type = T;
    using value_type = stl::pair<const key_type, mapped_type>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using hasher = Hash;
    using key_equal = KeyEqual;
    using allocator_type = Allocator;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using hashtable_type = stl::hashtable<key_type, mapped_type, hasher, key_equal, stl::keyExtractor<stl::pair<const key_type, mapped_type>>, allocator_type>;
    using iterator = typename hashtable_type::iterator;
    using const_iterator = typename hashtable_type::const_iterator;
    using local_iterator = typename hashtable_type::local_iterator;
    using const_local_iterator = typename hashtable_type::const_local_iterator;

protected:
    hashtable_type _ht;    // 哈希表

public:
    // 构造函数

    unordered_map()
        : _ht(50, hasher(), key_equal())
    {}

public:
    // 迭代器

    iterator begin() noexcept
    {
        return _ht.begin();
    }

    const_iterator begin() const noexcept
    {
        return _ht.begin();
    }

    const_iterator cbegin() const noexcept
    {
        return _ht.cbegin();
    }

    iterator end() noexcept
    {
        return _ht.end();
    }

    const_iterator end() const noexcept
    {
        return _ht.end();
    }

    const_iterator cend() const noexcept
    {
        return _ht.cend();
    }

    // 修改器

    void clear() noexcept
    {
        _ht.clear();
    }

    void swap(unordered_map& other) noexcept
    {
        _ht.swap(other._ht);
    }

    // 容量

    bool empty() const noexcept
    {
        return _ht.empty();
    }

    size_type size() const noexcept
    {
        return _ht.size();
    }

    size_type max_size() const noexcept
    {
        return _ht.max_size();
    }

    // 查找

    /**
     * @brief 返回匹配特定键的元素数量
     */
    size_type count(const key_type& key) const
    {
        return _ht.count(key);
    }

    /**
     * @brief 返回匹配特定键的元素
     */
    iterator find(const key_type& key)
    {
        return _ht.find(key);
    }

    /**
     * @brief 返回匹配特定键的元素
     */
    const_iterator find(const key_type& key) const
    {
        return _ht.find(key);
    }

    /**
     * @brief 返回匹配特定键的元素范围
     */
    stl::pair<iterator, iterator> equal_range(const key_type& key)
    {
        return _ht.equal_range(key);
    }

    /**
     * @brief 返回匹配特定键的元素范围
     */
    stl::pair<const_iterator, const_iterator> equal_range(const key_type& key) const
    {
        return _ht.equal_range(key);
    }

    // 桶接口

    /**
     * @brief 返回指向指定的桶的开始的迭代器
     */
    local_iterator begin(size_type n)
    {
        return _ht.begin(n);
    }

    /**
     * @brief 返回指向指定的桶的开始的迭代器
     */
    const_local_iterator begin(size_type n) const
    {
        return _ht.begin(n);
    }

    /**
     * @brief 返回指向指定的桶的开始的迭代器
     */
    const_local_iterator cbegin(size_type n) const
    {
        return _ht.cbegin(n);
    }

    /**
     * @brief 返回指向指定的桶的末尾的迭代器
     */
    local_iterator end(size_type n)
    {
        return _ht.end(n);
    }

    /**
     * @brief 返回指向指定的桶的末尾的迭代器
     */
    const_local_iterator end(size_type n) const
    {
        return _ht.end(n);
    }

    /**
     * @brief 返回指向指定的桶的末尾的迭代器
     */
    const_local_iterator cend(size_type n) const
    {
        return _ht.cend(n);
    }

    /**
     * @brief 返回桶数
     */
    size_type bucket_count() const
    {
        return _ht.bucket_count();
    }

    /**
     * @brief 返回桶的最大数量
     */
    size_type max_bucket_count() const
    {
        return _ht.max_bucket_count();
    }

    /**
     * @brief 返回特定的桶中的元素数量
     */
    size_type bucket_size(size_type n) const
    {
        return _ht.bucket_size(n);
    }

    /**
     * @brief 返回带有特定键的桶
     */
    size_type bucket(const key_type& key) const
    {
        return _ht.bucket(key);
    }

    // 散列策略

    /**
     * @brief 负载因子
     */
    float load_factor() const
    {
        return _ht.load_factor();
    }

    /**
     * @brief 最大负载因子
     */
    float max_load_factor() const
    {
        return _ht.max_load_factor();
    }

    /**
     * @brief 设置最大负载因子
     */
    void max_load_factor(float ml)
    {
        _ht.max_load_factor(ml);
    }

    /**
     * @brief 预留至少指定数量的桶并重新生成散列表
     */
    void rehash(size_type count)
    {
        _ht.rehash(count);
    }

    /**
     * @brief 为至少指定数量的元素预留空间并重新生成散列表
     */
    void reserve(size_type count)
    {
        _ht.reserve(count);
    }

    // 观察器

    /**
     * @brief 返回用于对键求散列的函数
     */
    hasher hash_function() const
    {
        return _ht.hash_function();
    }

    /**
     * @brief 返回用于比较键的相等性的函数
     */
    key_equal key_eq() const
    {
        return _ht.key_eq();
    }

};

}

#endif