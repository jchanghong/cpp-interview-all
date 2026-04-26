#include "cas_primitives.h"

#include <thread>

namespace casdemo {

// 构造函数：清空 atomic_flag 以表示未加锁状态。
SpinLock::SpinLock() : flag_(ATOMIC_FLAG_INIT) {}

// 业务函数：忙等直到 test_and_set 返回 false。
void SpinLock::Lock() {
    while (flag_.test_and_set(std::memory_order_acquire)) {
        std::this_thread::yield();
    }
}

// 业务函数：通过 clear 释放锁。
void SpinLock::Unlock() {
    flag_.clear(std::memory_order_release);
}

// 构造函数：把原子计数器设置为初始值。
AtomicCounter::AtomicCounter(int initial_value) : value_(initial_value) {}

// 业务函数：循环执行 CAS 直到递增成功。
int AtomicCounter::Increment() {
    // 整数变量：保存当前观察到的旧值。
    int expected = value_.load(std::memory_order_relaxed);
    while (!value_.compare_exchange_weak(expected,
                                         expected + 1,
                                         std::memory_order_release,
                                         std::memory_order_relaxed)) {
    }
    return expected + 1;
}

// 查询函数：返回原子变量中的当前值。
int AtomicCounter::Load() const {
    return value_.load(std::memory_order_acquire);
}

// 构造函数：初始化普通整数计数器。
LockedCounter::LockedCounter(int initial_value) : lock_(), value_(initial_value) {}

// 业务函数：在自旋锁保护下递增普通整数。
int LockedCounter::Increment() {
    lock_.Lock();
    ++value_;
    // 整数变量：缓存更新后的值，便于在解锁后返回。
    const int result = value_;
    lock_.Unlock();
    return result;
}

// 查询函数：在加锁状态下读取当前整数值。
int LockedCounter::Load() const {
    lock_.Lock();
    // 整数变量：缓存当前值，避免解锁后访问共享数据。
    const int result = value_;
    lock_.Unlock();
    return result;
}

}  // namespace casdemo
