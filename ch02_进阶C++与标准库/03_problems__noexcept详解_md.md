# problems__noexcept详解.md

### noexcept 是什么？有什么作用？

`noexcept` 是 C++11 引入的关键字，有两种用法：

1. **`noexcept` 说明符**：声明函数不会抛出异常
2. **`noexcept` 运算符**：编译期检查表达式是否可能抛出异常

```cpp
void func1() noexcept;            // 承诺不抛异常
void func2() noexcept(true);      // 等价
void func3() noexcept(false);     // 可能抛异常
void func4() noexcept(noexcept(T()));  // 条件 noexcept
```

**为什么重要？**

1. **移动优化**：`std::vector` 扩容时，如果元素的移动构造函数是 `noexcept`，会使用移动而非拷贝（`std::move_if_noexcept`）
2. **编译器优化**：noexcept 允许编译器生成更高效的代码（跳过异常处理栈展开逻辑）
3. **接口承诺**：明确表达函数不会抛异常，调用者可以简化错误处理

**何时使用？**

> 移动构造函数、移动赋值运算符、swap 函数、析构函数应尽可能标记为 `noexcept`。如果函数内部不抛异常且不调用可能抛异常的函数，考虑标记 `noexcept`。

```cpp
class MyClass {
public:
    MyClass(MyClass&& other) noexcept : data_(other.data_) {  // ✅ 移动构造 noexcept
        other.data_ = nullptr;
    }
    ~MyClass() noexcept { delete[] data_; }  // ✅ 析构默认 noexcept
};
```

---
