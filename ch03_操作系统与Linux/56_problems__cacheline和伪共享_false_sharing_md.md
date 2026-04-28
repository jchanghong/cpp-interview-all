# problems__cacheline和伪共享（false-sharing）.md

### 什么是 cacheline？什么是伪共享？

**cacheline（缓存行）** 是 CPU 缓存的最小读写单位，通常为 **64 字节**（x86-64）。CPU 每次从内存加载一整行到 L1/L2/L3 缓存。

**伪共享（false sharing）** 是多线程编程中的隐蔽性能杀手：两个线程修改**不同的变量**，但这些变量恰好落在**同一个 cacheline** 中，导致 CPU 间频繁的缓存一致性协议（MESI）通信。

```
线程 A 写 var_a  ←→  线程 B 写 var_b
       ↓                      ↓
    [var_a | var_b | ... ] ← 同一个 64 字节 cacheline！
```

每次任一线程写入，都会使另一个线程的缓存行失效（invalidate），迫使对方重新加载——尽管它们操作的是**不同变量**。

**影响有多大？** 伪共享可能使并行性能退化到甚至不如单线程。在高频写入场景（无锁队列、线程本地计数器）中尤为明显。

### 如何避免伪共享？

```cpp
#include <new>  // std::hardware_destructive_interference_size (C++17)

// C++17 方式：利用硬件干扰大小进行填充
struct alignas(std::hardware_destructive_interference_size) Counter {
    std::atomic<int64_t> value{0};
};

// 手动方式：填充至 64 字节
struct Counter {
    alignas(64) std::atomic<int64_t> value{0};
    // 确保每个 Counter 独占一个 cacheline
};

Counter counters[MAX_THREADS];  // 每个线程用不同下标
```

**面试标准答案：**

> cacheline 是 CPU 缓存的最小读写单位（通常 64 字节）。伪共享是指多个线程修改的不同变量位于同一 cacheline，导致缓存一致性协议频繁失效，性能严重退化。
>
> 避免方法：
> 1. 用 `alignas(64)` 或 `std::hardware_destructive_interference_size`（C++17）让变量独占 cacheline
> 2. 每个线程操作独立的 cacheline 对齐的数据（线程本地存储或填充 padding）
> 3. 将高频写入的共享数据分散到不同 cacheline

---
