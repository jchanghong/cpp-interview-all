#include "thread_pool.h"

namespace threadpooldemo {

// 构造函数：创建指定数量的后台工作线程。
ThreadPool::ThreadPool(std::size_t thread_count) : workers_(), tasks_(), mutex_(), condition_(), stop_(false) {
    // 大小变量：作为线程创建循环的控制变量。
    std::size_t index = 0;
    for (; index < thread_count; ++index) {
        workers_.emplace_back([this]() { WorkerLoop(); });
    }
}

// 析构函数：通知线程退出并等待全部线程结束。
ThreadPool::~ThreadPool() {
    {
        // 锁变量：保护停止标记更新。
        std::lock_guard<std::mutex> guard(mutex_);
        stop_ = true;
    }

    condition_.notify_all();

    // 引用变量：逐个等待工作线程结束。
    for (std::thread& worker : workers_) {
        worker.join();
    }
}

// 辅助函数：循环等待任务并在退出条件满足时结束。
void ThreadPool::WorkerLoop() {
    while (true) {
        // 对象变量：缓存当前待执行任务。
        std::function<void()> task;

        {
            // 锁变量：保护对队列和停止标记的访问。
            std::unique_lock<std::mutex> lock(mutex_);
            condition_.wait(lock, [this]() { return stop_ || !tasks_.empty(); });

            if (stop_ && tasks_.empty()) {
                return;
            }

            task = std::move(tasks_.front());
            tasks_.pop();
        }

        task();
    }
}

}  // namespace threadpooldemo
