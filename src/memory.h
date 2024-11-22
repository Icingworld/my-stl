#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <limits>

namespace stl
{

/**
 * @brief 分配器
 * @link https://zh.cppreference.com/w/cpp/memory/allocator
 */
template <class T>
class allocator
{
public:
    // 嵌套类型
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using propagate_on_container_move_assignment = std::true_type;
    using is_always_equal = std::true_type;

public:
    allocator() noexcept = default;

    allocator(const allocator & other) noexcept = default;

    template <typename U>
    allocator(const allocator<U> & other) noexcept
    {}

    ~allocator() = default;

    template <typename U>
    class rebind
    {
    public:
        using other = allocator<U>;
    };

    /**
     * @brief 分配n个元素的内存
     * @param n 元素个数
     * @param hint 会在hint附近分配内存，忽略
     */
    pointer allocate(size_type n, const void * hint = nullptr)
    {
        (void)hint;
        if (n > max_size())   // 超出最大尺寸
            throw std::bad_array_new_length();
        return n == 0 ? nullptr : static_cast<pointer>(::operator new(n * sizeof(value_type)));
    }

    /**
     * @brief 释放由allocate分配的内存
     * @param ptr 要释放的内存指针
     * @param n 元素个数，忽略
     */
    void deallocate(pointer ptr, size_type n = 0)
    {
        (void)n;
        if (ptr == nullptr) return;
        ::operator delete(ptr);
    }

    /**
     * @brief 构造对象
     * @param ptr 要构造的内存指针
     * @param args 构造函数参数包
     * @details 可选
     */
    template <typename U, typename... Args>
    pointer construct(U * ptr, Args&&... args)
    {
        // 定位new返回void *
        return static_cast<U *>(::new(ptr) U(std::forward<Args>(args)...));
    }

    /**
     * @brief 销毁对象
     * @param ptr 要销毁的内存指针
     * @details 可选
     */
    template <typename U>
    void destroy(U * ptr)
    {
        ptr->~U();
    }

    size_type max_size() const noexcept
    {
        // https://zh.cppreference.com/w/cpp/memory/allocator/max_size
        return std::numeric_limits<std::size_t>::max() / sizeof(value_type);
    }

    pointer address(reference r) const noexcept
    {
        return &r;
    }

    const_pointer address(const_reference r) const noexcept
    {
        return &r;
    }

    bool operator==(const allocator & other) const noexcept
    {   

        return true;    // 分配器默认无状态，所以默认相等
    }
    
    bool operator!=(const allocator & other) const noexcept
    {
        return false;
    }
};

} // namespace stl

#endif