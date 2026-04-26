#ifndef MEMORY_POOL_EXTENSIONS_H
#define MEMORY_POOL_EXTENSIONS_H

#include "fixed_memory_pool.h"

#include <cstddef>
#include <cstdint>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <utility>
#include <vector>

class ThreadSafeFixedMemoryPool {
public:
    // 构造函数：创建带互斥锁保护的定长内存池。
    ThreadSafeFixedMemoryPool(std::size_t block_size, std::size_t blocks_per_chunk);

    // 分配函数：在线程安全保护下获取一个内存块。
    void* Allocate();

    // 回收函数：在线程安全保护下归还一个内存块。
    void Deallocate(void* ptr);

    // 查询函数：返回当前空闲块数量。
    std::size_t GetFreeBlockCount() const;

    // 查询函数：返回当前总块数量。
    std::size_t GetTotalBlockCount() const;

private:
    // 可变成员变量：支持在 const 查询函数中加锁。
    mutable std::mutex mutex_;

    // 成员变量：被互斥锁保护的底层定长内存池。
    FixedMemoryPool pool_;
};

template <typename T>
class ThreadSafeObjectPool;

template <typename T>
struct ThreadSafePoolDeleter {
    // 指针变量：记录对象所属线程安全对象池。
    ThreadSafeObjectPool<T>* pool;

    // 调用函数：在智能指针释放时把对象安全归还到线程安全对象池。
    void operator()(T* object) const;
};

template <typename T>
using ThreadSafeUniquePoolPtr = std::unique_ptr<T, ThreadSafePoolDeleter<T>>;

template <typename T>
class ThreadSafeObjectPool {
public:
    // 构造函数：创建线程安全对象池。
    explicit ThreadSafeObjectPool(std::size_t objects_per_chunk)
        : pool_(objects_per_chunk) {}

    // 创建函数：在线程安全保护下构造对象。
    template <typename... Args>
    T* Create(Args&&... args) {
        // 锁变量：保护对象池内部状态。
        std::lock_guard<std::mutex> lock(mutex_);
        return pool_.Create(std::forward<Args>(args)...);
    }

    // 创建函数：返回自动归还的智能指针对象。
    template <typename... Args>
    ThreadSafeUniquePoolPtr<T> CreateUnique(Args&&... args) {
        // 锁变量：保护对象池内部状态。
        std::lock_guard<std::mutex> lock(mutex_);
        // 指针变量：创建原始对象。
        T* object = pool_.Create(std::forward<Args>(args)...);
        return ThreadSafeUniquePoolPtr<T>(object, ThreadSafePoolDeleter<T>{this});
    }

    // 销毁函数：在线程安全保护下销毁对象。
    void Destroy(T* object) {
        // 锁变量：保护对象池内部状态。
        std::lock_guard<std::mutex> lock(mutex_);
        pool_.Destroy(object);
    }

    // 查询函数：返回当前空闲对象数量。
    std::size_t GetFreeCount() const {
        // 锁变量：保护对象池内部状态。
        std::lock_guard<std::mutex> lock(mutex_);
        return pool_.GetFreeCount();
    }

    // 查询函数：返回池中总对象容量。
    std::size_t GetTotalCount() const {
        // 锁变量：保护对象池内部状态。
        std::lock_guard<std::mutex> lock(mutex_);
        return pool_.GetTotalCount();
    }

private:
    // 可变成员变量：支持在 const 查询函数中加锁。
    mutable std::mutex mutex_;

    // 成员变量：底层对象池实例。
    ObjectPool<T> pool_;
};

template <typename T>
void ThreadSafePoolDeleter<T>::operator()(T* object) const {
    if (pool != nullptr) {
        pool->Destroy(object);
    }
}

class DebugFixedMemoryPool {
public:
    // 构造函数：创建带越界检测与活跃块跟踪的调试池。
    DebugFixedMemoryPool(std::size_t user_block_size, std::size_t blocks_per_chunk);

    // 分配函数：返回给业务层使用的用户可见地址。
    void* Allocate();

    // 回收函数：校验守卫区并归还内存块。
    void Deallocate(void* ptr);

    // 查询函数：返回活跃分配数量。
    std::size_t GetActiveAllocationCount() const;

private:
    // 头部结构：记录调试校验所需的元信息。
    struct BlockHeader {
        // 标记变量：记录头部魔数。
        std::uint32_t head_magic;
    };

    // 计算函数：返回头部对齐后的大小。
    static std::size_t GetAlignedHeaderSize();

    // 计算函数：返回底层真实块大小。
    static std::size_t GetRawBlockSize(std::size_t user_block_size);

    // 校验函数：确认守卫区是否被破坏。
    void ValidateBlock(void* raw_block, void* user_ptr) const;

    // 常量变量：头部越界检测魔数。
    static constexpr std::uint32_t kHeadMagic = 0xDEADC0DEu;

    // 常量变量：尾部越界检测魔数。
    static constexpr std::uint32_t kTailMagic = 0xBADC0FFEu;

    // 大小变量：用户真实可用块大小。
    std::size_t user_block_size_;

    // 成员变量：承载真实块管理的底层定长池。
    FixedMemoryPool raw_pool_;

    // 容器变量：记录活跃用户指针与底层块地址的映射关系。
    std::unordered_map<void*, void*> active_blocks_;
};

class VariableMemoryPool {
public:
    // 构造函数：创建多尺寸分离适配的变长内存池。
    VariableMemoryPool(std::vector<std::size_t> size_classes, std::size_t blocks_per_chunk);

    // 分配函数：按请求大小选择合适桶位并返回内存。
    void* Allocate(std::size_t size);

    // 回收函数：根据块头信息把内存归还正确的桶位。
    void Deallocate(void* ptr);

    // 查询函数：返回当前尺寸类别数量。
    std::size_t GetSizeClassCount() const;

    // 查询函数：返回指定桶位的用户负载大小。
    std::size_t GetSizeClass(std::size_t index) const;

private:
    // 头部结构：记录回收时所需的尺寸类别信息。
    struct BlockHeader {
        // 布尔变量：标记是否来自回退系统分配。
        bool uses_fallback;

        // 数量变量：记录所属尺寸类别索引。
        std::size_t class_index;

        // 数量变量：记录本次真实请求大小。
        std::size_t requested_size;
    };

    // 尺寸类结构：描述一个桶位及其底层定长池。
    struct SizeClassEntry {
        // 构造函数：创建指定负载大小的桶位。
        SizeClassEntry(std::size_t payload_size, std::size_t blocks_per_chunk);

        // 数量变量：当前桶位可承载的用户负载大小。
        std::size_t payload_size;

        // 成员变量：该桶位对应的底层定长池。
        FixedMemoryPool pool;
    };

    // 计算函数：返回对齐后的头部大小。
    static std::size_t GetAlignedHeaderSize();

    // 计算函数：根据负载大小计算真实块大小。
    static std::size_t GetRawBlockSize(std::size_t payload_size);

    // 查找函数：返回适合请求大小的桶位索引。
    std::size_t FindSizeClassIndex(std::size_t size) const;

    // 容器变量：保存所有尺寸类别。
    std::vector<std::unique_ptr<SizeClassEntry>> size_classes_;
};

#endif
