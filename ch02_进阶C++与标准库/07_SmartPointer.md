## SmartPointer

这个工程实现了一个面试友好的最小版 `UniquePtr` 和 `SharedPtr`，用来解释 RAII、所有权转移和引用计数。

### 构建

```bash
cd <SmartPointer 项目目录>
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

### 目标

- `smart_pointer_demo`：演示独占和共享所有权的基本行为。
- `smart_pointer_tests`：验证移动语义、重置和引用计数。

### 面试知识点

- `unique_ptr` 体现的是独占所有权，不允许拷贝，允许移动。
- `shared_ptr` 通过控制块维护引用计数。
- 生产级 `shared_ptr` 通常需要线程安全的原子引用计数。
- `weak_ptr` 的意义是打破循环引用。
- 智能指针解决的是资源生命周期问题，不等于自动解决并发安全问题。


# STL__README.md

> ⚠️ 以下为早期从 cplusplus.com 搬运的 API 方法速查表。面试核心只需掌握常用方法（begin/end/size/empty/insert/erase/push_back/pop_back/find/count 等），完整 API 请查阅 [cppreference.com](https://en.cppreference.com/)。STL 底层原理和面试重点参见前文容器总览表和 STL__STL.md 章节。

STL 详细文档
