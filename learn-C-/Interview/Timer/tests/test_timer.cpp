#include "timer_scheduler.h"

#include <atomic>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>

using timerdemo::TimerScheduler;

// 函数：在条件不满足时抛出异常。
static void Require(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

// 函数：验证延迟任务在等待后会被触发。
static void TestSingleTask() {
    // 对象变量：创建被测定时调度器。
    TimerScheduler scheduler;
    // 原子变量：记录回调执行次数。
    std::atomic<int> executed(0);

    scheduler.ScheduleAfter(std::chrono::milliseconds(80), [&executed]() { executed.fetch_add(1); });
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    Require(executed.load() == 1, "timer single task mismatch");
}

// 函数：验证多个任务都能被依次触发。
static void TestMultipleTasks() {
    // 对象变量：创建被测定时调度器。
    TimerScheduler scheduler;
    // 原子变量：统计被执行的任务数量。
    std::atomic<int> executed(0);

    scheduler.ScheduleAfter(std::chrono::milliseconds(50), [&executed]() { executed.fetch_add(1); });
    scheduler.ScheduleAfter(std::chrono::milliseconds(90), [&executed]() { executed.fetch_add(1); });
    scheduler.ScheduleAfter(std::chrono::milliseconds(130), [&executed]() { executed.fetch_add(1); });
    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    Require(executed.load() == 3, "timer multiple task mismatch");
}

// 主函数：顺序执行所有定时器测试。
int main() {
    try {
        TestSingleTask();
        TestMultipleTasks();
        std::cout << "All timer tests passed." << std::endl;
        return EXIT_SUCCESS;
    } catch (const std::exception& ex) {
        std::cerr << "Timer test failed: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}
