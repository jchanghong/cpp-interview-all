## 二、Sanitizers（消毒剂/检测器）

Sanitizers 是编译器内置的运行时检测工具，用于发现各种 bug。

### 2.1 AddressSanitizer (ASan) — 内存错误检测

检测：缓冲区溢出、use-after-free、double-free、内存泄漏等

```bash
g++ -fsanitize=address -g -O1 -fno-omit-frame-pointer main.cpp -o main
./main  # 运行时发现内存错误会输出详细报告
```

### 2.2 ThreadSanitizer (TSan) — 数据竞争检测

检测：data race、锁顺序问题

```bash
g++ -fsanitize=thread -g -O1 main.cpp -o main
./main
```

### 2.3 UndefinedBehaviorSanitizer (UBSan) — 未定义行为检测

检测：整数溢出、空指针解引用、类型转换错误、对齐问题等

```bash
g++ -fsanitize=undefined -g -O1 main.cpp -o main
./main
```

### 2.4 MemorySanitizer (MSan) — 未初始化内存检测

检测：读取未初始化的内存

```bash
clang++ -fsanitize=memory -fsanitize-memory-track-origins -g -O1 main.cpp -o main
./main
```

> **注意**：MSan 仅支持 Clang，且需要所有依赖库也用 MSan 编译。

### 2.5 组合使用

```bash
# ASan + UBSan 可以同时启用
g++ -fsanitize=address,undefined -g -O1 main.cpp -o main
```
