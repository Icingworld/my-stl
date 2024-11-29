#ifndef __HASH_H__
#define __HASH_H__

#include <stdexcept>

namespace stl
{

template <class Ty, class Key>
class _hash_base
{
public:
    using argument_type = Key;
    using result_type = Ty;
};

/**
 * @brief hash
 * @link https://zh.cppreference.com/w/cpp/utility/hash
 * @details 参照std::hash标准，提供两种类型声明和一个operator()重载
 */
template <class Key>
class hash
    : public _hash_base<std::size_t, Key>
{
public:
    using typename _hash_base<std::size_t, Key>::argument_type;
    using typename _hash_base<std::size_t, Key>::result_type;

public:
    result_type operator()(const argument_type& key) const
    {
        throw std::runtime_error("type not implemented");
        return 0;
    }
};

#define _HASH_SPECIALIZATION_TEMPLATE(Key) \
    template<> \
    class hash<Key> \
        : public _hash_base<std::size_t, Key> \
    { \
    public: \
        result_type operator()(const argument_type& key) const \
        { \
            return static_cast<result_type>(key); \
        } \
    };

/**
 * @details 以下提供必须实现的hash特化，按照C++标准定义，需要为算术类型、枚举类型和指针类型(包括nullptr_t)提供特化
 * @link https://zh.cppreference.com/w/cpp/language/types
 * @link https://zh.cppreference.com/w/cpp/language/enum
 * @link https://zh.cppreference.com/w/cpp/language/pointer
 * @link https://zh.cppreference.com/w/cpp/types/nullptr_t
 */

// 这些是可以无损转换为size_t的类型

_HASH_SPECIALIZATION_TEMPLATE(signed char)

_HASH_SPECIALIZATION_TEMPLATE(unsigned char)

_HASH_SPECIALIZATION_TEMPLATE(short int)

_HASH_SPECIALIZATION_TEMPLATE(unsigned short int)

_HASH_SPECIALIZATION_TEMPLATE(int)

_HASH_SPECIALIZATION_TEMPLATE(unsigned int)

_HASH_SPECIALIZATION_TEMPLATE(long int)

_HASH_SPECIALIZATION_TEMPLATE(unsigned long int)

_HASH_SPECIALIZATION_TEMPLATE(long long int)

_HASH_SPECIALIZATION_TEMPLATE(unsigned long long int)

#undef _HASH_SPECIALIZATION

/**
 * @brief bool类型的hash特化
 */
template <>
class hash<bool>
    : public _hash_base<std::size_t, bool>
{
public:
    result_type operator()(const argument_type& key) const
    {
        return key ? 1 : 0;
    }
};

template <typename T>
class hash<T*>
    : public _hash_base<std::size_t, T*>
{
public:
    using argument_type = typename _hash_base<std::size_t, T*>::argument_type;
    using result_type = typename _hash_base<std::size_t, T*>::result_type;
public:
    result_type operator()(const argument_type& key) const
    {
        return reinterpret_cast<std::size_t>(key);  // 直接将指针的地址转换为size_t
    }
};

/**
 * @brief nullptr_t类型的hash特化
 */
template <>
class hash<std::nullptr_t>
    : public _hash_base<std::size_t, std::nullptr_t>
{
public:
    result_type operator()(const argument_type& key) const
    {
        return 0;   // nullptr_t的hash值为0
    }
};

} // namespace stl

#endif