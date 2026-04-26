# MemoryPool

这是一个使用 `C++17 + CMake` 编写的内存池进阶示例项目。现在它已经按照模块 9 的方向完成了扩展，包含：

- 定长块内存池 `FixedMemoryPool`
- 基于 placement new 的对象池封装 `ObjectPool<T>`
- 对象自动归还包装器 `UniquePoolPtr<T>`
- 线程安全版本 `ThreadSafeFixedMemoryPool` 与 `ThreadSafeObjectPool<T>`
- 带越界检测和调试标记的 `DebugFixedMemoryPool`
- 基于分离适配策略的 `VariableMemoryPool`
- 性能基准测试程序 `memory_pool_benchmark`
- 单元测试程序 `memory_pool_tests`
- 基于 `gcov` 的覆盖率统计能力

## 目录结构

```text
MemoryPool/
├── CMakeLists.txt
├── include/
│   ├── fixed_memory_pool.h
│   └── memory_pool_extensions.h
├── src/
│   ├── benchmark.cpp
│   ├── fixed_memory_pool.cpp
│   ├── main.cpp
│   └── memory_pool_extensions.cpp
└── README.md
```

## 如何构建

```bash
cd /home/wj/Code/C++/Interview/MemoryPool
cmake -S . -B build
cmake --build build
./build/memory_pool_demo
./build/memory_pool_benchmark
./build/memory_pool_tests
ctest --test-dir build --output-on-failure
```

如果你想生成覆盖率统计，可以单独使用覆盖率构建目录：

```bash
cmake -S . -B build-coverage -DENABLE_COVERAGE=ON
cmake --build build-coverage --target coverage
```

## 本次扩展对应模块 9 的实现

### 1. 增加线程安全版本

项目新增了：

- `ThreadSafeFixedMemoryPool`
- `ThreadSafeObjectPool<T>`

实现方式：

- 使用 `std::mutex + std::lock_guard` 对分配和释放加锁
- 适合多线程下共享对象池的基础演示
- 代码重点在“线程安全思路”，不是极限性能版本

面试可答：

> 最简单的线程安全内存池方案，就是在 Allocate/Deallocate 上加互斥锁，先保证正确性，再考虑 thread local 或无锁结构优化吞吐。

### 2. 增加越界检测与调试标记

项目新增了 `DebugFixedMemoryPool`，实现了：

- 块头魔数 `kHeadMagic`
- 块尾魔数 `kTailMagic`
- 活跃块映射表 `active_blocks_`
- 重复释放/非法释放检测

作用：

- 在回收时检查头尾守卫区是否被破坏
- 帮助定位越界写和 double free 问题
- 更适合调试环境，不适合直接当生产高性能版本

### 3. 增加对象自动归还包装器

项目新增了：

- `PoolDeleter<T>`
- `UniquePoolPtr<T>`
- `ObjectPool<T>::CreateUnique(...)`

这样可以把对象池使用方式写成：

```cpp
ObjectPool<GameEntity> pool(8);
auto entity = pool.CreateUnique(1, "Player");
```

当 `entity` 离开作用域时，会自动调用对象析构并把内存归还给对象池。

这一层本质上是：

- 对象池 + 自定义 deleter + `std::unique_ptr`

优点：

- 减少手动 `Destroy` 遗漏
- 更贴近现代 C++ 的 RAII 风格

### 4. 增加变长块管理策略

项目新增了 `VariableMemoryPool`，采用的是面试中很常见的“分离适配”思路：

- 维护多组尺寸类别，例如 `16/32/64/128/256`
- 小于等于某个桶位大小的请求，放进该桶位
- 超过最大桶位的请求，回退到系统分配

这个设计的特点：

- 小对象分配很快
- 容易实现
- 非常适合说明“变长块池并不一定真的为每个请求都精确分配”

它的代价是：

- 可能出现内部碎片
- 尺寸类别设计不合理时，浪费会明显增加

### 5. 增加性能基准测试

项目新增可执行文件：

- `memory_pool_benchmark`

测试内容：

- 普通 `new/delete`
- `ObjectPool<T>`
- `VariableMemoryPool`

用途：

- 帮你直观看到对象池在高频申请/释放场景下的收益
- 方便面试时从“理论优化”扩展到“如何验证优化”

注意：

- 这个基准程序是教学用途
- 结果会受编译器、机器、负载和优化等级影响
- 面试中重点不是绝对数值，而是“你知道如何设计验证方法”

## 各模块适合怎么讲

### `FixedMemoryPool`

关键词：

- 定长块
- 批量扩容
- 空闲链表
- `O(1)` 分配与回收

### `ObjectPool<T>`

关键词：

- placement new
- 显式析构
- 内存生命周期与对象生命周期分离

### `ThreadSafeObjectPool<T>`

关键词：

- 互斥锁保护
- 先保证正确性
- 后续可扩展为 thread local 或 lock-free

### `DebugFixedMemoryPool`

关键词：

- guard bytes
- magic number
- double free 检测
- 调试版内存池

### `VariableMemoryPool`

关键词：

- segregated free lists
- size classes
- 大块请求回退系统分配

## 面试高频延伸点

### 1. 为什么线程安全版本不一定更快？

因为互斥锁会带来竞争和上下文切换成本。线程安全版本的目标是先保证共享池在多线程下行为正确，而不是天然更快。

### 2. 调试版内存池为什么不适合生产环境？

因为它增加了：

- 额外头尾标记
- 活跃块映射表
- 更多校验逻辑

这些都意味着额外时间和空间开销。

### 3. 变长池为什么常用分离适配？

因为它在实现复杂度和运行效率之间比较平衡：

- 比完全通用分配器简单
- 比每次直接走系统分配更快
- 非常适合固定几类常见大小的对象

### 4. 自动归还包装器的本质是什么？

本质是用 RAII 管理“对象池归还动作”，把原来必须手动写的 `Destroy` 绑定到智能指针析构过程。

## 推荐运行方式

先看功能演示：

```bash
./build/memory_pool_demo
```

再看性能测试：

```bash
./build/memory_pool_benchmark
```

最后运行单元测试：

```bash
./build/memory_pool_tests
ctest --test-dir build --output-on-failure
```

如果需要覆盖率统计：

```bash
cmake -S . -B build-coverage -DENABLE_COVERAGE=ON
cmake --build build-coverage --target coverage
cat build-coverage/coverage/coverage_summary.txt
```

覆盖率输出位置：

- 文本摘要：`build-coverage/coverage/coverage_summary.txt`
- 测试输出：`build-coverage/coverage/test_output.txt`
- 每个源文件的 `.gcov` 明细：`build-coverage/coverage/`

## 适合面试时的总结表达

可以这样概括：

> 这个示例最初只有一个基础的定长内存池，后续我把它扩展成了一个小型内存池实验项目：包括线程安全封装、越界检测调试池、RAII 对象自动归还、分离适配的变长内存池，以及一个简单基准测试程序。这样不但能说明内存池怎么写，还能说明它在工程里怎么演进。
