#ifndef CAS_PRIMITIVES_H
#define CAS_PRIMITIVES_H

#include <atomic>
#include <cstddef>

namespace casdemo {

// 类：基于 atomic_flag 实现一个最小自旋锁。
class SpinLock {
public:
    // 构造函数：初始化锁状态为未持有。
    SpinLock();

    // 业务函数：循环尝试获取锁。
    void Lock();

    // 业务函数：释放当前锁。
    void Unlock();

private:
    // 成员变量：原子标志位表示锁是否被占用。
    std::atomic_flag flag_;
};

// 类：基于 compare_exchange_weak 实现无锁计数器。
class AtomicCounter {
public:
    // 构造函数：设置计数器初始值。
    explicit AtomicCounter(int initial_value = 0);

    // 业务函数：使用 CAS 自旋方式执行递增。
    int Increment();

    // 查询函数：读取当前计数值。
    int Load() const;

private:
    // 成员变量：保存共享计数值。
    std::atomic<int> value_;
};

// 类：在自旋锁保护下累加普通整数，便于与 CAS 思路对比。
class LockedCounter {
public:
    // 构造函数：初始化计数器初始值。
    explicit LockedCounter(int initial_value = 0);

    // 业务函数：在锁保护下执行递增。
    int Increment();

    // 查询函数：读取当前计数值。
    int Load() const;

private:
    // 成员变量：保护 value_ 的自旋锁。
    mutable SpinLock lock_;
    // 成员变量：保存普通整数值。
    int value_;
};

}  // namespace casdemo

#endif
