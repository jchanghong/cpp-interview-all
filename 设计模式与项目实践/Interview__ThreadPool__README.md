# ThreadPool

这个工程实现了一个固定大小线程池，覆盖面试中最常见的任务提交、工作线程循环、条件变量唤醒和 `future` 结果获取。

## 构建

```bash
cd /home/wj/Code/C++/Interview/ThreadPool
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

## 目标

- `thread_pool_demo`：提交平方计算任务并汇总结果。
- `thread_pool_tests`：验证有返回值和无返回值任务。

## 面试知识点

- 线程池的价值是复用线程，降低频繁创建销毁线程的成本。
- 核心结构一般包括任务队列、工作线程集合、互斥锁、条件变量、停止标记。
- 常见面试追问：队列是否有界、如何优雅关闭、任务异常如何传播、是否支持工作窃取。
- `std::packaged_task + future` 是一个很经典的最小实现方案。

