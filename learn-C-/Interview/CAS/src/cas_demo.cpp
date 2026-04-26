#include "cas_primitives.h"

#include <iostream>
#include <thread>
#include <vector>

using casdemo::AtomicCounter;
using casdemo::LockedCounter;

// 函数：让多个线程并发递增给定的计数器对象。
template <typename CounterType>
static void RunIncrementBenchmark(CounterType& counter, int thread_count, int increments_per_thread) {
    // 容器变量：保存所有工作线程。
    std::vector<std::thread> workers;

    // 整数变量：作为线程创建循环的控制变量。
    int thread_index = 0;
    for (; thread_index < thread_count; ++thread_index) {
        workers.emplace_back([&counter, increments_per_thread]() {
            // 整数变量：作为单线程递增次数控制变量。
            int iteration = 0;
            for (; iteration < increments_per_thread; ++iteration) {
                counter.Increment();
            }
        });
    }

    // 引用变量：依次等待所有工作线程结束。
    for (std::thread& worker : workers) {
        worker.join();
    }
}

// 主函数：演示 CAS 计数器与加锁计数器的结果一致性。
int main() {
    // 整数变量：定义演示中的线程数量。
    const int thread_count = 4;
    // 整数变量：定义每个线程的递增次数。
    const int increments_per_thread = 20000;

    // 对象变量：创建 CAS 计数器实例。
    AtomicCounter atomic_counter(0);
    // 对象变量：创建加锁计数器实例。
    LockedCounter locked_counter(0);

    RunIncrementBenchmark(atomic_counter, thread_count, increments_per_thread);
    RunIncrementBenchmark(locked_counter, thread_count, increments_per_thread);

    std::cout << "cas_demo atomic_result=" << atomic_counter.Load()
              << " locked_result=" << locked_counter.Load() << std::endl;
    return 0;
}
