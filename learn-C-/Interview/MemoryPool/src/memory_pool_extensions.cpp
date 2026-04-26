#include "memory_pool_extensions.h"

#include <algorithm>
#include <cstring>
#include <limits>

// 构造函数：创建带互斥锁保护的定长内存池。
ThreadSafeFixedMemoryPool::ThreadSafeFixedMemoryPool(std::size_t block_size, std::size_t blocks_per_chunk)
    : mutex_(),
      pool_(block_size, blocks_per_chunk) {}

// 分配函数：在线程安全保护下获取一个内存块。
void* ThreadSafeFixedMemoryPool::Allocate() {
    // 锁变量：保护底层池状态。
    std::lock_guard<std::mutex> lock(mutex_);
    return pool_.Allocate();
}

// 回收函数：在线程安全保护下归还一个内存块。
void ThreadSafeFixedMemoryPool::Deallocate(void* ptr) {
    // 锁变量：保护底层池状态。
    std::lock_guard<std::mutex> lock(mutex_);
    pool_.Deallocate(ptr);
}

// 查询函数：返回当前空闲块数量。
std::size_t ThreadSafeFixedMemoryPool::GetFreeBlockCount() const {
    // 锁变量：保护底层池状态。
    std::lock_guard<std::mutex> lock(mutex_);
    return pool_.GetFreeBlockCount();
}

// 查询函数：返回当前总块数量。
std::size_t ThreadSafeFixedMemoryPool::GetTotalBlockCount() const {
    // 锁变量：保护底层池状态。
    std::lock_guard<std::mutex> lock(mutex_);
    return pool_.GetTotalBlockCount();
}

// 构造函数：创建带越界检测与活跃块跟踪的调试池。
DebugFixedMemoryPool::DebugFixedMemoryPool(std::size_t user_block_size, std::size_t blocks_per_chunk)
    : user_block_size_(user_block_size),
      raw_pool_(GetRawBlockSize(user_block_size), blocks_per_chunk),
      active_blocks_() {
    if (user_block_size_ == 0) {
        throw std::invalid_argument("user_block_size must be greater than 0.");
    }
}

// 分配函数：返回给业务层使用的用户可见地址。
void* DebugFixedMemoryPool::Allocate() {
    // 指针变量：获取底层原始块地址。
    void* raw_block = raw_pool_.Allocate();

    // 字节指针变量：用于对原始块进行头尾布局。
    char* raw_bytes = static_cast<char*>(raw_block);

    // 指针变量：定位块头信息。
    BlockHeader* header = reinterpret_cast<BlockHeader*>(raw_bytes);
    header->head_magic = kHeadMagic;

    // 指针变量：定位用户区起始地址。
    void* user_ptr = raw_bytes + GetAlignedHeaderSize();

    // 指针变量：定位尾部守卫区。
    std::uint32_t* tail_guard = reinterpret_cast<std::uint32_t*>(
        static_cast<char*>(user_ptr) + user_block_size_);
    *tail_guard = kTailMagic;

    active_blocks_[user_ptr] = raw_block;
    return user_ptr;
}

// 回收函数：校验守卫区并归还内存块。
void DebugFixedMemoryPool::Deallocate(void* ptr) {
    if (ptr == nullptr) {
        return;
    }

    // 迭代器变量：查找当前用户指针是否处于活跃状态。
    const auto iterator = active_blocks_.find(ptr);
    if (iterator == active_blocks_.end()) {
        throw std::runtime_error("invalid pointer or double free detected.");
    }

    // 指针变量：取出底层原始块地址。
    void* raw_block = iterator->second;

    ValidateBlock(raw_block, ptr);
    active_blocks_.erase(iterator);

    // 字节指针变量：把用户区内容清零，帮助调试悬挂读问题。
    char* user_bytes = static_cast<char*>(ptr);
    std::memset(user_bytes, 0, user_block_size_);

    raw_pool_.Deallocate(raw_block);
}

// 查询函数：返回活跃分配数量。
std::size_t DebugFixedMemoryPool::GetActiveAllocationCount() const {
    return active_blocks_.size();
}

// 计算函数：返回头部对齐后的大小。
std::size_t DebugFixedMemoryPool::GetAlignedHeaderSize() {
    // 常量变量：统一使用的对齐粒度。
    const std::size_t alignment = alignof(std::max_align_t);

    // 常量变量：原始头部大小。
    const std::size_t header_size = sizeof(BlockHeader);
    return ((header_size + alignment - 1) / alignment) * alignment;
}

// 计算函数：返回底层真实块大小。
std::size_t DebugFixedMemoryPool::GetRawBlockSize(std::size_t user_block_size) {
    return GetAlignedHeaderSize() + user_block_size + sizeof(std::uint32_t);
}

// 校验函数：确认守卫区是否被破坏。
void DebugFixedMemoryPool::ValidateBlock(void* raw_block, void* user_ptr) const {
    // 指针变量：定位块头信息。
    const BlockHeader* header = reinterpret_cast<const BlockHeader*>(raw_block);
    if (header->head_magic != kHeadMagic) {
        throw std::runtime_error("head guard has been corrupted.");
    }

    // 指针变量：定位尾部守卫区。
    const std::uint32_t* tail_guard = reinterpret_cast<const std::uint32_t*>(
        static_cast<const char*>(user_ptr) + user_block_size_);
    if (*tail_guard != kTailMagic) {
        throw std::runtime_error("tail guard has been corrupted.");
    }
}

