#ifndef __RB_TREE_H__
#define __RB_TREE_H__

#include "memory.h"

namespace stl
{

using rb_tree_color_type = bool;
const rb_tree_color_type rb_tree_red = true;        // 红色是true
const rb_tree_color_type rb_tree_black = false;     // 黑色是false

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

    static node * maxmum(node * x)
    {
        while (x->_right != nullptr) {
            x = x->_right;
        }
        return x;
    }
};

}

#endif