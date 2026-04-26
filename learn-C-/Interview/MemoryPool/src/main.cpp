#include "memory_pool_extensions.h"

#include <array>
#include <exception>
#include <iostream>
#include <string>
#include <thread>
#include <utility>
#include <vector>

// 示例结构体：演示对象池中的业务对象。
struct GameEntity {
    // 成员变量：对象编号。
    int id;

    // 成员变量：对象名称。
    std::string name;

    // 构造函数：初始化演示对象。
    GameEntity(int entity_id, std::string entity_name)
        : id(entity_id), name(std::move(entity_name)) {}

    // 成员函数：输出对象信息。
    void Print() const {
        std::cout << "GameEntity{id=" << id << ", name=" << name << "}\n";
    }
};

// 演示函数：展示基础对象池与自动归还包装器。
void RunBasicObjectPoolDemo() {
    std::cout << "[1] Basic object pool and auto-return wrapper\n";

    // 对象变量：创建一个每批次可容纳 4 个对象的对象池。
    ObjectPool<GameEntity> entity_pool(4);

    // 智能指针变量：创建离开作用域后自动归还的对象。
    UniquePoolPtr<GameEntity> player = entity_pool.CreateUnique(1, "Player");

    // 智能指针变量：创建第二个自动归还对象。
    UniquePoolPtr<GameEntity> enemy = entity_pool.CreateUnique(2, "Enemy");

    player->Print();
    enemy->Print();

    std::cout << "Free blocks after two allocations: " << entity_pool.GetFreeCount() << "\n";

    enemy.reset();
    std::cout << "Free blocks after resetting one smart pointer: " << entity_pool.GetFreeCount() << "\n\n";
}

// 演示函数：展示线程安全对象池的并发使用方式。
void RunThreadSafePoolDemo() {
    std::cout << "[2] Thread-safe object pool\n";

    // 对象变量：创建线程安全对象池。
    ThreadSafeObjectPool<GameEntity> entity_pool(16);

    // 常量变量：线程数量。
    const std::size_t thread_count = 4;

    // 常量变量：每个线程执行的创建次数。
    const std::size_t iterations_per_thread = 1000;

    // 容器变量：保存工作线程对象。
    std::vector<std::thread> workers;
    workers.reserve(thread_count);

    // 数量变量：启动多个线程并发申请与释放对象。
    for (std::size_t thread_index = 0; thread_index < thread_count; ++thread_index) {
        workers.emplace_back([&entity_pool, thread_index, iterations_per_thread]() {
            // 数量变量：执行当前线程负责的循环次数。
            for (std::size_t iteration = 0; iteration < iterations_per_thread; ++iteration) {
                // 智能指针变量：创建一个自动归还的对象。
                ThreadSafeUniquePoolPtr<GameEntity> entity = entity_pool.CreateUnique(
                    static_cast<int>(thread_index * iterations_per_thread + iteration),
                    "WorkerEntity");
            }
        });
    }

    // 引用变量：等待所有线程结束。
    for (std::thread& worker : workers) {
        worker.join();
    }

    std::cout << "Free blocks after threaded workload: " << entity_pool.GetFreeCount() << "\n\n";
}

// 演示函数：展示调试版内存池的越界检测能力。
void RunDebugPoolDemo() {
    std::cout << "[3] Debug fixed memory pool with guard bytes\n";

    // 对象变量：创建支持头尾守卫区的调试池。
    DebugFixedMemoryPool debug_pool(32, 4);

    // 指针变量：申请一块 32 字节的用户区。
    void* user_memory = debug_pool.Allocate();

    // 字节指针变量：把内存视为字节数组方便演示。
    char* user_bytes = static_cast<char*>(user_memory);
    user_bytes[0] = 'O';
    user_bytes[31] = 'K';

    debug_pool.Deallocate(user_memory);
    std::cout << "Active allocations after release: " << debug_pool.GetActiveAllocationCount() << "\n\n";
}

// 演示函数：展示变长内存池如何按尺寸类别分配。
void RunVariablePoolDemo() {
    std::cout << "[4] Variable-size memory pool\n";

    // 容器变量：定义分离适配使用的尺寸类别。
    std::vector<std::size_t> size_classes{16, 32, 64, 128, 256};

    // 对象变量：创建变长内存池。
    VariableMemoryPool variable_pool(size_classes, 4);

    // 常量数组变量：准备多组不同大小的申请请求。
    const std::array<std::size_t, 4> request_sizes{12, 48, 120, 300};

    // 容器变量：保存申请到的指针，便于后续释放。
    std::vector<void*> allocations;
    allocations.reserve(request_sizes.size());

    // 数量变量：逐个执行不同大小的分配请求。
    for (std::size_t request_size : request_sizes) {
        // 指针变量：从变长池中申请对应大小的内存。
        void* memory = variable_pool.Allocate(request_size);
        allocations.push_back(memory);
        std::cout << "Allocated request size: " << request_size << " bytes\n";
    }

    // 指针变量：依次归还所有已申请内存。
    for (void* allocation : allocations) {
        variable_pool.Deallocate(allocation);
    }

    std::cout << "Size class count: " << variable_pool.GetSizeClassCount() << "\n";
    std::cout << "Largest built-in size class: "
              << variable_pool.GetSizeClass(variable_pool.GetSizeClassCount() - 1) << "\n\n";
}

// 主函数：串联展示模块 9 中的扩展功能。
int main() {
    try {
        RunBasicObjectPoolDemo();
        RunThreadSafePoolDemo();
        RunDebugPoolDemo();
        RunVariablePoolDemo();
        return 0;
    // 异常变量：接收演示阶段抛出的错误信息。
    } catch (const std::exception& exception) {
        std::cerr << "Error: " << exception.what() << "\n";
        return 1;
    }
}
