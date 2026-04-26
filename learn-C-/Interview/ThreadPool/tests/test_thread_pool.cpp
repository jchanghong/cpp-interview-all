#include "thread_pool.h"

#include <atomic>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using threadpooldemo::ThreadPool;

// 函数：在条件不满足时抛出异常。
static void Require(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

// 函数：验证线程池可以执行多个返回值任务。
static void TestTaskResults() {
    // 对象变量：创建被测线程池。
    ThreadPool pool(3);
    // 容器变量：保存 future 结果。
    std::vector<std::future<int>> results;

    // 整数变量：作为任务提交循环的控制变量。
    int value = 1;
    for (; value <= 4; ++value) {
        results.push_back(pool.Submit([value]() { return value + 10; }));
    }

    // 整数变量：累计 future 结果。
    int sum = 0;
    // 引用变量：依次提取每个 future 的结果。
    for (std::future<int>& result : results) {
        sum += result.get();
    }

    Require(sum == 50, "thread pool task result mismatch");
}

// 函数：验证线程池可以执行无返回值任务。
static void TestVoidTasks() {
    // 对象变量：创建被测线程池。
    ThreadPool pool(2);
    // 原子变量：统计完成任务数量。
    std::atomic<int> completed(0);
    // 容器变量：保存 future 句柄。
    std::vector<std::future<void>> results;

    // 整数变量：作为任务提交循环的控制变量。
    int index = 0;
    for (; index < 6; ++index) {
        results.push_back(pool.Submit([&completed]() { completed.fetch_add(1); }));
    }

    // 引用变量：依次等待每个 future 完成。
    for (std::future<void>& result : results) {
        result.get();
    }

    Require(completed.load() == 6, "thread pool void task mismatch");
}

// 主函数：顺序执行所有线程池测试。
int main() {
    try {
        TestTaskResults();
        TestVoidTasks();
        std::cout << "All thread pool tests passed." << std::endl;
        return EXIT_SUCCESS;
    } catch (const std::exception& ex) {
        std::cerr << "Thread pool test failed: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}
