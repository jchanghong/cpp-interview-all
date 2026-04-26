#include "cas_primitives.h"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

using casdemo::AtomicCounter;
using casdemo::LockedCounter;
using casdemo::SpinLock;

// 函数：在条件不满足时抛出异常。
static void Require(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

// 函数：验证自旋锁可以保护共享整数的并发访问。
static void TestSpinLock() {
    // 对象变量：创建被测自旋锁实例。
    SpinLock lock;
    // 整数变量：保存被多个线程共享访问的普通整数。
    int value = 0;
    // 容器变量：保存测试线程。
    std::vector<std::thread> workers;

    // 整数变量：作为线程创建循环的控制变量。
    int thread_index = 0;
    for (; thread_index < 4; ++thread_index) {
        workers.emplace_back([&lock, &value]() {
            // 整数变量：作为递增循环控制变量。
            int iteration = 0;
            for (; iteration < 3000; ++iteration) {
                lock.Lock();
                ++value;
                lock.Unlock();
            }
        });
    }

    // 引用变量：顺序等待每个线程结束。
    for (std::thread& worker : workers) {
        worker.join();
    }

    Require(value == 12000, "spin lock protected value mismatch");
}

// 函数：验证 CAS 计数器的最终结果正确。
static void TestAtomicCounter() {
    // 对象变量：创建被测 CAS 计数器。
    AtomicCounter counter(0);
    // 容器变量：保存测试线程。
    std::vector<std::thread> workers;

    // 整数变量：作为线程创建循环的控制变量。
    int thread_index = 0;
    for (; thread_index < 4; ++thread_index) {
        workers.emplace_back([&counter]() {
            // 整数变量：作为递增循环控制变量。
            int iteration = 0;
            for (; iteration < 5000; ++iteration) {
                counter.Increment();
            }
        });
    }

    // 引用变量：顺序等待每个线程结束。
    for (std::thread& worker : workers) {
        worker.join();
    }

    Require(counter.Load() == 20000, "atomic counter mismatch");
}

// 函数：验证加锁计数器的最终结果正确。
static void TestLockedCounter() {
    // 对象变量：创建被测加锁计数器。
    LockedCounter counter(0);
    // 容器变量：保存测试线程。
    std::vector<std::thread> workers;

    // 整数变量：作为线程创建循环的控制变量。
    int thread_index = 0;
    for (; thread_index < 3; ++thread_index) {
        workers.emplace_back([&counter]() {
            // 整数变量：作为递增循环控制变量。
            int iteration = 0;
            for (; iteration < 4000; ++iteration) {
                counter.Increment();
            }
        });
    }

    // 引用变量：顺序等待每个线程结束。
    for (std::thread& worker : workers) {
        worker.join();
    }

    Require(counter.Load() == 12000, "locked counter mismatch");
}

// 主函数：顺序执行所有 CAS 相关测试。
int main() {
    try {
        TestSpinLock();
        TestAtomicCounter();
        TestLockedCounter();
        std::cout << "All CAS tests passed." << std::endl;
        return EXIT_SUCCESS;
    } catch (const std::exception& ex) {
        std::cerr << "CAS test failed: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}
