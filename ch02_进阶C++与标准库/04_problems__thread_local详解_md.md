# problems__thread_local详解.md

### thread_local 是什么？和 static 有什么区别？

`thread_local` 是 C++11 引入的存储期关键字，**每个线程拥有变量的独立实例**。生命周期等于线程的生命周期。

| 特性 | `static` | `thread_local` |
|------|---------|---------------|
| 实例数 | 整个程序 1 份 | 每个线程 1 份 |
| 初始化 | 首次执行到时 | 每个线程首次访问时 |
| 可见性 | 遵循声明作用域 | 每个线程独立可见 |
| 销毁 | 程序结束时 | 线程退出时 |

```cpp
thread_local int counter = 0;  // 每个线程有自己的 counter

void increment() {
    counter++;  // 线程安全，操作的是本线程的副本
    std::cout << std::this_thread::get_id() << ": " << counter << "\n";
}
```

**典型使用场景：**
- 替代全局 `errno`（每个线程独立错误码）
- 线程级缓存（避免频繁加锁）
- 随机数生成器（每个线程维护独立状态）
- 数据库连接池的线程局部连接

**面试标准答案：**

> `thread_local` 使得变量在每个线程中有独立副本，生命周期与线程一致。与 `static` 的关键区别：`static` 全局共享一份，多线程访问需要同步；`thread_local` 天然线程安全，每个线程操作自己的副本。底层实现通常通过 TLS（Thread Local Storage）机制，在 TLS 段分配空间。

---
