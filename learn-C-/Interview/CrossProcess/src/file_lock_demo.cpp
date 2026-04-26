#include "ipc_file_lock.h"

#include <sys/wait.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>
#include <string>

using crossprocess::LockedFileWriter;

// 函数：让单个子进程连续写入多行带身份信息的文本。
static void RunFileWriter(const std::string& path, int worker_id) {
    // 对象变量：打开共享文件写入器。
    LockedFileWriter writer(path);

    // 整数变量：作为当前子进程的写入次数控制变量。
    int line_index = 0;
    for (; line_index < 3; ++line_index) {
        // 字符串变量：构造当前要写入的行内容。
        std::string line = "worker=" + std::to_string(worker_id) + " line=" + std::to_string(line_index);
        writer.AppendLine(line);
    }
}

// 主函数：演示两个进程使用 fcntl 文件锁安全写文件。
int main() {
    // 字符串变量：定义示例输出文件路径。
    const std::string path = "/tmp/cross_process_lock_demo.txt";
    LockedFileWriter::ClearFile(path);

    // 整数变量：作为 fork 循环的控制变量。
    int worker_id = 0;
    for (; worker_id < 2; ++worker_id) {
        // 整数变量：保存 fork 的返回结果。
        pid_t pid = fork();
        if (pid == 0) {
            RunFileWriter(path, worker_id);
            return EXIT_SUCCESS;
        }
    }

    // 整数变量：统计已经等待完成的子进程数量。
    int waited = 0;
    for (; waited < 2; ++waited) {
        wait(nullptr);
    }

    // 对象变量：由父进程读取最终文件内容。
    LockedFileWriter writer(path);
    std::cout << "file_lock_demo content:\n" << writer.ReadAll();
    return EXIT_SUCCESS;
}
