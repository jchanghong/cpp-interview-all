#ifndef FIXED_MEMORY_POOL_H
#define FIXED_MEMORY_POOL_H

#include <cstddef>
#include <memory>
#include <new>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

class FixedMemoryPool {
public:
    // 构造函数：初始化定长块大小与每批次块数量。
    FixedMemoryPool(std::size_t block_size, std::size_t blocks_per_chunk);

    // 析构函数：释放所有批次申请的大块内存。
    ~FixedMemoryPool();

    // 复制构造函数：禁用拷贝，避免多个池共享同一片底层内存。
    FixedMemoryPool(const FixedMemoryPool&) = delete;

    // 复制赋值函数：禁用拷贝赋值，避免重复释放问题。
    FixedMemoryPool& operator=(const FixedMemoryPool&) = delete;

    // 移动构造函数：禁用移动，保持池对象地址与生命周期语义简单清晰。
    FixedMemoryPool(FixedMemoryPool&&) = delete;

    // 移动赋值函数：禁用移动赋值，避免资源转移带来的状态复杂度。
    FixedMemoryPool& operator=(FixedMemoryPool&&) = delete;

    // 分配函数：返回一个可用的固定大小内存块。
    void* Allocate();

    // 归还函数：将固定大小内存块放回空闲链表。
    void Deallocate(void* ptr);

    // 查询函数：返回空闲块数量。
    std::size_t GetFreeBlockCount() const;

    // 查询函数：返回总块数量。
    std::size_t GetTotalBlockCount() const;

    // 查询函数：返回对齐后的单个块大小。
    std::size_t GetBlockSize() const;

private:
    // 空闲节点结构：复用块内存本身维护单链表。
    struct FreeNode {
        // 指针变量：指向下一个空闲块。
        FreeNode* next;
    };

    // 扩容函数：按批次申请并切分新的内存块。
    void AllocateChunk();

    // 计算函数：把大小向上对齐到最大对齐边界。
    static std::size_t AlignUp(std::size_t value);

    // 常量变量：统一使用的块对齐粒度。
    static constexpr std::size_t kAlignment = alignof(std::max_align_t);

    // 大小变量：单个块的对齐后大小。
    std::size_t block_size_;

    // 数量变量：每次扩容申请的块数量。
    std::size_t blocks_per_chunk_;

    // 指针变量：空闲链表头节点。
    FreeNode* free_list_;

    // 容器变量：保存所有大块内存的首地址，便于析构统一释放。
    std::vector<void*> chunks_;

    // 数量变量：当前空闲块数量。
    std::size_t free_block_count_;

    // 数量变量：历史累计总块数量。
    std::size_t total_block_count_;
};

template <typename T>
class ObjectPool;

template <typename T>
struct PoolDeleter {
    // 指针变量：记录对象所属对象池。
    ObjectPool<T>* pool;

    // 调用函数：在智能指针释放时把对象归还到对象池。
    void operator()(T* object) const;
};

template <typename T>
using UniquePoolPtr = std::unique_ptr<T, PoolDeleter<T>>;

template <typename T>
class ObjectPool {
public:
    // 构造函数：按对象类型大小初始化底层定长内存池。
    explicit ObjectPool(std::size_t objects_per_chunk)
        : pool_(sizeof(T), objects_per_chunk) {}

    // 创建函数：从池中获取内存并原地构造对象。
    template <typename... Args>
    T* Create(Args&&... args) {
        // 指针变量：接收池中分配出的原始内存地址。
        void* memory = pool_.Allocate();
        try {
            return new (memory) T(std::forward<Args>(args)...);
        } catch (...) {
            pool_.Deallocate(memory);
            throw;
        }
    }

    // 创建函数：返回一个离开作用域后会自动归还对象的智能指针。
    template <typename... Args>
    UniquePoolPtr<T> CreateUnique(Args&&... args) {
        // 指针变量：创建原始对象。
        T* object = Create(std::forward<Args>(args)...);
        return UniquePoolPtr<T>(object, PoolDeleter<T>{this});
    }

    // 销毁函数：显式调用析构并归还对象占用的内存块。
    void Destroy(T* object) {
        if (object == nullptr) {
            return;
        }

        object->~T();
        pool_.Deallocate(object);
    }

    // 查询函数：返回底层对象块大小。
    std::size_t GetBlockSize() const {
        return pool_.GetBlockSize();
    }

    // 查询函数：返回当前空闲对象数量。
    std::size_t GetFreeCount() const {
        return pool_.GetFreeBlockCount();
    }

    // 查询函数：返回池中总对象容量。
    std::size_t GetTotalCount() const {
        return pool_.GetTotalBlockCount();
    }

private:
    static_assert(!std::is_reference<T>::value, "T must not be a reference type.");
    static_assert(!std::is_void<T>::value, "T must not be void.");

    // 成员变量：底层定长块内存池。
    FixedMemoryPool pool_;
};

template <typename T>
void PoolDeleter<T>::operator()(T* object) const {
    if (pool != nullptr) {
        pool->Destroy(object);
    }
}

#endif
