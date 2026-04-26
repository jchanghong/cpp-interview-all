#include "fixed_memory_pool.h"
#include "memory_pool_extensions.h"

#include <cstddef>
#include <cstdint>
#include <exception>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>
#include <vector>

// 测试结构体：用于验证对象池的构造、析构与复用行为。
struct TestEntity {
    // 静态变量：统计当前存活对象数量。
    static int alive_count;

    // 成员变量：对象编号。
    int id;

    // 构造函数：创建测试对象并增加存活计数。
    explicit TestEntity(int value)
        : id(value) {
        ++alive_count;
    }

    // 析构函数：销毁测试对象并减少存活计数。
    ~TestEntity() {
        --alive_count;
    }
};

// 静态变量定义：初始化测试对象存活计数。
int TestEntity::alive_count = 0;

// 测试函数类型：统一保存测试函数签名。
using TestFunction = std::function<void()>;

// 断言函数：在条件不成立时抛出带描述的异常。
void ExpectTrue(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

// 断言函数：校验两个值是否相等。
template <typename T, typename U>
void ExpectEqual(const T& left, const U& right, const std::string& message) {
    if (!(left == right)) {
        throw std::runtime_error(message);
    }
}

// 断言函数：校验给定操作会抛出指定异常。
template <typename ExceptionType>
void ExpectThrows(const TestFunction& function, const std::string& message) {
    try {
        function();
    } catch (const ExceptionType&) {
        return;
    } catch (...) {
        throw std::runtime_error(message + " (threw unexpected exception type)");
    }

    throw std::runtime_error(message + " (no exception thrown)");
}

// 测试函数：验证定长内存池的基础分配与回收行为。
void TestFixedMemoryPoolAllocateAndReuse() {
    // 对象变量：创建每批次含 2 个块的定长池。
    FixedMemoryPool pool(8, 2);

    // 指针变量：申请第一个块。
    void* first = pool.Allocate();

    // 指针变量：申请第二个块。
    void* second = pool.Allocate();

    ExpectEqual(pool.GetTotalBlockCount(), static_cast<std::size_t>(2), "total block count should be 2");
    ExpectEqual(pool.GetFreeBlockCount(), static_cast<std::size_t>(0), "free block count should be 0");

    pool.Deallocate(first);
    ExpectEqual(pool.GetFreeBlockCount(), static_cast<std::size_t>(1), "free block count should become 1");

    // 指针变量：再次申请块，验证已释放地址会被复用。
    void* reused = pool.Allocate();

    ExpectTrue(reused == first, "pool should reuse the most recently returned block");
    pool.Deallocate(second);
    pool.Deallocate(reused);
    ExpectEqual(pool.GetFreeBlockCount(), static_cast<std::size_t>(2), "all blocks should be returned");
}

// 测试函数：验证定长池在容量不足时会自动扩容。
void TestFixedMemoryPoolExpandsWhenExhausted() {
    // 对象变量：创建每批次只含 2 个块的定长池。
    FixedMemoryPool pool(16, 2);

    // 指针变量：申请第一个块。
    void* first = pool.Allocate();

    // 指针变量：申请第二个块。
    void* second = pool.Allocate();

    // 指针变量：申请第三个块，触发扩容。
    void* third = pool.Allocate();

    ExpectTrue(first != nullptr && second != nullptr && third != nullptr, "allocated blocks should be valid");
    ExpectEqual(pool.GetTotalBlockCount(), static_cast<std::size_t>(4), "pool should allocate a second chunk");

    pool.Deallocate(first);
    pool.Deallocate(second);
    pool.Deallocate(third);
    ExpectEqual(pool.GetFreeBlockCount(), static_cast<std::size_t>(4), "all expanded blocks should be free");
}

// 测试函数：验证对象池会正确执行构造、析构和自动归还。
void TestObjectPoolCreateDestroyAndUnique() {
    // 对象变量：创建测试对象池。
    ObjectPool<TestEntity> pool(2);

    ExpectEqual(TestEntity::alive_count, 0, "alive count should start at 0");

    // 指针变量：手动创建测试对象。
    TestEntity* raw = pool.Create(7);
    ExpectEqual(TestEntity::alive_count, 1, "alive count should become 1 after raw create");

    pool.Destroy(raw);
    ExpectEqual(TestEntity::alive_count, 0, "alive count should return to 0 after destroy");

    {
        // 智能指针变量：创建自动归还的测试对象。
        UniquePoolPtr<TestEntity> unique = pool.CreateUnique(11);
        ExpectEqual(unique->id, 11, "unique object id should match constructor argument");
        ExpectEqual(TestEntity::alive_count, 1, "alive count should become 1 after unique create");
    }

    ExpectEqual(TestEntity::alive_count, 0, "alive count should return to 0 after unique pointer scope");
    ExpectEqual(pool.GetFreeCount(), pool.GetTotalCount(), "all object blocks should be returned");
}

// 测试函数：验证线程安全对象池在并发压力下可正常回收。
void TestThreadSafeObjectPoolConcurrentUsage() {
    // 对象变量：创建线程安全对象池。
    ThreadSafeObjectPool<TestEntity> pool(8);

    // 常量变量：线程数量。
    const std::size_t thread_count = 4;

    // 常量变量：每个线程循环次数。
    const std::size_t iterations_per_thread = 200;

    // 容器变量：保存工作线程。
    std::vector<std::thread> workers;
    workers.reserve(thread_count);

    // 数量变量：启动多个线程并发创建对象。
    for (std::size_t thread_index = 0; thread_index < thread_count; ++thread_index) {
        workers.emplace_back([&pool, thread_index, iterations_per_thread]() {
            // 数量变量：执行当前线程的循环任务。
            for (std::size_t iteration = 0; iteration < iterations_per_thread; ++iteration) {
                // 智能指针变量：创建自动归还对象。
                ThreadSafeUniquePoolPtr<TestEntity> entity =
                    pool.CreateUnique(static_cast<int>(thread_index * iterations_per_thread + iteration));
            }
        });
    }

    // 引用变量：等待线程结束。
    for (std::thread& worker : workers) {
        worker.join();
    }

    ExpectEqual(TestEntity::alive_count, 0, "all threaded objects should be destroyed");
    ExpectEqual(pool.GetFreeCount(), pool.GetTotalCount(), "thread-safe pool should reclaim all blocks");
}

// 测试函数：验证调试池可以检测重复释放。
void TestDebugPoolDetectsDoubleFree() {
    // 对象变量：创建调试版内存池。
    DebugFixedMemoryPool pool(32, 2);

    // 指针变量：申请一块用户内存。
    void* memory = pool.Allocate();
    pool.Deallocate(memory);

    // 函数变量：封装第二次释放动作。
    TestFunction second_free = [&pool, memory]() {
        pool.Deallocate(memory);
    };

    ExpectThrows<std::runtime_error>(second_free, "debug pool should detect double free");
}

// 测试函数：验证调试池可以检测尾部越界写。
void TestDebugPoolDetectsTailCorruption() {
    // 对象变量：创建调试版内存池。
    DebugFixedMemoryPool pool(8, 2);

    // 指针变量：申请一块用户内存。
    void* memory = pool.Allocate();

    // 字节指针变量：按字节访问用户区以制造越界写。
    std::uint8_t* bytes = static_cast<std::uint8_t*>(memory);
    bytes[8] = 0xFFu;

    // 函数变量：封装释放动作并触发守卫区校验。
    TestFunction free_corrupted = [&pool, memory]() {
        pool.Deallocate(memory);
    };

    ExpectThrows<std::runtime_error>(free_corrupted, "debug pool should detect tail corruption");
}

// 测试函数：验证变长池的尺寸类别排序与请求处理。
void TestVariableMemoryPoolBasicUsage() {
    // 对象变量：创建一个带重复与乱序输入的变长池。
    VariableMemoryPool pool({64, 16, 32, 32}, 2);

    ExpectEqual(pool.GetSizeClassCount(), static_cast<std::size_t>(3), "duplicate size classes should be removed");
    ExpectEqual(pool.GetSizeClass(0), static_cast<std::size_t>(16), "size classes should be sorted ascending");
    ExpectEqual(pool.GetSizeClass(2), static_cast<std::size_t>(64), "largest size class should be 64");

    // 指针变量：申请适合首个桶位的内存。
    void* small = pool.Allocate(12);

    // 指针变量：申请需要回退系统分配的内存。
    void* large = pool.Allocate(128);

    ExpectTrue(small != nullptr && large != nullptr, "variable pool allocations should succeed");

    pool.Deallocate(small);
    pool.Deallocate(large);
}

// 测试函数：验证非法参数与越界访问会抛异常。
void TestInvalidArgumentsThrow() {
    // 函数变量：封装非法固定池构造。
    TestFunction invalid_fixed_pool = []() {
        FixedMemoryPool pool(0, 1);
    };

    // 函数变量：封装非法调试池构造。
    TestFunction invalid_debug_pool = []() {
        DebugFixedMemoryPool pool(0, 1);
    };

    // 函数变量：封装非法变长池构造。
    TestFunction invalid_variable_pool = []() {
        VariableMemoryPool pool({}, 1);
    };

    // 函数变量：封装越界尺寸类访问。
    TestFunction invalid_size_class_index = []() {
        VariableMemoryPool pool({16, 32}, 1);
        static_cast<void>(pool.GetSizeClass(5));
    };

    ExpectThrows<std::invalid_argument>(invalid_fixed_pool, "zero-sized fixed pool should throw");
    ExpectThrows<std::invalid_argument>(invalid_debug_pool, "zero-sized debug pool should throw");
    ExpectThrows<std::invalid_argument>(invalid_variable_pool, "empty size class list should throw");
    ExpectThrows<std::out_of_range>(invalid_size_class_index, "invalid size class index should throw");
}

// 主函数：执行所有单元测试并输出结果摘要。
int main() {
    // 结构体变量：保存单个测试的名称与函数。
    struct TestCase {
        // 字符串变量：测试名称。
        const char* name;

        // 函数变量：测试执行体。
        TestFunction function;
    };

    // 容器变量：注册全部测试用例。
    const std::vector<TestCase> test_cases{
        {"FixedMemoryPool Allocate/Reuse", TestFixedMemoryPoolAllocateAndReuse},
        {"FixedMemoryPool Expansion", TestFixedMemoryPoolExpandsWhenExhausted},
        {"ObjectPool Create/Destroy/Unique", TestObjectPoolCreateDestroyAndUnique},
        {"ThreadSafeObjectPool Concurrent Usage", TestThreadSafeObjectPoolConcurrentUsage},
        {"DebugFixedMemoryPool Double Free", TestDebugPoolDetectsDoubleFree},
        {"DebugFixedMemoryPool Tail Corruption", TestDebugPoolDetectsTailCorruption},
        {"VariableMemoryPool Basic Usage", TestVariableMemoryPoolBasicUsage},
        {"Invalid Arguments Throw", TestInvalidArgumentsThrow},
    };

    // 数量变量：记录已通过测试数量。
    std::size_t passed_count = 0;

    // 常量变量：记录总测试数量。
    const std::size_t total_count = test_cases.size();

    // 引用变量：依次执行每个测试用例。
    for (const TestCase& test_case : test_cases) {
        try {
            test_case.function();
            ++passed_count;
            std::cout << "[PASS] " << test_case.name << "\n";
        } catch (const std::exception& exception) {
            std::cerr << "[FAIL] " << test_case.name << ": " << exception.what() << "\n";
            return 1;
        }
    }

    std::cout << "Passed " << passed_count << "/" << total_count << " tests\n";
    return 0;
}