// 构造函数：创建多尺寸分离适配的变长内存池。
VariableMemoryPool::VariableMemoryPool(std::vector<std::size_t> size_classes, std::size_t blocks_per_chunk)
    : size_classes_() {
    if (size_classes.empty()) {
        throw std::invalid_argument("size_classes must not be empty.");
    }

    // 迭代器变量：用于遍历尺寸类别数组。
    for (std::size_t payload_size : size_classes) {
        if (payload_size == 0) {
            throw std::invalid_argument("size class must be greater than 0.");
        }
    }

    std::sort(size_classes.begin(), size_classes.end());
    // 迭代器变量：用于按顺序创建桶位。
    for (std::size_t payload_size : size_classes) {
        if (!size_classes_.empty() && size_classes_.back()->payload_size == payload_size) {
            continue;
        }

        size_classes_.emplace_back(std::make_unique<SizeClassEntry>(payload_size, blocks_per_chunk));
    }
}

// 分配函数：按请求大小选择合适桶位并返回内存。
void* VariableMemoryPool::Allocate(std::size_t size) {
    if (size == 0) {
        throw std::invalid_argument("size must be greater than 0.");
    }

    // 常量变量：头部占用的对齐后字节数。
    const std::size_t header_size = GetAlignedHeaderSize();

    // 数量变量：定位适配当前请求的桶位索引。
    const std::size_t class_index = FindSizeClassIndex(size);

    // 布尔变量：标记是否需要回退到系统分配。
    const bool uses_fallback = class_index == size_classes_.size();

    // 指针变量：保存底层原始块地址。
    void* raw_block = nullptr;

    if (uses_fallback) {
        // 常量变量：系统回退场景下的真实块大小。
        const std::size_t raw_block_size = GetRawBlockSize(size);
        raw_block = ::operator new(raw_block_size);
    } else {
        raw_block = size_classes_[class_index]->pool.Allocate();
    }

    // 字节指针变量：用于写入块头。
    char* raw_bytes = static_cast<char*>(raw_block);

    // 指针变量：定位块头信息。
    BlockHeader* header = reinterpret_cast<BlockHeader*>(raw_bytes);
    header->uses_fallback = uses_fallback;
    header->class_index = class_index;
    header->requested_size = size;

    return raw_bytes + header_size;
}

// 回收函数：根据块头信息把内存归还正确的桶位。
void VariableMemoryPool::Deallocate(void* ptr) {
    if (ptr == nullptr) {
        return;
    }

    // 常量变量：头部占用的对齐后字节数。
    const std::size_t header_size = GetAlignedHeaderSize();

    // 字节指针变量：回退到原始块首地址。
    char* raw_bytes = static_cast<char*>(ptr) - header_size;

    // 指针变量：读取块头信息。
    BlockHeader* header = reinterpret_cast<BlockHeader*>(raw_bytes);

    if (header->uses_fallback) {
        ::operator delete(raw_bytes);
        return;
    }

    if (header->class_index >= size_classes_.size()) {
        throw std::runtime_error("invalid size class index detected.");
    }

    size_classes_[header->class_index]->pool.Deallocate(raw_bytes);
}

// 查询函数：返回当前尺寸类别数量。
std::size_t VariableMemoryPool::GetSizeClassCount() const {
    return size_classes_.size();
}

// 查询函数：返回指定桶位的用户负载大小。
std::size_t VariableMemoryPool::GetSizeClass(std::size_t index) const {
    if (index >= size_classes_.size()) {
        throw std::out_of_range("size class index is out of range.");
    }

    return size_classes_[index]->payload_size;
}

// 构造函数：创建指定负载大小的桶位。
VariableMemoryPool::SizeClassEntry::SizeClassEntry(std::size_t payload_size_value,
                                                   std::size_t blocks_per_chunk)
    : payload_size(payload_size_value),
      pool(GetRawBlockSize(payload_size_value), blocks_per_chunk) {}

// 计算函数：返回对齐后的头部大小。
std::size_t VariableMemoryPool::GetAlignedHeaderSize() {
    // 常量变量：统一使用的对齐粒度。
    const std::size_t alignment = alignof(std::max_align_t);

    // 常量变量：原始头部大小。
    const std::size_t header_size = sizeof(BlockHeader);
    return ((header_size + alignment - 1) / alignment) * alignment;
}

// 计算函数：根据负载大小计算真实块大小。
std::size_t VariableMemoryPool::GetRawBlockSize(std::size_t payload_size) {
    return GetAlignedHeaderSize() + payload_size;
}

// 查找函数：返回适合请求大小的桶位索引。
std::size_t VariableMemoryPool::FindSizeClassIndex(std::size_t size) const {
    // 数量变量：遍历尺寸类别索引。
    for (std::size_t index = 0; index < size_classes_.size(); ++index) {
        if (size <= size_classes_[index]->payload_size) {
            return index;
        }
    }

    return size_classes_.size();
}
