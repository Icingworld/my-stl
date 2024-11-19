#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stdexcept>
#include <cstring>

namespace stl
{

template<typename T>
class vector
{
public:
    using value_type = T;
    using pointer = value_type*;
    using reference = value_type&;
    using const_pointer = const value_type*;
    using const_reference = const value_type&;
    using difference_type = ptrdiff_t;

private:
    pointer first;
    pointer last;
    pointer end_storage;

    int growth_factor;

public:
    vector() : first(nullptr), last(nullptr), end_storage(nullptr), growth_factor(2) {}

    ~vector() {
        if (first != nullptr) {
            for (pointer it = first; it != last; ++it) {
                it->~value_type();
            }
            operator delete(first);
        }
        first = last = end_storage = nullptr;
    }

public:
    void push_back(const reference value)
    {
        emplace_back(value);
    }

    void push_back(value_type && value)
    {
        emplace_back(std::move(value));
    }

    template <typename... Args>
    void emplace_back(Args &&... args)
    {
        if (last == end_storage) {
            // 空间不足，扩容
            reserve((end_storage - first) == 0 ? 1 : (end_storage - first) * growth_factor);
        }
        // 构造新元素
        new (last) value_type(std::forward<Args>(args)...);
        ++last;
    }

    void pop_back()
    {
        if (last == first) {
            // 容器为空
            return;
        }
        // 销毁最后一个元素
        --last;
        last->~value_type();
    }

    pointer begin() const
    {
        return first;
    }

    pointer end() const
    {
        return last;
    }

    /**
     * @brief  返回元素个数
     */
    size_t size() const
    {
        return last - first;
    }

    /**
     * @brief  返回容量
     */
    size_t capacity() const
    {
        return end_storage - first;
    }

    const_reference at(size_t index) const
    {
        if (index >= size()) {
            throw std::out_of_range("index out of range");
        }
        return *(first + index); // 返回常量引用，不希望被修改
    }

    reference operator[](size_t index) const
    {
        // 不会进行越界检查
        return *(first + index);
    }

    /**
     * @brief  返回第一个元素
     */
    reference front() const
    {
        return *first;
    }

    /**
     * @brief  返回最后一个元素
     */
    reference back() const
    {
        return *(last - 1);
    }

    /**
     *  @brief  在指定位置插入元素
     */
    pointer insert(pointer position, const reference value)
    {
        size_t offset = position - first;
        // 是否需要扩容
        if (last == end_storage) {
            reserve((end_storage - first) == 0 ? 1 : (end_storage - first) * growth_factor);
            position = first + offset;
        }
        // 移动元素
        std::memmove(position + 1, position, (last - position) * sizeof(value_type));
        // 插入新元素
        *position = value;
        ++last;
        return position;
    }

    /**
     * @brief  在指定位置插入元素
     */
    pointer insert(pointer position, value_type && value)
    {
        size_t offset = position - first;
        // 是否需要扩容
        if (last == end_storage) {
            reserve((end_storage - first) == 0 ? 1 : (end_storage - first) * growth_factor);
            position = first + offset;
        }
        // 移动元素
        std::memmove(position + 1, position, (last - position) * sizeof(value_type));
        // 插入新元素
        *position = std::move(value);
        ++last;
        return position;
    }

    /**
     *  @brief  删除指定位置的元素
     */
    pointer erase(pointer position)
    {
        if (position < first || position >= last) {
            throw std::out_of_range("index out of range");
        }
        // 移动元素
        std::memmove(position, position + 1, (last - position - 1) * sizeof(value_type));
        // 销毁最后一个元素
        --last;
        last->~value_type();
        return position;
    }

    /**
     *  @brief  删除指定范围的元素
     */
    pointer erase(pointer first, pointer last)
    {
        if (first > last) {
            throw std::invalid_argument("invalid range");
        }
        if (first < this->first || last > this->last) {
            throw std::out_of_range("index out of range");
        }
        // 移动长度
        size_t move_count = this->last - last;
        // 移动元素
        std::memmove(first, first + (last - first), move_count * sizeof(value_type));
        // 销毁元素
        for (pointer it = first + move_count; it != this->last; ++it) {
            it->~value_type();
        }
        // 移动指针
        this->last = first + move_count;
        return first;
    }

    /**
     *  @brief  交换两个容器
     */
    void swap(vector & other)
    {
        std::swap(first, other.first);
        std::swap(last, other.last);
        std::swap(end_storage, other.end_storage);
        std::swap(growth_factor, other.growth_factor);
    }

    /**
     *  @brief  容器是否为空
     */
    bool empty() const
    {
        return first == last;
    }

    /**
     *  @brief  清空容器
     */
    void clear()
    {
        for (pointer it = first; it != last; ++it) {
            it->~value_type();
        }
        // 空间不释放，只是移动指针
        last = first;
    }

    void set_growth_factor(int factor) {
        growth_factor = factor;
    }

private:
    void reserve(size_t new_capacity)
    {
        // 申请新的空间
        value_type * new_first = static_cast<value_type *>(operator new(new_capacity * sizeof(value_type)));

        // 将旧数据移动到新空间
        value_type * new_last = new_first;
        for (value_type * it = first; it != last; ++it, ++new_last) {
            new (new_last) value_type(std::move(*it)); // 移动构造
            it->~value_type(); // 销毁旧对象
        }

        // 销毁旧内存
        operator delete(first);

        // 更新指针
        first = new_first;
        last = new_last;
        end_storage = new_first + new_capacity;
    }
};

}

#endif