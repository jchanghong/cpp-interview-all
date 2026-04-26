#ifndef CROSS_PROCESS_IPC_FILE_LOCK_H
#define CROSS_PROCESS_IPC_FILE_LOCK_H

#include <string>

namespace crossprocess {

// 类：封装基于 fcntl 的跨进程文件锁写入器。
class LockedFileWriter {
public:
    // 构造函数：打开或创建目标文件。
    explicit LockedFileWriter(const std::string& path);

    // 析构函数：关闭文件描述符。
    ~LockedFileWriter();

    // 删除拷贝构造：避免文件描述符重复管理。
    LockedFileWriter(const LockedFileWriter&) = delete;

    // 删除拷贝赋值：避免文件描述符重复管理。
    LockedFileWriter& operator=(const LockedFileWriter&) = delete;

    // 业务函数：在排它锁保护下追加一行文本。
    void AppendLine(const std::string& line);

    // 查询函数：读取文件全部内容。
    std::string ReadAll() const;

    // 静态函数：清空指定路径上的文件内容。
    static void ClearFile(const std::string& path);

private:
    // 辅助函数：对整个文件区域加锁或解锁。
    void ApplyLock(short type) const;

    // 成员变量：保存文件路径。
    std::string path_;
    // 成员变量：保存打开后的文件描述符。
    int fd_;
};

}  // namespace crossprocess

#endif
