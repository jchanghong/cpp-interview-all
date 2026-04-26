# CAS

这个工程演示 `CAS`（Compare-And-Swap）在 C++ 面试里的两个常见落点：无锁原子计数器和基于 `atomic_flag` 的自旋锁。

## 构建

```bash
cd /home/wj/Code/C++/Interview/CAS
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

## 目标

- `cas_demo`：并发比较 CAS 计数器和加锁计数器。
- `cas_tests`：验证自旋锁、原子计数器和加锁计数器正确性。

## 面试知识点

- CAS 的核心语义：比较内存中的旧值是否等于预期值，若相等则原子写入新值。
- 常见接口：`compare_exchange_weak` 和 `compare_exchange_strong`。
- `weak` 允许伪失败，适合放在循环里反复重试。
- 自旋锁适合临界区很短、线程切换代价较高的场景。
- CAS 解决的是“单个位置的原子更新”，不能自动解决复杂结构的一致性。
- 高频延伸问题：ABA 问题、内存序、忙等 CPU 消耗、饥饿与公平性。

