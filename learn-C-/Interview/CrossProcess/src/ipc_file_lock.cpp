#include "ipc_file_lock.h"

#include <fcntl.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <sstream>
#include <stdexcept>
#include <system_error>

namespace crossprocess {

// 构造函数：以读写追加模式打开目标文件。
LockedFileWriter::LockedFileWriter(const std::string& path) : path_(path), fd_(-1) {
    fd_ = open(path_.c_str(), O_CREAT | O_RDWR | O_APPEND, 0666);
    if (fd_ < 0) {
        throw std::system_error(errno, std::generic_category(), "open failed");
    }
}

// 析构函数：关闭已打开的文件描述符。
LockedFileWriter::~LockedFileWriter() {
    if (fd_ >= 0) {
        close(fd_);
    }
}

// 业务函数：先加写锁，再向文件追加一行文本。
void LockedFileWriter::AppendLine(const std::string& line) {
    ApplyLock(F_WRLCK);

    // 字符串变量：构造带换行的写入内容。
    std::string payload = line + "\n";
    // 指针变量：保存待写入内容的首地址。
    const char* data = payload.c_str();
    // 大小变量：记录本次已经写入的字节数。
    std::size_t written = 0;

    while (written < payload.size()) {
        // 整数变量：保存单次 write 的返回值。
        ssize_t result = write(fd_, data + written, payload.size() - written);
        if (result < 0) {
            ApplyLock(F_UNLCK);
            throw std::system_error(errno, std::generic_category(), "write failed");
        }
        written += static_cast<std::size_t>(result);
    }

    ApplyLock(F_UNLCK);
}

// 查询函数：从文件头读取全部文本内容。
std::string LockedFileWriter::ReadAll() const {
    // 整数变量：把文件偏移重置到起始位置。
    off_t seek_result = lseek(fd_, 0, SEEK_SET);
    if (seek_result < 0) {
        throw std::system_error(errno, std::generic_category(), "lseek failed");
    }

    // 输出流变量：按块拼接读取内容。
    std::ostringstream buffer;

    while (true) {
        // 数组变量：承接一次读取到的字节块。
        char chunk[256];
        // 整数变量：保存本次 read 的返回值。
        ssize_t read_size = read(fd_, chunk, sizeof(chunk));
        if (read_size < 0) {
            throw std::system_error(errno, std::generic_category(), "read failed");
        }
        if (read_size == 0) {
            break;
        }
        buffer.write(chunk, read_size);
    }

    return buffer.str();
}

// 静态函数：用截断模式清空指定文件。
void LockedFileWriter::ClearFile(const std::string& path) {
    // 整数变量：保存临时打开的文件描述符。
    int fd = open(path.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0666);
    if (fd < 0) {
        throw std::system_error(errno, std::generic_category(), "open failed");
    }
    close(fd);
}

// 辅助函数：对整个文件施加或释放记录锁。
void LockedFileWriter::ApplyLock(short type) const {
    // 结构体变量：描述加锁范围与锁类型。
    flock lock_info;
    lock_info.l_type = type;
    lock_info.l_whence = SEEK_SET;
    lock_info.l_start = 0;
    lock_info.l_len = 0;
    lock_info.l_pid = 0;

    // 整数变量：保存 fcntl 的返回值。
    int result = fcntl(fd_, F_SETLKW, &lock_info);
    if (result < 0) {
        throw std::system_error(errno, std::generic_category(), "fcntl lock failed");
    }
}

}  // namespace crossprocess
