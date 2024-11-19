# iterator

迭代器是一种按照顺序对容器进行遍历的类，它提供了一些统一的标准接口来提供访问容器的能力，而不将容器内部的实现暴露在外。对使用者来说，只需要操作对应类的迭代器，就可以获取容器内的数据，并对数据进行操作

## 种类

根据C++标准，将迭代器分为5类，并根据所需的迭代器类型对其所需的算法进行描述，也就是说，每种迭代器实现的功能是不一样的，每种容器所需要的迭代器也是不一样的

+ 输入迭代器
+ 输出迭代器
+ 正向迭代器
+ 双向迭代器
+ 随机访问迭代器

## 迭代器接口

无论是什么迭代器，都需要实现的接口：

+ 默认构造函数：

`iterator()`

+ 拷贝构造函数和赋值运算符：

`iterator(const iterator & other)`

`iterator & operator=(const iterator & other)`

+ 移动构造函数和赋值运算符：

`iterator(iterator && other)`

`iterator & operator=(iterator && other)`

## 输入迭代器

“输入”指容器将数据输入给程序，因此，输入迭代器可以让程序读取容器中的值，但不能够修改

特别的，输入迭代器是单通行的，只能从第一个元素遍历到超尾，而不能倒退，值得需要注意的是，迭代器移动时，不能保证前面的迭代器仍然有效，因为数据可能在这个过程中发生过变化

输入迭代器需要实现的接口有：

+ 解引用

`const value_type & operator*() const`

`const value_type * operator->() const`

+ 相等和不等

`bool operator==(const iterator & other) const`

`bool operator!=(const iterator & other) const`

+ 递增操作

`iterator & operator++()`

`iterator operator++(int)`

## 输出迭代器

“输出”指程序将数据保存到容器，因此，输出迭代器可以让程序写入容器，但不能读取

同样的，输出迭代器也是单通行的

输出迭代器需要实现的接口有：

暂不考虑，不提供可以使用的引用，而是通过创建辅助函数转化为`output_iterator`标准输出迭代器使用

## 正向迭代器

支持输入迭代器和输出迭代器的所有操作，且不是单通行的，这意味着可以反复访问某个位置，数据都是准确的

## 双向迭代器

支持正向迭代器的所有操作，并额外支持递减操作，也就是从后向前移动的能力

## 随机访问迭代器

支持双向迭代器的所有操作，并额外支持随机访问操作

- 算术运算：
  - `Iterator& operator+=(difference_type n);`
  - `Iterator& operator-=(difference_type n);`
  - `Iterator operator+(difference_type n) const;`
  - `Iterator operator-(difference_type n) const;`
- 两个迭代器之间的距离：
  - `difference_type operator-(const Iterator& other) const;`
- 比较运算：
  - `bool operator<(const Iterator& other) const;`
  - `bool operator>(const Iterator& other) const;`
  - `bool operator<=(const Iterator& other) const;`
  - `bool operator>=(const Iterator& other) const;`
- 直接访问：
  - `reference operator[](difference_type n) const;`

## 补充说明

迭代器需要定义 `iterator_category`，可使用以下类别：

- `std::input_iterator_tag`
- `std::output_iterator_tag`
- `std::forward_iterator_tag`
- `std::bidirectional_iterator_tag`
- `std::random_access_iterator_tag`