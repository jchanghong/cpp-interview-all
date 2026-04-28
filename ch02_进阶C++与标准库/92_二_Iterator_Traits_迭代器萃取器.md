## 二、Iterator Traits（迭代器萃取器）

### 2.1 作用

Iterator Traits 从迭代器类型中提取关键信息（值类型、差值类型、指针类型、引用类型、迭代器类别），使算法能泛型地处理不同迭代器。

### 2.2 `std::iterator_traits`

```cpp
template<typename Iterator>
struct iterator_traits {
    using difference_type   = typename Iterator::difference_type;
    using value_type        = typename Iterator::value_type;
    using pointer           = typename Iterator::pointer;
    using reference         = typename Iterator::reference;
    using iterator_category = typename Iterator::iterator_category;
};
```

### 2.3 迭代器分类（从弱到强）

| 类别 | 支持操作 | 典型容器 |
|------|---------|---------|
| Input | 只读、单遍、`++` | `istream_iterator` |
| Output | 只写、单遍、`++` | `ostream_iterator` |
| Forward | 读写、多遍、`++` | `forward_list` |
| Bidirectional | 双向、`++`/`--` | `list`, `set`, `map` |
| Random Access | 随机访问、`+`/`-`/`[]`/`<` | `vector`, `deque`, 数组指针 |

### 2.4 标签分发（Tag Dispatch）

算法根据迭代器类别选择最优实现：
```cpp
// std::advance 的伪代码
template<typename InputIt, typename Distance>
void advance(InputIt& it, Distance n, std::input_iterator_tag) {
    while (n--) ++it;  // O(n)
}
template<typename RandomIt, typename Distance>
void advance(RandomIt& it, Distance n, std::random_access_iterator_tag) {
    it += n;  // O(1)
}
template<typename InputIt, typename Distance>
void advance(InputIt& it, Distance n) {
    advance(it, n, typename std::iterator_traits<InputIt>::iterator_category{});
}
```
