# problems__如何在main函数前执行代码？.md

### 如何在 main 函数前执行代码？

**方法一：全局对象的构造函数（C++ 方式）**

```cpp
#include <iostream>
struct BeforeMain {
    BeforeMain() { std::cout << "before main\n"; }
};
BeforeMain g_before;  // 全局对象，构造在 main 之前执行

int main() {
    std::cout << "main\n";
}
```

**方法二：`__attribute__((constructor))`（GCC/Clang）**

```c
#include <stdio.h>
__attribute__((constructor)) void before_main() {
    printf("before main\n");
}
int main() { printf("main\n"); }
```

**方法三：MSVC `#pragma init_seg`**

```cpp
#pragma init_seg(lib)
struct BeforeMain { BeforeMain() { /* 在 main 前执行 */ } };
BeforeMain g_before;
```

**面试标准答案：**

> 在 C++ 中可以通过全局对象的构造函数在 main 前执行代码，因为全局/静态对象的初始化发生在 `main()` 被调用之前（在程序的静态初始化阶段）。GCC/Clang 还支持 `__attribute__((constructor))` 标记函数在 main 前自动执行。MSVC 可以使用 `#pragma init_seg` 控制初始化顺序。
>
> **注意**：不同编译单元的全局对象初始化顺序是未定义的，应避免依赖初始化顺序。执行顺序：全局对象构造 → main → 全局对象析构。
>
> **实际应用场景**：注册插件、初始化日志系统、加载配置文件等初始化操作。

---
