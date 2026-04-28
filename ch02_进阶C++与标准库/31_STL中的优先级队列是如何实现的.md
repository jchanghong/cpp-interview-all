## STL中的优先级队列是如何实现的？

`std::priority_queue` 定义在 `<queue>` 头文件中，底层通常使用 `std::vector<T>` 作为容器，并通过堆来维护优先级。

- 默认比较器是 `std::less<T>`，因此默认是**最大堆**。
- 底层会借助 `std::make_heap`、`std::push_heap`、`std::pop_heap` 维护堆性质。
- `push()` 先插入元素，再上浮调整；`pop()` 先把堆顶交换到末尾，再下沉调整；`top()` 直接返回堆顶元素。
- 通过自定义比较器，可以把它改造成**最小堆**。

```C++
#include <queue>
#include <vector>
using namespace std;

priority_queue<int> maxq; // 默认最大堆

priority_queue<int, vector<int>, greater<int>> minq; // 最小堆
```

### 合并前来源内容保留

以下保留合并前两个 priority_queue 文件中的有效原说明；截断句已在上方正文修复。

#### 原「STL中的优先级队列」有效说明

在 C++ STL 中，`std::priority_queue` 默认情况下使用 `std::vector` 作为其底层容器，并且使用 `std::make_heap`、`std::push_heap` 和 `std::pop_heap` 算法来维护堆的性质。

#### 原「简单说说STL中的优先级队列」说明

在C++标准模板库（STL）中，优先级队列是通过一个称为堆的数据结构实现的，通常用一个向量（通常是 `std::vector`）来表示。具体来说，默认情况下，优先级队列使用最大堆来组织元素，这意味着队列顶部总是最大的元素。如果需要最小元素优先，可以通过提供自定义比较函数来实现最小堆。

优先级队列在STL中是用模板类 `std::priority_queue` 实现的，该类在 `<queue>` 头文件中定义。它允许插入和取出元素的操作，其中插入操作是将新元素添加到正确位置以保持堆的性质，取出操作是移除队列顶部的元素。其余的元素会重新排列以保持堆的性质，确保下一个最大（或最小，取决于比较函数）元素移动到队列顶部。
