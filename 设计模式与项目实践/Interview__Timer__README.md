# Timer

这个工程实现了一个最小定时调度器，适合面试里讲延迟任务、最小堆、后台线程和条件变量等待。

## 构建

```bash
cd /home/wj/Code/C++/Interview/Timer
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

## 目标

- `timer_demo`：提交一个 100ms 的延迟任务并观察执行结果。
- `timer_tests`：验证单个任务和多个任务的触发行为。

## 面试知识点

- 定时器常见实现依赖最小堆、时间轮或有序链表。
- 最小堆适合中等规模任务调度，实现直观。
- 条件变量可以让后台线程睡到“最近一个到期点”，避免忙等。
- 高频追问：周期任务、取消任务、重复调度、精度误差、系统时钟回拨。

