## 一、Allocator（空间配置器）

### 1.1 作用

Allocator 是 STL 容器与底层内存分配之间的抽象层。它负责：
- **分配/释放内存**（`allocate` / `deallocate`）
- **构造/销毁对象**（`construct` / `destroy`）

### 1.2 默认分配器 `std::allocator`

```cpp
template<typename T>
class allocator {
public:
    using value_type = T;
    T* allocate(size_t n);      // 分配 n 个 T 的内存（不调用构造函数）
    void deallocate(T* p, size_t n);  // 释放内存（不调用析构函数）
    void construct(T* p, const T& val);  // 在 p 处构造对象（placement new）
    void destroy(T* p);           // 调用析构函数
};
```

### 1.3 分配与构造的分离

STL 容器将内存分配和对象构造分开：
```cpp
// vector 扩容时的伪代码
T* new_data = alloc.allocate(new_capacity);  // 1. 分配原始内存
for (size_t i = 0; i < old_size; ++i) {
    alloc.construct(new_data + i, old_data[i]);  // 2. 逐个构造
}
// ... 销毁旧元素、释放旧内存
```

### 1.4 面试常见问题

**Q：为什么 STL 要把分配和构造分开？**
A：分离后可以先分配一大块内存，再按需构造对象。这对于 `vector` 的 `reserve` 和 `capacity > size` 语义至关重要。

**Q：如何自定义 Allocator？**
A：实现标准 Allocator 接口（`allocate`、`deallocate`、`construct`、`destroy` 等），然后作为模板参数传给容器：`std::vector<int, MyAllocator<int>>`。
