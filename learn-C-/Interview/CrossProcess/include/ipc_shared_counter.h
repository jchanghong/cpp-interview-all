#ifndef CROSS_PROCESS_IPC_SHARED_COUNTER_H
#define CROSS_PROCESS_IPC_SHARED_COUNTER_H

#include <pthread.h>

#include <string>

namespace crossprocess {

// 布局体：定义共享计数器在共享内存中的存储结构。
struct SharedCounterLayout {
    // 互斥量：用于跨进程保护计数器更新。
    pthread_mutex_t mutex;
    // 数值：保存当前计数结果。
    int value;
};

// 类：封装基于 POSIX 共享内存的跨进程计数器。
class SharedMemoryCounter {
public:
    // 构造函数：创建或打开一个共享计数器对象。
    SharedMemoryCounter(const std::string& name, bool create);

    // 析构函数：释放映射并关闭文件描述符。
    ~SharedMemoryCounter();

    // 删除拷贝构造：避免重复管理同一块共享资源。
    SharedMemoryCounter(const SharedMemoryCounter&) = delete;

    // 删除拷贝赋值：避免重复管理同一块共享资源。
    SharedMemoryCounter& operator=(const SharedMemoryCounter&) = delete;

    // 业务函数：把计数器累加指定次数。
    void IncrementBy(int times);

    // 业务函数：重置共享计数器的值。
    void Reset(int value);

    // 查询函数：读取当前共享计数值。
    int Read() const;

    // 静态函数：删除同名共享内存对象。
    static void Remove(const std::string& name);

private:
    // 初始化函数：完成共享内存创建、映射和初始化。
    void Initialize();

    // 成员变量：保存共享内存对象名。
    std::string name_;
    // 成员变量：标记当前实例是否负责初始化对象。
    bool owner_;
    // 成员变量：保存共享内存文件描述符。
    int fd_;
    // 成员变量：保存映射后的共享布局指针。
    SharedCounterLayout* layout_;
};

}  // namespace crossprocess

#endif
