#include "ipc_shared_queue.h"

#include <sys/wait.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>
#include <string>

using crossprocess::SharedMemoryQueue;

// 函数：在子进程中向共享队列顺序投递消息。
static void RunProducer(const std::string& name) {
    // 对象变量：打开已经创建好的共享队列。
    SharedMemoryQueue queue(name, false);

    // 整数变量：保存当前要发送的值。
    int value = 1;
    for (; value <= 5; ++value) {
        queue.Push(value * 10);
    }
}

// 主函数：演示父子进程通过共享队列进行消息传递。
int main() {
    // 字符串变量：定义共享队列对应的对象名。
    const std::string name = "/cross_process_queue_demo";
    SharedMemoryQueue::Remove(name);

    // 对象变量：由父进程创建共享队列。
    SharedMemoryQueue queue(name, true);

    // 整数变量：保存 fork 返回结果。
    pid_t pid = fork();
    if (pid == 0) {
        RunProducer(name);
        return EXIT_SUCCESS;
    }

    // 整数变量：累计父进程取出的消息总和。
    int sum = 0;
    // 整数变量：统计当前已经消费的消息数量。
    int consumed = 0;
    for (; consumed < 5; ++consumed) {
        sum += queue.Pop();
    }

    wait(nullptr);
    std::cout << "shared_queue_demo received_sum=" << sum << std::endl;

    SharedMemoryQueue::Remove(name);
    return sum == 150 ? EXIT_SUCCESS : EXIT_FAILURE;
}
