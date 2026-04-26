#include "smart_pointer_demo.h"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

using smartptrdemo::DemoValue;
using smartptrdemo::MakeSharedDemoValue;
using smartptrdemo::MakeUniqueDemoValue;
using smartptrdemo::SharedPtr;
using smartptrdemo::UniquePtr;

// 函数：在条件不满足时抛出异常。
static void Require(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

// 函数：验证 UniquePtr 的移动和重置行为。
static void TestUniquePtr() {
    // 对象变量：创建独占指针。
    UniquePtr<DemoValue> first = MakeUniqueDemoValue(10);
    // 对象变量：通过移动构造转移所有权。
    UniquePtr<DemoValue> second(std::move(first));

    Require(first.Get() == nullptr, "unique pointer move should null source");
    Require(second->value == 10, "unique pointer value mismatch");

    second.Reset(new DemoValue{22});
    Require(second->value == 22, "unique pointer reset mismatch");
}

// 函数：验证 SharedPtr 的引用计数行为。
static void TestSharedPtr() {
    // 对象变量：创建共享指针。
    SharedPtr<DemoValue> first = MakeSharedDemoValue(5);
    Require(first.UseCount() == 1U, "shared pointer initial count mismatch");

    // 对象变量：通过拷贝构造共享控制块。
    SharedPtr<DemoValue> second = first;
    Require(first.UseCount() == 2U, "shared pointer count after copy mismatch");
    Require(second->value == 5, "shared pointer value mismatch");

    // 对象变量：通过移动构造转移句柄。
    SharedPtr<DemoValue> third = std::move(second);
    Require(second.Get() == nullptr, "shared pointer move should null source");
    Require(third.UseCount() == 2U, "shared pointer count after move mismatch");
}

// 主函数：顺序执行所有智能指针测试。
int main() {
    try {
        TestUniquePtr();
        TestSharedPtr();
        std::cout << "All smart pointer tests passed." << std::endl;
        return EXIT_SUCCESS;
    } catch (const std::exception& ex) {
        std::cerr << "Smart pointer test failed: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}
