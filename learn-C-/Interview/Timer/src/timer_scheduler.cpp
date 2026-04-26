#include "timer_scheduler.h"

namespace timerdemo {

// 运算符函数：让最早到期的任务排在堆顶。
bool TimerTaskCompare::operator()(const TimerTask& left, const TimerTask& right) const {
    return left.due_time > right.due_time;
}

// 构造函数：初始化成员并启动后台线程。
TimerScheduler::TimerScheduler() : tasks_(), mutex_(), condition_(), worker_(), stop_(false), next_id_(1) {
    worker_ = std::thread([this]() { WorkerLoop(); });
}

// 析构函数：通知后台线程退出并等待其结束。
TimerScheduler::~TimerScheduler() {
    {
        // 锁变量：保护停止标记更新。
        std::lock_guard<std::mutex> guard(mutex_);
        stop_ = true;
    }

    condition_.notify_all();
    worker_.join();
}

// 业务函数：计算到期时间并将任务压入优先队列。
std::size_t TimerScheduler::ScheduleAfter(std::chrono::milliseconds delay, std::function<void()> callback) {
    // 锁变量：保护队列写入和编号分配。
    std::lock_guard<std::mutex> guard(mutex_);
    // 大小变量：为当前任务分配新的编号。
    const std::size_t id = next_id_++;
    // 结构体变量：构造待调度任务对象。
    TimerTask task{id, std::chrono::steady_clock::now() + delay, std::move(callback)};
    tasks_.push(std::move(task));
    condition_.notify_one();
    return id;
}

// 辅助函数：后台等待最早到期任务并执行其回调。
void TimerScheduler::WorkerLoop() {
    while (true) {
        // 对象变量：缓存即将执行的任务。
        TimerTask task;
        // 布尔变量：标记本轮是否取到了可执行任务。
        bool should_run = false;

        {
            // 锁变量：保护队列和停止标记。
            std::unique_lock<std::mutex> lock(mutex_);

            if (tasks_.empty() && !stop_) {
                condition_.wait(lock, [this]() { return stop_ || !tasks_.empty(); });
            }

            if (stop_ && tasks_.empty()) {
                return;
            }

            if (!tasks_.empty()) {
                // 时间变量：读取当前时间点。
                const auto now = std::chrono::steady_clock::now();
                // 时间变量：读取堆顶任务到期时间。
                const auto due = tasks_.top().due_time;

                if (due <= now) {
                    task = std::move(tasks_.top());
                    tasks_.pop();
                    should_run = true;
                } else {
                    condition_.wait_until(lock, due);
                    continue;
                }
            }
        }

        if (should_run) {
            task.callback();
        }
    }
}

}  // namespace timerdemo
