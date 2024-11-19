#ifndef __LIST_H__
#define __LIST_H__

namespace stl
{

template <typename T>
class list
{
public:
    // 嵌套类型定义

private:
    // 链表节点类
    template <typename T>
    class __list_node
    {
    public:
        // 嵌套类型
        using value_type = T;
        using pointer = __list_node<value_type>*;
    public:
        pointer prev;
        pointer next;
        value_type data;
    }

public:
    // 链表迭代器
};

}

#endif