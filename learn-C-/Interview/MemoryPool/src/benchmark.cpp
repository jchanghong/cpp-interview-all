#include "memory_pool_extensions.h"

#include <chrono>
#include <cstddef>
#include <exception>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

// 基准结构体：模拟频繁创建和销毁的节点对象。
struct BenchmarkNode {
    // 成员变量：节点编号。
    int id;

    // 成员变量：节点负载文本。
    std::string payload;

    // 构造函数：初始化基准测试节点。
    BenchmarkNode(int node_id, std::string text)
        : id(node_id), payload(std::move(text)) {}
};

// 基准函数：测试普通 new/delete 的耗时。
std::chrono::microseconds BenchmarkNewDelete(std::size_t iterations) {
    // 时间点变量：记录开始时间。
    const auto begin = std::chrono::steady_clock::now();

    // 数量变量：重复执行创建和销毁。
    for (std::size_t index = 0; index < iterations; ++index) {
        // 指针变量：使用普通堆分配创建节点。
        BenchmarkNode* node = new BenchmarkNode(static_cast<int>(index), "plain-new");
        delete node;
    }

    // 时间点变量：记录结束时间。
    const auto end = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
}

// 基准函数：测试对象池的耗时。
std::chrono::microseconds BenchmarkObjectPool(std::size_t iterations) {
    // 对象变量：创建用于基准测试的对象池。
    ObjectPool<BenchmarkNode> pool(1024);

    // 时间点变量：记录开始时间。
    const auto begin = std::chrono::steady_clock::now();

    // 数量变量：重复执行创建和销毁。
    for (std::size_t index = 0; index < iterations; ++index) {
        // 智能指针变量：使用对象池创建自动回收对象。
        UniquePoolPtr<BenchmarkNode> node = pool.CreateUnique(static_cast<int>(index), "pooled");
    }

    // 时间点变量：记录结束时间。
    const auto end = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
}

// 基准函数：测试变长内存池的耗时。
std::chrono::microseconds BenchmarkVariablePool(std::size_t iterations) {
    // 对象变量：创建多个尺寸类别的变长内存池。
    VariableMemoryPool pool({16, 32, 64, 128, 256, 512}, 512);

    // 容器变量：准备一组循环复用的请求大小。
    const std::vector<std::size_t> request_sizes{8, 24, 60, 100, 180, 400};

    // 时间点变量：记录开始时间。
    const auto begin = std::chrono::steady_clock::now();

    // 数量变量：重复执行分配与回收。
    for (std::size_t index = 0; index < iterations; ++index) {
        // 数量变量：选择当前轮次的请求大小。
        const std::size_t request_size = request_sizes[index % request_sizes.size()];

        // 指针变量：从变长池中申请一块内存。
        void* memory = pool.Allocate(request_size);
        pool.Deallocate(memory);
    }

    // 时间点变量：记录结束时间。
    const auto end = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
}

// 主函数：输出模块 9 中性能基准测试的示例结果。
int main() {
    try {
        // 常量变量：控制每组基准测试的循环次数。
        const std::size_t iterations = 200000;

        // 时间变量：记录普通 new/delete 的耗时。
        const std::chrono::microseconds new_delete_time = BenchmarkNewDelete(iterations);

        // 时间变量：记录对象池的耗时。
        const std::chrono::microseconds object_pool_time = BenchmarkObjectPool(iterations);

        // 时间变量：记录变长池的耗时。
        const std::chrono::microseconds variable_pool_time = BenchmarkVariablePool(iterations);

        std::cout << "Iterations: " << iterations << "\n";
        std::cout << "new/delete: " << new_delete_time.count() << " us\n";
        std::cout << "object pool: " << object_pool_time.count() << " us\n";
        std::cout << "variable pool: " << variable_pool_time.count() << " us\n";
        return 0;
    // 异常变量：接收基准测试阶段抛出的错误信息。
    } catch (const std::exception& exception) {
        std::cerr << "Error: " << exception.what() << "\n";
        return 1;
    }
}
