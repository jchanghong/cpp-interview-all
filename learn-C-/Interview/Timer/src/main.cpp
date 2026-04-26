#include "timer_scheduler.h"

#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

using timerdemo::TimerScheduler;

// 主函数：演示定时调度器在延迟后执行任务。
int main() {
    // 对象变量：创建定时调度器实例。
    TimerScheduler scheduler;
    // 原子变量：记录任务是否已经执行。
    std::atomic<int> executed(0);

    scheduler.ScheduleAfter(std::chrono::milliseconds(100), [&executed]() { executed.fetch_add(1); });
    std::this_thread::sleep_for(std::chrono::milliseconds(250));

    std::cout << "timer_demo executed=" << executed.load() << std::endl;
    return 0;
}
