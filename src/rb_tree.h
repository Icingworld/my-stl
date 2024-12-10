#ifndef __RB_TREE_H__
#define __RB_TREE_H__

#include "memory.h"

namespace stl
{

using rb_tree_color_type = bool;
const rb_tree_color_type rb_tree_red = true;        // 红色是true
const rb_tree_color_type rb_tree_black = false;     // 黑色是false

/**
 * @brief 红黑树节点
 */
template <typename T>
class __rb_tree_node
{
public:
    using value_type = T;
    using node = __rb_tree_node;
    using color_type = rb_tree_color_type;

public:
    node * _parent;
    node * _left;
    node * _right;
    color_type _color;
    value_type _value;

public:
    __rb_tree_node() = default;

    static node * minimum(node * x)
    {
        while (x->_left != nullptr) {
            x = x->_left;
        }
        return x;
    }

    static node * maximum(node * x)
    {
        while (x->_right != nullptr) {
            x = x->_right;
        }
        return x;
    }
};

/**
 * @brief 红黑树迭代器
 */
template <typename T>
class __rb_tree_iterator
{
public:
    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::bidirectional_iterator_tag;

    using node = __rb_tree_node<value_type>;    // 节点
    using self = __rb_tree_iterator;

public:
    node * _node;       // 绑定红黑树节点

public:
    __rb_tree_iterator() = default;

    __rb_tree_iterator(node * x) : _node(x) {}

    __rb_tree_iterator(const __rb_tree_iterator & other) : _node(other._node) {}

public:
    reference operator*() const
    {
        return _node->_value;
    }

    pointer operator->() const
    {
        return &(operator*());
    }

    self & operator++()
    {
        // 寻找下一个节点
        if (_node->_right != nullptr) {
            // 右子树不为空，寻找右子树的最小节点
            _node = node::minimum(_node->_right);
        } else {
            // 右子树为空，寻找父节点
            node * y = _node->_parent;
            while (y != nullptr && _node == y->_right) {
                // 如果是右子节点，则继续上溯，直到变成左子节点
                _node = y;
                y = y->_parent;
            }
            _node = y;  // 不需要判断根节点且没有右子树的情况
        }
        return *this;
    }

    self operator++(int)
    {
        self tmp = *this;
        ++(*this);
        return tmp;
    }

    self & operator--()
    {
        // 寻找上一个节点
        if (_node->_left != nullptr) {
            // 左子树不为空，寻找左子树的最大节点
            _node = node::maximum(_node->_left);
        } else {
            // 左子树为空，寻找父节点
            node * y = _node->_parent;
            while (y != nullptr && _node == y->_left) {
                _node = y;
                y = y->_parent;
            }
            _node = y;
        }
        return *this;
    }

    self operator--(int)
    {
        self tmp = *this;
        --(*this);
        return tmp;
    }
};

template <
    class Key,
    class Value,
    class ExtractKey,
    class Compare,
    class Allocator
> class rb_tree
{
public:
    using key_type = Key;
    using mapped_type = Value;
    using value_type = std::pair<const key_type, mapped_type>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using key_compare = Compare;
    using value_compare = Compare;
    using allocator_type = Allocator;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = __rb_tree_iterator<value_type>;
    using const_iterator = const iterator;

    using node_allocator_type = typename Allocator::template rebind<__rb_tree_node<value_type>>::other;

protected:
    size_type _size;                        // 元素个数
    node * _head;                           // 虚拟节点
    key_compare _key_comp;
    node_allocator_type _node_allocator;
};

}

#endif