#include "smart_pointer_demo.h"

namespace smartptrdemo {

// 函数：构造并返回独占智能指针。
UniquePtr<DemoValue> MakeUniqueDemoValue(int value) {
    // 指针变量：创建新的演示对象。
    DemoValue* raw = new DemoValue{value};
    return UniquePtr<DemoValue>(raw);
}

// 函数：构造并返回共享智能指针。
SharedPtr<DemoValue> MakeSharedDemoValue(int value) {
    // 指针变量：创建新的演示对象。
    DemoValue* raw = new DemoValue{value};
    return SharedPtr<DemoValue>(raw);
}

}  // namespace smartptrdemo
