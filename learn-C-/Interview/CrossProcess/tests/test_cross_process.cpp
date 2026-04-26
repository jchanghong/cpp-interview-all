#include "ipc_file_lock.h"
#include "ipc_shared_counter.h"
#include "ipc_shared_queue.h"

#include <sys/wait.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using crossprocess::LockedFileWriter;
using crossprocess::SharedMemoryCounter;
using crossprocess::SharedMemoryQueue;

// 函数：当条件不成立时抛出异常，作为简单断言工具。
static void Require(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

// 函数：生成带当前进程号的唯一共享内存名字。
static std::string MakeUniqueSharedName(const std::string& prefix) {
    // 字符串变量：按 POSIX 共享内存命名规则构造对象名。
    std::string name = "/" + prefix + "_" + std::to_string(getpid());
    return name;
}

// 函数：校验共享计数器在多进程并发下的正确性。
static void TestSharedCounter() {
    // 字符串变量：保存本测试使用的共享内存名字。
    const std::string name = MakeUniqueSharedName("counter_test");
    SharedMemoryCounter::Remove(name);

    // 对象变量：由父进程创建共享计数器。
    SharedMemoryCounter counter(name, true);
    counter.Reset(0);

    // 整数变量：定义每个子进程的自增次数。
    const int increments_per_child = 2000;
    // 整数变量：定义测试中创建的子进程数量。
    const int child_count = 2;

    // 整数变量：作为 fork 循环的控制变量。
    int child_index = 0;
    for (; child_index < child_count; ++child_index) {
        // 整数变量：保存 fork 的返回结果。
        pid_t pid = fork();
        if (pid == 0) {
            // 对象变量：在子进程中打开共享计数器。
            SharedMemoryCounter child_counter(name, false);
            child_counter.IncrementBy(increments_per_child);
            _exit(EXIT_SUCCESS);
        }
    }

    // 整数变量：统计已经回收的子进程数量。
    int waited = 0;
    for (; waited < child_count; ++waited) {
        wait(nullptr);
    }

    // 整数变量：计算理论期望值。
    const int expected = increments_per_child * child_count;
    // 整数变量：读取最终共享计数值。
    const int actual = counter.Read();
    Require(actual == expected, "shared counter result mismatch");

    SharedMemoryCounter::Remove(name);
}

// 函数：校验共享队列的 FIFO 行为和同步机制。
static void TestSharedQueue() {
    // 字符串变量：保存本测试使用的共享内存名字。
    const std::string name = MakeUniqueSharedName("queue_test");
    SharedMemoryQueue::Remove(name);

    // 对象变量：由父进程创建共享队列。
    SharedMemoryQueue queue(name, true);

    // 整数变量：保存 fork 的返回结果。
    pid_t pid = fork();
    if (pid == 0) {
        // 对象变量：在子进程中打开共享队列。
        SharedMemoryQueue child_queue(name, false);
        child_queue.Push(7);
        child_queue.Push(11);
        child_queue.Push(13);
        _exit(EXIT_SUCCESS);
    }

    // 整数变量：缓存第一次出队结果。
    const int first = queue.Pop();
    // 整数变量：缓存第二次出队结果。
    const int second = queue.Pop();
    // 整数变量：缓存第三次出队结果。
    const int third = queue.Pop();

    wait(nullptr);

    Require(first == 7, "shared queue first element mismatch");
    Require(second == 11, "shared queue second element mismatch");
    Require(third == 13, "shared queue third element mismatch");

    SharedMemoryQueue::Remove(name);
}

// 函数：校验文件锁可以让多进程追加写入保持完整行。
static void TestFileLock() {
    // 字符串变量：定义本测试使用的临时文件路径。
    const std::string path = "/tmp/cross_process_file_lock_test.txt";
    LockedFileWriter::ClearFile(path);

    // 整数变量：作为 fork 循环的控制变量。
    int worker_id = 0;
    for (; worker_id < 2; ++worker_id) {
        // 整数变量：保存 fork 的返回结果。
        pid_t pid = fork();
        if (pid == 0) {
            // 对象变量：在子进程中打开文件写入器。
            LockedFileWriter writer(path);
            // 整数变量：控制每个子进程写入的行数。
            int line_index = 0;
            for (; line_index < 3; ++line_index) {
                // 字符串变量：构造一条完整记录行。
                std::string line = "worker=" + std::to_string(worker_id) + " line=" + std::to_string(line_index);
                writer.AppendLine(line);
            }
            _exit(EXIT_SUCCESS);
        }
    }

    // 整数变量：统计已经回收的子进程数量。
    int waited = 0;
    for (; waited < 2; ++waited) {
        wait(nullptr);
    }

    // 对象变量：读取最终文件内容用于断言。
    LockedFileWriter writer(path);
    // 字符串变量：保存全部文件内容。
    const std::string content = writer.ReadAll();
    // 输入流变量：按行扫描文本内容。
    std::istringstream stream(content);
    // 字符串变量：保存当前读取到的行。
    std::string line;
    // 整数变量：统计读取到的行数。
    int line_count = 0;
    // 布尔变量：记录是否读到了 worker=0 的内容。
    bool saw_worker_zero = false;
    // 布尔变量：记录是否读到了 worker=1 的内容。
    bool saw_worker_one = false;

    while (std::getline(stream, line)) {
        ++line_count;
        if (line.find("worker=0") != std::string::npos) {
            saw_worker_zero = true;
        }
        if (line.find("worker=1") != std::string::npos) {
            saw_worker_one = true;
        }
    }

    Require(line_count == 6, "file lock line count mismatch");
    Require(saw_worker_zero, "missing worker=0 lines");
    Require(saw_worker_one, "missing worker=1 lines");
}

// 主函数：顺序执行所有单元测试并输出结果。
int main() {
    try {
        TestSharedCounter();
        TestSharedQueue();
        TestFileLock();
        std::cout << "All cross-process tests passed." << std::endl;
        return EXIT_SUCCESS;
    } catch (const std::exception& ex) {
        std::cerr << "Test failed: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}
