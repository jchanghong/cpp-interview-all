#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <condition_variable>
#include <cstddef>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

namespace threadpooldemo {

// 类：实现一个固定大小的基础线程池。
class ThreadPool {
public:
    // 构造函数：启动指定数量的工作线程。
    explicit ThreadPool(std::size_t thread_count);

    // 析构函数：停止线程池并回收所有线程。
    ~ThreadPool();

    // 删除拷贝构造：线程池不允许被拷贝。
    ThreadPool(const ThreadPool&) = delete;

    // 删除拷贝赋值：线程池不允许被拷贝。
    ThreadPool& operator=(const ThreadPool&) = delete;

    // 模板函数：提交任务并返回 future 结果。
    template <typename Function, typename... Args>
    auto Submit(Function&& function, Args&&... args)
        -> std::future<typename std::invoke_result<Function, Args...>::type> {
        // 类型别名：表示任务最终返回值类型。
        using ResultType = typename std::invoke_result<Function, Args...>::type;
        // 对象变量：封装可调用对象与参数。
        auto packaged_task = std::make_shared<std::packaged_task<ResultType()>>(
            std::bind(std::forward<Function>(function), std::forward<Args>(args)...));
        // 对象变量：获取 future 用于接收任务结果。
        std::future<ResultType> result = packaged_task->get_future();

        {
            // 锁变量：保护任务队列。
            std::lock_guard<std::mutex> guard(mutex_);
            tasks_.push([packaged_task]() { (*packaged_task)(); });
        }

        condition_.notify_one();
        return result;
    }

private:
    // 辅助函数：工作线程循环获取并执行任务。
    void WorkerLoop();

    // 成员变量：保存所有工作线程。
    std::vector<std::thread> workers_;
    // 成员变量：保存待执行任务队列。
    std::queue<std::function<void()>> tasks_;
    // 成员变量：保护任务队列和停止标记。
    std::mutex mutex_;
    // 成员变量：用于唤醒等待中的工作线程。
    std::condition_variable condition_;
    // 成员变量：标记线程池是否已经停止。
    bool stop_;
};

}  // namespace threadpooldemo

#endif
