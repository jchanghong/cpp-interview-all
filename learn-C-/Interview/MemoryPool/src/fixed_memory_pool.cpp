#include "fixed_memory_pool.h"

#include <algorithm>

// 构造函数：校验参数并初始化池状态。
FixedMemoryPool::FixedMemoryPool(std::size_t block_size, std::size_t blocks_per_chunk)
    : block_size_(AlignUp(std::max(block_size, sizeof(FreeNode)))),
      blocks_per_chunk_(blocks_per_chunk),
      free_list_(nullptr),
      chunks_(),
      free_block_count_(0),
      total_block_count_(0) {
    if (block_size == 0) {
        throw std::invalid_argument("block_size must be greater than 0.");
    }

    if (blocks_per_chunk_ == 0) {
        throw std::invalid_argument("blocks_per_chunk must be greater than 0.");
    }
}

// 析构函数：遍历并释放所有批次大块内存。
FixedMemoryPool::~FixedMemoryPool() {
    // 范围变量：遍历每一批次内存首地址。
    for (void* chunk : chunks_) {
        ::operator delete(chunk);
    }
}

// 分配函数：优先复用空闲块，不足时触发扩容。
void* FixedMemoryPool::Allocate() {
    if (free_list_ == nullptr) {
        AllocateChunk();
    }

    // 指针变量：保存当前返回的空闲节点。
    FreeNode* node = free_list_;
    free_list_ = free_list_->next;
    --free_block_count_;
    return node;
}

// 归还函数：把块头插回空闲链表。
void FixedMemoryPool::Deallocate(void* ptr) {
    if (ptr == nullptr) {
        return;
    }

    // 指针变量：把原始地址解释为空闲节点。
    FreeNode* node = static_cast<FreeNode*>(ptr);
    node->next = free_list_;
    free_list_ = node;
    ++free_block_count_;
}

// 查询函数：返回当前空闲块数量。
std::size_t FixedMemoryPool::GetFreeBlockCount() const {
    return free_block_count_;
}

// 查询函数：返回历史累计总块数量。
std::size_t FixedMemoryPool::GetTotalBlockCount() const {
    return total_block_count_;
}

// 查询函数：返回单个块大小。
std::size_t FixedMemoryPool::GetBlockSize() const {
    return block_size_;
}

// 计算函数：把大小向上对齐到最大对齐边界。
std::size_t FixedMemoryPool::AlignUp(std::size_t value) {
    return ((value + kAlignment - 1) / kAlignment) * kAlignment;
}

// 扩容函数：申请一整块大内存并切分为多个定长节点。
void FixedMemoryPool::AllocateChunk() {
    // 大小变量：本次批量申请的总字节数。
    const std::size_t chunk_size = block_size_ * blocks_per_chunk_;

    // 指针变量：记录新申请的大块内存首地址。
    void* raw_chunk = ::operator new(chunk_size);
    chunks_.push_back(raw_chunk);

    // 字节指针变量：用于按块大小进行地址偏移。
    char* begin = static_cast<char*>(raw_chunk);

    // 数量变量：逐块切分本批次内存。
    for (std::size_t index = 0; index < blocks_per_chunk_; ++index) {
        // 指针变量：定位当前块对应的空闲节点地址。
        FreeNode* node = reinterpret_cast<FreeNode*>(begin + index * block_size_);
        node->next = free_list_;
        free_list_ = node;
    }

    free_block_count_ += blocks_per_chunk_;
    total_block_count_ += blocks_per_chunk_;
}
