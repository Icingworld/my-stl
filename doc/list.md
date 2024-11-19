# List

list是一个双向链表

## 迭代器

list的迭代器是一个双向迭代器

## 接口设计

### 1. 构造与析构

构造函数

+ 默认构造函数 list()：初始化一个空链表。&#9745;
+ 拷贝构造函数 list(const list& other)：深拷贝一个链表。&#9744;
+ 移动构造函数 list(list&& other)：移动一个链表。&#9744;
+ 指定大小和默认值的构造函数 list(size_t count, const T& value)：构造一个指定大小并填充默认值的链表。&cross;
+ 初始化列表构造函数 list(std::initializer_list<T> init)：支持 {} 初始化。&#9744;

析构函数

+ ~list()：销毁链表，释放所有内存。&#9745;

### 2. 元素访问

+ front()：返回链表的第一个元素（引用）。&#9745;
+ back()：返回链表的最后一个元素（引用）。&#9745;

### 3. 迭代器

实现支持范围 for 循环的迭代器：

+ begin()：返回指向链表第一个有效元素的迭代器。&#9745;
+ end()：返回指向链表尾部之后位置的迭代器。&#9745;
+ cbegin() / cend()：返回常量迭代器。&#9744;
+ rbegin() / rend()：支持反向迭代器。&#9744;
+ crbegin() / crend()：常量反向迭代器。&#9744;

### 4. 容量管理

+ empty()：检查链表是否为空。&#9745;
+ size()：返回链表中元素的个数。&#9745;
+ max_size()：返回链表的最大可能大小（通常与系统实现相关）。&cross;

### 5. 修改操作

+ clear()：清空链表。&#9745;
+ insert(iterator pos, const T& value)：在指定位置前插入元素。&#9745;
+ insert(iterator pos, T&& value)：在指定位置前插入移动元素。&#9745;
+ insert(iterator pos, size_t count, const T& value)：在指定位置前插入 count 个元素。&cross;
+ erase(iterator pos)：删除指定位置的元素。&#9744;
+ erase(iterator first, iterator last)：删除指定范围的元素。&#9744;
+ push_front(const T& value)：在链表头部插入元素。&#9745;
+ push_back(const T& value)：在链表尾部插入元素。&#9745;
+ pop_front()：删除链表头部元素。&#9745;
+ pop_back()：删除链表尾部元素。&#9745;
+ resize(size_t count)：调整链表大小。&#9744;
+ swap(list& other)：交换两个链表的内容。&#9745;

### 6. 链表专有操作

+ merge(list& other)：将 other 链表合并到当前链表，前提是两者已排序。&#9744;
+ splice(iterator pos, list& other)：将 other 链表的内容插入到当前链表的 pos 位置。&#9744;
+ splice(iterator pos, list& other, iterator it)：将 other 链表的一个元素插入到 pos 位置。&#9744;
+ splice(iterator pos, list& other, iterator first, iterator last)：将 other 链表的指定范围插入到 pos 位置。&#9744;
+ remove(const T& value)：删除所有值等于 value 的节点。&#9744;
+ remove_if(Predicate pred)：删除满足谓词条件的所有节点。&#9744;
+ unique()：删除相邻的重复元素（适用于已排序链表）。&#9744;
+ reverse()：反转链表。&#9744;
+ sort()：对链表进行排序。&#9744;

### 7. 比较操作

+ operator==：比较两个链表是否相等。&#9744;
+ operator!=：比较两个链表是否不相等。&#9744;
+ operator< / operator> / operator<= / operator>=：支持链表的字典序比较。&#9744;

### 8. 友元函数

实现一些和链表强相关的非成员函数：

+ std::swap(list<T>& lhs, list<T>& rhs)：全局 swap 函数。&#9744;

### 9. 异常处理

+ 检查非法操作，比如在空链表上调用 pop_front 或 pop_back。&#9745;
