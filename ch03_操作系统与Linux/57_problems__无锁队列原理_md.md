# problems__无锁队列原理.md

### 无锁队列如何实现？ABA 问题是什么？

无锁队列基于 **CAS（Compare-And-Swap）** 原子操作，不使用互斥锁即可在多线程间安全传递数据。

**简单 SPSC 无锁队列（单生产者单消费者）：**

```cpp
template<typename T, size_t N>
class LockFreeQueue {
    std::array<T, N> buffer_;
    std::atomic<size_t> head_{0};  // 消费者下标
    std::atomic<size_t> tail_{0};  // 生产者下标

public:
    bool push(const T& item) {
        size_t t = tail_.load(std::memory_order_relaxed);
        size_t next = (t + 1) % N;
        if (next == head_.load(std::memory_order_acquire))
            return false;  // 队列满
        buffer_[t] = item;
        tail_.store(next, std::memory_order_release);
        return true;
    }
    bool pop(T& item) {
        size_t h = head_.load(std::memory_order_relaxed);
        if (h == tail_.load(std::memory_order_acquire))
            return false;  // 队列空
        item = buffer_[h];
        head_.store((h + 1) % N, std::memory_order_release);
        return true;
    }
};
```

**ABA 问题：**

线程 A 读到值 X，被调度走；线程 B 将 X 改为 Y，再改回 X；线程 A 回来执行 CAS 时，发现值仍然是 X，CAS 成功——但实际上状态已变化。

```cpp
// ABA 问题示意（使用带 tag 的指针避免）
// 解决：使用 double-width CAS（如 128 位 CAS）
struct TaggedPtr {
    Node* ptr;
    uintptr_t tag;  // 每次修改递增，防止 ABA
};
std::atomic<TaggedPtr> head_;  // 需平台支持 128 位 CAS
```

**面试标准答案：**

> 无锁队列通过 CAS 原子操作实现入队/出队，避免互斥锁的开销。MPSC（多生产者单消费者）需要额外处理多个生产者竞争 tail 的问题。
>
> ABA 问题：线程在 CAS 期间，目标值可能被其他线程改为其他值又改回原值，导致 CAS 误判"未变化"。解决方案是使用带版本号的指针（tagged pointer）或 double-width CAS。
>
> 生产环境推荐使用成熟的无锁库（如 `boost::lockfree::queue`、`folly::MPMCQueue`）而非手写。

---

# Interview__Network__README.md

> 说明：当前目录仅保留 README.md，正文为历史文档合并/知识点汇总；文中的旧文件名式标题仅表示章节来源，不代表当前目录存在对应文件。

> **注意**：原仓库已合并为各目录的 README.md，文中旧链接（`../` 开头的相对路径）已失效，以下仅保留知识点标题。
