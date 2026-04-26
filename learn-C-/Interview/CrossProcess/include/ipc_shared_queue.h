#ifndef CROSS_PROCESS_IPC_SHARED_QUEUE_H
#define CROSS_PROCESS_IPC_SHARED_QUEUE_H

#include <pthread.h>
#include <semaphore.h>

#include <cstddef>
#include <string>

namespace crossprocess {

// 常量：限定共享队列容量，便于示例保持固定布局。
constexpr std::size_t kSharedQueueCapacity = 8;

// 布局体：定义共享队列在共享内存中的存储结构。
struct SharedQueueLayout {
    // 信号量：记录当前剩余空槽数量。
    sem_t slots;
    // 信号量：记录当前已有元素数量。
    sem_t items;
    // 互斥量：保护 head、tail 和 buffer 的并发访问。
    pthread_mutex_t mutex;
    // 索引：记录下一个出队位置。
    std::size_t head;
    // 索引：记录下一个入队位置。
    std::size_t tail;
    // 数组：保存队列中的整数消息。
    int buffer[kSharedQueueCapacity];
};

// 类：封装基于共享内存的有界队列。
class SharedMemoryQueue {
public:
    // 构造函数：创建或打开一个共享队列对象。
    SharedMemoryQueue(const std::string& name, bool create);

    // 析构函数：释放映射并关闭句柄。
    ~SharedMemoryQueue();

    // 删除拷贝构造：避免资源重复释放。
    SharedMemoryQueue(const SharedMemoryQueue&) = delete;

    // 删除拷贝赋值：避免资源重复释放。
    SharedMemoryQueue& operator=(const SharedMemoryQueue&) = delete;

    // 业务函数：向共享队列压入一个整数。
    void Push(int value);

    // 业务函数：从共享队列弹出一个整数。
    int Pop();

    // 静态函数：删除同名共享内存对象。
    static void Remove(const std::string& name);

private:
    // 初始化函数：完成共享队列的创建、映射和原语初始化。
    void Initialize();

    // 成员变量：保存共享内存对象名。
    std::string name_;
    // 成员变量：标记当前实例是否为创建者。
    bool owner_;
    // 成员变量：保存共享内存文件描述符。
    int fd_;
    // 成员变量：保存映射后的队列布局指针。
    SharedQueueLayout* layout_;
};

}  // namespace crossprocess

#endif
