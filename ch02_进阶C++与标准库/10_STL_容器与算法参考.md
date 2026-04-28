## STL 容器与算法参考

> 本章节为 STL 各容器的方法速查。详细的逐方法代码示例请参见 cppreference.com。
> 容器方法速查表已在前文的 [STL__README.md](#stl__readmemd) 中列出。

### 组成

- 容器（containers）：序列式容器（vector, deque, list, forward_list, array）、关联式容器（set, map, multiset, multimap）、无序容器（unordered_set, unordered_map 等）
- 算法（algorithms）：find, sort, copy, remove, transform 等 100+ 通用算法
- 迭代器（iterators）：输入/输出/前向/双向/随机访问五种迭代器
- 仿函数（functors）：重载 operator() 的函数对象
- 配接器（adapters）：stack, queue, priority_queue（容器适配器）
- 空间配置器（allocator）：内存分配与释放管理

### 序列式容器

| 容器 | 底层结构 | 随机访问 | 头尾插入/删除 | 中间插入/删除 |
|------|---------|---------|------------|------------|
| array | 固定数组 | O(1) | N/A（固定大小） | N/A |
| vector | 动态数组 | O(1) | 尾部 O(1) 均摊 | O(n) |
| deque | 分段数组 | O(1) | 头尾 O(1) | O(n) |
| forward_list | 单向链表 | - | 头部 O(1) | O(1)（已知前驱） |
| list | 双向链表 | - | 头尾 O(1) | O(1)（已知位置） |

### 关联式容器

| 容器 | 底层结构 | 是否有序 | 查找/插入/删除 |
|------|---------|---------|-------------|
| set / multiset | 红黑树 | 有序 | O(log n) |
| map / multimap | 红黑树 | 按键有序 | O(log n) |
| unordered_set / unordered_multiset | 哈希表 | 无序 | 平均 O(1)，最坏 O(n) |
| unordered_map / unordered_multimap | 哈希表 | 按键无序 | 平均 O(1)，最坏 O(n) |

### 容器适配器

- **stack**：LIFO，默认基于 deque
- **queue**：FIFO，默认基于 deque
- **priority_queue**：最大堆（默认），默认基于 vector

### 常用算法分类

- 非修改序列：find, count, for_each, search, binary_search
- 修改序列：copy, move, fill, transform, replace
- 排序：sort, stable_sort, partial_sort, nth_element
- 二分查找：lower_bound, upper_bound, equal_range, binary_search
- 集合操作：merge, set_union, set_intersection, set_difference
- 堆操作：make_heap, push_heap, pop_heap
- 最值：min, max, minmax, min_element, max_element

> 完整的算法列表和使用方法参见前文 [STL 算法](#算法algorithms)。

### 迭代器类型

| 迭代器 | 能力 | 典型提供者 |
|--------|-----|----------|
| Input | 只读，单遍 | istream_iterator |
| Output | 只写，单遍 | ostream_iterator |
| Forward | 读写，多遍，只能前进 | forward_list |
| Bidirectional | 读写，多遍，可前进后退 | list, set, map |
| Random Access | 读写，多遍，支持 +n/-n | vector, deque, array |

# docs__02-cpp-oop__class__README.md
