#include "ipc_shared_counter.h"

#include <sys/wait.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>
#include <string>

using crossprocess::SharedMemoryCounter;

// 函数：创建子进程执行累加逻辑。
static void RunChildIncrementer(const std::string& name, int times) {
    // 对象变量：打开已经创建好的共享计数器。
    SharedMemoryCounter counter(name, false);
    counter.IncrementBy(times);
}

// 主函数：演示两个子进程共享同一个计数器。
int main() {
    // 字符串变量：定义共享内存对象名。
    const std::string name = "/cross_process_counter_demo";
    SharedMemoryCounter::Remove(name);

    // 对象变量：由父进程创建共享计数器。
    SharedMemoryCounter counter(name, true);
    counter.Reset(0);

    // 整数变量：定义每个子进程执行的累加次数。
    const int increments_per_child = 5000;
    // 整数变量：定义总共创建的子进程数量。
    const int child_count = 2;

    // 整数变量：作为 fork 循环的控制变量。
    int child_index = 0;
    for (; child_index < child_count; ++child_index) {
        // 整数变量：保存 fork 的返回结果。
        pid_t pid = fork();
        if (pid == 0) {
            RunChildIncrementer(name, increments_per_child);
            return EXIT_SUCCESS;
        }
    }

    // 整数变量：作为 wait 循环的控制变量。
    int waited = 0;
    for (; waited < child_count; ++waited) {
        wait(nullptr);
    }

    // 整数变量：记录理论上的期望值。
    const int expected = increments_per_child * child_count;
    // 整数变量：读取共享计数器中的最终值。
    const int actual = counter.Read();

    std::cout << "shared_counter_demo expected=" << expected << " actual=" << actual << std::endl;

    SharedMemoryCounter::Remove(name);
    return actual == expected ? EXIT_SUCCESS : EXIT_FAILURE;
}
