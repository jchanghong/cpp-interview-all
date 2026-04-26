#include "thread_pool.h"

#include <iostream>
#include <vector>

using threadpooldemo::ThreadPool;

// 主函数：演示线程池提交多个整数计算任务。
int main() {
    // 对象变量：创建一个拥有 4 个线程的线程池。
    ThreadPool pool(4);
    // 容器变量：保存所有 future 结果。
    std::vector<std::future<int>> results;

    // 整数变量：作为任务提交循环的控制变量。
    int value = 1;
    for (; value <= 5; ++value) {
        results.push_back(pool.Submit([value]() { return value * value; }));
    }

    // 整数变量：累计所有任务返回结果。
    int sum = 0;
    // 引用变量：依次提取每个 future 的结果。
    for (std::future<int>& result : results) {
        sum += result.get();
    }

    std::cout << "thread_pool_demo result_sum=" << sum << std::endl;
    return 0;
}
