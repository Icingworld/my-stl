#ifndef __UNORDERED_MULTISET_H__
#define __UNORDERED_MULTISET_H__

#include "hashtable.h"
#include "functional.h"

namespace stl
{

template <
    class Key,
    class Hash = stl::hash<Key>,
    class Equal = stl::equal_to<Key>,
    class Allocator = stl::allocator<Key>
> class unordered_multiset
{
public:
    using key_type = Key;
    using value_type = Key;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using hasher = Hash;
    using key_equal = Equal;
    using allocator_type = Allocator;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using hashtable_type = stl::hashtable<key_type, key_type, hasher, key_equal, stl::keyExtractor<stl::pair<const key_type, key_type>>, allocator_type>;
    using iterator = typename hashtable_type::iterator;
    using const_iterator = typename hashtable_type::const_iterator;
    using local_iterator = typename hashtable_type::local_iterator;
    using const_local_iterator = typename hashtable_type::const_local_iterator;

protected:
    hashtable_type _ht;     // 哈希表

public:
    unordered_set()
        : _ht(50, hasher(), key_equal())
    {}

public:
    // 迭代器

    iterator begin()
    {
        return _ht.begin();
    }

    const_iterator begin() const
    {
        return _ht.cbegin();
    }

    const_iterator cbegin() const
    {
        return _ht.cbegin();
    }

    iterator end()
    {
        return _ht.end();
    }

    const_iterator end() const
    {
        return _ht.cend();
    }

    const_iterator cend() const
    {
        return _ht.cend();
    }

    // 容量

    bool empty() const
    {
        return _ht.empty();
    }

    size_type size() const
    {
        return _ht.size();
    }

    size_type max_size() const
    {
        return _ht.max_size();
    }

    // 查找

    size_type count(const key_type & key) const
    {
        return _ht.count(key);
    }

    iterator find(const key_type & key)
    {
        return _ht.find(key);
    }

    const_iterator find(const key_type & key) const
    {
        return _ht.find(key);
    }

    stl::pair<iterator, iterator> equal_range(const key_type & key)
    {
        return _ht.equal_range(key);
    }

    stl::pair<const_iterator, const_iterator> equal_range(const key_type & key) const
    {
        return _ht.equal_range(key);
    }

    // 桶接口

    local_iterator begin(size_type n)
    {
        return _ht.begin(n);
    }

    const_local_iterator begin(size_type n) const
    {
        return _ht.cbegin(n);
    }

    const_local_iterator cbegin(size_type n) const
    {
        return _ht.cbegin(n);
    }

    local_iterator end(size_type n)
    {
        return _ht.end(n);
    }

    const_local_iterator end(size_type n) const
    {
        return _ht.cend(n);
    }

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