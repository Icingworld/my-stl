#ifndef __ALLOC_H__
#define __ALLOC_H__

namespace stl
{

template <class T>
class __default_alloc_template
{
public:
    using value_type = T;
    using pointer = T*;
    using size_type = size_t;
    using void_pointer = void*;

public:

    static void_pointer allocate(size_type n)
    {
        return ::operator new(n);
    }

    static void deallocate(pointer p, size_type n)
    {
        ::operator delete(p);
    }

    template <typename U, typename... Args>
    static U * construct(U * p, Args&&... args)
    {
        return ::new (p) U(std::forward<Args>(args)...);
    }

    template <typename U>
    static void destroy(U * p)
    {
        p->~U();
    }
};

/**
 * @brief 简单分配器
 * @details 分配器的封装类，根据传入的分配器模板选择分配方式
 */
template <class T, class Alloc = __default_alloc_template<T>>
class simple_alloc
{
public:
    using value_type = T;
    using void_pointer = void*;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

public:
    template <typename U>
    struct rebind {
        using other = simple_alloc<U, Alloc>;
    };

    pointer allocate()
    {
        return allocate(1);
    }

    pointer allocate(size_type n)
    {
        return n == 0 ? nullptr : static_cast<pointer>(Alloc::allocate(n * sizeof(value_type)));
    }

    void deallocate(pointer p)
    {
        deallocate(p, 1);
    }

    void deallocate(pointer p, size_type n)
    {
        if (p == nullptr) return;
        if (n == 0) return;
        Alloc::deallocate(p, n * sizeof(value_type));
    }

    template <typename U, typename... Args>
    pointer construct(U * p, Args&&... args)
    {
        return Alloc::construct(p, std::forward<Args>(args)...);
    }

    // 销毁元素
    template <typename U>
    void destroy(U * p)
    {
        Alloc::destroy(p);
    }
};

} // namespace stl

#endif