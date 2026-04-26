#include "ipc_shared_queue.h"

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cerrno>
#include <stdexcept>
#include <system_error>

namespace crossprocess {

// 构造函数：保存基础参数并执行资源初始化。
SharedMemoryQueue::SharedMemoryQueue(const std::string& name, bool create)
    : name_(name), owner_(create), fd_(-1), layout_(nullptr) {
    Initialize();
}

// 析构函数：释放共享内存映射并关闭文件描述符。
SharedMemoryQueue::~SharedMemoryQueue() {
    if (layout_ != nullptr) {
        // 大小变量：描述当前映射的字节数。
        const std::size_t layout_size = sizeof(SharedQueueLayout);
        munmap(layout_, layout_size);
    }

    if (fd_ >= 0) {
        close(fd_);
    }
}

// 业务函数：等待空槽后在共享队列尾部写入元素。
void SharedMemoryQueue::Push(int value) {
    sem_wait(&layout_->slots);
    pthread_mutex_lock(&layout_->mutex);
    layout_->buffer[layout_->tail] = value;
    layout_->tail = (layout_->tail + 1U) % kSharedQueueCapacity;
    pthread_mutex_unlock(&layout_->mutex);
    sem_post(&layout_->items);
}

// 业务函数：等待元素可用后从共享队列头部取出数据。
int SharedMemoryQueue::Pop() {
    // 整数变量：缓存当前弹出的消息值。
    int value = 0;
    sem_wait(&layout_->items);
    pthread_mutex_lock(&layout_->mutex);
    value = layout_->buffer[layout_->head];
    layout_->head = (layout_->head + 1U) % kSharedQueueCapacity;
    pthread_mutex_unlock(&layout_->mutex);
    sem_post(&layout_->slots);
    return value;
}

// 静态函数：删除具名 POSIX 共享内存对象。
void SharedMemoryQueue::Remove(const std::string& name) {
    shm_unlink(name.c_str());
}

// 初始化函数：完成共享队列所需的内核对象与同步原语初始化。
void SharedMemoryQueue::Initialize() {
    // 整数变量：根据是否创建选择不同打开模式。
    int flags = owner_ ? (O_CREAT | O_RDWR) : O_RDWR;
    fd_ = shm_open(name_.c_str(), flags, 0666);
    if (fd_ < 0) {
        throw std::system_error(errno, std::generic_category(), "shm_open failed");
    }

    if (owner_) {
        // 整数变量：保存调整共享内存大小的返回值。
        int truncate_result = ftruncate(fd_, static_cast<off_t>(sizeof(SharedQueueLayout)));
        if (truncate_result != 0) {
            throw std::system_error(errno, std::generic_category(), "ftruncate failed");
        }
    }

    // 指针变量：接收映射后的原始地址。
    void* mapped = mmap(nullptr,
                        sizeof(SharedQueueLayout),
                        PROT_READ | PROT_WRITE,
                        MAP_SHARED,
                        fd_,
                        0);
    if (mapped == MAP_FAILED) {
        throw std::system_error(errno, std::generic_category(), "mmap failed");
    }

    // 指针变量：把原始地址转换为布局体指针。
    layout_ = static_cast<SharedQueueLayout*>(mapped);

    if (owner_) {
        // 属性变量：配置进程共享互斥量属性。
        pthread_mutexattr_t mutex_attr;
        pthread_mutexattr_init(&mutex_attr);
        pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED);
        pthread_mutex_init(&layout_->mutex, &mutex_attr);
        pthread_mutexattr_destroy(&mutex_attr);
        sem_init(&layout_->slots, 1, kSharedQueueCapacity);
        sem_init(&layout_->items, 1, 0);
        layout_->head = 0;
        layout_->tail = 0;
    }
}

}  // namespace crossprocess
