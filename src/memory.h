#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <limits>

namespace stl
{

/**
 * @brief 默认分配器
 * @details 全部使用new和delete
 */
template <class T>
class __default_allocator
{
public:
    using value_type = T;
    using pointer = T*;
    using size_type = size_t;

public:
    /**
     * @brief 重新绑定，用于在simple_alloc中实现rebind
     */
    template <typename U>
    class __rebind
    {
    public:
        using other = __default_allocator<U>;
    };

    /**
     * @brief 分配n字节的内存
     * @param n 字节数
     */
    static pointer __allocate(size_type n)
    {
        if (n > __max_size())   // 超出最大尺寸
            throw std::bad_alloc();
        return static_cast<pointer>(::operator new(n));
    }

    /**
     * @brief 释放内存
     * @param ptr 内存指针
     * @param n 字节数（忽略）
     */
    static void __deallocate(pointer ptr, size_type n)
    {
        (void)n;    // 忽略n
        ::operator delete(ptr);
    }

    template <typename U, typename... Args>
    static U * __construct(U * ptr, Args&&... args)
    {
        // 定位new返回void *
        return static_cast<U *>(::new(ptr) U(std::forward<Args>(args)...));
    }

    template <typename U>
    static void __destroy(U * ptr)
    {
        ptr->~U();
    }

    static size_type __max_size()
    {
        return std::numeric_limits<std::size_t>::max() / sizeof(value_type);
    }
};

/**
 * @brief 简单分配器
 * @details 分配器的封装类，根据传入的分配器模板选择分配方式
 */
template <class T, class Alloc = __default_allocator<T>>
class simple_alloc
{
public:
    // 必须的嵌套类型
    using value_type = T;
    // 可选的嵌套类型
    using void_pointer = void*;
    using pointer = T*;
    using const_pointer = const T*;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

public:
    simple_alloc() noexcept = default;
    simple_alloc(const simple_alloc &) noexcept = default;
    /**
     * @brief rebind拷贝构造
     */
    template <typename U>
    simple_alloc(const simple_alloc<U, typename Alloc::template __rebind<U>::other> &) noexcept {}

    ~simple_alloc() noexcept = default;

    template <typename U>
    class rebind
    {
    public:
        // 需要传入新模板参数的分配器类，而不是使用原来的Alloc(即__default_allocator<T>)
        using other = simple_alloc<U, typename Alloc::template __rebind<U>::other>;
    };

    pointer allocate()
    {
        return allocate(1);
    }

    /**
     * @brief 分配n个元素的内存
     * @param n 元素个数
     */
    pointer allocate(size_type n, const void_pointer hint = nullptr)
    {
        (void)hint; // 忽略hint
        return n == 0 ? nullptr : Alloc::__allocate(n * sizeof(value_type));
    }

    void deallocate(pointer p)
    {
        deallocate(p, 1);
    }

    void deallocate(pointer p, size_type n)
    {
        if (p == nullptr) return;
        if (n == 0) return;
        Alloc::__deallocate(p, n * sizeof(value_type));
    }

    /**
     * @brief 构造对象
     * @details 可选
     */
    template <typename U, typename... Args>
    pointer construct(U * p, Args&&... args)
    {
        return Alloc::__construct(p, std::forward<Args>(args)...);
    }

    /**
     * @brief 销毁对象
     * @details 可选
     */
    template <typename U>
    void destroy(U * p)
    {
        Alloc::__destroy(p);
    }

    size_type max_size() const noexcept
    {
        return Alloc::__max_size();
    }

    bool operator==(const simple_alloc &) const noexcept { return true; }
    bool operator!=(const simple_alloc &) const noexcept { return false; }
};

} // namespace stl

#endif