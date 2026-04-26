#include "smart_pointer_demo.h"

#include <iostream>

using smartptrdemo::MakeSharedDemoValue;
using smartptrdemo::MakeUniqueDemoValue;
using smartptrdemo::SharedPtr;

// 主函数：演示独占所有权和共享所有权的基本行为。
int main() {
    // 对象变量：构造一个独占智能指针。
    auto unique_value = MakeUniqueDemoValue(42);
    // 对象变量：构造一个共享智能指针。
    auto shared_value = MakeSharedDemoValue(7);
    // 对象变量：拷贝共享智能指针以增加引用计数。
    SharedPtr<smartptrdemo::DemoValue> shared_copy = shared_value;

    std::cout << "smart_pointer_demo unique=" << unique_value->value
              << " shared=" << shared_value->value
              << " use_count=" << shared_copy.UseCount() << std::endl;
    return 0;
}
