#ifndef SMART_POINTER_DEMO_H
#define SMART_POINTER_DEMO_H

#include <cstddef>
#include <utility>

namespace smartptrdemo {

// 模板类：实现一个最小版独占所有权智能指针。
template <typename T>
class UniquePtr {
public:
    // 构造函数：接管原始指针所有权。
    explicit UniquePtr(T* ptr = nullptr) : ptr_(ptr) {}

    // 析构函数：释放当前持有的对象。
    ~UniquePtr() {
        delete ptr_;
    }

    // 删除拷贝构造：独占所有权不允许拷贝。
    UniquePtr(const UniquePtr&) = delete;

    // 删除拷贝赋值：独占所有权不允许拷贝。
    UniquePtr& operator=(const UniquePtr&) = delete;

    // 移动构造函数：转移原始指针所有权。
    UniquePtr(UniquePtr&& other) noexcept : ptr_(other.ptr_) {
        other.ptr_ = nullptr;
    }

    // 移动赋值函数：释放旧资源并转移新资源。
    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            delete ptr_;
            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
        }
        return *this;
    }

    // 运算符函数：返回被管理对象的引用。
    T& operator*() const {
        return *ptr_;
    }

    // 运算符函数：返回被管理对象的地址。
    T* operator->() const {
        return ptr_;
    }

    // 查询函数：返回当前持有的原始指针。
    T* Get() const {
        return ptr_;
    }

    // 业务函数：放弃所有权并返回原始指针。
    T* Release() {
        // 指针变量：缓存释放前的原始地址。
        T* raw = ptr_;
        ptr_ = nullptr;
        return raw;
    }

    // 业务函数：替换当前托管对象。
    void Reset(T* ptr = nullptr) {
        if (ptr_ != ptr) {
            delete ptr_;
            ptr_ = ptr;
        }
    }

private:
    // 成员变量：保存被管理对象的原始地址。
    T* ptr_;
};

// 结构体：保存共享计数与原始对象指针。
template <typename T>
struct ControlBlock {
    // 成员变量：记录当前共享引用计数。
    std::size_t ref_count;
    // 成员变量：保存被管理对象的地址。
    T* ptr;
};

// 模板类：实现一个最小版共享所有权智能指针。
template <typename T>
class SharedPtr {
public:
    // 构造函数：从原始指针创建控制块。
    explicit SharedPtr(T* ptr = nullptr) : control_(nullptr) {
        if (ptr != nullptr) {
            control_ = new ControlBlock<T>{1, ptr};
        }
    }

    // 析构函数：递减引用计数并在必要时销毁资源。
    ~SharedPtr() {
        ReleaseCurrent();
    }

    // 拷贝构造函数：共享同一控制块并递增计数。
    SharedPtr(const SharedPtr& other) : control_(other.control_) {
        if (control_ != nullptr) {
            ++control_->ref_count;
        }
    }

    // 拷贝赋值函数：释放旧资源并共享新控制块。
    SharedPtr& operator=(const SharedPtr& other) {
        if (this != &other) {
            ReleaseCurrent();
            control_ = other.control_;
            if (control_ != nullptr) {
                ++control_->ref_count;
            }
        }
        return *this;
    }

    // 移动构造函数：转移控制块指针。
    SharedPtr(SharedPtr&& other) noexcept : control_(other.control_) {
        other.control_ = nullptr;
    }

    // 移动赋值函数：释放旧资源并转移控制块。
    SharedPtr& operator=(SharedPtr&& other) noexcept {
        if (this != &other) {
            ReleaseCurrent();
            control_ = other.control_;
            other.control_ = nullptr;
        }
        return *this;
    }

    // 运算符函数：返回被管理对象引用。
    T& operator*() const {
        return *control_->ptr;
    }

    // 运算符函数：返回被管理对象地址。
    T* operator->() const {
        return control_->ptr;
    }

    // 查询函数：返回原始指针。
    T* Get() const {
        return control_ == nullptr ? nullptr : control_->ptr;
    }

    // 查询函数：返回当前共享引用数。
    std::size_t UseCount() const {
        return control_ == nullptr ? 0U : control_->ref_count;
    }

private:
    // 辅助函数：释放当前控制块持有的资源。
    void ReleaseCurrent() {
        if (control_ != nullptr) {
            --control_->ref_count;
            if (control_->ref_count == 0U) {
                delete control_->ptr;
                delete control_;
            }
            control_ = nullptr;
        }
    }

    // 成员变量：保存共享控制块地址。
    ControlBlock<T>* control_;
};

// 结构体：作为演示对象，保存一个整数值。
struct DemoValue {
    // 成员变量：保存演示用整数。
    int value;
};

// 函数：创建一个持有 DemoValue 的独占指针。
UniquePtr<DemoValue> MakeUniqueDemoValue(int value);

// 函数：创建一个持有 DemoValue 的共享指针。
SharedPtr<DemoValue> MakeSharedDemoValue(int value);

}  // namespace smartptrdemo

#endif
