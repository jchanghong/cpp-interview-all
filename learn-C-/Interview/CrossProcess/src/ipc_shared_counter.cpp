#include "ipc_shared_counter.h"

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <system_error>

namespace crossprocess {

// 构造函数：保存基础参数并执行资源初始化。
SharedMemoryCounter::SharedMemoryCounter(const std::string& name, bool create)
    : name_(name), owner_(create), fd_(-1), layout_(nullptr) {
    Initialize();
}

// 析构函数：释放共享内存映射并关闭文件描述符。
SharedMemoryCounter::~SharedMemoryCounter() {
    if (layout_ != nullptr) {
        // 大小变量：描述当前映射的字节数。
        const std::size_t layout_size = sizeof(SharedCounterLayout);
        munmap(layout_, layout_size);
    }

    if (fd_ >= 0) {
        close(fd_);
    }
}

// 业务函数：在进程共享互斥量保护下多次累加计数器。
void SharedMemoryCounter::IncrementBy(int times) {
    if (times < 0) {
        throw std::invalid_argument("times must be non-negative");
    }

    // 整数变量：作为循环控制次数。
    int iteration = 0;
    for (; iteration < times; ++iteration) {
        pthread_mutex_lock(&layout_->mutex);
        ++layout_->value;
        pthread_mutex_unlock(&layout_->mutex);
    }
}

// 业务函数：在互斥量保护下设置新值。
void SharedMemoryCounter::Reset(int value) {
    pthread_mutex_lock(&layout_->mutex);
    layout_->value = value;
    pthread_mutex_unlock(&layout_->mutex);
}

// 查询函数：在互斥量保护下读取共享计数值。
int SharedMemoryCounter::Read() const {
    // 整数变量：缓存读取到的共享值。
    int current_value = 0;
    pthread_mutex_lock(&layout_->mutex);
    current_value = layout_->value;
    pthread_mutex_unlock(&layout_->mutex);
    return current_value;
}

// 静态函数：删除具名 POSIX 共享内存对象。
void SharedMemoryCounter::Remove(const std::string& name) {
    shm_unlink(name.c_str());
}

// 初始化函数：完成 shm_open、ftruncate、mmap 与互斥量初始化。
void SharedMemoryCounter::Initialize() {
    // 整数变量：根据是否创建选择不同打开模式。
    int flags = owner_ ? (O_CREAT | O_RDWR) : O_RDWR;
    fd_ = shm_open(name_.c_str(), flags, 0666);
    if (fd_ < 0) {
        throw std::system_error(errno, std::generic_category(), "shm_open failed");
    }

    if (owner_) {
        // 整数变量：保存调整共享内存大小的返回值。
        int truncate_result = ftruncate(fd_, static_cast<off_t>(sizeof(SharedCounterLayout)));
        if (truncate_result != 0) {
            throw std::system_error(errno, std::generic_category(), "ftruncate failed");
        }
    }

    // 指针变量：接收映射后的原始地址。
    void* mapped = mmap(nullptr,
                        sizeof(SharedCounterLayout),
                        PROT_READ | PROT_WRITE,
                        MAP_SHARED,
                        fd_,
                        0);
    if (mapped == MAP_FAILED) {
        throw std::system_error(errno, std::generic_category(), "mmap failed");
    }

    // 指针变量：把原始地址转换为布局体指针。
    layout_ = static_cast<SharedCounterLayout*>(mapped);

    if (owner_) {
        // 属性变量：配置进程共享互斥量属性。
        pthread_mutexattr_t mutex_attr;
        pthread_mutexattr_init(&mutex_attr);
        pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED);
        pthread_mutex_init(&layout_->mutex, &mutex_attr);
        pthread_mutexattr_destroy(&mutex_attr);
        layout_->value = 0;
    }
}

}  // namespace crossprocess
