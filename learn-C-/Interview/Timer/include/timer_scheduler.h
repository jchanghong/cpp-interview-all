#ifndef TIMER_SCHEDULER_H
#define TIMER_SCHEDULER_H

#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace timerdemo {

// 结构体：表示一个待执行的定时任务。
struct TimerTask {
    // 成员变量：记录任务的唯一编号。
    std::size_t id;
    // 成员变量：记录任务的触发时间点。
    std::chrono::steady_clock::time_point due_time;
    // 成员变量：保存实际执行的回调函数。
    std::function<void()> callback;
};

// 比较器：让 priority_queue 按最早到期时间排序。
struct TimerTaskCompare {
    // 运算符函数：比较两个任务的到期时间先后。
    bool operator()(const TimerTask& left, const TimerTask& right) const;
};

// 类：实现一个最小的后台定时任务调度器。
class TimerScheduler {
public:
    // 构造函数：启动后台调度线程。
    TimerScheduler();

    // 析构函数：停止后台线程并回收资源。
    ~TimerScheduler();

    // 删除拷贝构造：调度器不允许拷贝。
    TimerScheduler(const TimerScheduler&) = delete;

    // 删除拷贝赋值：调度器不允许拷贝。
    TimerScheduler& operator=(const TimerScheduler&) = delete;

    // 业务函数：提交一个延迟执行任务并返回任务编号。
    std::size_t ScheduleAfter(std::chrono::milliseconds delay, std::function<void()> callback);

private:
    // 辅助函数：后台线程循环等待并执行到期任务。
    void WorkerLoop();

    // 成员变量：保存按到期时间排序的任务堆。
    std::priority_queue<TimerTask, std::vector<TimerTask>, TimerTaskCompare> tasks_;
    // 成员变量：保护任务堆与停止标记。
    std::mutex mutex_;
    // 成员变量：用于唤醒后台调度线程。
    std::condition_variable condition_;
    // 成员变量：保存后台工作线程。
    std::thread worker_;
    // 成员变量：标记调度器是否停止。
    bool stop_;
    // 成员变量：生成递增任务编号。
    std::size_t next_id_;
};

}  // namespace timerdemo

#endif
