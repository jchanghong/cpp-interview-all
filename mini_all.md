# 02_1_2_数组与字符串

### 1.2 数组与字符串

> 作者：青松与桑叶 | 数组是最基础的数据结构，字符串是面试必考

---

#### 数组是什么

数组是同一类型元素的有序集合，在内存中开辟连续空间存储同类型值。

#### 数组的内存布局

数组最大的特点是内存连续，所有元素紧挨排列，可利用 CPU 缓存局部性原理加速访问。// sizeof(arr)/sizeof(arr[0]) 计算数组元素个数，仅在定义作用域有效

#### 数组名与指针的关系

##### 数组名"退化"为指针

在大多数表达式中，数组名自动"退化"（decay）为指向首元素的指针。`arr` 和 `&arr` 值相同但类型不同：`arr` 是 `int*`，`&arr` 是 `int (*)[5]`。

##### 数组名和指针的三个关键区别

- `sizeof(数组名)` 得整个数组大小，`sizeof(指针)` 得指针本身大小（4或8字节）
- 数组名是常量不能被赋值，指针可以改变指向
- `&arr` 与 `&arr[0]` 类型不同

##### 用指针遍历数组

`*(p + i)` 等价于 `p[i]`；指针自增遍历：`for (int *q = arr; q < arr + 5; q++)`

#### 二维数组

二维数组是"数组的数组"——每个元素本身是一个数组，内存中按行优先连续存储。

##### 用指针访问二维数组

`arr[i][j]` 等价于 `*(*(arr + i) + j)`。`arr` 退化为 `int (*)[3]`（指向行的指针）。

##### 数组指针 vs 指针数组

- `int *ptr_arr[5]`：指针数组，5 个元素每个是 `int*`
- `int (*arr_ptr)[5]`：数组指针，指向含 5 个 int 的数组。看 `*` 和 `[]` 谁先结合

#### 数组作为函数参数

数组传给函数时退化为指针，丢失长度信息，必须额外传长度。三种写法等价：`void func(int arr[], int len)`、`void func(int *arr, int len)`、`void func(int arr[5], int len)`（5 被忽略）。

#### C语言字符串

C语言没有专门的字符串类型，字符串是以 `\0` 结尾的字符数组。
`char str1[] = "hello"`：字符数组可修改；`char *str2 = "hello"`：指向字符串常量（只读，修改为未定义行为）。

#### 常用字符串操作函数

// strlen(s) 返回 \0 前的字符数，不含 \0
// strcpy(dest, src) 将 src 拷贝到 dest（含 \0），不检查目标大小
// strcmp(s1, s2) 逐字符比较 ASCII，返回 0 / <0 / >0
// strcat(dest, src) 将 src 拼接到 dest 末尾
// strncpy / strncat / strncmp 可指定最大操作长度，更安全

#### 常见面试题

##### 面试题1：strlen 和 sizeof 的区别？

`strlen` 是运行时函数，计算到 `\0` 的字符数（不含 `\0`）；`sizeof` 是编译期运算符，返回变量/类型占用的总字节数（对数组含 `\0`，对指针返回指针自身大小）。

##### 面试题2：strcpy 和 strncpy 的区别？

`strcpy` 不检查目标缓冲区大小，可能造成缓冲区溢出。`strncpy` 可指定最大拷贝长度，但源串长度超 n 时不自动加 `\0`，推荐 `snprintf` 或封装安全版本。

##### 面试题3：数组越界会怎样？

数组越界是未定义行为，可能崩溃（段错误）、读到脏数据、或悄悄修改其他变量。C/C++ 不自动检查边界，可用 AddressSanitizer 检测。

##### 面试题4：实现反转字符串

双指针从两端向中间交换，时间复杂度 O(n)，空间复杂度 O(1)。

#### 踩坑提醒

- 字符数组不以 `\0` 结尾会导致未定义行为，数组大小至少比字符串长度多 1
- 用 `==` 比较字符串比较的是地址，应用 `strcmp`
- `strcpy` 目标空间不够造成缓冲区溢出
- `sizeof` 在函数参数中退化为指针大小

---

## docs__01-c-basics__memory__README.md
# 03_1_3_内存管理

### 1.3 内存管理

> 作者：青松与桑叶 | 内存管理是C/C++程序员的核心能力

---

#### 程序中的内存区域

从低地址到高地址：代码段(Text, 只读) → 数据段(Data, 已初始化全局/静态变量) → BSS段(未初始化全局/静态变量, 自动归零) → 堆(Heap, 向上增长, 手动管理) → 栈(Stack, 向下增长, 自动管理)。

#### 栈与堆的区别

| 特性 | 栈 (Stack) | 堆 (Heap) |
|------|-----------|-----------|
| 分配方式 | 编译器自动分配释放 | 程序员手动分配释放 |
| 分配速度 | 非常快（指针移动） | 较慢（查找空闲块） |
| 空间大小 | 较小（通常1MB~8MB） | 较大（取决于物理内存） |
| 生长方向 | 高地址向低地址 | 低地址向高地址 |
| 内存连续性 | 连续 | 不一定连续 |
| 碎片问题 | 无碎片 | 容易产生碎片 |
| 生命周期 | 函数结束自动释放 | 需要手动 free/delete |

#### malloc / calloc / realloc / free

- `malloc(size)`：分配 size 字节，不初始化（脏数据），失败返回 NULL
- `calloc(num, size)`：分配 num*size 字节并初始化为 0
- `realloc(ptr, size)`：调整内存大小，可能移动位置；ptr=NULL 等价 malloc；size=0 等价 free；失败返回 NULL 原内存不释放
- `free(ptr)`：释放堆内存；free(NULL) 安全；不能 double free；free 后建议置 NULL

#### new / delete（C++）

`new` 分配内存并调用构造函数，`delete` 调用析构函数并释放内存。new 失败抛 `std::bad_alloc`，malloc 返回 NULL。new/delete 配对，new[]/delete[] 配对，不可混用。`new` 自动计算大小、类型安全。

#### 内存泄漏

分配了内存但忘记释放，长期运行的程序可用内存逐渐减少。检测：Valgrind、AddressSanitizer、VS 内存诊断。预防：谁分配谁释放；goto 统一清理；C++ 优先使用智能指针。

#### 内存对齐

CPU 从对齐地址读取更快。基本类型对齐要求通常是自身大小。结构体对齐规则：首成员偏移 0；每成员偏移量为 min(成员大小, pack值) 的整数倍；总大小为最大成员对齐的整数倍。成员按大小降序排列可减少填充。// `#pragma pack(n)` 限制对齐为 n 字节；`alignas(k)` / `alignof(T)` C++11 标准对齐控制

#### 常见面试题

##### 栈和堆的区别？
见对比表。核心：栈自动管理速度快空间小，堆手动管理速度慢空间大。

##### malloc 的底层实现原理？
空闲链表 + 首次/最佳适应 + brk/sbrk（小内存）+ mmap（大内存）。glibc 的 ptmalloc 维护多内存池减少锁竞争。

##### 什么是内存泄漏？怎么检测和避免？
已分配但未释放的内存。检测：Valgrind、AddressSanitizer。避免：malloc/free 成对、goto 统一清理、智能指针。

##### new/delete 和 malloc/free 的区别？
new/delete 是运算符（调构造/析构），malloc/free 是函数（不调构造/析构）；new 失败抛异常，malloc 返回 NULL。

##### 什么是内存对齐？
数据起始地址为其大小的整数倍。原因：硬件效率、平台兼容性、原子操作。

#### 踩坑提醒

- malloc 不初始化，内容随机
- double free 未定义行为
- 不能 free 栈上内存
- new[] 必须用 delete[]
- 注意内存越界

---

## docs__01-c-basics__pointer__README.md
# 04_1_1_指针与引用

### 1.1 指针与引用

> 作者：青松与桑叶 | 指针是C/C++的灵魂

---

#### 什么是指针

指针变量存储内存地址。`&` 取地址符，`*` 解引用符。`int *p = &a;` 表示 p 存着 a 的地址，`*p` 访问 a 的值。

#### 指针的声明与初始化

`int *p;`（野指针危险）；`int *p = &a;`（常用）；`int *p = NULL;`（空指针）。声明后必须初始化，否则是野指针（随机地址）。C++ 推荐用 `nullptr`。

#### 指针的基本操作

`*p` 解引用访问/修改值。指针加减按元素个数计算（不是字节数）：`p + 1` 地址加 `sizeof(元素类型)`。

#### 多级指针

二级指针 `int **pp = &p;` 指向指针的指针，常用于在函数中修改指针的值（如分配内存）。`*pp` 取 p 的值，`**pp` 取 a 的值。

#### 函数指针

`int (*fp)(int, int) = add;` 指向函数的指针。可用于回调函数（如 qsort 的比较函数）、策略模式、接口解耦。// `typedef int (*MathFunc)(int, int);` 简化声明

#### 指针与引用的区别

| 特性 | 指针 | 引用 |
|------|------|------|
| 本质 | 存放地址的变量 | 变量的别名 |
| 必须初始化 | 可以不初始化（强烈建议） | 声明时必须初始化 |
| 能否为空 | 可为 NULL | 不能为空 |
| 能否改变指向 | 可随时改变 | 一旦绑定不能更改 |
| 占用内存 | 占用（4或8字节） | 不一定（编译器优化） |
| 解引用 | 需要 `*` | 直接使用 |
| sizeof | 指针本身大小 | 所绑定变量的大小 |

核心区别三点：引用必须初始化、引用不能改变指向、引用不需要解引用。

#### 常见面试题

##### 什么是野指针？怎么避免？
指向不可用内存的指针。原因：未初始化、free 后未置空、返回局部变量地址。避免：声明时初始化为 NULL、free 后置 NULL、不返回局部变量地址。

##### 函数指针有什么用？
回调函数（qsort、信号处理）、策略模式（运行时选函数）、接口解耦。

##### const 指针和指针 const 有什么区别？
`const int *p`：指向常量的指针（不能改值，可改指向）；`int * const p`：指针常量（不能改指向，可改值）。从右往左读。

##### 数组名和指针有什么区别？
sizeof 不同（数组大小 vs 指针大小）；数组名是常量不能赋值。

#### 踩坑提醒

- 未初始化指针（野指针）解引用是未定义行为
- free 后继续使用（悬垂指针），free 后立即置 NULL
- 不同类型指针解引用读取字节数不同
- 返回局部变量的地址（栈上变量函数结束销毁）
- 指针加减越界

---

## docs__01-c-basics__string__README.md
# 05_1_5_预处理与编译链接

### 1.5 预处理与编译链接

> 作者：青松与桑叶 | 搞懂编译链接过程才能真正理解C/C++程序的构建

---

#### 编译链接四阶段

1. **预处理**（`gcc -E`）：处理 `#` 指令（宏展开、头文件包含、条件编译、删除注释）
2. **编译**（`gcc -S`）：词法/语法/语义分析、优化，生成汇编代码
3. **汇编**（`gcc -c`）：汇编代码→机器码，生成目标文件（`.o`）
4. **链接**（`gcc`）：合并目标文件，符号解析、地址重定位，生成可执行文件

##### 静态链接 vs 动态链接

| 特性 | 静态链接 | 动态链接 |
|------|---------|---------|
| 库文件 | `.a`(Linux) / `.lib`(Windows) | `.so`(Linux) / `.dll`(Windows) |
| 方式 | 编译时将库代码复制到可执行文件 | 运行时加载库文件 |
| 优点 | 不依赖外部库，部署简单 | 文件小，多程序可共享库 |
| 缺点 | 文件大，更新需重新编译 | 运行时需依赖库存在 |

#### 预处理

预处理指令：`#include < >`（系统目录）vs `#include " "`（当前目录→系统目录）。`#pragma once` 防重复包含。`#pragma pack(n)` 设置对齐。

#### 宏定义

宏是预处理阶段的文本替换。// `#define PI 3.14159` 对象宏；`#define MAX(a,b) ((a)>(b)?(a):(b))` 函数宏，参数和整体必须加括号防优先级错误
宏无类型检查、不可调试、参数可能有副作用。现代 C++ 推荐用 `const`/`constexpr`/`inline` 替代宏。

#### 条件编译

`#ifdef` / `#ifndef` / `#endif` / `#if` / `#elif` / `#else` / `defined`。常用于跨平台代码、调试开关、头文件保护（`#ifndef` 宏卫士或 `#pragma once`）。

#### 头文件包含

头文件放：函数声明、类型定义、宏、内联函数、模板。不放：函数定义、变量定义、`using namespace`。// `extern int global_var;` 声明变量，在 `.c` 文件中定义

#### 预定义宏

`__FILE__`、`__LINE__`、`__DATE__`、`__TIME__`、`__STDC__`、`__func__`（C99）。

#### 常见面试题

##### C/C++ 编译链接的过程？
预处理（处理 # 指令）→ 编译（词法/语法/语义分析→汇编）→ 汇编（→机器码）→ 链接（符号解析+重定位）。静态链接编译时复制库代码，动态链接运行时加载。

##### 头文件为什么要加 #ifndef 保护？
防止头文件被多次包含导致重复定义（类型、函数声明等）。`#ifndef` 宏卫士或 `#pragma once`。

##### 宏定义和函数的区别？
宏是预处理文本替换（无类型检查、可导致代码膨胀、参数有副作用），函数是编译调用（有类型检查、可调试）。

##### define 和 typedef 的区别？
`#define` 预处理文本替换（无类型检查），`typedef` 编译阶段处理（有类型检查）。// `#define INT_PTR int*` 在 `INT_PTR a, b` 中 b 是 int

##### 静态链接和动态链接的区别？
静态链接编译时复制库代码（不依赖外部库、文件大、更新需重编），动态链接运行时加载（文件小、共享、需依赖库）。

#### 踩坑提醒

- 宏参数副作用：`SQUARE(a++)` 展开多次自增
- 宏多行定义每行末尾需 `\`
- 头文件定义变量导致多文件重复定义，用 `extern` 声明
- `#define` 无作用域概念

---

# 06_1_4_struct_与_union

### 1.4 struct 与 union

> 作者：青松与桑叶 | 结构体是C语言面向对象的基础，搞懂内存对齐是关键

---

#### struct 基本用法

结构体组合不同类型数据。`->` 箭头运算符用于指针访问成员。// `typedef struct Student { ... } Student;` 简化声明
结构体作为函数参数：值传递拷贝整个结构体（效率低），指针传递只传地址（推荐）。结构体占用内存不一定等于成员大小之和（内存对齐）。

#### struct 内存对齐

对齐规则：首成员偏移 0；每成员偏移量为 `min(成员大小, pack值)` 的整数倍；总大小为最大成员对齐的整数倍。// `#pragma pack(1)` 取消填充；`__attribute__((packed))` GCC 紧凑模式
成员按大小降序排列可减少填充。

#### 位域

指定结构体成员占用的位数。// `unsigned int readable : 1;` 占 1 位。类型通常用 `unsigned int`，不能取地址，位数不超过类型位数。常用于网络协议头、硬件寄存器。

#### 大小端

大端：高位字节存低地址；小端：低位字节存低地址。x86/x64 小端，ARM 默认小端，网络字节序（TCP/IP）大端。// `htonl()` / `ntohl()` 主机序↔网络序。判断：将 int 1 的地址强转为 char*，看首字节。

#### union 的用途

联合体所有成员共享同一块内存，大小等于最大成员。同一时刻只有一个成员有效。用途：判断大小端、类型转换（IEEE 754 float 字节查看）、节省内存的变体类型（配合 type 字段）。

#### 常见面试题

##### 结构体为什么要内存对齐？
硬件效率（对齐地址一次读完）、平台兼容性（某些架构不支持未对齐访问）、原子操作。

##### 怎么计算结构体的大小？
所有成员大小之和 + 填充字节。每个成员对齐到自身大小（或 pack 值）的整数倍，总大小对齐到最大成员对齐值。

##### 什么是大小端？怎么判断？
多字节存储顺序。大端高位在低地址，小端低位在低地址。强转 int 为 char* 看首字节。x86 小端，网络字节序大端。

##### union 和 struct 的区别？
struct 成员独立内存（总和+填充），union 成员共享内存（=最大成员的大小）。union 适用"同一时刻只用一个成员"的场景。

#### 踩坑提醒

- 结构体不能直接用 `==` 比较，需逐成员比较或 `memcmp`
- union 同时使用多个成员，前一成员被覆盖
- 位域成员不能取地址

---
# 07_前言

## docs__README.md

📚 本仓库是面向 C/C++ 技术方向校招求职者、初学者的基础知识总结，包括语言、程序库、数据结构、算法、系统、网络、链接装载库等知识及面试经验、招聘、内推等信息。本仓库遵循 CC BY-NC-SA 4.0 协议。

---
<a id="cc"></a>
# 09_CC

#### ➕ C/C++

##### const

1. 修饰变量，说明该变量不可被改变
2. 修饰指针，分指向常量的指针（pointer to const）和常量指针（const pointer）
3. 修饰引用（reference to const），避免拷贝的同时避免函数修改值
4. 修饰成员函数，说明该成员函数内不能修改成员变量
// `const int *p` = 指向常量的指针（不能改值可改指向）；`int * const p` = 常量指针（不能改指向可改值）；`const int * const p` = 两者都不能改

| 宏定义 #define | const 常量 |
|---|---|
| 预处理器处理（文本替换） | 编译器处理（有类型检查） |
| 不分配内存 | 可能分配存储 |
| 可通过 `#undef` 取消 | 不可取消 |

##### static

1. 修饰普通变量：存储在静态区，main 前分配，默认初始化
2. 修饰普通函数：仅在该文件内可见，防命名冲突
3. 修饰成员变量：所有对象共享一个，无需对象即可访问
4. 修饰成员函数：无需对象即可访问，函数内不能访问非静态成员

##### this 指针

隐含于每个非静态成员函数的特殊指针，指向调用该成员函数的对象。隐含声明为 `ClassName *const this`；const 成员函数中为 `const ClassName* const`。this 是右值，不能取地址。// 用于链式引用、避免自我赋值

##### inline 内联函数

在调用处展开代码，省去函数调用开销。有类型检查（优于宏）。编译器一般不内联含循环/递归/switch 的复杂函数。类内定义隐式内联。// `inline virtual` 唯一可内联时：编译器知道具体对象类型（非多态调用时）

##### volatile

告诉编译器不应对该变量进行优化，每次访问从内存取值（而非寄存器）。const 和 volatile 可同时使用（如只读状态寄存器）。

##### assert()

断言，宏而非函数。条件为假时终止程序。定义 `NDEBUG` 可关闭 assert（需在 `#include <assert.h>` 之前）。

##### sizeof()

对数组得整个数组大小；对指针得指针本身大小。

##### 编译器扩展与标准对齐控制

| 特性 | #pragma pack | alignas |
|------|--------------|---------|
| 标准化 | ❌ 编译器扩展 | ✅ C++11 标准 |
| 对齐方向 | ⬇️ 只能减小 | ⬆️ 只能增大 |
| 作用范围 | 整个结构 | 可针对单个成员 |

##### 位域

`unsigned int mode: 2;` 类可定义位域，用于传递二进制数据。位域类型必须是整型或枚举类型；不能取地址；布局与机器有关。

##### extern 与 extern "C"

`extern` 声明变量/函数具有外部链接。`extern "C"` 是链接指示，指定使用 C 语言链接（禁止 C++ 名称修饰），实现 C/C++ 互操作。

##### struct 和 typedef struct

C 中：`typedef struct Student { ... } S;` 后 `S` 等价于 `struct Student`。C++ 中：可直接用 `Student` 定义对象，但若定义了同名函数，则 `Student` 只代表函数。

##### C++ 中 struct 和 class

默认访问权限不同：struct 默认 public，class 默认 private。默认继承访问权限也不同：struct 为 public，class 为 private。

##### union 联合

所有成员共享同一块内存，同一时刻只有一个成员有值。默认访问控制 public；可含构造/析构函数；不能有引用成员；不能继承/被继承；不能有虚函数。支持匿名 union（可直接访问成员）。

##### C 实现 C++ 类

封装：函数指针属性+方法；继承：结构体嵌套；多态：函数指针不同。

##### explicit（显式）关键字

修饰构造函数防止隐式转换和复制初始化；修饰转换函数防止隐式转换（按语境转换除外）。

##### friend 友元类和友元函数

能访问私有成员；破坏封装性；关系不可传递；单向性；数量不限。

##### using

`using 声明` 引入命名空间单个成员（安全推荐）。`using 指示`（`using namespace std`）引入所有名字，易污染命名空间。C++11 允许派生类 `using Base::Base;` 重用基类构造函数。

##### :: 范围解析运算符

全局作用域 `::name`；类作用域 `class::name`；命名空间作用域 `namespace::name`。

##### enum 枚举类型

限定作用域：`enum class open_modes { input, output, append };`。不限定作用域：`enum color { red, yellow, green };`。

##### decltype

检查实体的声明类型或表达式的类型及值分类。// `decltype(expression)` 常用于尾置返回类型。`auto fcn(It beg, It end) -> decltype(*beg)`

##### 引用

左值引用：常规引用，表示对象身份。右值引用：绑定到右值（临时对象），实现转移语义和完美转发。引用折叠：`X& &`、`X& &&`、`X&& &` 折叠为 `X&`；`X&& &&` 折叠为 `X&&`。

##### 宏

宏定义实现类似函数功能，但终归是文本替换而非真正的函数调用。

##### 成员初始化列表

好处：更高效（少一次默认构造）；必须用：常量成员、引用类型、无默认构造函数的类类型。

##### initializer_list 列表初始化

花括号初始化器列表初始化对象，对应构造函数接受 `std::initializer_list` 参数。

##### 面向对象

三大特征：封装（public/protected/private）、继承、多态。

##### 封装

`public` 任意实体访问；`protected` 子类及本类访问；`private` 仅本类及友元访问。不写默认为 private。

##### 继承

基类→派生类。

##### 多态

C++ 多态分类：重载多态（函数/运算符重载，编译期）、子类型多态（虚函数，运行期）、参数多态（模板，编译期）、强制多态（类型转换）。
静态多态（编译期）：函数重载。动态多态（运行期）：虚函数。动态绑定条件：基类引用/指针调用虚函数。
注意：构造函数不能是虚函数（虚表指针还未形成）；static 函数不能是虚函数；内联函数不能是表现多态性的虚函数。

##### 虚析构函数

解决基类指针指向派生类对象，delete 时正确调用派生类析构函数的问题。若基类有虚函数，应声明虚析构函数。

##### 纯虚函数

`virtual int A() = 0;` 基类不提供实现，由派生类完成。含有纯虚函数的类是抽象类，不能实例化。

##### 虚函数 vs 纯虚函数

虚函数有默认实现（可被覆盖），纯虚函数只有接口声明（必须在子类实现）。虚函数用于"实作继承"，纯虚函数关注接口统一性。

##### 虚函数指针、虚函数表

含虚函数的对象有一个 vptr 指向虚函数表（vtbl，存于 .rodata 段）。派生类覆盖基类虚函数时，虚表中对应项被覆盖。

##### 虚继承

解决多继承下的菱形继承问题（二义性+存储冗余）。通过虚基类指针（vbptr）和虚基类表实现，避免公共基类的多份拷贝。

##### 抽象类、接口类、聚合类

抽象类：含纯虚函数；接口类：仅含纯虚函数；聚合类：所有成员 public、无构造函数、无类内初始化、无基类无虚函数。

#### 内存分配和管理

##### malloc/calloc/realloc/alloca

malloc 不初始化；calloc 初始化为 0；realloc 调整大小；alloca 栈上分配（不可移植）。

##### new/delete

new 先 malloc 分配内存再调构造函数；delete 先调析构函数再 free。new 自动计算大小。

##### 定位 new

`new (place_address) type` 在预先指定的内存区域创建对象。

##### delete this 合法吗？

合法但条件苛刻：对象必须由 new 分配（非 new[]、非 placement new、非栈上）；此成员函数必须是最后一个调用 this 的函数；delete this 后不可再访问 this。

##### 智能指针

`#include <memory>`。C++11: `shared_ptr`（共享拥有/引用计数）、`unique_ptr`（独占拥有/无拷贝语义但有 move）、`weak_ptr`（打破循环引用）。C++98 `auto_ptr` 已弃用（缺乏 move 语义）。

#### 强制类型转换运算符

| 转换符 | 阶段 | 安全检查 |
|--------|------|---------|
| `static_cast` | 编译时 | 数值/向上转型安全，向下转型不安全 |
| `dynamic_cast` | 运行时(RTTI) | 安全（失败返回 nullptr 或抛 bad_cast）；基类需有虚函数 |
| `const_cast` | 编译时 | 仅修改 const/volatile 属性，移除 const 有风险 |
| `reinterpret_cast` | 编译时 | 最危险，二进制位级重新解释 |

##### bad_cast

`dynamic_cast` 引用转换失败的异常类型。

#### 运行时类型信息 (RTTI)

`typeid` 返回 `type_info` 对象引用，可在运行时确定对象类型（基类需有虚函数）。`type_info` 提供 `name()`、`==`、`!=` 等操作。

---
<a id="effective"></a>
# 10_Effective

#### ⭐️ Effective

##### Effective C++

1. 视 C++ 为一个语言联邦（C、面向对象 C++、Template C++、STL）
2. 宁以编译器替换预处理器（尽量用 const/enum/inline 替换 #define）
3. 尽可能使用 const
4. 确定对象被使用前已先被初始化
5. 了解 C++ 默默编写并调用哪些函数（默认构造、拷贝构造、拷贝赋值、析构）
6. 若不想使用编译器自动生成函数，应明确拒绝（声明为 private 且不实现）
7. 为多态基类声明 virtual 析构函数
8. 别让异常逃离析构函数
9. 绝不在构造和析构过程中调用 virtual 函数
10. 令 `operator=` 返回 `reference to *this`
11. 在 `operator=` 中处理自我赋值
12. 复制对象时确保复制所有成员变量及基类成分
13. 以对象管理资源（RAII，智能指针）
14. 在资源管理类中小心 copying 行为
15. 在资源管理类中提供对原始资源的访问
16. 成对使用 new 和 delete 要采取相同形式
17. 以独立语句将 newed 对象置入智能指针
18. 让接口容易被正确使用，不易被误用
19. 设计 class 犹如设计 type
20. 宁以 pass-by-reference-to-const 替换 pass-by-value
21. 必须返回对象时，别妄想返回其 reference
22. 将成员变量声明为 private
23. 宁以 non-member、non-friend 替换 member 函数
24. 若所有参数皆需类型转换，请采用 non-member 函数
25. 考虑写一个不抛异常的 swap 函数
26. 尽可能延后变量定义式的出现时间
27. 尽量少做转型动作
28. 避免使用 handles 指向对象内部
29. 为异常安全而努力
30. 透彻了解 inlining 的里里外外
31. 将文件间的编译依存关系降至最低
32. 确定 public 继承塑模出 is-a 关系
33. 避免遮掩继承而来的名字
34. 区分接口继承和实现继承
35. 考虑 virtual 函数以外的其他选择
36. 绝不重新定义继承而来的 non-virtual 函数
37. 绝不重新定义继承而来的缺省参数值
38. 通过复合塑模 has-a 或"根据某物实现出"
39. 明智审慎地使用 private 继承
40. 明智审慎地使用多重继承
41. 了解隐式接口和编译期多态
42. 了解 typename 的双重意义
43. 学习处理模板化基类内的名称
44. 将与参数无关的代码抽离 templates
45. 运用成员函数模板接受所有兼容类型
46. 需要类型转换时请为模板定义非成员函数
47. 请使用 traits classes 表现类型信息
48. 认识 template 元编程
49. 了解 new-handler 的行为
50. 了解 new 和 delete 的合理替换时机
51. 编写 new 和 delete 时需固守常规
52. 写了 placement new 也要写 placement delete
53. 不要轻忽编译器的警告
54. 让自己熟悉包括 TR1 在内的标准程序库
55. 让自己熟悉 Boost

##### More Effective C++

1. 仔细区别 pointers 和 references
2. 最好使用 C++ 转型操作符
3. 绝不要以多态方式处理数组
4. 非必要不提供 default constructor
5. 对定制"类型转换函数"保持警觉
6. 区别 increment/decrement 操作符的前置和后置形式
7. 千万不要重载 `&&`、`||` 和 `,` 操作符
8. 了解各种不同意义的 new 和 delete
9. 利用 destructors 避免泄漏资源
10. 在 constructors 内阻止资源泄漏
11. 禁止异常流出 destructors 之外
12. 了解"抛出一个 exception"与"传递一个参数"或"调用一个虚函数"之间的差异
13. 以 by reference 方式捕获 exceptions
14. 明智运用 exception specifications
15. 了解异常处理的成本
16. 谨记 80-20 法则
17. 考虑使用 lazy evaluation
18. 分期摊还预期的计算成本

---
<a id="stl"></a>
# 11_STL

#### 📦 STL

##### STL 容器

| 容器 | 底层数据结构 | 时间复杂度 | 有序/无序 | 可重复 |
|------|-------------|-----------|-----------|--------|
| array | 数组 | 随机 O(1) | 无序 | 可重复 |
| vector | 数组 | 尾部 O(1), 头/中 O(n) | 无序 | 可重复 |
| deque | 双端队列 | 头尾 O(1) | 无序 | 可重复 |
| forward_list | 单向链表 | 插入删除 O(1) | 无序 | 可重复 |
| list | 双向链表 | 插入删除 O(1) | 无序 | 可重复 |
| stack | deque/list | 顶部 O(1) | 无序 | 可重复 |
| queue | deque/list | 尾插头删 O(1) | 无序 | 可重复 |
| priority_queue | vector+heap | 插入删除 O(log n) | 有序 | 可重复 |
| set | 红黑树 | O(log n) | 有序 | 不可重复 |
| multiset | 红黑树 | O(log n) | 有序 | 可重复 |
| map | 红黑树 | O(log n) | 有序 | 不可重复 |
| multimap | 红黑树 | O(log n) | 有序 | 可重复 |
| unordered_set | 哈希表 | O(1), 最差 O(n) | 无序 | 不可重复 |
| unordered_multiset | 哈希表 | O(1), 最差 O(n) | 无序 | 可重复 |
| unordered_map | 哈希表 | O(1), 最差 O(n) | 无序 | 不可重复 |
| unordered_multimap | 哈希表 | O(1), 最差 O(n) | 无序 | 可重复 |

##### STL 算法

| 算法 | 底层 | 时间复杂度 |
|------|------|-----------|
| find | 顺序查找 | O(n) |
| sort | 内省排序 | O(n log n) |

---
<a id="data-structure"></a>
# 12_数据结构

#### 〽️ 数据结构

##### 顺序结构

顺序栈：栈顶指针 top。队列：非循环队列 rear++，循环队列 `rear = (rear + 1) % maxSize`。顺序表：动态数组。

##### 链式结构

单链表 LNode（data + next）。链队列、双向链表、循环链表。

##### 哈希表

哈希函数构造：直接定址、除留余数、数字分析、折叠、平方取中。冲突处理：链地址法、开放定址法（线性探测、二次探测、随机探测）。

##### 递归

函数直接或间接调用自身。分治法：折半查找、归并排序、快速排序。迭代：用变量旧值推出新值。

##### 广义表

头尾链表存储表示（ATOM/LIST 联合）。扩展线性链表存储表示。

##### 二叉树

性质：第 i 层最多 2^(i-1) 个结点；深度 k 最多 2^k-1 个结点；n0 = n2 + 1；完全二叉树深度 k = floor(log2(n)) + 1。
存储结构：顺序存储 / 链式存储（BiTNode: data, lchild, rchild）。遍历：先序、中序、后序、层次。
分类：满二叉树、完全二叉树（大顶堆/小顶堆）、二叉查找树（左<根<右）、平衡二叉树（AVL，|左高-右高|<=1）。AVL 调整：LL/RR/LR/RL 旋转。
红黑树特征：节点红或黑；根黑；叶(NIL)黑；红节点子必黑；任一节点到叶的路径含相同数量黑节点。调整：变色、左旋、右旋。应用：STL map/set。
B树/B+树：多路平衡查找树，用于文件系统和数据库索引。B+树只有叶子节点带 ROWID，叶子节点通过指针连接，范围扫描简单。

##### 并查集

不相交子集的集合 S = {S1, S2, ..., Sn}。

##### 八叉树

三维空间划分，每个节点有八个子节点，用于三维图形和最邻近搜索。

---
<a id="algorithm"></a>
# 13_算法

#### ⚡️ 算法

##### 排序

| 算法 | 平均 | 最差 | 空间 | 稳定性 |
|------|------|------|------|--------|
| 冒泡排序 | O(n²) | O(n²) | O(1) | 稳定 |
| 选择排序 | O(n²) | O(n²) | O(1) | 不稳定 |
| 插入排序 | O(n²) | O(n²) | O(1) | 稳定 |
| 快速排序 | O(n log n) | O(n²) | O(log n) | 不稳定 |
| 堆排序 | O(n log n) | O(n log n) | O(1) | 不稳定 |
| 归并排序 | O(n log n) | O(n log n) | O(n) | 稳定 |
| 希尔排序 | O(n log²n) | O(n²) | O(1) | 不稳定 |
| 计数排序 | O(n+m) | O(n+m) | O(n+m) | 稳定 |
| 桶排序 | O(n) | O(n) | O(m) | 稳定 |
| 基数排序 | O(k*n) | O(n²) | | 稳定 |

##### 查找

| 算法 | 平均时间复杂度 | 空间 | 条件 |
|------|--------------|------|------|
| 顺序查找 | O(n) | O(1) | 无序/有序 |
| 二分查找 | O(log n) | O(1) | 有序 |
| 插值查找 | O(log log n) | O(1) | 有序 |
| 哈希查找 | O(1) | O(n) | 无序/有序 |
| 二叉搜索树 | O(log n) | | |
| 红黑树 | O(log n) | | |
| B/B+树 | O(log n) | | |

##### 图搜索算法

| 算法 | 数据结构 | 时间复杂度 | 空间复杂度 |
|------|---------|-----------|-----------|
| BFS | 邻接矩阵 | O(|V|²) | O(|V|²) |
| BFS | 邻接链表 | O(|V|+|E|) | O(|V|+|E|) |
| DFS | 邻接矩阵 | O(|V|²) | O(|V|²) |
| DFS | 邻接链表 | O(|V|+|E|) | O(|V|+|E|) |

##### 其他算法

分治法（分解→求解→合并，快排/归并/循环赛日程）；动态规划（重叠子问题+最优子结构，背包/斐波那契）；贪心法（每步最优，最短路径/最小生成树/哈夫曼编码）。

---
<a id="problems"></a>
# 14_Problems

#### ❓ Problems

棋盘覆盖、背包问题、冯诺依曼邻居问题、循环赛日程安排、输油管道问题。LeetCode / 剑指 Offer / 程序员面试金典 / 牛客网。

---
<a id="os"></a>
# 15_操作系统

#### 💻 操作系统

##### 进程与线程

进程是资源分配独立单位，线程是资源调度独立单位。

###### 进程间通信方式

管道（无名：亲缘关系单向；有名：任意关系双向）、信号量（计数器同步）、信号（通知事件）、消息队列（内核链表）、共享内存（最快需同步）、套接字（跨网络）。

###### 线程间通信方式

互斥锁、读写锁、自旋锁、条件变量、信号量、信号、屏障。

###### 进程私有和共享资源

私有：地址空间、堆、全局变量、栈、寄存器；共享：代码段、公共数据、进程目录、进程 ID。

###### 线程私有和共享资源

私有：线程栈、寄存器、程序计数器；共享：堆、地址空间、全局/静态变量。

###### 多进程 vs 多线程

多进程：编程调试简单、可靠性高、适合多机分布式；但创建销毁切换慢、资源占用大。多线程：创建销毁切换快、资源占用小；但编程调试复杂、一个线程崩溃影响整个进程。

##### Linux 内核同步方式

原子操作、信号量、读写信号量、自旋锁、大内核锁(BKL)、读写锁、大读者锁(brlock)、RCU、顺序锁(seqlock)。

##### 死锁

原因：资源不足、分配不当、推进顺序不合适。四个必要条件：互斥、请求保持、不剥夺、环路。预防：打破任一个条件。银行家算法避免死锁。

##### 文件系统

Windows：FCB+FAT+位图。Unix：inode+混合索引+成组链接。

##### 主机字节序与网络字节序

大端（高位在低地址）、小端（低位在低地址）。x86 小端，网络字节序（TCP/IP）大端。判断方法：int 1 强转为 char* 看首字节。

##### 页面置换算法

最佳置换(OPT)、先进先出(FIFO)、最近最久未使用(LRU)、时钟(Clock)、工作集、缺页率置换。

---
<a id="computer-network"></a>
# 16_计算机网络

#### ☁️ 计算机网络

##### 各层作用及协议

| 分层 | 作用 | 协议 |
|------|------|------|
| 物理层 | 传输比特，机械电气规范 | RJ45, IEEE802.3 |
| 数据链路层 | 比特→帧，点到点传递 | PPP, MAC(交换机) |
| 网络层 | 数据包路由和网际互连 | IP, ICMP, ARP, OSPF(路由器) |
| 运输层 | 端到端可靠/不可靠传递 | TCP, UDP |
| 会话层 | 建立管理终止会话 | NFS, SQL, RPC |
| 表示层 | 翻译加密压缩 | JPEG, MPEG, ASCII |
| 应用层 | 应用协议数据单元 | HTTP, FTP, DNS, SMTP |

##### 物理层

传输单位：比特。通道：单工、半双工、全双工。复用技术：FDM、TDM、WDM、CDM。

##### 数据链路层

封装成帧、透明传输（字节填充）、差错检测（CRC）。点对点协议(PPP)。广播信道：MAC 地址、单播/广播/多播。

##### 网络层

IP 协议（A/B/C/D/E 类地址）、ARP（地址解析 IP→MAC）、ICMP（ping/TTL）、IGMP（组管理）。路由协议：内部 RIP/OSPF、外部 BGP。VPN、NAT。路由表：网络ID、子网掩码、下一跳地址。

##### 运输层

TCP：面向连接、可靠、全双工、字节流。保证可靠：确认+超时重传、分片排序、流量控制（滑动窗口）、拥塞控制（慢开始、拥塞避免、快重传、快恢复）、校验。TCP 首部标志：URG/ACK/PSH/RST/SYN/FIN。
UDP：无连接、不可靠、面向报文、无拥塞控制、首部开销小（8字节）。
TCP vs UDP：面向连接/无连接；可靠/不可靠；点到点/多对多；字节流/报文；有拥塞控制/无；首部20B/8B。
三次握手：SYN→SYN+ACK→ACK。四次挥手：FIN→ACK→FIN→ACK。TIME-WAIT 2MSL 保证最后 ACK 可达和防止旧报文干扰。
黏包：TCP 字节流无边界。解决：定长包、包头+包体长度、特殊分隔符、应用层协议。

##### 应用层

DNS（域名→IP，UDP/TCP 53）、FTP（TCP，文件传输）、TELNET（远程登录）、HTTP/HTTPS、SMTP（邮件）、DHCP（动态IP分配）、SNMP（网络管理）。
HTTP 请求方法：GET/POST/PUT/DELETE/HEAD/OPTIONS/TRACE。状态码：1xx（信息）、2xx（成功200）、3xx（重定向301）、4xx（客户端错误400/401/403/404）、5xx（服务器错误500/503/504）。

---
<a id="network-programming"></a>
# 17_网络编程

#### 🌩 网络编程

##### Socket

// `ssize_t read(int fd, void *buf, size_t count);` 从 fd 读取，返回实际字节数（0 表示结束，<0 出错）
// `ssize_t write(int fd, const void *buf, size_t count);` 写入 fd，返回写入字节数（<0 出错）
TCP 三次握手与 socket 函数对应：connect()→SYN；accept()→SYN+ACK；connect()返回→ACK；accept()返回→连接建立。
四次挥手：close()→FIN；被动关闭→ACK；close()→FIN；确认→ACK。

---
<a id="database"></a>
# 18_数据库

#### 💾 数据库

基本概念：数据、数据库(DB)、数据库管理系统(DBMS)、数据库系统(DBS)、实体、属性、码、模式/外模式/内模式。
常用数据模型：层次、网状、关系（表/元组/属性/码/域）、面向对象、对象关系、半结构化。
关系操作：查询（选择/投影/连接/除/并/差/交/笛卡尔积）、插入、删除、修改。完整性约束：实体、参照、用户定义。
索引：顺序索引、B+树索引、hash 索引。
完整性 vs 安全性：完整性防止不符语义数据，安全性防止恶意破坏。
范式：1NF(属性不可分)→2NF(消除部分依赖)→3NF(消除传递依赖)→BCNF(主属性完全依赖)→4NF(消除非平凡多值依赖)。
事务 ACID：原子性、一致性、隔离性、持久性。恢复：数据转储+日志。
并发控制：封锁（X锁排他/S锁共享）、时间戳、乐观控制、多版本并发控制(MVCC)。死锁预防：一次封锁、顺序封锁。可串行化调度。

---
<a id="design-pattern"></a>
# 19_设计模式

#### 📏 设计模式

单例模式、抽象工厂模式、适配器模式、桥接模式、观察者模式。
六大原则：单一职责(SRP)、里氏替换(LSP)、依赖倒置(DIP)、接口隔离(ISP)、迪米特法则(LoD)、开放封闭(OCP)。

---
<a id="link-loading-library"></a>
# 20_链接装载库

#### ⚙️ 链接装载库

##### 内存、栈、堆

栈（函数返回地址/参数/局部变量/上下文）、堆（空闲链表/位图/对象池）、可执行文件映像、保留区。段错误：非法指针解引用（未初始化/随机值）。

##### 编译链接

| 平台 | 可执行 | 目标 | 动态库 | 静态库 |
|------|--------|------|--------|--------|
| Windows | exe | obj | dll | lib |
| Linux | ELF | o | so | a |
| Mac | Mach-O | o | dylib | a |

编译链接：预编译→编译（词法/语法/语义分析→优化）→汇编（→机器码）→链接（地址分配/符号决议/重定位）。
目标文件格式：PE(Windows)、ELF(Linux)、OMF、a.out。段：File Header、.text(代码)、.data(已初始化全局)、.bss(未初始化)、.rodata(只读数据/常量)。
符号表：函数/变量名→地址。

##### Linux 共享库

命名 `libname.so.x.y.z`（主/次/发布版本号）。路径：/lib、/usr/lib、/usr/local/lib。环境变量：LD_LIBRARY_PATH、LD_PRELOAD、LD_DEBUG。

##### Windows DLL

入口函数 `DllMain`（DLL_PROCESS_ATTACH/DETACH、DLL_THREAD_ATTACH/DETACH）。载入：LoadLibrary/FreeLibrary。导出符号：GetProcAddress。

##### 运行库

典型程序运行：创建进程→入口函数初始化→main→清理→结束。glibc 入口：`_start→__libc_start_main→exit→_exit`。MSVC CRT：`mainCRTStartup`。

---
<a id="books"></a>
# 21_书籍

#### 📚 书籍

语言：《C++ Primer》《Effective C++》《More Effective C++》《深度探索C++对象模型》《深入理解C++11》《STL源码剖析》。算法：《剑指Offer》《编程珠玑》《程序员面试宝典》。系统：《深入理解计算机系统》《Windows核心编程》《Unix环境高级编程》。网络：《Unix网络编程》《TCP/IP详解》。其他：《程序员的自我修养》。

---
<a id="cc-development-direction"></a>
# 22_CC_发展方向

#### 🔱 C/C++ 发展方向

后台/服务器、桌面客户端(MFC/Windows)、图形学/游戏/VR/AR(Unity/Unreal)、测试开发、网络安全/逆向、嵌入式/物联网、音视频/流媒体/SDK(HEVC/H264)、计算机视觉/机器学习。

---
<a id="review-of-brush-questions-website"></a>
# 23_复习刷题网站

#### 💯 复习刷题网站

cplusplus、cppreference、runoob、leetcode、lintcode、nowcoder。

---
<a id="interview-questions-experience"></a>
# 24_面试题目经验

#### 📝 面试题目经验

牛客网面经合集、知乎面经讨论、CSDN C++面试题整理、cnblogs C++面试集锦、segmentfault C++面试问题总结。

---
<a id="recruitment-time-post"></a>
# 25_招聘时间岗位

#### 📆 招聘时间岗位

牛客网名企校招日程。

---
<a id="recommend"></a>
# 26_内推

#### 👍 内推

互联网内推信息（社招/校招/实习）、AI岗位招聘信息。

---
<a id="contributor"></a>
# 27_贡献者

#### 👬 贡献者

---
<a id="license"></a>
# 28_License

#### 📜 License

CC BY-NC-SA 4.0（署名-非商业性使用-相同方式共享）。

---
## file__入门教程阿里云大学.md
# 29_C_入门教程精简说明

### C++ 入门教程精简说明

> ⚠️ 阿里云大学 C++ 入门教程的核心知识已在各专题章节覆盖：指针(1.1)、字符串(1.2)、内存(1.3)、struct/union(1.4)、类与对象(2.1)、STL、异常/模板/命名空间、Lambda/右值引用等。

---
# 30_参考资料

### 参考资料

来源：https://github.com/0voice/cpp_new_features

---
## file__学习笔记.md
# 31_C_学习笔记

### C++学习笔记

> 本节为入门级速查，详细内容见各专题章节。

- **goto**：无条件跳转，不建议使用
- **数组名**：常量（不可赋值），可取首地址和 sizeof
- **值传递 vs 地址传递**：值传递不改变实参，地址传递（指针/引用）可改变实参
- **指针大小**：32位 4 字节，64位 8 字节
- **nullptr**：C++11 类型安全空指针字面量，替代 NULL
- **const 修饰指针**：`const int *p`（指向常量的指针）vs `int * const p`（指针常量）
- **引用本质**：指针常量 `int* const`，必须初始化，不可改变绑定
- **内存四区**：代码区（只读共享）、全局区（全局/静态/常量）、栈区（局部）、堆区（new/delete）
- **new/delete**：堆区操作，配对使用（数组用 new[]/delete[]）
- **函数默认参数**：从右向左连续设置，声明和实现只能一处有默认值
- **函数重载**：同作用域同名参数不同（类型/个数/顺序），返回值不参与重载

---
# 32_problems__如何在main函数前执行代码_md

## problems__如何在main函数前执行代码？.md

#### 如何在 main 函数前执行代码？

方法一：全局对象构造函数（C++方式），全局/静态对象初始化在静态初始化阶段早于 main。
方法二：`__attribute__((constructor))`（GCC/Clang）。
方法三：MSVC `#pragma init_seg`。

**核心答案**：全局对象的构造函数在 main 前执行（静态初始化阶段）。不同编译单元初始化顺序未定义，应避免依赖。应用场景：注册插件、初始化日志、加载配置。


---


# 02_现代_C_特性速览_C_1114172023

### 现代 C++ 特性速览（C++11/14/17/20/23）

> 面试高频：熟悉现代 C++ 特性是社招的基本要求，以下为各版本核心特性摘要。

#### C++11 (2011) — 现代 C++ 起点
- **自动类型推导**：`auto`、`decltype`
- **移动语义**：右值引用 `&&`、`std::move`、`std::forward`、引用折叠规则
- **智能指针**：`std::shared_ptr`、`std::unique_ptr`、`std::weak_ptr`（取代 `auto_ptr`）
- **Lambda 表达式**：`[capture](params) -> ret { body }`
- **范围 for 循环**：`for (auto& x : container)`
- **`nullptr`**：类型安全的空指针，取代 `NULL`
- **`constexpr`**：编译期常量表达式
- **静态断言**：`static_assert`
- **`override` / `final`**：虚函数重写与禁止继承
- **`enum class`**：限定作用域的强类型枚举
- **可变参数模板**：`template<typename... Args>`
- **`std::tuple` / `std::bind` / `std::function`**
- **Unordered 容器**：`unordered_map` / `unordered_set` 等

#### C++14 (2014) — 增量改进
- **泛型 Lambda**：`auto` 参数
- **`constexpr` 放宽**：允许局部变量、循环、分支
- **变量模板**：`template<T> constexpr T pi = T(3.14);`
- **`std::make_unique`**
- **`decltype(auto)`**
- **二进制字面量**：`0b1010`
- **数字分隔符**：`1'000'000`

#### C++17 (2017) — 关键改进
- **结构化绑定**：`auto [x, y] = func_returning_pair();`
- **`if constexpr`**：编译期条件分支
- **折叠表达式**：`(args + ...)`
- **`std::optional`**：可能为空的值
- **`std::variant`**：类型安全的联合体
- **`std::any`**：可存储任意类型的对象
- **`std::string_view`**：非 owning 字符串视图
- **`inline` 变量**：头文件中定义全局变量
- **嵌套命名空间**：`namespace A::B::C {}`
- **类模板参数推导 (CTAD)**：`std::pair p(1, 2.0);`
- **`[[fallthrough]]` / `[[maybe_unused]]` / `[[nodiscard]]`**
- **文件系统库**：`std::filesystem`
- **并行算法**：`std::sort(std::execution::par, ...)`

#### C++20 (2020) — 重大更新
- **概念 (Concepts)**：`template<Sortable T>`，约束模板参数
- **范围库 (Ranges)**：`std::ranges::sort`，惰性视图与适配器
- **协程 (Coroutines)**：`co_await` / `co_yield` / `co_return`
- **模块 (Modules)**：`import` / `export`，替代头文件
- **三路比较运算符 `<=>`**：自动生成比较运算符
- **`std::span`**：连续内存的视图（类似 `string_view` 但用于数组）
- **`constexpr` 扩展**：`constexpr` 虚函数、`constexpr` `new`、`constexpr` `vector`/`string`
- **`consteval`**：强制编译期求值
- **`constinit`**：保证静态初始化顺序
- **`std::format`**：Python 风格的字符串格式化
- **`std::jthread`**：自动 `join` 的线程 / **`std::stop_token`**：协程/线程的协作式取消
- **设计化 Lambda**：允许模板参数 `[]<typename T>(T x){}`
- **位域默认成员初始化**

#### C++23 (2023) — 新特性
- **`std::expected`**：带错误码的返回值 / **`std::mdspan`**：多维数组视图
- **`std::print` / `std::println`** / **`if consteval`**
- **`deducing this`**：推导 `this` 参数
- **范围库增强**：`views::enumerate`、`views::zip`、`views::adjacent`
- **`std::optional` 与范围交互增强**

---
# 03_problems__noexcept详解_md

## problems__noexcept详解.md

#### noexcept 是什么？有什么作用？

`noexcept` 是 C++11 引入的关键字，有两种用法：1) **`noexcept` 说明符**声明函数不会抛出异常；2) **`noexcept` 运算符**编译期检查表达式是否可能抛出异常。
// `void func1() noexcept;` — 承诺不抛异常；`void func4() noexcept(noexcept(T()));` — 条件 noexcept

**为什么重要？** 1) **移动优化**：`std::vector` 扩容时，若元素移动构造为 `noexcept` 则用移动而非拷贝（`std::move_if_noexcept`）；2) **编译器优化**：跳过异常处理栈展开逻辑；3) **接口承诺**：明确表达函数不抛异常。

**何时使用？** 移动构造函数、移动赋值运算符、swap 函数、析构函数应尽可能标记 `noexcept`。

---
# 04_problems__thread_local详解_md

## problems__thread_local详解.md

#### thread_local 是什么？和 static 有什么区别？

`thread_local` 是 C++11 引入的存储期关键字，每个线程拥有变量的独立实例，生命周期等于线程的生命周期。

| 特性 | `static` | `thread_local` |
|------|---------|---------------|
| 实例数 | 整个程序 1 份 | 每个线程 1 份 |
| 初始化 | 首次执行到时 | 每个线程首次访问时 |
| 销毁 | 程序结束时 | 线程退出时 |

// 示例：`thread_local int counter = 0;` 每个线程有自己的 counter

**典型使用场景**：替代全局 `errno`、线程级缓存、随机数生成器、数据库连接池的线程局部连接。
**面试标准答案**：`thread_local` 使变量在每个线程中有独立副本，生命周期与线程一致。与 `static` 关键区别：`static` 全局共享一份需同步，`thread_local` 天然线程安全。底层通过 TLS（Thread Local Storage）机制实现。

---
# 05_problems__emplace_back和push_back有什么区别_md

## problems__emplace_back和push_back有什么区别？.md

#### emplace_back 和 push_back 有什么区别？

| 特性 | `push_back` | `emplace_back` |
|------|------------|---------------|
| 参数 | 接受已有对象（触发拷贝/移动） | 接受构造函数参数（原地构造） |
| 临时对象 | 会产生临时对象 | 不产生临时对象 |
| 性能 | 多一次移动/拷贝 | 直接构造，更高效 |

// `std::vector::emplace_back` 在容器内原地构造，避免临时对象；`push_back` 先构造再拷贝/移动

**建议**：构造新元素时优先用 `emplace_back`；传递已有对象时用 `push_back`。注意：`emplace_back` 不会阻止隐式类型转换，可能导致意外行为。

---
# 06_CAS

### CAS

CAS（Compare-And-Swap）在 C++ 面试里的两个常见落点：无锁原子计数器和基于 `atomic_flag` 的自旋锁。

// CAS 核心语义：比较内存中的旧值是否等于预期值，若相等则原子写入新值
// `compare_exchange_weak` 允许伪失败，适合放在循环里重试；`compare_exchange_strong` 更严格
// 自旋锁适合临界区很短、线程切换代价较高的场景
// 高频延伸问题：ABA 问题、内存序、忙等 CPU 消耗、饥饿与公平性


## Interview__SmartPointer__README.md

> 本节为面试项目说明与速查要点。完整教学讲解请参见 [2.4 智能指针](#24-智能指针)。
# 07_SmartPointer

### SmartPointer

实现了一个面试友好的最小版 `UniquePtr` 和 `SharedPtr`，用来解释 RAII、所有权转移和引用计数。

// `unique_ptr` 独占所有权，禁止拷贝，允许移动
// `shared_ptr` 通过控制块维护引用计数；生产级需要线程安全的原子引用计数
// `weak_ptr` 解决循环引用
// 智能指针解决资源生命周期问题，不等于自动解决并发安全问题


## STL__README.md

> 以下为早期从 cplusplus.com 搬运的 API 方法速查表。面试核心只需掌握常用方法（begin/end/size/empty/insert/erase/push_back/pop_back/find/count 等），完整 API 请查阅 cppreference.com。

STL 详细文档
# 08_容器_containers

### 容器（containers）

#### array

array 是固定大小的顺序容器，保存以严格的线性顺序排列的特定数量的元素。

方法 | 含义
---|---
begin/end | 首元素/尾后迭代器
rbegin/rend | 反向迭代器
cbegin/cend/crbegin/crend | 常量版本迭代器
size/max_size/empty | 大小/容量/判空
operator[]/at | 下标访问/带边界检查访问
front/back | 首/尾元素引用
data | 指向首元素的指针
fill | 用 val 填充所有元素
swap | 交换两个 array 内容
get(array) | `std::get<N>(arr)` 获取第 N 个元素引用
relational operators (array) | 逐元素比较

#### vector

vector 是表示可以改变大小的数组的序列容器。

方法 | 含义
---|---
vector/~vector/operator= | 构造/析构/赋值
begin/end/rbegin/rend/cbegin/cend/crbegin/crend | 迭代器
size/max_size/resize/capacity/empty/reserve/shrink_to_fit | 容量管理
operator[]/at/front/back/data | 元素访问
assign/push_back/pop_back/insert/erase/swap/clear | 修改操作
emplace/emplace_back | 原地构造（C++11）
get_allocator | 获取分配器

#### deque

deque（双端队列）是具有动态大小的序列容器，可在两端扩展或收缩。

特有方法：push_front/pop_front/emplace_front 以及 emplace_back、push_back、pop_back

#### forward\_list

forward_list（单向链表）在已知前驱位置时可常数时间插入和擦除。

特有方法：before\_begin/cbefore\_begin 返回首元素之前位置的迭代器

#### list

list 双向链表，允许在序列中任何地方常数时间插入和擦除，支持双向迭代。

#### stack

stack 容器适配器，LIFO（后进先出），元素仅从容器一端插入和提取。

#### queue

queue 容器适配器，FIFO（先进先出），元素插入到一端并从另一端提取。

#### priority_queue

#### set

set 是按照特定顺序存储唯一元素的容器。

#### multiset

#### map

map 是关联容器，按特定顺序存储 key-value 组合形成的元素。

map 特有方法：key_comp/value_comp/find/count/lower_bound/upper_bound/equal_range

#### multimap

#### unordered\_set / unordered\_multiset / unordered\_map / unordered\_multimap

#### tuple

元组是能够容纳元素集合的对象，每个元素可以是不同类型。

#### pair

pair 将一对值结合在一起，值可以是不同类型（T1 和 T2），通过 `first`/`second` 访问。

# 09_算法_algorithms

### 算法（algorithms）

// **简单查找（输入迭代器）**：`find`（首个等于 val）、`find_if`（首个满足谓词）、`count` / `count_if`、`all_of` / `any_of` / `none_of`
// **查找重复（前向迭代器）**：`adjacent_find`（相邻重复）、`search_n`（连续 count 个 val）
// **查找子序列**：`search`（子序列首次出现）、`find_first_of`（任意匹配）、`find_end`（子序列末次出现）
// **其他只读**：`for_each`、`mismatch`、`equal`
// **二分搜索（有序序列）**：`lower_bound`（首个 >= val）、`upper_bound`（首个 > val）、`equal_range`（pair<lower,upper>）、`binary_search`（bool）
// **只写**：`fill`、`fill_n`、`generate`、`generate_n`
// **写操作（输入→输出）**：`copy`、`copy_if`、`copy_n`、`move`、`transform`、`replace_copy`、`replace_copy_if`、`merge`
// **前向迭代器写**：`iter_swap`、`swap_ranges`、`replace`、`replace_if`
// **双向迭代器写**：`copy_backward`、`move_backward`、`inplace_merge`
// **划分（双向迭代器）**：`is_partitioned`、`partition_copy`、`partition_point`、`stable_partition`、`partition`
// **排序（随机访问迭代器）**：`sort`、`stable_sort`、`is_sorted`、`is_sorted_until`、`partial_sort`、`partial_sort_copy`、`nth_element`
// **重排（前向迭代器）**：`remove`、`remove_if`、`remove_copy`、`remove_copy_if`、`unique`、`unique_copy`、`rotate`、`rotate_copy`
// **重排（双向迭代器）**：`reverse`、`reverse_copy`
// **重排（随机访问迭代器）**：`random_shuffle`（C++14 废弃 C++17 移除，用 `shuffle` 替代）
// **最值**：`min`、`max`、`minmax`、`min_element`、`max_element`、`minmax_element`
// **字典序比较**：`lexicographical_compare`

## STL__STL.md
# 10_STL_容器与算法参考

### STL 容器与算法参考

> 本章节为 STL 各容器的方法速查。详细逐方法代码示例请参见 cppreference.com。

#### 组成
- 容器（containers）：序列式（vector/deque/list/forward_list/array）、关联式（set/map/multiset/multimap）、无序（unordered_*）
- 算法（algorithms）：find/sort/copy/remove/transform 等 100+ 通用算法
- 迭代器（iterators）：输入/输出/前向/双向/随机访问五种
- 仿函数（functors）：重载 operator() 的函数对象
- 配接器（adapters）：stack/queue/priority_queue
- 空间配置器（allocator）：内存分配与释放管理

#### 序列式容器

| 容器 | 底层结构 | 随机访问 | 头尾插入/删除 | 中间插入/删除 |
|------|---------|---------|------------|------------|
| array | 固定数组 | O(1) | N/A | N/A |
| vector | 动态数组 | O(1) | 尾部 O(1) 均摊 | O(n) |
| deque | 分段数组 | O(1) | 头尾 O(1) | O(n) |
| forward_list | 单向链表 | - | 头部 O(1) | O(1)（已知前驱） |
| list | 双向链表 | - | 头尾 O(1) | O(1)（已知位置） |

#### 关联式容器

| 容器 | 底层结构 | 是否有序 | 查找/插入/删除 |
|------|---------|---------|-------------|
| set/multiset | 红黑树 | 有序 | O(log n) |
| map/multimap | 红黑树 | 按键有序 | O(log n) |
| unordered_set/unordered_multiset | 哈希表 | 无序 | 平均 O(1)，最坏 O(n) |
| unordered_map/unordered_multimap | 哈希表 | 按键无序 | 平均 O(1)，最坏 O(n) |

#### 容器适配器
- **stack**：LIFO，默认基于 deque
- **queue**：FIFO，默认基于 deque
- **priority_queue**：最大堆（默认），默认基于 vector

#### 常用算法分类
- 非修改序列：find, count, for_each, search, binary_search
- 修改序列：copy, move, fill, transform, replace
- 排序：sort, stable_sort, partial_sort, nth_element
- 二分查找：lower_bound, upper_bound, equal_range, binary_search
- 集合操作：merge, set_union, set_intersection, set_difference
- 堆操作：make_heap, push_heap, pop_heap
- 最值：min, max, minmax, min_element, max_element

#### 迭代器类型

| 迭代器 | 能力 | 典型提供者 |
|--------|-----|----------|
| Input | 只读，单遍 | istream_iterator |
| Output | 只写，单遍 | ostream_iterator |
| Forward | 读写，多遍，只能前进 | forward_list |
| Bidirectional | 读写，多遍，可前进后退 | list, set, map |
| Random Access | 读写，多遍，支持 +n/-n | vector, deque, array |

## docs__02-cpp-oop__class__README.md
# 11_2_1_类与对象

### 2.1 类与对象

> 作者：青松与桑叶
> C/C++ 面试通关指南 -- 第二部分：C++ 面向对象

#### 一、什么是类和对象
- 类是用户自定义的数据类型（设计蓝图），定义了数据成员和成员函数；对象是类的具体实例，在内存中占有实际空间。
- 类本身不占用数据内存（只有代码段），每个对象有自己的数据副本。
- **面试答案**：类就像建筑图纸，对象就是根据图纸建造的房子。

#### 二、类的定义
- C++ 中用 `class` 关键字定义类，提供三种访问控制：`public`（外部可访问）、`protected`（子类可访问）、`private`（仅类内部）。
- 成员变量通常设为 `private`（封装性），可通过 public getter/setter 暴露接口。
- 声明和实现可分离：`.h` 头文件放声明，`.cpp` 源文件放实现，使用 `::` 作用域解析符。

#### 三、构造函数
- 构造函数在创建对象时自动调用，名字与类名相同，无返回值。
- 默认构造函数：不带参数；若定义了带参构造，编译器不再自动生成默认构造。
- 初始化列表比函数体内赋值更高效，`const` 和引用成员必须使用初始化列表。
- 拷贝构造函数：`Student(const Student& other)`，在用一个对象初始化另一个对象、按值传参、按值返回时调用。
- 移动构造函数（C++11）：`Student(Student&& other) noexcept`，窃取源对象资源而非拷贝，源对象被置为有效但未指定状态。
- 委托构造函数（C++11）：一个构造函数可调用同类的另一个构造函数以减少重复代码。

#### 四、析构函数
- 析构函数在对象生命周期结束时自动调用，名字为 `~类名`，无参数无返回值，不可重载。
- 析构顺序与构造顺序相反（栈的后进先出）。
- **虚析构函数**：若类可能被继承，基类析构函数必须声明为 `virtual`，否则通过基类指针 `delete` 派生类对象时只调用基类析构，造成内存泄漏。

#### 五、this 指针
- 每个成员函数（静态除外）都有一个隐藏参数 `this`，指向调用该函数的对象。
- 常见用途：区分成员变量与参数同名、返回 `*this` 支持链式调用。
- `this` 是 const 指针（不能修改指向）；在 const 成员函数中为 `const ClassName* const`；静态成员函数没有 `this`。

#### 六、拷贝控制：深拷贝 vs 浅拷贝
- 浅拷贝：默认拷贝构造函数逐字节复制，对于含指针的类会导致两个对象指向同一块内存 → double free + 数据污染。
- 深拷贝：为指针成员分配新内存并复制数据，确保每个对象拥有独立资源。
- **Rule of Three（三法则）**：若需自定义析构函数、拷贝构造函数、拷贝赋值运算符中的任何一个，通常需要自定义全部三个。
- **Rule of Five（五法则，C++11）**：加上移动构造函数和移动赋值运算符。

#### 七、面试真题
- **构造函数可以是虚函数吗？** 不可以。vptr 在构造函数中才初始化，构造完成前无法通过 vtable 找到虚函数。
- **拷贝构造函数和赋值运算符区别？** 拷贝构造用已存在对象创建新对象；赋值运算符将值赋给已存在对象，需检查自赋值并释放原资源。
- **explicit 关键字作用？** 阻止构造函数的隐式类型转换，单参数构造函数建议加 `explicit`。
- **static 成员特点？** 类级别共享，必须在类外初始化；static 成员函数无 this 指针，只能访问 static 成员。

---
## docs__02-cpp-oop__inheritance__README.md
# 12_2_2_继承与组合

### 2.2 继承与组合

#### 一、继承的基本概念
- 继承允许派生类从基类继承属性和行为，实现代码复用，体现 "is-a" 关系。

#### 二、继承的语法和类型
- public 继承：基类 public → public，protected → protected，最常用，体现 "is-a"。
- protected 继承：基类 public/protected → protected。
- private 继承：所有可访问成员 → private。

#### 三、单继承
- 构造顺序：基类构造 → 成员对象构造（按声明顺序）→ 派生类构造。析构顺序相反。
- 向上转型安全：派生类对象可赋值给基类引用或指针。

#### 四、多继承
- 一个类可同时继承多个基类；名字冲突时需用 `Base::func()` 作用域解析符区分。
- 多继承下内存布局按基类声明顺序排列各基类子对象。

#### 五、虚继承——解决菱形问题
- 菱形继承：派生类同时继承两个基类，而这两个基类又继承自同一个更高层基类，导致两份顶层基类数据。
- 解决方案：使用 `virtual public` 虚继承，保证虚基类只有一份实例，通过虚基类表（vbtable）实现。
- 虚继承代价：额外 vptr 内存开销、间接寻址性能开销、虚基类由最远派生类负责构造。

#### 六、组合 vs 继承
- 组合体现 "has-a" 关系，耦合度低，封装性好；继承体现 "is-a" 关系，耦合度高。
- 设计原则：**组合优于继承**。只有明确 "is-a" 关系且需要多态时使用继承，其他情况优先使用组合。

#### 七、面试真题
- C++ 有 public/protected/private 三种继承方式，实际开发几乎只用 public 继承。
- 构造顺序：先基类再成员再派生类；析构顺序相反。

---
## docs__02-cpp-oop__polymorphism__README.md
# 13_2_3_多态与虚函数

### 2.3 多态与虚函数

#### 一、什么是多态
- 多态指同一接口不同实现。C++ 中静态多态（编译期）通过重载/模板实现；动态多态（运行期）通过虚函数实现。
- 动态多态：通过基类指针或引用调用虚函数，运行时根据实际对象类型决定调用哪个函数。

#### 二、虚函数的声明和使用
- 在基类成员函数前加 `virtual`，派生类中签名完全相同的函数构成重写（推荐加 `override`）。
- 重写条件：函数名、参数列表、返回类型完全相同（协变返回类型例外），基类函数必须是虚函数。
- 构造函数不能是虚函数；静态成员函数、友元函数、全局函数不能是虚函数。

#### 三、纯虚函数和抽象类
- 纯虚函数：`virtual double area() const = 0;`，基类中声明无实现，迫使派生类必须实现。
- 含有纯虚函数的类是抽象类，不能实例化。纯虚函数可以有函数体，但子类仍需重写才能实例化。

#### 四、动态绑定 vs 静态绑定
- 静态绑定（非虚函数）：编译器根据指针/引用类型确定调用，无额外开销。
- 动态绑定（虚函数）：运行时根据实际对象类型确定调用，通过 vptr → vtable → 函数指针间接寻址。
- 动态绑定条件：基类函数为虚函数 + 通过基类指针/引用调用 + 派生类重写了该虚函数。

#### 五、override 和 final 关键字
- `override`：放在派生类虚函数声明末尾，编译器检查是否真正重写了基类虚函数，推荐始终使用。
- `final`：修饰类（禁止继承）或修饰虚函数（禁止重写），均为编译期检查，零运行时开销。

#### 六、面试真题
- 虚函数和纯虚函数区别：虚函数有默认实现可选择性重写；纯虚函数无实现，含纯虚函数的类为抽象类，派生类必须实现。
- 构造函数中调用虚函数不会产生多态效果——构造基类部分时 vptr 指向基类 vtable。
- 虚函数表（vtable）是编译器为每个含虚函数的类创建的静态数组，存储虚函数地址；每个对象有 vptr 指向其类的 vtable。

---
## docs__02-cpp-oop__smart-pointer__README.md
# 14_2_4_智能指针

### 2.4 智能指针

> 面试核心：`unique_ptr` 独占所有权不允许拷贝允许移动；`shared_ptr` 通过控制块维护引用计数；`weak_ptr` 解决循环引用。

#### 一、为什么需要智能指针
- 手动内存管理三大问题：内存泄漏（忘记 delete）、重复释放（double free）、悬空指针。
- C++11 引入三种智能指针（`<memory>`）：`unique_ptr`（独占，无引用计数）、`shared_ptr`（共享，有引用计数）、`weak_ptr`（弱引用，不拥有所有权）。

#### 二、RAII 思想
- RAII（资源获取即初始化）：将资源生命周期绑定到对象生命周期，构造时获取资源，析构时自动释放。
- 局部对象离开作用域时析构函数一定会被调用（包括异常退出时的栈展开），确保资源释放。

#### 三、std::unique_ptr —— 独占所有权
- 独占所有权，不可拷贝只能移动。推荐使用 `std::make_unique`（C++14）创建，异常安全且代码简洁。
// `std::unique_ptr` 独占所有权，禁止拷贝；`std::move` 转移所有权
// `release()` 释放所有权返回裸指针但不删除对象；`reset()` 删除当前对象并可选指向新对象
- 可作为工厂函数返回值（通过移动语义），也可通过 `std::move` 传递所有权。

#### 四、std::shared_ptr —— 共享所有权
- 多个 shared_ptr 共享同一对象，通过引用计数跟踪所有者数量，计数为 0 时自动删除对象。
// `std::make_shared` 一次分配对象+控制块内存，比 `new shared_ptr` 更高效
// `use_count()` 查看引用计数；`unique()` 检查是否唯一持有
- `make_shared` 优势：一次内存分配、更好局部性、异常安全。缺点：对象和控制块一起分配，weak_ptr 全部过期后才能释放整块内存。

#### 五、std::weak_ptr —— 解决循环引用
- weak_ptr 不增加引用计数，通过 `lock()` 提升为 shared_ptr 后安全访问对象；`expired()` 检查对象是否存活。
- 典型场景：打破 shared_ptr 循环引用、观察者模式、缓存系统、parent-child 关系中 child 持有 parent 的 weak_ptr。

#### 六、循环引用问题详解
- 循环引用：A 持有 shared_ptr\<B\>，B 持有 shared_ptr\<A\>，引用计数永不为 0 → 内存泄漏。将其中一方的 shared_ptr 改为 weak_ptr 即可解决。

#### 七、面试真题
- shared_ptr 引用计数操作是原子操作（控制块线程安全），但访问所指向对象不是线程安全的。
- 优先用 unique_ptr，语义清晰零开销，可隐式转换为 shared_ptr。只有在确实需要共享所有权时才用 shared_ptr。
- `make_shared` 一次分配 vs `new shared_ptr` 两次分配；前者性能好，但若 weak_ptr 生命周期长可能导致内存回收延迟。

---
## docs__02-cpp-oop__virtual__README.md
# 15_2_5_虚函数深度剖析

### 2.5 虚函数深度剖析

#### 一、虚函数表（vtable）的内存布局
- 编译器为每个含虚函数的类生成一个 vtable（静态数组，存函数指针），每个对象含一个 vptr 指向其类的 vtable。
- vtable 是类级别的（同类的所有对象共享），vptr 是对象级别的（在构造函数中初始化）。
- 虚函数调用过程：对象 → vptr → vtable → 函数指针 → 实际调用，一次额外间接寻址。

#### 二、单继承下的 vtable
- 派生类 vtable 中：重写的函数指针替换为派生类版本，未重写的保持基类版本，新增虚函数追加在末尾。
- vptr 通常位于对象内存起始位置。
- 构造派生类对象时 vptr 会变化：先指向基类 vtable，派生类构造时改为指向派生类 vtable。

#### 三、多重继承下的 vtable（多个 vptr）
- 每个含虚函数的基类在派生类对象中有自己的 vptr 和对应的 vtable。
- 派生类指针赋给不同基类指针时地址可能不同（编译器自动偏移），因为不同基类子对象在对象中位置不同。

#### 四、虚析构函数
- 当通过基类指针删除派生类对象时，若基类析构非虚，则只调基类析构 → 派生类资源泄漏。
- 虚析构函数在 vtable 中有条目，`delete` 时通过 vtable 调用派生类析构，再自动调用基类析构。
- 纯虚析构函数必须提供实现（因为基类析构总会被调用），可使基类成为抽象类同时拥有虚析构。

#### 五、虚函数的性能开销
- 空间：每个对象多一个 vptr（8 字节，64位），每个类多一个 vtable。
- 时间：每次调用需间接寻址（约 2-3 条额外指令 + 一次内存访问）。
- 优化障碍：难以内联（除非编译器去虚化优化）。
- 优化方法：用 `final` 帮助去虚化、用 CRTP 实现静态多态、用 `std::variant` 替代简单多态层次。

#### 六、面试真题
- 多重继承下有几个 vptr？每个有虚函数的基类对应一个 vptr。
- 构造函数中调用虚函数：调用的是当前正在构造的类的版本，不会产生多态。
- vtable 存在只读数据段（.rodata），编译器检查虚函数调用需要 RTTI（`dynamic_cast`/`typeid`）。

---
<!-- C++ Primer 原始代码片段已移除 -->
# 16_problems___define和const的区别有哪些_md

## problems__#define和const的区别有哪些？.md

1. `#define` 是预处理宏，编译前文本替换，无类型检查，无调试信息，作用域为定义处到文件结束。
2. `const` 是真正的变量，有类型检查，可被调试器识别，作用域根据定义位置而定。

# 17_problems__C_中struct和class有什么区别_md

## problems__C++中struct和class有什么区别？.md

1. 默认访问权限：struct 默认为 public，class 默认为 private。
2. 默认继承方式：struct 默认为 public 继承，class 默认为 private 继承。

# 18_problems__C_中动态链接库和静态连接库的区别是什么_md

## problems__C++中动态链接库和静态连接库的区别是什么？.md

- 静态链接库：编译时复制到可执行文件，运行时不需要库文件，文件大，更新需重新编译。
- 动态链接库：编译时不复制代码，运行时加载，文件小，多个程序共享内存中一份副本，更新只需替换库文件。
- 静态链接：更独立但二进制大；动态链接：共享库提高效率但需确保目标系统有正确版本的库。

# 19_problems__C_中右值引用有什么作用_md

## problems__C++中右值引用有什么作用？.md

C++11 引入右值引用支持两种场景：1) **移动语义**：直接转移资源避免拷贝，对大对象或独占所有权资源特别有用；2) **完美转发**：通过万能引用 `T&&` + `std::forward` 保持参数的值类别无损传递。

# 20_problems__C_中四种cast的转换_md

## problems__C++中四种cast的转换？.md

1. **`static_cast`**：相关类型间编译期安全转换（如 int↔float、基类↔派生类指针），不能用于多态向下转型。
2. **`dynamic_cast`**：多态继承体系中安全向下转型，失败返回 nullptr（指针）或抛 `std::bad_cast`（引用），需 RTTI。
3. **`const_cast`**：移除/添加 const/volatile 属性，修改真正 const 对象是未定义行为。
4. **`reinterpret_cast`**：低级别强制类型转换（如指针↔整数），最危险，应避免使用。

# 21_problems__C_中常用的类优化技术有哪些_md

## problems__C++中常用的类优化技术有哪些？.md

使用成员函数而非友元函数、引用参数而非传值参数、移动语义、智能指针、const 成员函数、避免不必要的动态内存分配、内联函数。

# 22_problems__C_中类成员的访问权限有哪些_md

## problems__C++中类成员的访问权限有哪些？.md

- **public**：任何代码均可访问，定义类的外部接口。
- **protected**：类本身和派生类可访问，对类外部隐藏。
- **private**：仅类内部（及友元）可访问，是实现封装的关键。

# 23_problems__C_中结构体内存布局的规则是什么_md

## problems__C++中结构体内存布局的规则是什么？.md

成员按声明顺序排列；编译器根据对齐规则插入 padding（类型地址需为其大小的整数倍）；`#pragma pack` 可改变对齐。有虚函数时通常在对象开头有一个 vptr。静态成员不占用对象内存。位域可紧密打包。

# 24_problems__C_中_结构体可以直接赋值吗_md

## problems__C++中，结构体可以直接赋值吗？.md

结构体可以像内置数据类型一样进行赋值操作，使用 `=` 直接拷贝所有成员（对指针成员是浅拷贝）。

# 25_problems__C_内存分配_md

## problems__C++内存分配.md

三种分配方式：1) 静态存储区（编译时分配，如全局/static 变量）；2) 栈（函数内局部变量，效率高但容量有限）；3) 堆（动态分配，灵活但需手动管理）。BSS 段存放未初始化的全局/static 变量。

# 26_problems__C_多态的实现有那几种_他们有什么不同_md

## problems__C++多态的实现有那几种？他们有什么不同？.md

两种：1) **编译时多态（静态多态）**：函数重载/运算符重载/模板，编译期确定，性能高；2) **运行时多态（动态多态）**：虚函数+继承，运行时通过 vtable 动态绑定，灵活性高但有一次间接寻址开销。

# 27_problems__C_的atomic_bool代码底层是如何实现的_md

## problems__C++的atomic-bool代码底层是如何实现的？.md

`std::atomic<bool>` 底层依赖硬件支持：x86 的 `LOCK` 前缀指令、`CMPXCHG` 等原子指令、编译器内建函数（如 `__atomic_fetch_and`）和内存屏障。bool 类型简单，大多数平台可提供无锁原子操作。

# 28_problems__C_的智能指针及其原理_md

## problems__C++的智能指针及其原理.md

智能指针基于 RAII 原理，构造时获取资源，析构时自动释放。`unique_ptr` 独占所有权（不可拷贝可移动），`shared_ptr` 共享所有权（引用计数），`weak_ptr` 是弱引用（不增加计数）。侵入式智能指针（如 `boost::intrusive_ptr`）需被管理类提供接口；非侵入式（如 `shared_ptr`）无需修改被管理类。

# 29_problems__C_结构体内存对齐_md

## problems__C++结构体内存对齐.md

对齐原则：结构体起始地址能被最宽基本类型成员大小整除；每个成员偏移量是自身大小的整数倍；总大小是最宽基本类型成员大小的整数倍。编译器可能插入 padding 满足对齐要求。可通过 `#pragma pack` 修改默认对齐。

# 30_problems__STL中一般都有哪些常见的算法库呢_md

## problems__STL中一般都有哪些常见的算法库呢？.md

非修改性序列操作（for_each/count/find/binary_search）、修改性序列操作（sort/reverse/swap/rotate）、排列组合（next_permutation/merge）、数值操作（accumulate/inner_product/partial_sum）、堆操作（make_heap/push_heap/pop_heap）、划分操作（partition/stable_partition）。

# 31_STL中的优先级队列是如何实现的

## problems__STL中的优先级队列是如何实现的？.md

`std::priority_queue` 定义在 `<queue>` 中，底层使用 `std::vector` + 堆算法（`make_heap`/`push_heap`/`pop_heap`）。默认比较器 `std::less` 实现最大堆；自定义比较器可改造成最小堆。`push()` 上浮调整，`pop()` 下沉调整。

# 32_problems__STL中_map的底层是如何实现的_md

## problems__STL中，map的底层是如何实现的？.md

`std::map` 底层使用红黑树（自平衡二叉搜索树），保证插入/删除/查找均为 O(log n)。红黑树特性：节点红或黑、根黑、叶黑、红节点子必黑、每个节点到叶的黑色节点数相同。C++11 还提供 `unordered_map`（哈希表，平均 O(1)，不保证顺序）。

# 33_problems__STL中_set的底层是如何实现的_md

## problems__STL中，set的底层是如何实现的？.md

`std::set` 底层通常采用红黑树，插入/删除/搜索为 O(log n)，元素按顺序存储。`unordered_set` 基于哈希表，平均 O(1) 但不保证顺序。

# 34_problems__STL原理及实现_md

## problems__STL原理及实现.md

STL 六大组件：容器（数据结构 class template）、算法（function template，操作 [first, last) 前闭后开区间）、迭代器（容器与算法间胶合剂，泛型指针）、仿函数（重载 operator() 的函数对象）、配接器（修饰容器/仿函数/迭代器接口）、配置器（空间配置与管理）。vector 底层连续数组；deque 分段连续数组通过中控器 map 管理；list 双向链表；stack/queue 默认基于 deque；priority_queue 基于 vector+heap。关联容器（set/map/multiset/multimap）基于红黑树；无序容器基于哈希表（链地址法）。

# 35_problems__STL容器是线程安全的吗_md

## problems__STL容器是线程安全的吗？.md

STL 容器本身不是线程安全的，多线程同时写或读写同一个容器实例会导致数据竞争和未定义行为。需通过互斥锁、读写锁、原子操作或并发容器（如 tbb）来确保线程安全。

# 36_problems__git的merge和rebase有什么区别_md

## problems__git的merge和rebase有什么区别.md

merge 合并两个分支并创建新的合并提交节点，保留完整历史；rebase 将当前分支提交按序重放到目标分支最新提交之后，历史更线性但会修改提交历史。

# 37_problems__lower_bound_和upper_bound_有什么区别_md

## problems__lower_bound()和upper_bound()有什么区别？.md

`lower_bound` 返回首个**不小于**（>=）给定值的迭代器；`upper_bound` 返回首个**大于**（>）给定值的迭代器。两者均要求有序序列。

# 38_problems__override和overload的区别有哪些_md

## problems__override和overload的区别有哪些.md

函数重载（Overload）：同一作用域内函数名相同但参数列表不同，编译期多态。函数覆盖（Override）：派生类函数与基类虚函数签名完全相同，运行期多态，构成重写。

# 39_problems__set_multiset_map_multimap之间都有什么区别_md

## problems__set，multiset，map，multimap之间都有什么区别？.md

set 存储唯一键值，multiset 允许重复键；map 存储键值对（key 唯一），multimap 允许重复键。四者均基于红黑树有序存储。不需要排序时选用 unordered_* 版本（哈希表，平均 O(1)）。

# 40_problems__weak_ptr是如何解决shared_ptr循环引用的_md

## problems__weak_ptr是如何解决shared_ptr循环引用的？.md

两个对象通过 shared_ptr 互相持有对方形成循环引用，导致引用计数永不为 0 而内存泄漏。将其中一方的 shared_ptr 改为 weak_ptr（不增加引用计数）即可打破循环。

# 41_problems__为什么需要allocator_他在STL中有什么作用_md

## problems__为什么需要allocator？他在STL中有什么作用？.md

Allocator 提供内存管理抽象层：1) 统一接口使容器独立于内存管理细节；2) 支持自定义内存分配策略（内存池等）；3) 性能优化（小对象优化、对齐控制）；4) 分离分配与构造，支持 reserve 语义。

# 42_problems__为什么需要虚析构函数_什么时候不需要_md

## problems__为什么需要虚析构函数,什么时候不需要.md

当类被用作基类且通过基类指针删除派生类对象时，需要虚析构函数以确保派生类析构被调用。若类不用于继承或不通过基类指针删除，则不需要虚析构函数（避免增加 vptr 开销）。

# 43_problems__为什么需要虚继承_md

## problems__为什么需要虚继承.md

虚继承解决多重继承中的菱形继承问题（两个派生类继承同一基类，最终派生类有两份基类数据）。虚继承使 virtual 基类在所有派生类中共享同一份实例。

# 44_problems__什么时候会产生栈溢出_为什么一直递归就会栈溢出_md

## problems__什么时候会产生栈溢出，为什么一直递归就会栈溢出.md

栈溢出原因：递归层次过深（每次调用压栈参数和返回地址）、大量局部变量、无限循环调用。递归容易栈溢出是因为没有合适终止条件或递归深度超过栈容量。

# 45_problems__什么是C_的内存模型_md

## problems__什么是C++的内存模型？.md

C++ 内存模型涵盖：1) 对象存储期（自动/静态/动态/线程局部）；2) 内存分区（栈/堆/全局静态区/常量区/代码区）；3) 多线程原子操作与内存序（memory_order）；4) 同步互斥机制（mutex/condition_variable）的行为规则；5) 顺序一致性保证。

# 46_problems__什么是RAII原则_他在STL是怎么应用的_md

## problems__什么是RAII原则，他在STL是怎么应用的？.md

RAII：构造时获取资源，析构时释放资源，确保异常安全。STL 应用：智能指针（unique_ptr/shared_ptr 自动管理内存）、容器（vector/string/map 自动管理内部内存）、锁管理类（lock_guard/unique_lock 自动加解锁）。

# 47_problems__什么是完美转发_md

## problems__什么是完美转发？.md

完美转发允许函数模板将参数以原始值类别（左值/右值）无损传递给另一个函数。通过万能引用 `T&&` + `std::forward<T>(arg)` 实现引用折叠和值类别保持。

# 48_problems__什么是左值_什么是右值_有什么不同_md

## problems__什么是左值？什么是右值？有什么不同？.md

左值：表达式结束后依然存在，有明确内存地址，可取地址。右值：临时对象/字面量，无固定地址，不可取地址。左值持久可移动（需显式 std::move），右值临时可被移动语义窃取资源。

# 49_problems__什么是泛型编程_他在STL中是怎么使用的_md

## problems__什么是泛型编程，他在STL中是怎么使用的？.md

泛型编程通过模板使算法/数据结构独立于具体数据类型。STL 是典范：容器通过模板参数化元素类型，算法通过迭代器与容器交互，函数对象和 lambda 自定义操作行为。

# 50_problems__内存池是什么_在C_中如何设计一个简单的内存池_md

## problems__内存池是什么？在C++中如何设计一个简单的内存池？.md

内存池预分配一定数量的内存块形成"池"，分配时从池中取块，释放时块返回池中，减少频繁 malloc/free 的开销和内存碎片。设计关键：预分配、分配/释放接口、空闲块管理策略（通常用链表或 list）。

# 51_problems__内联函数_构造函数_静态成员函数可以是虚函数吗_md

## problems__内联函数、构造函数、静态成员函数可以是虚函数吗.md

三者均不能为虚函数。内联是编译期展开，虚函数需运行时确定，矛盾。构造函数调用时 vptr 尚未正确设置，无法实现多态。静态成员函数属于类本身，无 this 指针，不参与动态绑定。

# 52_problems__写string类的构造_析构_拷贝函数_md

## problems__写string类的构造，析构，拷贝函数.md

// string 类需实现：默认构造（空字符串）、参数化构造（从 const char*）、拷贝构造（深拷贝）、析构（delete[]）、拷贝赋值（检查自赋值后深拷贝）。注意指针资源的正确管理。

# 53_problems__函数参数的入栈顺序是什么_从左到右还是从右到左_md

## problems__函数参数的入栈顺序是什么，从左到右还是从右到左.md

C++ 函数参数入栈顺序是从右到左（最后一个参数先入栈）。

# 54_problems__动态链接和静态链接的区别_md

## problems__动态链接和静态链接的区别.md

静态链接：编译时将库代码包含进可执行文件，独立运行，文件大，更新需重新编译。动态链接：运行时加载库，文件小，多程序共享一份库内存，更新只需替换库文件，但需确保目标系统有正确版本的库。

# 55_problems__原子变量的内存序是什么_md

## problems__原子变量的内存序是什么？.md

C++11 定义六种内存序：`memory_order_relaxed`（最弱，仅保证原子性）、`memory_order_consume`（数据依赖）、`memory_order_acquire`（禁止后序重排到前）、`memory_order_release`（禁止前序重排到后）、`memory_order_acq_rel`（同时 acquire+release）、`memory_order_seq_cst`（顺序一致性，默认最强）。

# 56_problems__同一个类的两个对象的虚函数表是如何维护的_md

## problems__同一个类的两个对象的虚函数表是如何维护的？.md

vtable 是类级别维护的，同一类所有对象共享同一个 vtable。每个对象有自己的 vptr 指向这个共享的 vtable。派生类重写虚函数时拥有自己的 vtable，其 vptr 指向派生类的 vtable。

# 57_problems__在32位和64位系统中_指针分别为多大_md

## problems__在32位和64位系统中，指针分别为多大？.md

32 位系统：指针 4 字节（32 位地址总线，寻址 4GB）。64 位系统：指针 8 字节（64 位地址总线，寻址 2^64 字节）。

# 58_problems__在C_中为什么需要深拷贝_浅拷贝会存在哪些问题_md

## problems__在C++中为什么需要深拷贝，浅拷贝会存在哪些问题？.md

浅拷贝问题：双重释放（两个析构函数释放同一块内存）、数据竞争（操作同一资源）、野指针（一个释放后另一个继续访问）。深拷贝为每个对象分配独立内存并复制数据，避免上述问题。

# 59_problems__在C_中_对一个对象先malloc后delete这样使用可以吗_有什么风险_md

## problems__在C++中，对一个对象先malloc后delete这样使用可以吗？有什么风险.md

不可以。风险：malloc 不调用构造函数（对象未初始化），delete 会调用析构函数（未初始化的对象析构是未定义行为），new/delete 和 malloc/free 的内存管理机制可能不兼容导致堆损坏。

# 60_problems__在C_中_用堆和用栈谁更快一点_md

## problems__在C++中，用堆和用栈谁更快一点？.md

栈分配更快：只需移动栈指针，连续内存缓存局部性好。堆分配较慢：需要查找空闲块、处理碎片化，对象在堆上分布不连续导致缓存命中率降低。

# 61_problems__在C_的map中_与insert有哪些区别_md

## problems__在C++的map中，[]与insert有哪些区别？.md

`[]`：键不存在时插入默认值并返回引用，存在时覆盖原值。`insert`：传入 pair，键已存在时不覆盖，返回 pair<迭代器, 是否成功>。

# 62_problems__在C_的算法库中_find_和binary_search_有什么区别_md

## problems__在C++的算法库中，find()和binary_search()有什么区别？.md

`find`：线性查找 O(n)，不要求有序，返回迭代器。`binary_search`：二分查找 O(log n)，要求有序，返回 bool（不返回位置）。

# 63_problems__如何定义一个只能在堆上定义对象的类栈上呢_md

## problems__如何定义一个只能在堆上定义对象的类栈上呢.md

**只能在堆上**：将析构函数设为 private，提供 public 静态 Create/Destroy 接口。**只能在栈上**：将 operator new 设为 delete（或 private）。

# 64_problems__如何构造一个类使得只能在堆上或者栈上分配内存_md

## problems__如何构造一个类使得只能在堆上或者栈上分配内存？.md

只在栈上：delete operator new；只在堆上：私有析构函数 + public Create/Destroy 接口（可用智能指针封装避免忘记释放）。

# 65_problems__如何选择合适的STL容器_md

## problems__如何选择合适的STL容器.md

首选 vector 除非有特定理由。需要高效头尾插入用 deque；需要高效中间插入用 list/forward_list；需要有序唯一元素用 set；需要键值映射用 map；不需排序追求快速查找用 unordered_*。特定数据结构需求用 stack/queue/priority_queue 适配器。

# 66_problems__如何避免悬挂指针_md

## problems__如何避免悬挂指针？.md

释放内存后立即置 nullptr；优先使用智能指针（unique_ptr/shared_ptr）；声明指针时初始化 nullptr；避免从函数返回局部变量地址；注意回调和事件处理中及时注销指针。

# 67_problems__如果A是某一个类的指针_那么在它等于nullptr的情况下能直接调用里面的A对应类里面的public函数吗_m

## problems__如果A是某一个类的指针，那么在它等于nullptr的情况下能直接调用里面的A对应类里面的public函数吗.md

不能。nullptr 解引用是未定义行为，会导致程序崩溃。

# 68_problems__如果A这个对象对应的类是一个空类_那么sizeof_A_的值是多少_md

## problems__如果A这个对象对应的类是一个空类，那么sizeof(A)的值是多少？.md

sizeof(空类) = 1。C++ 标准要求每个对象有唯一地址，编译器给空类实例分配一个字节。

# 69_problems__如果A这个指针指向一个数组_那么sizeof_A_的值是多少_md

## problems__如果A这个指针指向一个数组，那么sizeof(A)的值是多少？.md

sizeof(指针) 取决于系统位数：32 位为 4，64 位为 8。与指针指向什么无关。

# 70_problems__如果拿到虚函数表的储存地址_是否可以改写虚函数表的内容_md

## problems__如果拿到虚函数表的储存地址，是否可以改写虚函数表的内容？.md

理论上可以但危险。现代操作系统将 vtable 存放在只读数据段（.rodata），直接修改会导致崩溃。破坏虚函数表会破坏类型安全，可能引入安全漏洞。DEP 和 ASLR 等保护机制防止此类攻击。

# 71_problems__宏定义和展开_内联函数区别_md

## problems__宏定义和展开、内联函数区别.md

宏是预处理器文本替换，不检查类型，可操作 token 在当前作用域生成变量。内联函数是编译器控制，检查参数类型，是真正的函数但会在调用处展开以消除调用开销。内联函数体过大时编译器会放弃内联。

# 72_problems__悬挂指针和野指针有什么区别_md

## problems__悬挂指针和野指针有什么区别？.md

悬挂指针：指向已被释放的内存（delete/free 后未置空）。野指针：未初始化的指针，指向随机地址。两者解引用均为未定义行为。最佳实践：声明时初始化（nullptr），释放后置 nullptr。

# 73_problems__指针和引用在内存中的表现形式有何不同_md

## problems__指针和引用在内存中的表现形式有何不同？.md

实现上引用通常通过指针实现，但语义上引用是别名，操作引用相当于直接操作原对象。编译器可能对引用优化到不分配地址空间（尤其是作为函数参数时）。指针支持算术运算和重新赋值，限制编译器优化。

# 74_problems__指针和引用有什么区别呢_md

## problems__指针和引用有什么区别呢？.md

指针是存储地址的变量，可为空，可重新赋值，有自己的内存地址，需 `*` 解引用。引用是变量的别名，必须初始化且不能改变绑定，无自己的内存空间（语义上），无需特殊操作符。引用通常用作函数参数/返回值以简化代码。

# 75_problems__指针常量和常量指针的区别_md

## problems__指针常量和常量指针的区别.md

常量指针 `const int* p`：指向常量的指针，不能通过 p 修改所指值，但 p 可指向别处。指针常量 `int* const p`：指针本身是常量，不能改指向，但可通过 p 修改所指值。两者可结合：`const int* const p` 均不可改。

# 76_problems__构造函数中可以调用虚函数吗_md

## problems__构造函数中可以调用虚函数吗.md

语法上可以，但不会产生多态效果。在基类构造期间 vptr 指向基类 vtable，调用的虚函数是基类版本而非派生类版本。因为派生类部分尚未构造完成，所以应避免在构造函数中调用虚函数。

# 77_problems__用C_设计一个不能被继承的类_md

## problems__用C++设计一个不能被继承的类.md

使用 `final` 关键字：`class FinalClass final { ... };`，任何继承尝试会导致编译错误。

# 78_problems__类构造和析构的顺序_md

## problems__类构造和析构的顺序.md

构造顺序：基类 → 成员对象（按声明顺序）→ 派生类自身。析构顺序：反之。成员初始化列表中的顺序不影响实际构造顺序（始终按声明顺序）。

# 79_problems__纯虚函数_为什么需要纯虚函数_md

## problems__纯虚函数，为什么需要纯虚函数.md

纯虚函数 `virtual void func() = 0` 使类成为抽象类，不能实例化，强制派生类必须实现该函数。目的：定义接口规范，方便使用多态，避免不合理的基类对象创建。

# 80_problems__虚函数是否可以声明为static_md

## problems__虚函数是否可以声明为static？.md

不能。静态成员函数无 this 指针，属于类而不属于对象，不参与多态和动态绑定，没有 vtable 条目。

# 81_problems__虚函数是针对类还是针对对象的_md

## problems__虚函数是针对类还是针对对象的？.md

虚函数是类层面的概念：编译器为每个含虚函数的类创建 vtable。影响的是通过该类创建的对象的行为——通过基类指针/引用调用时根据对象实际类型动态分派。

# 82_problems__虚函数的作用和实现原理_什么是虚函数_有什么作用_md

## problems__虚函数的作用和实现原理，什么是虚函数,有什么作用.md

虚函数实现动态多态：通过 vtable（函数指针数组）+ vptr（每个对象指向 vtable 的指针）。派生类重写虚函数时替换 vtable 中对应条目。单继承下调用开销与非虚函数相差无几；多继承下需多个 vptr 和地址调整（thunk）。主要代价：不能内联（编译器可通过去虚化优化绕过）、增加对象大小（一个 vptr）。

# 83_problems__讲一讲C_中的原子操作有哪些_md

## problems__讲一讲C++中的原子操作有哪些？.md

`<atomic>` 提供的原子操作：`store`/`load`、`exchange`、`compare_exchange_weak`/`strong`（CAS）、`fetch_add`/`fetch_sub`/`fetch_and`/`fetch_or`/`fetch_xor`、前置/后置 `++`/`--`。通过 memory_order 控制同步与重排语义。

# 84_problems__讲讲函数调用的过程_md

## problems__讲讲函数调用的过程.md

函数调用步骤：压入返回地址 → 参数从右到左压栈 → 保存寄存器状态 → 跳转到函数体 → 执行函数（栈上分配局部变量）→ 弹出栈中内容（局部变量、恢复寄存器、弹出参数和返回地址）→ 返回到调用者。

# 85_problems__迭代器与普通指针有什么区别_md

## problems__迭代器与普通指针有什么区别.md

迭代器是容器与算法间的抽象层：提供更高级的安全性（越界检查）、可移植性（隐藏底层实现细节）、功能扩展（五种迭代器类别支持不同操作集）。普通指针操作底层内存。

# 86_problems__静态分配内存和动态分配内存有什么区别_md

## problems__静态分配内存和动态分配内存有什么区别？.md

静态分配（编译期确定）：全局变量/static/局部变量，大小固定，管理简单但可能浪费空间。动态分配（运行时）：通过 new/malloc 按需分配，灵活高效但需手动释放，管理不当会导致内存泄漏。

# 87_problems__面向对象的三大特征是什么_md

## problems__面向对象的三大特征是什么.md

封装（数据+操作结合，隐藏实现细节）、继承（子类共享父类结构，代码复用）、多态（同一接口不同实现，通过虚函数/重载/模板实现）。

---

## 现代 C++11/14/17 系统专题
# 88_一_C_11_核心特性

### 一、C++11 核心特性

#### 1.1 自动类型推导（auto）
`auto` 让编译器在编译期根据初始化表达式推导变量类型。推导时忽略顶层 const 和引用，需显式指定 `const auto` / `auto&`。
// `auto i = 42;` → int；`const auto y = ci;` → const int；`auto& z = ci;` → const int&

#### 1.2 范围 for 循环（Range-based for）
// `for (const auto& x : v) {}` 只读遍历；`for (auto& x : v) {}` 可修改

#### 1.3 右值引用与移动语义
左值有名称可取地址；右值为临时对象/字面量不可取地址。移动构造函数窃取源对象资源而非拷贝，源对象置为有效但未指定状态。`std::move` 将左值强制转为右值引用触发移动语义。
// `Buffer(Buffer&& other) noexcept : data_(other.data_), size_(other.size_) { other.data_ = nullptr; other.size_ = 0; }`

#### 1.4 完美转发（Perfect Forwarding）
万能引用 `T&&` + `std::forward<T>(arg)` 实现参数类型和值类别的无损传递。引用折叠规则：`T&&` + `int&` → `int&`；`T&&` + `int&&` → `int&&`。

#### 1.5 Lambda 表达式
语法：`[捕获列表](参数列表) -> 返回类型 { 函数体 }`。捕获方式：值捕获 `[x]`、引用捕获 `[&x]`、按值捕获全部 `[=]`、按引用捕获全部 `[&]`、混合捕获。`mutable` 允许修改值捕获的变量。

#### 1.6 智能指针（C++11）
// `std::unique_ptr` 独占所有权，不可拷贝可移动；`std::shared_ptr` 共享所有权，引用计数线程安全；`std::weak_ptr` 弱引用，不增加计数，解决循环引用

#### 1.7 并发编程基础（`<thread>`, `<mutex>`, `<condition_variable>`, `<future>`）
// `std::thread` 创建线程（`t.join()` 等待）；`std::lock_guard` RAII 加锁；`std::condition_variable` 条件等待（`wait`/`notify_one`）；`std::promise`/`std::future` 异步获取值

#### 1.8 其他 C++11 重要特性
- `nullptr`（类型安全空指针）、`enum class`（强类型枚举）、`static_assert`（编译期断言）、`override`/`final`、`noexcept`、`constexpr`
- `std::array`/`std::tuple`/`std::unordered_map`/`std::unordered_set` 新容器
- `std::function`/`std::bind` 通用函数包装器
- `std::chrono` 时间库、`std::regex` 正则表达式、`std::initializer_list` 初始化列表

# 89_二_C_14_核心特性

### 二、C++14 核心特性

#### 2.1 泛型 Lambda
// `auto add = [](auto a, auto b) { return a + b; };` — Lambda 参数可用 auto

#### 2.2 函数返回类型推导
// `auto add(int a, int b) { return a + b; }` — 编译器推导返回类型

#### 2.3 `std::make_unique`
// `auto p = std::make_unique<int>(42);` — C++14 引入的工厂函数

#### 2.4 二进制字面量和数字分隔符
// `int x = 0b1010;` — 二进制字面量；`int y = 1'000'000;` — 数字分隔符提高可读性

#### 2.5 变量模板
// `template<typename T> constexpr T pi = T(3.14159);` — 模板化的常量变量

# 90_三_C_17_核心特性

### 三、C++17 核心特性

#### 3.1 结构化绑定（Structured Binding）
// `auto [key, value] = *map.begin();` — 解构 pair/tuple/struct 为独立变量

#### 3.2 `std::optional`
// `std::optional<int> find(int key) { if (key > 0) return key*2; return std::nullopt; }` — 可能为空的值

#### 3.3 `std::variant`（类型安全的 union）
// `std::variant<int, double, std::string> v; v = "hello"; std::visit([](auto&& arg){}, v);` — 类型安全的联合体

#### 3.4 `std::any`
// `std::any a = 42; a = std::string("hello"); std::any_cast<std::string>(a);` — 可存储任意类型

#### 3.5 `std::string_view`
// 零拷贝字符串视图，`void print(std::string_view sv) {}` — 避免不必要的字符串拷贝

#### 3.6 `if` / `switch` 带初始化器
// `if (auto it = m.find(key); it != m.end()) {}` — 条件中嵌入变量声明

#### 3.7 内联变量（Inline Variables）
// `inline constexpr double pi = 3.14;` — 头文件中定义变量，无 ODR 违规

#### 3.8 文件系统库（`<filesystem>`）
// `namespace fs = std::filesystem; fs::exists(path); fs::directory_iterator(dir)` — 跨平台文件系统操作

#### 3.9 并行 STL 算法
// `std::sort(std::execution::par, v.begin(), v.end());` — 并行排序

#### 3.10 折叠表达式（Fold Expressions）
// `template<typename... Args> auto sum(Args... args) { return (args + ...); }` — 参数包折叠

#### 3.11 `std::byte`
// `std::byte b{0x0F};` — 类型安全的字节类型

---

## STL 底层原理
# 91_一_Allocator_空间配置器

### 一、Allocator（空间配置器）

#### 1.1 作用
Allocator 是 STL 容器与底层内存分配之间的抽象层，负责分配/释放内存（`allocate`/`deallocate`）和构造/销毁对象（`construct`/`destroy`）。

#### 1.2 默认分配器 `std::allocator`
// `T* allocate(size_t n);` 分配 n 个 T 的内存（不调用构造）；`void construct(T* p, const T& val);` 在 p 处 placement new 构造对象

#### 1.3 分配与构造的分离
STL 容器将内存分配和对象构造分开，支持 `reserve` 和 `capacity > size` 语义：先分配一大块原始内存，再按需构造对象。

#### 1.4 面试常见问题
**为何分配与构造分离？** 支持 reserve 语义（先分配内存，后构造元素）。**如何自定义 Allocator？** 实现标准接口并作为模板参数传给容器。

# 92_二_Iterator_Traits_迭代器萃取器

### 二、Iterator Traits（迭代器萃取器）

#### 2.1 作用
从迭代器类型中提取关键信息（value_type、difference_type、iterator_category 等），使算法能泛型处理不同迭代器。

#### 2.2 `std::iterator_traits`
// `iterator_traits<Iterator>::value_type`、`iterator_traits<Iterator>::iterator_category` 等类型萃取

#### 2.3 迭代器分类（从弱到强）
| 类别 | 支持操作 | 典型容器 |
|------|---------|---------|
| Input | 只读、单遍、++ | istream_iterator |
| Output | 只写、单遍、++ | ostream_iterator |
| Forward | 读写、多遍、++ | forward_list |
| Bidirectional | 双向、++/-- | list, set, map |
| Random Access | 随机访问、+/-/[]/< | vector, deque, 数组指针 |

#### 2.4 标签分发（Tag Dispatch）
算法根据迭代器类别标签选择最优实现：如 `std::advance` 对 Random Access 迭代器用 `it += n`（O(1)），对 Input 迭代器用 `while (n--) ++it`（O(n)）。

# 93_三_核心容器底层实现

### 三、核心容器底层实现

#### 3.1 `std::vector`
底层：连续动态数组（三个指针：start/finish/end_of_storage）。扩容策略：GCC 通常 2 倍，MSVC 通常 1.5 倍。push_back 均摊 O(1)。插入/删除非末尾元素及扩容时迭代器失效。

#### 3.2 `std::deque`
底层：分段连续数组（中控器 map + 多个固定大小 buffer）。头尾插入 O(1)，随机访问 O(1)。仅头尾插入/删除不使迭代器失效。

#### 3.3 `std::list` / `std::forward_list`
list 双向链表（prev/next 指针），forward_list 单向链表（仅 next 指针，无 size() 方法）。插入/删除已知位置 O(1)，不使其他迭代器失效。

#### 3.4 `std::set` / `std::map`
底层：红黑树（自平衡二叉搜索树）。插入/删除/查找 O(log n)。中序遍历得有序序列。插入不使迭代器失效，删除仅使被删元素迭代器失效。

#### 3.5 `std::unordered_set` / `std::unordered_map`
底层：哈希表（libstdc++ 链地址法）。平均 O(1)，最坏 O(n)。负载因子超过 max_load_factor 时 rehash（所有迭代器失效）。

#### 3.6 `std::priority_queue`
底层：vector + 堆算法（push_heap/pop_heap）。默认最大堆，top() 返回最大元素。插入 O(log n) 上浮，删除堆顶 O(log n) 下沉。

---

## C++ 工程工具链
# 94_一_CMake_构建系统

### 一、CMake 构建系统

#### 1.1 为什么用 CMake
CMake 是跨平台构建系统生成器，通过 `CMakeLists.txt` 生成 Makefile/Ninja/VS 等构建文件。

#### 1.2 最小 CMake 项目
```cmake
cmake_minimum_required(VERSION 3.14)
project(MyProject LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(myapp src/main.cpp src/utils.cpp)
target_include_directories(myapp PRIVATE include)
target_link_libraries(myapp PRIVATE pthread)
```

#### 1.3 现代 CMake 最佳实践
使用 target-based 命令而非全局命令：
```cmake
target_compile_options(myapp PRIVATE -Wall -Wextra -Wpedantic)
target_compile_features(myapp PUBLIC cxx_std_17)
find_package(Boost REQUIRED COMPONENTS filesystem system)
install(TARGETS myapp DESTINATION bin)
```

#### 1.4 构建与测试
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
ctest --test-dir build --output-on-failure
```

# 95_二_Sanitizers_消毒剂检测器

### 二、Sanitizers（消毒剂/检测器）

编译器内置的运行时检测工具：
- **AddressSanitizer (ASan)**：检测缓冲区溢出、use-after-free、double-free、内存泄漏。编译：`-fsanitize=address`
- **ThreadSanitizer (TSan)**：检测数据竞争、锁顺序问题。编译：`-fsanitize=thread`
- **UndefinedBehaviorSanitizer (UBSan)**：检测整数溢出、空指针解引用等未定义行为。编译：`-fsanitize=undefined`
- **MemorySanitizer (MSan)**：检测未初始化内存读取（仅 Clang）。编译：`-fsanitize=memory`
- ASan + UBSan 可同时启用

# 96_三_性能分析工具

### 三、性能分析工具

- **perf**（Linux）：`perf record -g ./myapp` 热点分析；`perf stat -e cache-references,cache-misses` 缓存命中率
- **Google Benchmark**：微基准测试库，`BENCHMARK(BM_Func)->Arg(1000);`
- **Valgrind**：`valgrind --leak-check=full` 内存泄漏检测；`--tool=cachegrind` 缓存性能分析

# 97_四_静态分析工具

### 四、静态分析工具

- **clang-tidy**：C++ 静态分析，检查代码风格/bug 模式/现代 C++ 用法。`clang-tidy src/main.cpp -- -std=c++17`
- **clang-format**：代码格式化。`clang-format -i src/main.cpp`，常用配置 BasedOnStyle: Google
- **Cppcheck**：独立于编译器的静态分析。`cppcheck --enable=all src/`

# 98_五_CICD_与代码质量

### 五、CI/CD 与代码质量

推荐的 CI 检查流程：Build → Test → ASan → clang-tidy 检查。编译警告级别推荐开启所有警告并将警告视为错误：`-Wall -Wextra -Wpedantic -Werror -Wconversion -Wsign-conversion -Wold-style-cast`

---


---


# 02_当前目录仅包含_README_md_正文为历史文档合并知识点汇总_若出现旧文件名式标题_表示章节来源_不代表当前目录存在对应文件

## 当前目录仅包含 README.md，正文为历史文档合并/知识点汇总。若出现旧文件名式标题，表示章节来源，不代表当前目录存在对应文件。
# 03_problems__32位系统一个进程最多有多少堆内存_md

## problems__32位系统一个进程最多有多少堆内存.md

32位Linux中虚拟地址空间4GB，典型3G/1G划分（3GB用户空间，1GB内核空间）。堆位于用户空间，无固定上限，受代码段、数据段、栈、mmap区域共同影响。限制是虚拟地址而非物理内存限制。

# 04_problems__Linux上有个二进制程序一直在运行_我修改代码置换重新编译把原来的二进制程序覆盖了_会怎么样_md

## problems__Linux上有个二进制程序一直在运行，我修改代码置换重新编译把原来的二进制程序覆盖了，会怎么样？.md

正在运行的程序不受影响（程序启动时内容已载入内存），但会影响后续启动的进程。区别：共享库可热更新（依赖动态链接），二进制程序本身不能。

# 05_problems__linux文件系统_inode_inode存储了哪些东西_目录名_文件名存在哪里_md

## problems__linux文件系统：inode，inode存储了哪些东西，目录名，文件名存在哪里.md

inode存储文件元数据（用户ID、组ID、大小、时间戳、权限、链接数、数据块位置、文件类型），不存储文件名——文件名存储在目录文件的目录项中，每个目录项包含文件名和指向inode的指针。

# 06_problems__linux的内存管理机制内存寻址方式什么叫虚拟内存内存调页算法任务调度算法_md

## problems__linux的内存管理机制内存寻址方式什么叫虚拟内存内存调页算法任务调度算法.md

内存管理：Linux使用分页机制，每个进程有独立虚拟地址空间，通过页表映射到物理页帧。内存寻址：使用虚拟内存寻址，MMU通过页表完成虚拟到物理地址转换。虚拟内存：允许进程使用超过物理内存的地址空间，缺页时从swap载入。调页算法：近似LRU（active/inactive列表），非简单的Second Chance/Clock。任务调度：CFS（完全公平调度器，红黑树）、实时调度器（FIFO/RR）、Deadline调度器（动态优先级）。

# 07_problems__linux系统的各类同步机制_linux系统的各类异步机制_md

## problems__linux系统的各类同步机制、linux系统的各类异步机制.md

同步机制：互斥锁、读写锁、条件变量、信号量（原始+Pthreads）、自旋锁、屏障。异步机制：异步IO（AIO）、信号（Signal）、回调函数、epoll/select/poll I/O多路复用（并非真异步，就绪后通知读写）。

# 08_problems__一段代码从程序到执行经历怎么样的过程_程序在计算机中是如何运行起来的_md

## problems__一段代码从程序到执行经历怎么样的过程（程序在计算机中是如何运行起来的）.md

1.编写代码→2.编译器编译为目标代码→3.链接器链接多目标文件为可执行文件→4.加载器加载到内存（创建PCB、分配空间）→5.CPU执行指令→6.MMU将虚拟地址映射为物理地址→7.系统调用进入内核态→8.异常处理→9.结束执行，释放资源。

# 09_problems__为什么CPU访问寄存器的速度比访问内存或CPUCache的速度快_md

## problems__为什么CPU访问寄存器的速度比访问内存或CPUCache的速度快.md

1.寄存器位于CPU核心内部，物理距离近、信号路径短；2.寄存器由触发器（flip-flop）构成，一个时钟周期完成读写，Cache用SRAM，内存用DRAM；3.寄存器直接CPU访问，无总线协议开销；4.寄存器容量极小。

# 100_problems__介绍下Socket编程_md

## problems__介绍下Socket编程.md

Socket编程实现网络进程间通信。基本流程：创建Socket→绑定地址端口→监听连接→接受连接→发送/接收数据→关闭连接。

# 101_problems__从本地读取一个文件通过网络发送到另一端_中间涉及几次拷贝_md

## problems__从本地读取一个文件通过网络发送到另一端，中间涉及几次拷贝.md

涉及4次拷贝：本地文件→内存缓冲区、内存缓冲区→网络发送缓冲区、网络接收缓冲区→内存缓冲区、内存缓冲区→目标文件。

# 102_problems__列举你所知道的tcp选项_md

## problems__列举你所知道的tcp选项.md

MSS（最大分段大小）、窗口缩放（Window Scaling）、时间戳（RTT测量/PAWS）、选择性确认SACK、NOP（填充对齐）、结束标志（EOL）。

# 103_problems__同步_异步_阻塞和非阻塞的概念_md

## problems__同步，异步，阻塞和非阻塞的概念.md

同步：调用者等待操作完成才返回。异步：发起操作后立即返回，完成后通知。阻塞：等待期间无法做其他事。非阻塞：立即返回，可继续执行。典型组合：同步阻塞I/O、同步非阻塞I/O、异步阻塞I/O、异步非阻塞I/O。

# 104_problems__在TCP三次握手的时候_如果网络情况非常好且百分百不会发生拥塞_不会重传_不会有历史链接问题_那么三次握手可以

## problems__在TCP三次握手的时候，如果网络情况非常好且百分百不会发生拥塞，不会重传，不会有历史链接问题，那么三次握手可以改为两次吗？.md

不能。三次握手核心在于双向确认通信双方的接收能力——第二次握手仅是服务器告知客户端"我准备好了"，第三次握手是客户端告知服务器"我也准备好接收你的数据"。两次握手无法确保双向确认。

# 105_problems__在TCP拥塞控制中_使用了什么样的算法_md

## problems__在TCP拥塞控制中，使用了什么样的算法？.md

慢启动（指数增长至阈值）→拥塞避免（线性增长）→快重传（连续3个重复ACK立即重传）→快恢复（窗口减半后线性增长）。

# 106_problems__对称加密和非对称加密的区别都有哪些_md

## problems__对称加密和非对称加密的区别都有哪些？.md

密钥：对称加密使用同一密钥加解密；非对称使用公钥加密、私钥解密。速度：对称快，非对称慢（复杂数学运算）。安全性：对称密钥分发有风险；非对称公钥公开仍安全。场景：对称用于大批量数据（文件/数据库加密）；非对称用于数字签名、SSL/TLS、安全邮件。算法：对称有AES、DES、3DES、RC4；非对称有RSA、ECC、Diffie-Hellman、ElGamal。

# 107_problems__局域网的IP分配策略是什么_它是怎么实现的_md

## problems__局域网的IP分配策略是什么？它是怎么实现的？.md

静态IP分配：管理员手动配置唯一IP，适合服务器等需要稳定IP的设备。动态IP分配：通过DHCP自动分配（DHCP发现→DHCP提供→DHCP请求→DHCP确认）。

# 108_problems__说说你对TCP流量控制的理解_md

## problems__说说你对TCP流量控制的理解？.md

TCP流量控制通过滑动窗口调整发送速率，避免发送方数据超过接收方处理能力。每个报文段含窗口字段（接收方当前可接收字节数）。窗口为0时发送方暂停。

# 109_problems__说说你对TCP滑动窗口的理解_md

## problems__说说你对TCP滑动窗口的理解？.md

TCP滑动窗口是流量控制与拥塞控制机制。发送方根据接收方通知的窗口大小发送数据，等待确认后继续。接收方就绪后增大窗口，繁忙时减小或关闭。

# 10_problems__为什么有了进程还需要线程和协程_md

## problems__为什么有了进程还需要线程和协程？.md

进程创建管理重，不适用于频繁交互场景。线程更轻量，共享进程内存，上下文切换快，但调度仍在内核态。协程完全在用户态，更轻量，适合I/O密集型任务和微服务，提供更细粒度操作和高效CPU利用率。

# 110_problems__请你介绍一下http1_0_md

## problems__请你介绍一下http1.0.md

简单请求-响应模型。无状态协议，每个请求独立。非持久连接，每次请求新TCP连接，导致C10k问题。方法：GET、POST、HEAD。缺点：效率低，无Host头（每IP只能托管一个网站）。

# 111_problems__请你介绍一下http1_1_md

## problems__请你介绍一下http1.1.md

相比HTTP/1.0改进：持久连接（默认keep-alive，复用TCP）、管道化（可连发多请求）、分块传输编码（动态内容边生成边发送）、缓存控制（Cache-Control）、内容协商、扩展状态码。

# 112_problems__请你介绍一下http2_0_md

## problems__请你介绍一下http2.0.md

二进制帧层（非文本格式）、多路复用（单连接同时多发请求，解决队头阻塞）、服务器推送（主动推资源）、HPACK头部压缩、优先级和流控制。

# 113_problems__请你介绍一下http3_0_md

## problems__请你介绍一下http3.0.md

基于QUIC协议。减少连接建立时间（TLS握手集成）、避免队头阻塞（独立数据流互不影响）、连接迁移（IP变化连接不中断）、内置加密（TLS 1.3默认）、流优先级和流量控制。

# 114_problems__针对于ipv4地址不够用的情况_我们是如何解决的_md

## problems__针对于ipv4地址不够用的情况，我们是如何解决的？.md

IPv6：128位地址空间，约340亿亿亿亿个地址。优势：更大地址空间、内置IPSec安全、简化头部格式、原生支持多播和任播。

# 115_problems__长连接和短连接_md

## problems__长连接和短连接.md

短连接：每次通信建新连接，适合轻量偶尔交互，频繁握手消耗大。长连接：保持开放，多次数据传输，减少建连开销，HTTP/1.1默认。心跳机制维持活性。选择依据：交互频率、资源消耗。

# 11_problems__为什么用户态和内核态的相互切换过程开销比较大_md

## problems__为什么用户态和内核态的相互切换过程开销比较大.md

1.上下文切换（保存/恢复PC、寄存器、栈指针）；2.权限级别变更；3.地址空间切换和映射；4.TLB刷新导致缓存失效。

# 12_problems__五种IO模式_md

## problems__五种IO模式.md

1.阻塞I/O：数据未准备好则阻塞挂起。2.非阻塞I/O：立即返回错误或数据，需轮询。3.I/O多路复用：select/poll/epoll同时监控多个FD。4.信号驱动I/O：注册信号处理函数，数据就绪时发信号。5.异步I/O：发起后立即返回，内核完成拷贝并通知。

# 13_problems__什么是PCB_md

## problems__什么是PCB？.md

PCB（进程控制块）是操作系统存储进程信息的数据结构，包含：PID、进程状态、程序计数器、CPU寄存器信息、CPU调度信息（优先级、队列指针等）。

# 14_problems__什么是僵尸进程_md

## problems__什么是僵尸进程？.md

子进程结束但父进程未调用wait()/waitpid()收割退出状态，导致PCB无法释放成为僵尸进程。僵尸进程仅消耗PCB（进程表空间），大量累积会阻止创建新进程。父进程终止后由init进程接管清理。

# 15_problems__什么是缺页中断_md

## problems__什么是缺页中断.md

程序访问的虚拟页不在物理内存中时触发缺页中断。步骤：CPU收到中断→暂停进程、内核处理→从磁盘加载页面到空闲页框→更新页表→恢复进程执行。

# 16_problems__什么是进程_什么是线程_他们的区别是什么_md

## problems__什么是进程？什么是线程？他们的区别是什么？.md

进程：资源分配基本单位，独立内存空间，相互隔离。线程：调度最小单位，共享进程内存与资源，有独立栈和PC（轻量级进程）。区别：进程有独立地址空间（一个崩溃不影响其他）；线程共享地址空间（一个错误可能崩溃整个进程）。进程通信需IPC；线程直接读写共享数据。进程创建/切换开销大；线程较小。进程独立执行；线程依赖进程存在。

# 17_problems__什么是页表_什么是快表_md

## problems__什么是页表、什么是快表.md

页表：操作系统用于虚拟地址到物理地址映射的数据结构（多级结构）。TLB（快表）：页表的高速缓存，位于CPU芯片内部，存储最近使用的虚拟-物理地址映射，加速地址转换。TLB命中率直接影响系统性能。

# 18_problems__介绍下分层存储体系和CPU三级缓存_md

## problems__介绍下分层存储体系和CPU三级缓存.md

分层存储：寄存器（最快）→L1/L2/L3 Cache（SRAM，容量逐增速度逐降）→主存RAM→辅助存储器（磁盘/SSD）。CPU三级缓存：L1（核心内，指令+数据分离，最快），L2（核心外，L1未命中），L3（多核共享，相对最慢）。

# 19_problems__介绍下进程的地址空间_虚拟地址和物理地址_md

## problems__介绍下进程的地址空间（虚拟地址和物理地址）.md

虚拟地址空间：每个进程独立的逻辑地址空间（代码段、数据段、堆、栈）。物理地址空间：实际硬件内存地址，不含swap。地址映射：MMU将虚拟地址转换为物理地址，未映射时触发缺页中断。地址空间划分：页面（通常4KB），虚拟页→物理页通过页表映射。

# 20_problems__内存映射文件是什么_如何用它来处理大文件_md

## problems__内存映射文件是什么？如何用它来处理大文件？.md

内存映射文件将磁盘文件内容映射到进程地址空间（mmap），通过指针直接读写，无需传统read/write。处理大文件：mmap映射→指针访问→msync同步→munmap撤销。性能优于传统I/O，尤其适合随机访问大文件。

# 21_problems__分段和分页的区别有哪些_md

## problems__分段和分页的区别有哪些？.md

目的：分段为程序和数据分开处理（有逻辑意义）；分页为简化内存管理、避免外部碎片。单位：段（可变长度，有逻辑意义）；页（固定大小，一般4KB，无逻辑意义）。地址结构：段号+段内偏移；页号+页内偏移。碎片：分段有外部碎片；分页有内部碎片（最后页未满）。

# 22_problems__分页和分段的区别是什么_md

## problems__分页和分段的区别是什么？.md

分页：简化内存管理、消除外部碎片，对程序员透明。分段：反映程序逻辑结构，更灵活但要求程序员更多内存管理责任。

# 23_problems__协程是什么_为什么需要协程_md

## problems__协程是什么，为什么需要协程.md

协程是用户态轻量级执行单元，拥有自己的寄存器上下文和栈，切换时保存/恢复上下文。原因：节省CPU（避免内核级线程频繁切换）、节约内存（几十KB vs 线程MB级栈）、稳定性（线程共享内存易互相影响崩溃）、开发效率（方便IO操作异步化）。

# 24_problems__固态硬盘和机械硬盘区别_md

## problems__固态硬盘和机械硬盘区别.md

SSD用闪存芯片（电子读写，无移动部件），速度快、抗震好、功耗低、无噪音，有P/E周期限制。HDD用旋转磁盘和磁头（机械运动），速度慢（尤其随机读写），易受震动影响，但无写入寿命限制，价格便宜容量大。

# 25_problems__外中断和内中断有什么区别_md

## problems__外中断和内中断有什么区别？.md

外中断：由处理器外部事件引起（I/O设备、计时器等），异步发生，不可预知。内中断：由程序执行事件引起（除零、无效指令、越界等），同步发生，可由INT指令触发。两者都需保存状态→执行中断处理例程→恢复。

# 26_problems__如何使用gdb来定位C_程序中的死锁_md

## problems__如何使用gdb来定位C++程序中的死锁？.md

编译加`-g`。gdb加载后使用：`run`运行，`thread apply all bt`打印所有线程调用栈，`info threads`查看线程，`thread <id>`切换，分析互斥锁和资源依赖关系。

# 27_problems__如何实现守护进程_md

## problems__如何实现守护进程.md

1.fork()创建子进程（父进程退出）；2.子进程setsid()创建新会话；3.再次fork()（保证非会话领头进程）；4.chdir("/")改变工作目录；5.umask(0)重设权限掩码；6.关闭所有文件描述符；7.重定向stdin/stdout/stderr到/dev/null或日志文件。

# 28_problems__如果发现自己的Linux服务器负载过高_应该怎么排查原因呢_md

## problems__如果发现自己的Linux服务器负载过高，应该怎么排查原因呢？.md

1.top/htop查看整体状态；2.检查CPU（top按CPU排序）；3.free -m检查内存（物理内存耗尽+swap使用多则不足）；4.iostat检查磁盘I/O；5.netstat/ss检查网络连接状态（大量TIME_WAIT/CLOSE_WAIT）。

# 29_problems__常见的信号_系统如何将一个信号通知到进程_md

## problems__常见的信号、系统如何将一个信号通知到进程.md

常见信号：SIGHUP(挂起)、SIGINT(中断)、SIGQUIT(退出+core)、SIGILL、SIGTRAP、SIGABRT、SIGBUS、SIGFPE、SIGKILL(不可阻塞/忽略)、SIGUSR1/2、SIGSEGV、SIGPIPE、SIGALRM、SIGTERM、SIGCHLD、SIGCONT、SIGSTOP、SIGTSTP/SIGTTIN/SIGTTOU。处理方式：忽略、捕捉（信号处理函数）、默认操作。通知过程：内核修改进程上下文设标识→进程被调度时检查未处理信号→执行对应处理函数或默认操作。

# 30_problems__并行和并发的区别_md

## problems__并行和并发的区别.md

并行：多个任务同一时刻同时运行（多核/多线程真同时）。并发：多个任务同一时间段交替执行（时间片轮转看似同时，实际交替）。

# 31_problems__操作系统本身为用户提供什么功能_md

## problems__操作系统本身为用户提供什么功能.md

资源管理（CPU/内存/硬盘/网络）、进程管理（创建/调度/终止）、文件系统、设备管理、用户界面（GUI/CLI）、内存管理（分配/虚拟内存/保护）、安全性（认证/权限/加密）、错误处理。

# 32_problems__标准库函数和系统调用的区别_md

## problems__标准库函数和系统调用的区别.md

来源：系统调用来自内核，标准库来自运行时环境。实现：系统调用需切内核态，标准库在用户态实现（可能内部调用系统调用）。性能：系统调用含上下文切换开销更大。可移植性：系统调用依赖操作系统，标准库跨平台一致。

# 33_problems__栈和队列的区别_适用场景_md

## problems__栈和队列的区别、适用场景.md

> 注意：本节内容属于数据结构范畴，建议归类到「数据结构与算法」目录。

栈：LIFO，插入删除O(1)，适用函数调用、表达式求值、括号匹配。队列：FIFO，插入删除O(1)，适用任务调度、缓冲区管理、BFS。

# 34_problems__死锁必要条件及避免算法_md

## problems__死锁必要条件及避免算法.md

四个必要条件：互斥、请求与保持、不可剥夺、循环等待。避免方法：银行家算法（分配前判断安全性）、资源有序分配法（破坏循环等待）、可抢占调度算法。无万能解法，内核中断等特殊场景会使算法失效。

# 35_problems__用户态和内核态的区别_md

## problems__用户态和内核态的区别.md

权限：用户态受限（不能执行敏感指令、不能直接访问硬件）；内核态无限制。功能：用户态运行应用程序，内核态运行内核和驱动。资源访问：用户态通过系统调用请求服务，内核态直接访问硬件。切换：系统调用/中断触发切换，频繁切换影响性能。

# 36_problems__线程间的通信方式有哪些_md

## problems__线程间的通信方式有哪些？.md

共享内存（需同步机制）、互斥锁、读写锁、条件变量、信号量、事件（event）、屏障（barrier）、线程局部存储（TLS，独立副本不共享）、原子操作（CPU原子指令）。

# 37_problems__线程_进程和协程是否有自己独立的堆区和栈区_md

## problems__线程，进程和协程是否有自己独立的堆区和栈区？.md

进程有独立堆和栈。线程共享进程堆，有独立栈。协程共享堆，有独立栈（由运行时分配，更灵活高效）。

# 38_problems__软中断和硬中断分别指的是什么_md

## problems__软中断和硬中断分别指的是什么？.md

硬中断：由硬件设备产生（键盘、磁盘等），异步。软中断：由程序产生（I/O请求等），用于执行不紧急的内核任务。两者都会暂停当前任务处理中断请求，处理完成后恢复。

# 39_problems__进程的状态转换有哪些_md

## problems__进程的状态转换有哪些？.md

创建（分配资源初始化）→就绪（等待CPU时间片）→运行（获得CPU执行指令）→等待（I/O或资源不可用）→终止（执行完毕或被强制停止，回收资源）。

# 40_problems__进程终止的方式有哪些_md

## problems__进程终止的方式有哪些？.md

正常退出（任务完成自愿结束）、错误退出（遇到无法处理的错误自愿终止）、致命错误（段错误等OS强制结束）、被其他进程杀死（SIGKILL/SIGTERM等信号）、父进程终止（子进程变为孤儿进程，被init收养继续执行，不会自动杀死）。

# 41_problems__进程间的通信方式有哪些_md

## problems__进程间的通信方式有哪些？.md

常见：管道（匿名/命名）、信号、消息队列、共享内存（最快IPC，无需复制）、信号量（同步）、Socket（跨网络或本地）。扩展：文件系统、共享文件/映射文件、RPC、MPI、LPC、Domain Sockets。

---

## 网络编程实战
# 42_一_IO_多路复用深入

### 一、I/O 多路复用深入

#### 1.1 select / poll / epoll 对比

| 特性 | select | poll | epoll |
|------|--------|------|-------|
| 最大 FD 数 | 1024（FD_SETSIZE） | 无限制 | 无限制 |
| 时间复杂度 | O(n) 每次全量扫描 | O(n) 每次全量扫描 | O(1) 仅活跃 FD |
| 内核到用户空间 | 每次调用拷贝全量 fd_set | 每次拷贝全量 pollfd 数组 | epoll_wait 仅返回就绪 fd 列表 |
| 触发模式 | 仅水平触发（LT） | 仅水平触发（LT） | LT + 边缘触发（ET） |
| 适用场景 | 跨平台兼容 | FD 数中等 | 高并发、大量连接 |

#### 1.2 epoll 两种触发模式

**水平触发（LT）**：只要FD就绪，每次epoll_wait都会通知，默认模式，编程简单。**边缘触发（ET）**：仅在FD状态变化时通知一次，必须使用非阻塞I/O，循环读写直到EAGAIN，性能更高。// ET下读处理：循环read直到返回EAGAIN

# 43_二_Reactor_模式

### 二、Reactor 模式

#### 2.1 核心思想

Reactor将I/O事件分发与处理分离：Reactor负责监听事件并分发给Handler，Handler负责具体I/O和业务逻辑，底层通过epoll/select多路复用。

#### 2.2 单 Reactor 单线程

所有I/O事件在单线程中处理。优点：简单、无线程同步开销。缺点：无法利用多核，阻塞操作会阻塞整个服务。适用：Redis、轻量级服务。

#### 2.3 单 Reactor 多线程

Reactor线程只负责accept和事件分发，业务逻辑交给Worker线程池。适用：Memcached、大多数网络服务。

#### 2.4 主从 Reactor 多线程

主Reactor只负责accept，将FD分配给从Reactor。从Reactor独立管理各自连接的I/O事件。优点：充分利用多核。适用：Netty、高性能网关。

# 44_三_Proactor_模式

### 三、Proactor 模式

| 特性 | Reactor | Proactor |
|------|---------|----------|
| I/O 模型 | 同步 I/O 多路复用 | 异步 I/O（AIO） |
| 事件通知 | "FD 可读/可写" | "读写操作已完成" |
| 数据拷贝 | 用户态手动 read/write | 内核自动完成，回调通知 |
| Linux 支持 | epoll（成熟） | io_uring / AIO（有限） |

Proactor流程：发起异步读请求（传递buffer和回调）→内核DMA拷贝到用户buffer→通知用户态"读完成"。Linux原生AIO有限，io_uring是最接近Proactor的方案。

# 45_四_io_uring

### 四、io_uring

io_uring是Linux 5.1+异步I/O接口，通过共享环形缓冲区实现零拷贝通信。核心优势：零系统调用（SQ+CQ）、真正异步、支持通用I/O（read/write/accept/send/recv等）、性能高于epoll。// io_uring_queue_init()初始化→io_uring_get_sqe()获取提交项→io_uring_prep_read()准备→io_uring_submit()提交→io_uring_wait_cqe()等待完成

| 特性 | epoll | io_uring |
|------|-------|----------|
| 系统调用次数 | 每次事件循环至少1次 | 批量提交大幅减少 |
| 数据拷贝 | 需用户态read/write | 内核直接拷贝到用户buffer |
| 适用场景 | 事件驱动、连接管理 | 高吞吐I/O、文件I/O |
| 内核版本 | 2.6+ | 5.1+ |

# 46_五_零拷贝_Zero_Copy

### 五、零拷贝（Zero Copy）

传统I/O路径：磁盘→内核缓冲区（DMA）→用户缓冲区（CPU拷贝）→Socket缓冲区（CPU拷贝）→网卡（DMA），共4次上下文切换+4次数据拷贝（2次CPU拷贝）。

sendfile()：磁盘→内核缓冲区（DMA）→Socket缓冲区（CPU拷贝）→网卡（DMA），减少到2次上下文切换+3次数据拷贝（1次CPU拷贝）。

splice()：磁盘→内核缓冲区（DMA）→Socket缓冲区（零拷贝，指针传递）→网卡（DMA），减少到2次上下文切换+2次数据拷贝（0次CPU拷贝，仅DMA）。

| 技术 | CPU拷贝次数 | 上下文切换 | 适用场景 |
|------|------------|-----------|---------|
| 传统read+write | 2 | 4 | 通用 |
| mmap+write | 1 | 4 | 需修改数据 |
| sendfile | 1 | 2 | 文件→socket |
| splice | 0 | 2 | 内核缓冲区间传递 |

# 47_六_高性能网络编程要点

### 六、高性能网络编程要点

#### 6.1 CPU Cache 友好性

避免false sharing（不同线程变量放在不同cache line，64字节对齐），数据局部性（vector优于list），__builtin_prefetch()预取。

#### 6.2 连接管理

TIME_WAIT用SO_REUSEADDR/SO_REUSEPORT处理，连接池复用TCP，优雅关闭shutdown(SHUT_WR)后再close()。

#### 6.3 多线程模型

One Loop Per Thread（muduo模型），线程池+任务队列（I/O和计算分离），无锁队列减少锁竞争。

#### 6.4 常用高性能网络库

muduo（C++ Reactor）、Boost.Asio（跨平台Proactor/Reactor）、libevent（C Reactor）、brpc（百度RPC框架）、DPDK（旁路内核用户态网卡驱动）。

---

## docs__linux常用操作命令.md

> 以下内容原位于《计算机网络》目录，现移至本目录（与操作系统/Linux 主题更相关）。
# 48_OP

### OP

#### CPU

// cat /proc/cpuinfo | grep "physical id" | sort | uniq 查看物理CPU数
// cat /proc/cpuinfo | grep "cores" | uniq 查看每CPU核心数
// cat /proc/cpuinfo | grep "processor" | wc -l 查看逻辑CPU数

#### IO

// iostat -x -d 1 查看IO读写状态
// iotop 查看谁在占用IO

#### Memory

// top / free -g / free -m 查看内存使用

#### free

total=总内存，used=已用，free=空闲，buffers=Buffer Cache，cached=Page Cache。-buffers/cache（used-buffers-cached）反映程序实际占用内存，+buffers/cache（free+buffers+cached）反映可挪用内存。

#### top

VIRT：进程需要的虚拟内存（含库/代码/数据/malloc/栈）。RES：进程当前使用的物理内存（含共享库，不含swap out）。SHR：共享内存大小（含其他进程的共享）。RES-SHR近似计算进程独占物理内存。

#### OS

// lsb_release -a 系统版本
// uname -r 内核版本

#### Disk

// du -sh filename 看文件大小
// df -H 查看磁盘空间
// lsblk -d -o name,rota ROTA=1为HDD，0为SSD

#### Network

// ethtool xgbe0 查看网卡速率
// curl ip:port 判断端口是否监听
// lsof -i:port 列出监听端口
// netstat -tunlp | grep port 列出监听端口号
// telnet www.test.com 8090 测试域名+端口可达性
// netstat | grep 端口号 查看哪些IP在访问

#### Linux

// tar -zxvf 解压 / tar -zcvf output.tar output/ 压缩
// ldd 查看应用依赖的动态库
// curl url -o obj 下载文件
// nohup cmd & 后台运行
// ssh -i id_rsa root@ip 私钥登录
// 2>&1 标准错误输出到标准输出
// ctrl+R 查找历史命令
// find ./ -name "*.xml" 递归查找文件
// rsync -av src/ dest 远程同步
// ln -s target link 软链
// dig www.baidu.com DNS查询
// pstack pid 打印线程栈
// uptime 查看机器启动时间
// ulimit -c 查看core文件设置
// cat /proc/sys/fs/file-max 查看系统最大描述符
// lsof -p pid 查看进程打开的fd
// top -Hp pid 查看进程的线程情况
// sysctl -a | grep keepalive 查看TCP探活设置

##### user_manager

// useradd / passwd / userdel 用户管理

#### git

// git reset HEAD 取消add；git checkout filename 恢复文件
// git commit --amend 修改提交信息
// git reset --hard logid 回滚（已commit未push）
// git branch -d 删除本地分支
// git push origin --delete 删除远端分支
// git clone --depth 1 浅克隆
// git stash / git stash pop 暂存/恢复
// git rebase / git cherry-pick 变基/挑选提交
// git format-patch / git am 生成和应用patch

#### Mysql

// select count(*) from table_name 表记录数
// SHOW CREATE TABLE tablename 查看建表语句

#### Gdb

// gdb program core 加载core文件
// bt(backtrace) 查看堆栈
// f(frame) n 切换栈帧

# 49_Software_shortcuts

### Software shortcuts

#### Tmux

ctrl+b 前缀键；" 水平分屏；% 垂直分屏；x 关闭面板；& 关闭窗口；c 创建窗口；方向键选择面板；[ 进入copy-mode

#### Taglist

u 更新tag窗口；x 放大缩小

#### Chrome

⌘+t 新标签页；⌘+9 最后一个标签；⌘+w 关闭标签

# 50_Java_compile

### Java compile

// javac -cp ".:./third-party/*:./lib/*" Sample.java 编译带依赖jar
// java -cp ".:./third-party/*:./lib/*" Sample 运行

# 51_Maven

### Maven

~/.m2/settings.xml 用户配置；mvn install 下载依赖；mvn package 编译打包；-Dmaven.test.skip=true 跳过测试

# 52_Ack

### Ack

// ack -w wordname 全词匹配搜索

# 53_crontab

### crontab

// crontab -e 编辑任务；crontab -l 列表

# 54_size

### size

mysql 5.0+ varchar(20)指20字符；bigint 8字节

# 55_排除问题的思路

### 排除问题的思路

先查看机器内存和CPU，再看日志。

---
# 56_problems__cacheline和伪共享_false_sharing_md

## problems__cacheline和伪共享（false-sharing）.md

cacheline是CPU缓存最小读写单位（通常64字节）。伪共享：两个线程修改不同变量但位于同一cacheline，导致缓存一致性协议（MESI）频繁失效，性能严重退化。避免方法：`alignas(64)`或`std::hardware_destructive_interference_size`（C++17）让变量独占cacheline；每个线程操作独立cacheline对齐的数据；分散高频写入的共享数据到不同cacheline。

# 57_problems__无锁队列原理_md

## problems__无锁队列原理.md

无锁队列基于CAS原子操作。SPSC（单生产者单消费者）用两个atomic下标（head/tail）实现。ABA问题：线程A读到值X后被调度走，线程B将X改为Y再改回X，线程A CAS时误判。解决：带版本号的tagged pointer或double-width CAS（128位）。生产环境推荐boost::lockfree::queue或folly::MPMCQueue。

---

## Interview__Network__README.md

> 说明：当前目录仅保留 README.md，正文为历史文档合并/知识点汇总；文中的旧文件名式标题仅表示章节来源，不代表当前目录存在对应文件。
# 58_Network

### Network

历史工程说明页：最小本地TCP echo server/client示例，适合面试讲socket生命周期。

面试知识点：// socket()→bind()→listen()→accept()→recv/send→close() TCP服务端流程；// socket()→connect()→send/recv→close() 客户端流程；高频追问：半包粘包、阻塞与非阻塞、IO多路复用、连接关闭时序、心跳。RAII封装fd是加分点。

# 59_problems__Cookie和Session_md

## problems__Cookie和Session.md

Cookie：存储在客户端的小数据片段（键值对），通过HTTP头传输，约4KB限制，有安全隐患。用途：跟踪会话、存储偏好、自动登录。Session：服务器端存储信息，分配唯一Session ID（通常通过Cookie下发），数据在服务端更安全。典型流程：用户访问→服务器创建Session→Session ID写入Cookie→后续请求携带ID→服务器识别会话。

# 60_problems__DNS和HTTP协议_HTTP请求方式_md

## problems__DNS和HTTP协议，HTTP请求方式.md

DNS：将域名转换为IP地址的分布式服务，通常用UDP查询（快）。HTTP：应用层协议，构建在TCP/IP上，客户端-服务器架构。请求方法：GET（获取资源）、POST（提交数据）、PUT（替换资源，幂等）、DELETE（删除）、HEAD（仅头部）、OPTIONS（查看支持方法）、PATCH（部分修改）、CONNECT（SSL隧道）、TRACE（回显测试）。

# 61_problems__DNS域名缓存是什么_md

## problems__DNS域名缓存是什么？.md

减少对远端DNS服务器的查询，加速解析。缓存位置：浏览器缓存、操作系统缓存（ipconfig /displaydns）、递归DNS服务器缓存、权威DNS服务器通过TTL控制缓存时长。

# 62_problems__DNS解析的过程_md

## problems__DNS解析的过程？.md

浏览器缓存→系统缓存→路由器缓存→ISP DNS服务器缓存→根DNS服务器→顶级域(TLD)服务器→权威DNS服务器→缓存结果返回客户端→浏览器用IP访问网站。

# 63_problems__GET请求中_URL编码有什么含义_md

## problems__GET请求中，URL编码有什么含义？.md

处理非ASCII字符（中文等）、转义保留字符（/ ? & =等）、处理空格（+或%20）和控制字符。

# 64_problems__HTTP中常用的状态码都有哪些_md

## problems__HTTP中常用的状态码都有哪些？.md

1xx信息：100 Continue。2xx成功：200 OK、201 Created、204 No Content。3xx重定向：301 Moved Permanently、302 Found、304 Not Modified。4xx客户端错误：400 Bad Request、401 Unauthorized、403 Forbidden、404 Not Found、405 Method Not Allowed。5xx服务器错误：500 Internal Server Error、501 Not Implemented、502 Bad Gateway、503 Service Unavailable、504 Gateway Timeout。

# 65_problems__HTTP方法都有哪些_md

## problems__HTTP方法都有哪些？.md

GET（获取资源，无副作用）、POST（提交数据，可创建/修改资源）、PUT（创建/替换资源，幂等）、DELETE（删除资源）、HEAD（仅获取头部）、OPTIONS（查看支持方法）、PATCH（部分修改资源）、CONNECT（SSL隧道）、TRACE（回环测试）。

# 66_problems__HTTP长连接和短链接都用在那些场景_md

## problems__HTTP长连接和短链接都用在那些场景？.md

短连接：低频请求、简单请求响应、负载均衡分配。长连接：高频请求（减少建连开销）、需快速响应（网页浏览/在线游戏）、实时通信（聊天/数据推送）。HTTP/1.1默认持久连接。

# 67_problems__IO模型了解哪些_md

## problems__IO模型了解哪些？.md

阻塞IO（数据未准备好则阻塞）、非阻塞IO（立即返回，需轮询）、多路复用IO（select/poll/epoll监控多FD）、信号驱动IO（注册信号处理函数，就绪时通知）、异步IO（发起后继续执行，内核完成拷贝并通知）。

# 68_problems__NGINX在Linux上是如何工作的_简单描述一下_md

## problems__NGINX在Linux上是如何工作的？简单描述一下.md

事件驱动+非阻塞I/O处理高并发；反向代理和负载均衡（轮询/最少连接等策略转发请求）；静态资源服务直接读磁盘返回；Master-Worker架构（Master管理配置和Worker生命周期，Worker处理实际请求）。

# 69_problems__OSI七层模型分别是_各自的功能分别是什么_md

## problems__OSI七层模型分别是？各自的功能分别是什么？.md

1.物理层：比特流传输，电/光/无线信号。2.数据链路层：帧封装、MAC寻址、错误检测。3.网络层：路由寻址、IP、分组转发。4.传输层：端到端通信、TCP/UDP、分段重组。5.会话层：建立/管理/终止会话。6.表示层：数据编码/加密/压缩。7.应用层：用户网络服务接口（HTTP/FTP/SMTP等）。

# 70_problems__POST和GET的主要区别有哪些_md

## problems__POST和GET的主要区别有哪些？.md

GET获取资源不改变状态，POST提交数据创建/更新资源。GET数据在URL查询参数（长度受限、不安全、可缓存可收藏），POST数据在请求体（无大小限制、相对安全、不可缓存不可收藏）。

# 71_problems__SYN队列和Accept队列_md

## problems__SYN队列和Accept队列.md

SYN半连接队列：存储收到SYN包的连接，回复SYN+ACK并等待客户端ACK。收到ACK后验证匹配，从SYN队列移除并加入Accept队列。Accept全连接队列：存放已完成三次握手的连接，等待accept()取出给上层应用。

# 72_problems__SYN队列溢出了怎么办_md

## problems__SYN队列溢出了怎么办.md

// netstat -natp | grep SYN_RECV | wc -l 查看SYN队列（SYN_RECV状态连接数）。// netstat -s 查看溢出情况。预防SYN攻击：增大半连接队列、开启SYN cookies、减少SYN+ACK重传次数。

# 73_problems__Socket和WebSocket的区别_md

## problems__Socket和WebSocket的区别.md

Socket：传统网络编程接口，基于TCP/UDP，全双工点对点通信，需手动处理粘包等问题，适用于实时一对一通信。WebSocket：在单个TCP上全双工通信，基于HTTP升级握手（101 Switching Protocols），更高层抽象封装数据帧，适用于Web实时应用（在线游戏/聊天/数据展示）。

# 74_problems__TCP协议是如何保证可靠传输的_md

## problems__TCP协议是如何保证可靠传输的？.md

序列号和确认应答（排序和确认）、校验和与重传机制（检测损坏并重传）、滑动窗口和流量控制（控制发送速率）、三次握手四次挥手（同步状态）、拥塞控制（慢启动/拥塞避免/快重传/快恢复）。

# 75_problems__TCP和UDP三次握手和四次挥手状态及消息类型_md

## problems__TCP和UDP三次握手和四次挥手状态及消息类型.md

TCP三次握手：客户端SYN（SYN_SENT）→服务器SYN-ACK（SYN_RCVD）→客户端ACK（ESTABLISHED）。四次挥手：客户端FIN（FIN_WAIT_1）→服务器ACK（FIN_WAIT_2）→服务器FIN（LAST_ACK）→客户端ACK（TIME_WAIT，后释放）。UDP无连接，无握手挥手过程，直接封装数据报发送。

# 76_problems__TCP和UDP区别_md

## problems__TCP和UDP区别.md

TCP面向连接（需握手）、可靠交付（确认/重传/拥塞控制）、速度较慢、头部20字节（可变），适用网页/邮件等。UDP无连接、尽最大努力交付、速度快、头部8字节固定，适用视频/语音/直播等。TCP一对一，UDP支持一对多/多对多。

# 77_problems__TCP和UDP头部字节定义_md

## problems__TCP和UDP头部字节定义.md

TCP头部（20字节+选项）：源端口(16)、目的端口(16)、序列号(32)、确认号(32)、头部长度(4)、保留(6)、控制位(6：URG/ACK/PSH/RST/SYN/FIN)、窗口(16)、校验和(16)、紧急指针(16)、选项(可变)。UDP头部（8字节）：源端口(16)、目的端口(16)、长度(16)、校验和(16)。

# 78_problems__TCP和UDP对应常见的应用层协议有哪些_md

## problems__TCP和UDP对应常见的应用层协议有哪些？.md

TCP：HTTP、FTP、SMTP、Telnet。UDP：DNS、DHCP、SNMP、TFTP。

# 79_problems__TCP和UDP的区别_md

## problems__TCP和UDP的区别.md

TCP面向连接（需三次握手），UDP无连接即刻传输。TCP一对一，UDP支持一对多/多对多。TCP可靠（确认应答+重传），UDP不保证可靠。TCP有流量控制和拥塞控制，UDP不降速。TCP头部20字节+选项，UDP固定8字节。TCP基于字节流（有边界但保证顺序），UDP基于数据包（有边界，可能乱序丢包）。TCP在传输层分片（MSS），UDP在IP层分片（MTU，分片丢失需全部重传）。

# 80_problems__TCP和UDP的首部长什么样子_md

## problems__TCP和UDP的首部长什么样子？.md

TCP首部不使用选项20字节，使用选项可变；UDP首部固定8字节（源端口+目的端口+长度+校验和）。

# 81_problems__TCP的最大连接数是多少_md

## problems__TCP的最大连接数是多少？.md

无固定最大值，受操作系统限制（内核参数可调）、硬件资源（CPU/内存）、网络设备（防火墙/路由器）共同影响。

# 82_problems__TCP粘包是怎么产生的_md

## problems__TCP粘包是怎么产生的？.md

TCP是面向流协议，发送方数据分块发送，接收方可能一次性接收多数据包合并。原因：流式传输、接收方未及时读取缓冲区、网络延迟/拥塞、OS处理方式差异。避免：数据包加长度字段、使用分隔符、序列化/反序列化、合理设计应用层协议。

# 83_problems__UDP中使用connect的好处_md

## problems__UDP中使用connect的好处.md

可指定默认对等方（用send/recv替代sendto/recvfrom）、获取错误报告（目标不可达等ECONNREFUSED）、过滤接收数据包（只接受来自指定端点数据）、略微提高效率、简化状态管理、增强安全性。代价：套接字从多对等通信变为单一对等通信。

# 84_problems__WebSocket底层原理_md

## problems__WebSocket底层原理.md

客户端发HTTP请求带Upgrade和Connection头部→服务器返回101 Switching Protocols→建立持久连接。数据采用帧格式（FIN/Opcode/Mask等字段）。心跳机制维持连接稳定。断开时发送关闭帧。

# 85_problems__connect会阻塞怎么解决_md

## problems__connect会阻塞怎么解决.md

设置非阻塞套接字（O_NONBLOCK），connect返回EINPROGRESS，用select/poll/epoll监视写事件，通过getsockopt SO_ERROR判断是否成功。或用异步I/O库（libevent/libuv/Boost.Asio提供回调）。或创建辅助线程执行connect。

# 86_problems__epoll哪些触发模式_有啥区别_md

## problems__epoll哪些触发模式，有啥区别？.md

水平触发（LT）：只要FD有数据可读/写就一直通知，默认模式，编程简单。边缘触发（ET）：仅在FD状态变化时通知一次，需非阻塞I/O+循环读写到EAGAIN，效率更高但需小心处理避免丢数据。

# 87_problems__forward和redirect的区别是什么_md

## problems__forward和redirect的区别是什么？.md

Forward（转发）：服务器内部转发请求到另一资源，客户端不知，URL不变。Redirect（重定向）：服务器告诉客户端资源已移动（301/302），客户端重新请求新URL。

# 88_problems__ip地址和mac地址的区别都有哪些_md

## problems__ip地址和mac地址的区别都有哪些？.md

IP网络层逻辑寻址（跨网段路由），MAC数据链路层硬件寻址（局域网）。IP随网络环境变化，MAC固定绑定网卡。IP端到端传递，MAC仅当前链路有效。ARP将IP解析为MAC。跨网段时IP不变，每跳MAC重新生成。

# 89_problems__keepalive是什么_如何使用_md

## problems__keepalive是什么？如何使用？.md

Keepalive检测TCP连接是否活跃，定期发送探测包。默认不开启（避免额外流量）。// setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof(optval)) 启用。系统参数：tcp_keepalive_time（空闲时长，默认7200秒）、tcp_keepalive_probes（探测次数，默认9）、tcp_keepalive_intvl（间隔，默认75秒）。

##### select、poll和epoll的区别

select：轮询检查socket就绪，用户态内核态切换+全量数据结构拷贝，FD上限1024（位图存储）。poll：无FD数量上限（链表存储），事件类型更丰富，只关注就绪FD。epoll：Linux特有，红黑树管理，无最大并发限制，epoll_ctl注册FD、epoll_wait仅获取就绪FD，O(1)效率，适合高并发。

# 90_problems__time_wait_close_wait状态产生原因_keepalive_md

## problems__time_wait，close_wait状态产生原因，keepalive.md

TIME_WAIT：四次挥手最后阶段，发送最后一个ACK后进入，确保对方收到ACK（若对方重发FIN可重发ACK），避免旧数据包干扰新连接。持续2MSL（通常1-4分钟）。CLOSE_WAIT：收到对方FIN后发送ACK进入，等待应用程序关闭套接字。长时间停留CLOSE_WAIT表示应用未正确关闭socket。TCP Keepalive：可选心跳机制，空闲时发探测包检测对方是否活跃。默认每2小时开始探测。

# 91_problems__为什么DNS会使用UDP而不使用TCP呢_md

## problems__为什么DNS会使用UDP而不使用TCP呢？.md

DNS请求小，UDP无需建连更快、资源消耗少、简单轻量、广泛兼容。但响应超512字节时（DNSSEC、区域传输AXFR/IXFR）则使用TCP。

# 92_problems__为什么需要三次握手_两次不行_md

## problems__为什么需要三次握手，两次不行？.md

确认双方的接收与发送能力：第一次客户端发送能力OK，第二次服务器接收+发送能力OK，第三次客户端接收能力OK。防止失效连接请求延迟到达服务器导致资源浪费（两次握手时服务器收到过期SYN会建立无意义的连接等待数据）。保证可靠性协议要求。

# 93_problems__什么是IP地址_可以简单介绍下吗_md

## problems__什么是IP地址？可以简单介绍下吗？.md

IP地址唯一标识网络设备。IPv4：32位点分十进制（约42亿个），已枯竭。IPv6：128位冒号分隔十六进制（约340万亿亿亿亿个）。

# 94_problems__什么是mac地址_可以简单介绍下吗_md

## problems__什么是mac地址？可以简单介绍下吗？.md

MAC地址是数据链路层物理地址，48位二进制（6组十六进制数），与网卡硬件绑定固定不变，仅在局域网内有效。IP端到端路由，MAC局域网内直接通信。

# 95_problems__什么是大小端_他在计算机网络中都有什么应用呢_md

## problems__什么是大小端，他在计算机网络中都有什么应用呢.md

大端：高字节存低地址。小端：低字节存低地址（x86）。网络字节序统一为大端。应用程序需做字节序转换（htons/htonl等）。文件格式（如class文件、UTF-16）也可能规定特定字节序。

# 96_problems__什么是拆包和粘包_md

## problems__什么是拆包和粘包？.md

拆包：完整数据包被拆成多小包发送。粘包：多小包合并成一个包接收。原因：TCP面向流，发送方可任意分块，接收方可能一次读多包。避免：加长度字段、分隔符、序列化、应用层协议消息头+消息体格式。

# 97_problems__什么是数字签名_md

## problems__什么是数字签名？.md

数字签名验证数字信息完整性和来源。基于非对称加密：发送方用私钥加密消息哈希值→接收方用公钥解密哈希并与本地计算哈希比对→一致则完整且来源可信。广泛用于软件分发、电子邮件、电子商务等。

# 98_problems__什么是数字证书_md

## problems__什么是数字证书？.md

数字证书由CA签发，包含持有者信息、公钥、CA信息、有效期、序列号、CA数字签名。作用：认证身份、建立信任关系、数据加密、安全通信（TLS/SSL HTTPS）。浏览器自动检查证书有效性（CA可信度+域名匹配）。

# 99_problems__什么是滑动窗口_超时重传_md

## problems__什么是滑动窗口，超时重传.md

滑动窗口：TCP流量控制技术，发送窗口（可发数据量）和接收窗口（可接收数据量）动态调整，防止发送方过多数据导致接收方无法处理。超时重传：发送方发出数据包后启动定时器，超时未收到ACK则重传。超时时间根据RTT动态调整。


---


# 数据库与缓存

# 当前目录仅包含 README.md，正文为历史文档合并/知识点汇总。若出现旧文件名式标题，表示章节来源，不代表当前目录存在对应文件。

# problems__B+Tree和B树的区别.md

B+树因其读写性能稳定和叶子节点顺序链表特性，更适合用于数据库索引；B树则用于需要频繁更新数据的场景。

**共同点**：都是多路平衡查找树；节点可拥有多个子项；节点键值有序排列；搜索/插入/删除时间复杂度O(log n)。

**主要区别**：
1. **键和数据的存储**：B树每个节点包含键和数据（非叶子节点也能找到数据）；B+树所有数据在叶子节点（链表连接），非叶子节点只存键。
2. **叶子节点的性质**：B树叶子与内部节点相似；B+树叶节点通过指针串联成链表，方便范围扫描。
3. **空间利用率**：B+树内部节点不存数据，可存更多键使树更矮胖，减少IO；B树内部节点存数据，树可能更高。
4. **搜索效率**：B树可在找到匹配键时停止（任何层）；B+树必须走到叶子节点。
5. **复制和维护**：B+树更适合全库扫描；B树更新数据时修改更简单。

# problems__InnoDB和MyISAM的区别.md

1. **事务**：InnoDB支持事务（事务安全）；MyISAM不支持。
2. **锁**：InnoDB行级锁；MyISAM表级锁。
3. **索引**：InnoDB自5.6起支持全文索引，MyISAM也支持全文索引，两者实现细节和性能不同。
4. **适用场景**：MyISAM只读场景效率可能更高，适合小型应用；InnoDB支持事务和ACID，适合需要事务控制的场景。

# problems__MySQL的主从复制是如何实现的？.md

1. **二进制日志**：主服务器将数据更改记录在二进制日志中。
2. **配置主服务器**：启用二进制日志，配置唯一服务器ID。
3. **配置从服务器**：配置唯一服务器ID，连接主服务器；从服务器I/O线程将二进制日志事件复制到中继日志，SQL线程重放中继日志事件。
4. **数据复制过程**：主记录二进制日志→I/O线程获取更新写入中继日志→SQL线程应用中继日志事件。
5. **监控和维护**：可监控延迟、错误、日志大小等状态。

# problems__MySQL的行级锁有哪些种类？.md

1. **记录锁**：直接作用于索引记录的基本行级锁。
2. **间隙锁**：锁定范围但不包括记录本身，防止幻读。
3. **临键锁**：记录锁+间隙锁的组合，InnoDB默认行级锁形式。

# problems__RedisHash的原理和使用场景.md

#### Redis Hash 原理：
Redis Hash是键值对集合，内部通过哈希表和链地址法解决冲突；支持快速插入、查找、更新和删除字段。

#### Redis Hash 使用场景：
1. 存储对象属性（如用户信息、商品详情）
2. 缓存数据
3. 计数器（字段值可为整数）
4. 配置信息
5. 存储用户会话信息

# problems__RedisList的原理和使用场景.md

#### Redis List 原理：
底层使用quicklist（双向链表+ziplist/listpack的混合结构），支持两端插入删除；小列表用listpack压缩存储；允许重复元素。

#### Redis List 使用场景：
1. 消息队列（生产者推入尾部，消费者弹出头部）
2. 实时排行榜
3. 循环任务调度（BLPOP/BRPOP阻塞弹出）
4. 数据同步（记录变更日志）

# problems__RedisSet的原理和使用场景.md

#### Redis Set 原理：
无序不重复集合，内部使用哈希表实现；添加/删除/判断存在O(1)；支持交集、并集、差集运算。

#### Redis Set 使用场景：
1. 标签系统
2. 好友关系（共同好友）
3. 唯一性校验/去重
4. 点赞/收藏功能

# problems__RedisString原理和使用场景（分布式锁）.md

#### Redis String原理：
二进制安全动态字符串，可存储任意类型数据；支持设置值、获取值、追加、自增、自减等O(1)操作。

#### Redis String使用场景：
1. 缓存
2. 计数器
3. 分布式锁：`SETNX`实现互斥锁

# problems__RedisZSet的原理和使用场景（延迟队列）.md

#### Redis ZSet 原理：
有序集合，在Set基础上增加score字段用于排序；内部使用跳跃表（Skip List）+哈希表实现；支持按分数范围、排名等条件获取元素。

#### Redis ZSet 使用场景：
1. 排行榜（积分/热度）
2. 时间轴（时间戳排序）
3. 延迟队列（时间戳作为score，定时轮询获取到期任务）
4. 范围查询

# problems__Redis不是单线程吗，为什么会存在并发安全问题.md

Redis主线程单线程，但仍可能因竞态条件（多个客户端同时读写同一键）、非原子复合操作（先读后写中间被插入）等导致并发安全问题。解决方案：使用Redis事务（MULTI/EXEC/WATCH）保证原子性、使用乐观锁/悲观锁、分布式锁、应用层并发控制。

# problems__Redis中Stream的原理和使用场景.md

Redis Stream是基于日志结构的数据结构，消息按时间顺序存储（唯一ID+消息内容），支持不断添加、消费和实时流处理。使用场景：消息队列（支持发布-订阅和多播）、日志引擎、实时数据处理（消费者组）、定时任务调度。

# problems__Redis中数据（键值对）是怎么存储的.md

Redis使用全局哈希表（dict）存储所有键值对，键→哈希值→桶定位；支持多种数据类型（String/List/Set/ZSet/Hash），不同数据类型底层存储结构不同但大多基于哈希表。

# problems__Redis为什么这么快.md

1. 基于内存存储 2. 非阻塞IO（多路复用） 3. 单线程事件驱动模型（减少线程切换和同步开销） 4. 精简高效的数据结构 5. 数据分区和持久化 6. 内置命令优化 7. 高效网络通信（自定义轻量协议）

# problems__Redis主从复制、哨兵、集群的区别.md

1. **主从复制**：主节点数据复制到从节点，提供数据冗余和读写分离。
2. **哨兵（Sentinel）**：监控主从架构，主节点失效时自动将从节点升级为新主节点，保证高可用。
3. **集群（Cluster）**：数据分16384个哈希槽分片存储，支持水平扩展、自动故障转移和无中心化通信。

# problems__Redis内存淘汰策略.md

通过`maxmemory-policy`配置：1. **LRU**（最近最少使用） 2. **LFU**（最不经常使用） 3. **volatile-ttl**（优先淘汰即将过期的键） 4. **volatile-random / allkeys-random**（随机淘汰） 5. **noeviction**（拒绝写入，默认策略）。`volatile-*`只淘汰设置了过期时间的键，`allkeys-*`对所有键生效。

# problems__Redis单线程在多核机器里使用会不会浪费机器资源.md

单个Redis实例只能利用一个CPU核心，可能浪费多核资源。优化方式：多实例部署（每个绑定不同核心）、集群模式横向扩展、Redis 6.0+多线程I/O（仅网络I/O和协议解析并行，命令执行仍单线程）、结合其他技术外移计算/IO密集型任务。

# problems__Redis和Memcached的区别.md

1. **数据结构**：Redis丰富（String/List/Set/Hash/ZSet等）；Memcached仅key-value。
2. **持久化**：Redis支持RDB/AOF；Memcached不支持持久化。
3. **数据淘汰**：Redis支持多种策略（LRU/LFU/TTL等）；Memcached仅LRU。
4. **分布式**：Redis通过集群实现；Memcached天生分布式但无内置分片和故障转移。
5. **性能**：Memcached简单读写略优；Redis复杂数据结构和计算密集型任务更优。

# problems__Redis如何判断键是否过期？过期键的删除策略有哪些.md

Redis通过**惰性删除**（访问时检查并删除过期键）和**定期主动过期**（后台周期性抽样检查）管理过期键，不为每个key创建定时器。内存淘汰策略（LRU/LFU等）是独立机制，在内存压力下触发。

# problems__Redis数据类型（对象）有哪些.md

五种主要数据类型：String（字符串）、List（列表）、Set（集合）、Hash（哈希）、Sorted Set/ZSet（有序集合）。其他类型：HyperLogLog（基数统计）、GeoSpatial（地理位置）、Bitmaps（位图）。

# problems__Redis是单线程还是多线程？Redis6.0之后为何又引入了多线程.md

Redis核心命令执行是单线程事件循环模型；Redis 6.0引入多线程I/O仅用于网络I/O读写和协议解析，命令执行仍单线程。目的：利用多核CPU、处理I/O密集型操作、降低阻塞风险。

# problems__Redis有什么作用？为什么要用 Redis.md

Redis作为开源内存数据库，主要优势：快速（内存读写）、丰富数据结构（String/List/Set/Hash/ZSet）、持久化（RDB/AOF）、高并发（原子性操作）、分布式（主从复制/集群）。

# problems__Redis的BigKey问题及其解决方案.md

BigKey指占用大量内存的key，会导致内存过高、性能下降甚至宕机。解决方案：监控排查（`MEMORY USAGE`命令）、拆分大key为小key、使用Hash数据结构、数据压缩（如GZIP，注意CPU负载）、设置过期时间、持久化不常访问的数据到磁盘、应用层限制数据大小。

# problems__Redis的Bitmap的原理和使用场景.md

Bitmap用位表示状态，底层通过字符串实现（一个字节8个位），通过位操作实现设置、清除和查询。使用场景：状态标记（签到/在线）、去重统计、集合运算（交集/并集/差集）、压缩存储布尔值。

# problems__Redis的HyperLogLog的原理和使用场景.md

HyperLogLog是基数估计算法，使用固定小空间估算大数据集不重复元素数量（概率统计方法，有一定误差）。使用场景：统计UV（独立访客）、数据去重、实时计数。

# problems__Redis的key设定24h过期时间，那么24h后就一定会过期吗.md

不一定。Redis过期键删除策略（惰性删除+定期删除）不会为每个key创建定时器，过期后可能存在延迟，不能保证在精确时间点立即删除。

# problems__Redis的两种持久化方式以及优缺点.md

1. **RDB**：定期快照到磁盘。优点：快速高效、文件小、对性能影响小；缺点：可能丢失最后一次快照后的数据。
2. **AOF**：追加写操作日志。优点：数据完整性高；缺点：占空间大、恢复慢、需定期重写压缩。

# problems__Redis的底层数据结构有哪些.md

1. **SDS**（动态字符串，二进制安全，预分配空间） 2. **ziplist**（压缩列表） 3. **listpack**（ziplist升级版，7.0+，解决级联更新） 4. **intset**（整数集合，自动升级编码） 5. **skiplist**（跳表，ZSet有序性） 6. **quicklist**（双向链表+ziplist/listpack混合，List底层） 7. **dict**（哈希表，链地址法+渐进式rehash）

# problems__Redis缓存击穿问题及其解决方案.md

缓存击穿指热点key失效后大量并发请求穿透到数据库。解决方案：热点数据预加载、互斥锁/分布式锁控制回源、逻辑过期+异步重建、熔断策略、一致性哈希分散请求、提升缓存容量。

# problems__Redis缓存穿透问题及其解决方案.md

缓存穿透指请求的key在缓存和数据库中都不存在，每次请求都直接访问数据库。解决方案：布隆过滤器预过滤、空值缓存（短过期时间）、参数校验/限流、缓存标记。

# problems__Redis缓存雪崩问题及其解决方案.md

缓存雪崩指大量缓存同时失效导致请求直达数据库。解决方案：过期时间加随机值分散、二级缓存（本地缓存）策略、缓存数据自动刷新、限流熔断降级、监控报警系统。

# problems__mysql为啥会产生死锁呢？如何避免他？.md

死锁四个必要条件：互斥、占有且等待、非强制性释放、循环等待。避免方法：有序资源分配、超时机制回滚、InnoDB自动死锁检测与回滚、尽量使用行级锁、减少事务大小、避免不必要锁定、使用低隔离级别、优化查询逻辑。

# problems__MySQL数据库中，产生的redo log都会直接写入磁盘吗？.md

并非直接写入。Redo Log分内存日志缓冲区和磁盘日志文件。`innodb_flush_log_at_trx_commit`参数控制刷盘：0（每秒刷一次）、1（每次提交刷盘，最高持久性）、2（每次提交写入OS缓冲，由OS决定刷盘）。

# problems__mysql架构是什么样的？.md

MySQL架构分5层：1. 客户端连接层（连接处理、授权认证）；2. 查询解析和优化器层（SQL解析、预处理、优化）；3. 存储引擎层（插件式，如InnoDB/MyISAM）；4. 文件系统（存储表数据和索引）。

# problems__mysql的索引都有哪些？.md

1. **B+Tree索引**（最常见，支持全键值/范围/排序） 2. **哈希索引**（仅等值匹配，Memory引擎默认） 3. **FULLTEXT全文索引**（文本搜索，CHAR/VARCHAR/TEXT列）

# problems__mysql索引失效有哪几种情况？.md

1. 使用`!=`或`<>`操作符 2. 对索引列进行计算或函数操作 3. 隐式类型转换 4. 联合索引未使用最左前缀原则 5. `LIKE`以`%`开头 6. NULL值问题 7. 数据分布不均匀（如性别字段）

# problems__union和join.md

**UNION**：垂直合并两个或多个SELECT结果集（去重），`UNION ALL`保留重复行。**JOIN**：水平合并不同表中相关联的行，包括INNER/LEFT/RIGHT JOIN，通过ON指定连接条件。

# problems__一条SQL语句在数据库框架中的执行过程.md

连接（身份验证）→ 解析（语法语义检查）→ 预处理（表和列存在性检查）→ 优化（选择成本最低的执行计划）→ 执行（读取/修改数据）→ 返回结果。

# problems__为什么AOF后台重写和BGSAVE命令都用子进程而不是线程？.md

原因：数据隔离（独立地址空间）、稳定性（子进程崩溃不影响父进程）、并发处理（更好利用多核）、资源管理（独立资源空间，更易控制）。

# problems__为什么Redis使用跳表而不是红黑树来实现Zset.md

1. 实现简单直观，无需复杂平衡调整 2. 实际性能更稳定（无需旋转和颜色调整） 3. 空间占用更少（无需维护颜色信息） 4. 天然支持并发访问

# problems__为什么Zset需要同时使用跳表和字典来实现？.md

跳表保证元素按score有序存储并支持范围查询；字典提供O(1)快速查找和元素唯一性保证。两者结合在内存占用和性能间取得平衡。

# problems__什么是一致性哈希，Redis集群为什么不用一致性哈希.md

一致性哈希将数据映射到圆环上寻找最近节点，支持动态扩展和平滑迁移。Redis集群采用16384个哈希槽分片（更简化数据定位），内置故障检测和自动迁移功能，无中心化通信。

# problems__介绍 Redis的skiplist.md

Redis skiplist通过多级索引实现跳跃查找，平均O(log n)的插入、删除和查找时间复杂度；实现简单，为ZSet提供高效的有序集合操作。

# problems__介绍Redis事务（Redis能实现ACID吗）.md

Redis通过MULTI/EXEC/DISCARD/WATCH命令实现事务：命令排队按顺序执行。但不支持回滚（运行时错误不回滚已执行的命令），不严格满足ACID。

# problems__介绍SDSRedis为什么要使用SDS而不是C字符串.md

SDS（Simple Dynamic Strings）优势：动态扩容（自动分配内存）、O(1)获取长度、二进制安全（不受'\0'限制）、兼容部分C字符串函数。

# problems__介绍dict，什么是rehash？什么是渐进式rehash.md

dict是Redis的哈希表结构。rehash指扩容或缩容时重建哈希表。渐进式rehash将rehash分解为多个小步骤，每次处理部分键值对，避免一次性阻塞服务。

# problems__介绍intset及其升级过程，支持降级吗.md

intset是整数集合，使用int16/int32/int64编码。升级：插入更大类型元素时重新分配更大intset并复制所有元素。不支持降级（降级可能导致精度丢失）。

# problems__介绍ziplist，什么是连锁更新？quicklist、listpack.md

ziplist是紧凑数据结构，连续存储元素以节省内存。连锁更新：插入时某个条目长度变化导致后续prevlen字段连续更新，可能带来较大性能开销。quicklist：多个ziplist组成的双向链表。listpack：ziplist升级替代，解决级联更新问题。

# problems__优化索引的办法有哪些.md

1. 仅为WHERE/ORDER BY/GROUP BY中的列创建索引 2. 选择最适合的索引类型 3. 复合索引注意列顺序（最左前缀原则） 4. 保持索引尽量小（前缀索引） 5. 定期维护重建索引 6. 监控并清理无用索引 7. 谨慎对更新频繁的列建索引 8. 适当的数据库设计（反范式化）

# problems__关系型数据库和非关系数据库的特点.md

**关系型数据库**：SQL、表格结构、数据完整性、ACID事务、规范化。适用于金融、会计等需要严格数据完整性的场景。**非关系型数据库**：灵活数据模型、水平可扩展、多样化类型（键值/文档/列族/图）、最终一致性、高吞吐量。适用于社交网络、大规模在线服务、实时分析。

# problems__在MySQL中，数据要写入磁盘，redo log也要写入磁盘，为什么要多此一举？.md

Redo log提供数据恢复能力——系统崩溃时恢复未写入磁盘的数据页。Redo log追加写入，可高效连续写入磁盘，减少磁盘压力。

# problems__如何解决Redis集群数据丢失问题（异步复制、集群脑裂）.md

1. 同步复制（减少丢失，增加写入延迟） 2. 持久化策略（RDB/AOF/混合） 3. 监控和警报系统 4. 合理配置集群参数 5. 使用哨兵系统监控和自动故障转移以规避脑裂

# problems__如何解决缓存和数据库一致性问题.md

1. 读写分离 2. 空值缓存处理穿透 3. 双写策略（同时更新DB和缓存） 4. 主动/定时刷新缓存 5. 缓存失效（数据变更时失效） 6. 版本控制 7. 缓存锁

# problems__数据库中乐观锁与悲观锁的区别.md

**悲观锁**：假设冲突常见，读写前先加锁，保证一致性但降低并发。适用于高冲突环境。**乐观锁**：假设冲突较少，提交时通过版本号检测冲突。适用于读多写少环境，冲突时需重试。

# problems__数据库中常见锁都有哪些.md

1. **共享锁（S锁）**：允许读，禁止写，可多个事务同时加S锁 2. **排他锁（X锁）**：允许写，其他事务不能加任何锁 3. **意向锁**：表级锁，表示对某些行加锁的意图（IS/IX/SIX） 4. **更新锁**：防止死锁 5. **记录锁**：锁住索引记录 6. **间隙锁**：锁定索引记录间的范围，防止幻读 7. **临键锁**：记录锁+间隙锁组合

# problems__数据库事务隔离级别有哪些？.md

1. **读未提交**：看到未提交修改，导致脏读/不可重复读/幻读 2. **读已提交**：只能看到已提交修改，避免脏读 3. **可重复读**：同一事务多次读取结果一致，避免脏读和不可重复读，仍可能幻读 4. **序列化**：最高级别，每行加锁，避免所有问题，性能开销最大

# problems__数据库日志类型作用.md

1. **事务日志**：记录所有修改操作，保证持久性和崩溃恢复 2. **审计日志**：记录操作历史用于安全合规 3. **错误日志**：记录运行错误用于故障排除 4. **二进制日志**（MySQL）：记录数据更改事件，用于复制和数据恢复 5. **归档日志**（Oracle）：用于时间点恢复

# problems__数据库的ACID特性.md

1. **原子性**：事务操作全部完成或全部回滚 2. **一致性**：事务前后数据库完整性约束不被破坏 3. **隔离性**：并发事务互不干扰，通过隔离级别控制 4. **持久性**：提交后更改永久保存，通过Redo Log保证

# problems__数据库范式第一第二第三范式.md

**第一范式（1NF）**：字段值不可分割原子值，每列唯一定义，每行有唯一标识（主键）。**第二范式（2NF）**：满足1NF，消除非主属性对候选键的部分函数依赖。**第三范式（3NF）**：满足2NF，消除非主属性对候选键的传递函数依赖。

# problems__检查手机号是否存在于百万数据电话号中.md

使用哈希表（O(1)查找）或数据库索引提高效率。数据量极大时可用分布式计算框架（如Hadoop/Spark）进行数据分片和并行计算。

# problems__现在普通关系数据库用得数据结构是什么类型的数据结构.md

关系数据库通常使用B+树实现索引，因为：平衡性（所有叶子同层）、高分支因子（减少树高和磁盘IO）、磁盘友好（节点大小匹配磁盘块）、叶子节点顺序连接（高效范围查询）、空间利用率高。

# problems__索引的优点和缺点.md

**优点**：提高查询速度、加速排序、保证唯一性、更有效的数据访问计划、加速COUNT/MIN/MAX。**缺点**：增加写操作成本、占用额外存储空间、需维护（碎片整理）、设计不当损害性能、可能误导优化器。

# problems__讲一讲Redis主从复制.md

主节点将数据复制到从节点：从节点发送SYNC命令→主节点全量发送数据集→后续增量复制（命令流）。功能：读写分离、故障恢复、数据备份、横向扩展读能力。

# problems__讲一讲Redis的哨兵.md

哨兵（Sentinel）监控Redis主从集群，主节点故障时自动故障转移（投票选举新主节点+通知从节点切换）。功能：监控、故障检测、自动故障转移、配置维护、集群管理、自动恢复（原主恢复后作为从节点接入）。

# problems__讲一讲Redis的集群.md

Redis集群将数据分16384个哈希槽均匀分布在多个节点上，节点通过集群总线通信；每个主节点有从节点备份；客户端根据槽位路由；支持自动故障转移、动态扩容缩容、负载均衡。

# problems__谈谈数据库中索引的理解，索引和主键区别.md

索引是提高查询速度的数据结构（类似目录）。作用：提升查询效率、保持数据排序、唯一性检验。**主键**：唯一标识记录，一个表只能一个，不能为NULL，自动创建聚簇索引。**索引**：可多个，可唯一或非唯一，不改变物理存储（除非聚簇索引）。

# problems__锁：互斥锁，乐观锁，悲观锁.md

**互斥锁**：保证同一时刻只有一个线程访问共享资源。**乐观锁**：操作前不加锁，提交时通过版本号检测冲突，适合读多写少。**悲观锁**：每次读写前先加锁，适合高冲突场景。

---

# problems__MySQL-online-DDL详解.md

### MySQL online DDL 是什么？有哪些算法？

MySQL 5.6+支持online DDL，通过`ALGORITHM`子句指定实现方式：

| 算法 | 原理 | 是否阻塞读写 | 适用场景 |
|------|------|-------------|---------|
| INSTANT (8.0+) | 仅修改数据字典元数据 | 不阻塞 | 添加列、修改默认值 |
| INPLACE | 在原表上重建 | 不阻塞（短暂MDL） | 添加/删除索引、修改列类型 |
| COPY | 创建新表+复制数据+重命名 | 阻塞写入 | 不支持INPLACE的操作 |

// ALTER TABLE t ADD COLUMN c INT, ALGORITHM=INPLACE, LOCK=NONE; -- 不阻塞读写

**并发控制三阶段**：Prepare（获取MDL）→ Execute（执行DDL，DML可正常进行）→ Commit（重放日志，提交元数据变更）。

---

# problems__MySQL-EXPLAIN执行计划详解.md

### MySQL EXPLAIN 执行计划怎么看？如何判断索引是否合理？

EXPLAIN关键字段：id（查询序号）、select_type（查询类型）、type（访问类型，最重要）、key（实际使用索引）、rows（预估扫描行数）、Extra（额外信息）。

**type从好到差**：const（主键等值）→ eq_ref（JOIN主键关联）→ ref（非唯一索引等值）→ range（索引范围扫描）→ index（全索引扫描）→ ALL（全表扫，必须优化）

**Extra关键信号**：Using index（覆盖索引✅）、Using filesort（需额外排序⚠️）、Using temporary（使用临时表❌）、Using join buffer（JOIN未走索引❌）。

// 判断索引是否合理：type为ref或以上、key走预期索引、Extra无filesort/temporary，通常是合理的。


---


# 数据结构与算法

# 数据结构与算法历史文档汇总

> 当前目录仅包含 README.md，正文为历史文档合并/知识点汇总；若出现旧文件名式标题，表示章节来源，不代表当前目录存在对应文件。

### 棋盘覆盖问题

#### 代码
棋盘覆盖问题代码

#### 问题说明
在一个 2^k × 2^k 棋盘中，恰有一个特殊方格，需用 4 种 L 型骨牌覆盖所有非特殊方格且不重叠。

#### 功能说明
本程序用分治法解决棋盘覆盖问题，显示输出。

#### 代码简述
// 分治法：将棋盘分成四份递归求解，计算结果存储到 board 二维数组中并显示输出。

# Problems__KnapsackProblem__README.md

### 背包问题

#### 代码
背包问题代码

#### 问题说明
有 N 件物品和一个容量为 V 的背包，每件物品有重量 w[i] 和价值 v[i]，求价值总和最大的装入方案。

#### 功能说明
本程序用动态规划（迭代法和递归法）求解背包问题，迭代法中实现了打印背包问题的表格。

#### 代码简述
// 01背包 DP：dp[i][w] = max(dp[i-1][w], dp[i-1][w-wt[i]] + val[i])
迭代法遍历 n 行 W 列逐行求解；递归法通过返回前 i 个物品和承重 j 的最优解递归计算。

# Problems__NeumannNeighborProblem__Formula__README.md

### 冯诺依曼邻居问题

#### 问题说明
从一个 1×1 方格开始，每次在上次图形周围再加一圈方格，求第 n 次生成的方格数。

#### 解法一：通项公式
递推：a(n) = a(n-1) + 4n，化简得 a(n) = 2n² + 2n + 1。

#### 解法二：递推关系
递推公式 a(n) = a(n-1) + 4n，a(0) = 1，可直接递归或迭代实现。

# Problems__NeumannNeighborProblem__README.md
> 原文内容已合并至上方「冯诺依曼邻居问题」统一条目。

# Problems__NeumannNeighborProblem__Recursive__README.md
> 原文内容已合并至上方「冯诺依曼邻居问题」统一条目。

# Problems__RoundRobinProblem__README.md

### 循环赛日程安排问题

#### 代码
循环赛日程安排问题代码

#### 问题说明
设有 n=2^k 个选手进行网球循环赛，要求每个选手与其他 n-1 个选手各赛一次且每天只赛一次，用 n 行 n-1 列的二维表表示日程。

#### 功能说明
本程序运用分治思想实现循环赛日程安排问题，生成并输出日程表。

#### 代码简述
// 分治：先安排第一行，然后分若干份填充，最终生成日程表。

# Problems__TubingProblem__README.md

### 输油管道问题

#### 代码
输油管道问题代码

#### 问题说明
给定 n 口油井的坐标，求一条东西向主管道位置，使各油井到主管道的南北向输油管道长度总和最小。

#### 功能说明
本程序用排序求中值的方法求解输油管道问题。

#### 代码简述
用户输入油井数 n 和每个油井的 y 坐标，排序（快速排序）后求中值：n 为奇数时最优位置为第 n/2 个 y 坐标，n 为偶数时为中间两个 y 坐标的区间。

# problems__BloomFilter原理.md

BloomFilter（布隆过滤器）是一种概率数据结构，用固定大小位数组和多个哈希函数判断元素是否属于集合。添加时将哈希值对应位置 1；查询时若所有对应位为 1 则可能存在（有误判），若有 0 则一定不存在（无漏判）。误判率与位数组大小、元素数量和哈希函数数量有关。优点：空间效率高、查询/插入快。缺点：存在误判、无法删除元素、不能获取元素具体值。

# problems__Map-Reduce原理.md

Map-Reduce 是 Google 提出的并行计算框架。Map 阶段将输入分割为子问题并行处理，输出中间键值对；Shuffle 阶段按 key 分组排序；Reduce 阶段对每个 key 的值列表合并聚合。优点：可扩展至 PB 级数据、自动容错、只需编写 Map/Reduce 函数、适用数据分析/机器学习等。

# problems__Trie树原理.md

Trie 树（前缀树）利用字符串公共前缀节省存储和加速查询。每个节点表示一个字符，含布尔标志和子节点指针。插入：沿字符遍历，子节点不存在则创建，末尾标志置 true。查询：沿字符遍历，子节点不存在或末尾标志为 false 则不存在。前缀查找：沿前缀遍历后 DFS/BFS 收集。优点：查询 O(len)、高效前缀查找、节省空间。缺点：节点和边导致空间占用较大、插入删除较复杂、大字符集下更显著。

# problems__bitmap.md

Bitmap 以位为单位表示数据，适用于海量数据的查询、去重和压缩。每个数据项用一个 bit 表示状态，常见操作：set/clear/test，可用 C++ bitset、vector<bool> 或自定义位操作实现。去重判重 O(1)，可配合压缩算法，适合实时性要求高的场景。

# problems__二叉树和链表的区别.md

1. 结构：链表是线性结构每个节点指向下一个；二叉树是树状结构每个节点最多两个子节点。
2. 存储方式：链表非连续分布通过指针连接；二叉树可链式或数组存储。
3. 操作效率：链表插入删除 O(1)（已知位置），查找 O(n)；平衡二叉树插入查找删除 O(log n)。
4. 应用场景：链表适合频繁插入删除；二叉树适合搜索排序。

# problems__什么是二叉树、二叉搜索树、平衡二叉树、完全二叉树、满二叉树.md

1. 二叉搜索树：左子树所有节点值小于根，右子树大于根，中序遍历有序。
2. 平衡二叉树（AVL）：左右子树高度差不超过 1，插入删除时自平衡调整。
3. 完全二叉树：除最后一层外全满，最后一层节点从左向右排列，常用数组存储。
4. 满二叉树（Perfect Binary Tree）：除叶子节点外每个节点有两个子节点，所有叶子在同一层，每层节点数达最大值。注意区分中文"满二叉树"（Perfect）与"真二叉树/严格二叉树"（Full：每个节点有 0 或 2 个子节点）。

# problems__什么是哈夫曼树？构造过程？应用场景.md

哈夫曼树是带权路径长度最短的二叉树。构造：按权值升序排序，每次取最小两个合并为新节点，重复至只剩根节点。哈夫曼编码（频率高编码短）用于文本/图像/音频等无损压缩及网络传输。

# problems__什么是堆？如何维护堆.md

堆是树形数据结构，用于实现优先队列。最大堆：父节点 ≥ 子节点；最小堆：父节点 ≤ 子节点。插入：新元素放末尾后上浮（sift-up）。删除堆顶：与末尾交换后删除末尾，新堆顶下沉（sift-down）。时间复杂度 O(log n)。

# problems__什么是红黑树？红黑树与平衡二叉树、B和B+树的区别.md

红黑树是一种自平衡二叉查找树：节点红或黑；根和叶子（NIL）为黑；红节点子节点必为黑；任一节点到叶子路径的黑色节点数相同。通过着色和旋转维持平衡，最坏情况 O(log n)。与 AVL 比红黑树更灵活；与 B/B+ 树比，B/B+ 树是多路搜索树适合磁盘，红黑树更适用于内存。

# problems__什么是缓冲区溢出？.md

缓冲区溢出因未做边界检查或不安全函数导致，攻击者可执行恶意代码或获取权限。后果：程序崩溃、安全漏洞、信息泄露。防范：做边界检查、使用 strncpy() 等安全函数、避免固定大小缓冲区。

# problems__什么是跳表？跳表和平衡二叉树的区别.md

跳表是基于并行链表的数据结构，通过多层索引加速有序链表查找，时间复杂度 O(log n)。与平衡二叉树区别：结构不同（链表 vs 树）；跳表用多级索引无需旋转，插入删除更简单；跳表节点含多层 forward 指针消耗更多内存；跳表每层独立有序链表更易并行。

# problems__内存有限，如何在100亿数据中找到中位数.md

分块 + 中位数选择：数据分块排序，取每块中位数组成数组，用快速选择找出中位数，据此二分原数据递归至找到全局中位数。

# problems__内存有限，如何在20亿个整数中找到出现次数最多的数.md

外部排序 + 哈希计数：数据分文件排序，遍历每个文件用哈希表计数字次数，合并后取最大值。

# problems__内存有限，如何在2亿个整数中找出不连续的最小数.md

位图法：遍历整数在位图标记，从最小整数开始找到第一个未被标记的位置。

# problems__内存有限，如何在40亿个非负整数中找到所有未出现的数.md

分块算法：将整数划分为多个块，每块排序后记录范围，遍历所有块计算未出现的数字。可结合 BitMap 优化。

# problems__内存有限，怎么对100亿数据进行排序（大数据小内存排序问题）.md

外部排序：数据分块，每块内存内排序后写入磁盘，再用多路归并合并为完整有序数据集。

# problems__如何判断图中是否有环（拓扑排序）.md

统计每个顶点入度，将入度为 0 的顶点入队列，依次取出并将其邻接点入度减 1，入度变为 0 则入队，重复至队列空。

# problems__如何判断某网页的URL是否存在于包含100亿条数据的黑名单上（大数据小内存去重问题）.md

使用布隆过滤器：初始化位数组，将黑名单 URL 经多个哈希函数映射置位；查询时计算哈希检查对应位是否全为 1。有假阳性率但无假阴性。

# problems__常见排序算法及其时间复杂度、各种排序算法对比.md

1. 冒泡排序：平均/最坏 O(n²)，最好 O(n)。
2. 选择排序：平均/最好/最坏 O(n²)。
3. 插入排序：平均/最坏 O(n²)，最好 O(n)。
4. 快速排序：平均/最好 O(nlogn)，最坏 O(n²)（已有序时）。
5. 归并排序：O(nlogn)。
6. 堆排序：O(nlogn)。

# problems__循环队列怎么实现.md

用数组和 front/rear 指针实现。初始化 front = rear = 0。入队：(rear+1)%len == front 则满，否则插入后 rear = (rear+1)%len。出队：front == rear 则空，否则取出 front 元素后 front = (front+1)%len。判空：front == rear。判满：(rear+1)%len == front。

# problems__数组和链表的区别、适用场景.md

数组：连续存储、固定大小、随机访问 O(1)、插入删除 O(n)。链表：非连续动态大小、插入删除 O(1)、随机访问 O(n)（需遍历）。数组适合固定大小随机访问场景；链表适合频繁插入删除或元素个数变化大的场景。

# problems__时间复杂度和空间复杂度的定义？时间换空间&空间换时间的例子有哪些.md

时间复杂度是大 O 表示的算法执行步骤与输入规模的关系；空间复杂度是算法所需额外存储空间与输入规模的关系。空间换时间例子：缓存技术、归并/快速排序（需额外空间）、动态规划保存中间结果、哈希表。


---


# 02_算法刷题题解历史文档汇总

## 算法刷题题解历史文档汇总

> 当前目录仅包含 README.md，正文为历史来源内容的合并/汇总；文中出现的 sword_for_offer、selected_coding_interview、leetbook_ioa 等旧文件名/子目录名仅表示内容来源，不代表当前目录存在对应子目录。

《图解算法数据结构》是一本面向算法初学者和互联网求职者编写的 LeetBook 手册。图文详解 75 道题目，覆盖主要算法知识点。

> 本目录内容来自历史文档合并，仅作来源说明：
> - sword_for_offer：历史来源《剑指 Offer》题目解析、配套代码、刷题计划
> - selected_coding_interview：历史来源《Krahets 笔面试精选 88 题》题目解析、配套代码
> - leetbook_ioa：历史来源《图解算法数据结构》题目解析

> 若本仓库对您有所帮助，请在页面右上角点个 **Star** 支持一下，谢谢！

#### 如何学习算法

##### 第一步：看入门书
《Hello 算法》—— 动画图解、一键运行的数据结构与算法教程。此书旨在引导初学者探索数据结构与算法的知识地图。

> 建议先读完这本书，再开始刷题。

##### 第二步：刷算法题
推荐以下 LeetCode 题单：
1. Krahets 笔面试精选 88 题
2. LeetCode 热题 100
3. 面试经典 150 题
4. LeetCode-75

对初学者刷题的一些建议：
1. 建议每日刷 2～3 题
2. 质量比数量更重要
3. 按照题单顺序做题
4. 注意比较多种解法
5. 需要复习三次以上才能掌握
6. 坚持至关重要

#### License
The texts, code, and images in this repository are licensed under CC BY-NC-SA-4.0.
# 03_刷leetcode究竟要不要使用库函数

### 刷leetcode究竟要不要使用库函数

如果题目关键的部分直接用库函数就可以解决，建议不要使用库函数。如果库函数仅仅是解题过程中的一小部分，并且你已经很清楚这个库函数的内部实现原理的话，可以考虑使用库函数。

> 章节来源：0 引言
# 04_引言

### 引言

> 以下内容来自《图解算法数据结构》(LeetBook)，包含多语言解法与详细讲解。

《图解算法数据结构》面向算法初学者、互联网求职者设计，主要内容包括：
- 图文详解 75 道题目，覆盖主要算法知识点
- 数据结构与算法专栏：基础知识、数据结构、算法专题

#### 配套代码
仓库链接：https://github.com/krahets/LeetCode-Book

> 章节来源：0.1 刷题建议
# 05_刷题建议

### 刷题建议

1. 建议每日刷 2~3 题
2. 按照目录顺序逐题解答
3. 比较和探讨各种方法的特点
4. 通常需要复习至少3次
5. 坚持至关重要

> 章节来源：0.2 题目分类
# 06_题目分类

### 题目分类

| 题目 | 算法分类 | 数据结构分类 |
| --- | --- | --- |
| 寻找文件副本 | 查找 | 数组 |
| 寻找目标值 - 二维数组 | 查找 | 数组 |
| 路径加密 | | 字符串 |
| 图书整理 I | | 栈与队列，链表 |
| 推理二叉树 | 分治 | 树，哈希表 |
| 图书整理 II | | 栈与队列 |
| 斐波那契数 | 动态规划 | 数组 |
| 跳跃训练 | 动态规划 | 数组 |
| 库存管理 I | 查找 | 数组 |
| 字母迷宫 | 回溯，搜索 | 数组，图 |
| 衣橱整理 | 回溯，搜索 | 数组，图 |
| 砍竹子 I | 贪心，数学 | |
| 砍竹子 II | 贪心，分治，数学 | |
| 位 1 的个数 | 位运算 | |
| Pow(x, n) | 分治，位运算 | |
| 报数 | | 数组 |
| 删除链表节点 | 双指针 | 链表 |
| 模糊搜索验证 | 动态规划 | 字符串 |
| 有效数字 | | 字符串 |
| 训练计划 I | 双指针 | 数组 |
| 训练计划 II | 双指针 | 链表 |
| 训练计划 III | 双指针 | 链表 |
| 训练计划 IV | 双指针 | 链表 |
| 子结构判断 | 搜索 | 树 |
| 翻转二叉树 | 搜索 | 栈与队列，树 |
| 判断对称二叉树 | 搜索 | 树 |
| 螺旋遍历二维数组 | 模拟 | 数组 |
| 最小栈 | 排序 | 栈与队列 |
| 验证图书取出顺序 | 模拟 | 栈与队列 |
| 彩灯装饰记录 I | 搜索 | 栈与队列，树 |
| 彩灯装饰记录 II | 搜索 | 栈与队列，树 |
| 彩灯装饰记录 III | 搜索 | 栈与队列，树 |
| 验证二叉搜索树的后序遍历序列 | 分治 | 栈与队列，树 |
| 二叉树中和为目标值的路径 | 回溯，搜索 | 树 |
| 随机链表的复制 | | 链表 |
| 将二叉搜索树转化为排序的双向链表 | 搜索，双指针 | 树 |
| 序列化与反序列化二叉树 | 搜索 | 树 |
| 套餐内商品的排列顺序 | 回溯 | 字符串，哈希表 |
| 库存管理 II | | 数组 |
| 库存管理 III | 排序 | 数组，堆 |
| 数据流中的中位数 | 排序 | 堆 |
| 连续天数的最高销售额 | 动态规划 | 数组 |
| 数字 1 的个数 | 数学 | |
| 找到第 k 位数字 | 数学 | |
| 破解闯关密码 | 排序 | 字符串 |
| 解密数字 | 动态规划 | 字符串 |
| 珠宝的最高价值 | 动态规划 | 数组 |
| 招式拆解 I | 动态规划，双指针 | 哈希表 |
| 丑数 | 动态规划 | |
| 招式拆解 II | | 哈希表 |
| 交易逆序对的总数 | 分治 | 数组 |
| 训练计划 V | 双指针 | 链表 |
| 统计目标成绩的出现次数 | 查找 | 数组 |
| 点名 | 查找 | 数组 |
| 寻找二叉搜索树中的目标节点 | 搜索 | 树 |
| 计算二叉树的深度 | 搜索 | 树 |
| 判断是否为平衡二叉树 | 搜索 | 树 |
| 撞色搭配 | 位运算 | 数组 |
| 训练计划 VI | 位运算 | 数组 |
| 查找总价格为目标值的两个商品 | 双指针 | 数组 |
| 文件组合 | 双指针 | 数组 |
| 字符串中的单词反转 | 双指针 | 字符串 |
| 动态口令 | | 字符串 |
| 望远镜中最高的海拔 | 排序 | 数组，栈与队列 |
| 设计自助结算系统 | 排序 | 数组，栈与队列 |
| 统计结果概率 | 动态规划 | |
| 文物朝代判断 | 排序 | 数组，哈希表 |
| 破冰游戏 | 数学 | |
| 买卖芯片的最佳时机 | 动态规划 | 数组 |
| 设计机械累加器 | | |
| 加密运算 | 位运算 | |
| 按规则计算统计结果 | 数学 | 数组 |
| 不使用库函数的字符串转整数 | | 字符串 |
| 求二叉搜索树的最近公共祖先 | 搜索 | 树 |
| 寻找二叉树的最近公共祖先 | 搜索 | 树 |

> 数据结构简介、算法复杂度（时间/空间复杂度）相关内容已移至《数据结构与算法》目录，此处不再重复。

> 章节来源：11.1 动态规划解题框架
# 07_动态规划解题框架

### 动态规划解题框架

动态规划包含了「分治思想」、「空间换时间」、「最优解」等多种基石算法思想。适合用 DP 解决的问题具有「重叠子问题」和「最优子结构」两大特性。

##### 重叠子问题
子问题之间有重叠，暴力法会产生大量重复计算。DP 通过保存子问题解（查表）保证每个独立子问题只被计算一次。

##### 最优子结构
如果一个问题的最优解可以由其子问题的最优解组合构成，则称此问题具有最优子结构。

---

#### 重叠子问题示例：斐波那契数列

> 数列定义：F0=0, F1=1, Fn=F(n-1)+F(n-2)

三种解法：暴力递归 O(2ⁿ) → 记忆化递归 O(n) → 动态规划 O(n)

// 暴力递归：直接递归调用 f(n-1)+f(n-2)，大量重复计算
// 记忆化递归：用 dp 数组存已计算结果，遇到直接返回
// 动态规划：dp[i] = dp[i-1] + dp[i-2]，从底至顶迭代，可优化为两个变量交替
// 状态转移: dp[i] = dp[i-1] + dp[i-2]

---

#### 最优子结构示例：蛋糕最高售价

重量为 n 的蛋糕，售价 p(n)，求切分最高总售价 f(n)。
- 子问题：f(0)...f(n-1)
- 最优子结构：f(n) = max_{0≤i<n} (f(i) + p(n-i))
// 暴力递归：遍历所有切分组合，O(2ⁿ)
// 记忆化递归：缓存子问题结果
// 动态规划：dp[j] = max(dp[j], dp[i] + priceList[j-i])，O(n²)

---

#### 动态规划解题框架

四步：1. 状态定义 2. 初始状态 3. 转移方程 4. 返回值

##### 示例：斐波那契数列
- 状态定义：一维 dp，dp[i] 为第 i 个斐波那契数
- 初始状态：dp[0]=0, dp[1]=1
- 转移方程：dp[i] = dp[i-1] + dp[i-2]
- 返回值：dp[n]

##### 示例：蛋糕最高售价
- 状态定义：一维 dp，dp[i] 为重量 i 蛋糕最高售价
- 初始状态：dp[0]=0, dp[1]=p(1)
- 转移方程：dp[n] = max_{0≤i<n} (dp[i] + p(n-i))
- 返回值：dp[n]

---

#### 例题练习

| 题目 | 难度 | 描述 |
| --- | --- | --- |
| 跳跃训练 | 简单 | 等价于斐波那契数列 |
| 连续天数的最高销售额 | 简单 | 求最大值，关键在状态定义 |
| 珠宝的最高价值 | 简单 | 二维 dp |
| 统计结果概率 | 中等 | 暴力枚举 → DP，正向递推较 tricky |
| 模糊搜索验证 | 困难 | 状态定义易得出，但转移方程复杂 |

> 排序算法简介（冒泡/快速/归并）相关内容已移至《数据结构与算法》目录，此处不再重复。

// 模糊搜索验证（正则表达式匹配）：二维 DP，dp[i][j] 表示 s[:i] 与 p[:j] 能否匹配
// 当 p[j-1]=='*' 时：dp[i][j] = dp[i][j-2] || (dp[i-1][j] && (s[i-1]==p[j-2] || p[j-2]=='.'))
// 否则：dp[i][j] = dp[i-1][j-1] && (p[j-1]=='.' || s[i-1]==p[j-1])
// 初始化：dp[0][0]=true，首行偶数位为*时 dp[0][j]=true
// 时间复杂度 O(MN)，空间 O(MN)

// 对称二叉树判断：递归检查 L.val==R.val && L.left vs R.right && L.right vs R.left
// root==null → true，recur(L.left,R.right) && recur(L.right,R.left)，O(N)

// 二叉树层序遍历（BFS）：队列实现，每层放入一个 tmp 列表
// 每层按 que.size() 循环，node出队 → 值入tmp → 左右子节点入队
// O(N)时间，O(N)空间

// 二叉树锯齿形层序遍历：双端队列，奇数层尾部添加、偶数层头部添加；或先正常层序再偶数层 reverse
// O(N)

// 二叉树深度（后序遍历DFS）：max(maxDepth(left), maxDepth(right)) + 1，O(N)
// 层序遍历BFS：每遍历一层 res++，O(N)

// 重建二叉树（前序+中序）：分治，前序首位为根，中序划分左右子树
// 用哈希表存中序值→索引，递归构建，O(N)

// 盛最多水的容器：双指针，短板向内移动（因为移动长板面积一定变小）
// S(i,j) = min(h[i],h[j])*(j-i)，每轮移动较短的板，O(N)

// 平衡二叉树判断：后序遍历+剪枝（从底至顶），返回深度or-1
// 也可先序遍历+判断深度（从顶至底，有重复计算），O(N) vs O(NlogN)

# 08_11_盛最多水的容器

### 11. 盛最多水的容器

// 双指针法：左右指针指向数组两端，每轮将短板向内移动一格，更新最大面积
// S(i,j)=min(h[i],h[j])×(j-i)，证明：移动短板面积可能增大，移动长板面积一定变小
// 时间复杂度 O(N)，空间 O(1)

# 09_113_路径总和_II

### 113. 路径总和 II

// 回溯法：先序遍历+路径记录，递归路径更新→目标值更新→满足条件记录→左右递归→路径恢复
// 关键：res.push_back(path) 是拷贝 path 对象
// O(N)

# 10_122_买卖股票的最佳时机_II

### 122. 买卖股票的最佳时机 II

// 贪心：所有上涨交易日都买卖（赚到所有利润），下降交易日不买卖
// 遍历 prices，tmp=prices[i]-prices[i-1]，若 tmp>0 则加入总利润
// O(N)，O(1)

# 100_剑指_Offer_59_I_滑动窗口的最大值

### 剑指 Offer 59 - I. 滑动窗口的最大值

// 单调队列（双端队列）：保持队列非严格递减
// 每轮窗口滑动：①若队首元素是被删除元素则出队 ②删除队列中所有<nums[j]的元素 ③nums[j]入队尾 ④若已形成窗口，队首为最大值
// O(n)，O(k)

# 101_剑指_Offer_60_n_个骰子的点数

### 剑指 Offer 60. n 个骰子的点数

// DP：f(n,x) = Σ_{i=1}^{6} f(n-1, x-i) × 1/6
// 正向递推：遍历 f(n-1) 中各点数和概率，将其累加至 f(n) 中相关项
// 用两个一维数组 dp, tmp 交替前进
// O(n²)，O(n)

# 102_剑指_Offer_61_扑克牌中的顺子

### 剑指 Offer 61. 扑克牌中的顺子

// 充分条件：①除大小王(0)外无重复 ②max-min<5
// 方法一：Set + 遍历，方法二：排序 + 遍历
// O(1)（N≡5）

# 103_剑指_Offer_62_圆圈中最后剩下的数字

### 剑指 Offer 62. 圆圈中最后剩下的数字

// 约瑟夫环问题，DP 解：dp[i] = (dp[i-1] + m) % i，dp[1]=0
// 用一个变量 x 执行状态转移即可
// O(n)，O(1)

# 104_剑指_Offer_63_股票的最大利润

### 剑指 Offer 63. 股票的最大利润

// DP：dp[i] = max(dp[i-1], prices[i] - min(prices[0:i]))
// 优化：profit = max(profit, price - cost)，cost 更新为 min(cost, price)
// O(N)，O(1)

# 105_剑指_Offer_64_求_1_2_n

### 剑指 Offer 64. 求 1 + 2 + … + n

// 不能用乘除、for/while/if，使用逻辑运算符短路效应：n > 1 && (n += sumNums(n-1))
// O(N)，O(N)

# 106_剑指_Offer_65_不用加减乘除做加法

### 剑指 Offer 65. 不用加减乘除做加法

// 位运算：非进位和 n = a ^ b，进位 c = (a & b) << 1
// 循环直到进位为0，适用于正负数（补码）
// O(1)，O(1)

# 107_剑指_Offer_67_把字符串转换成整数

### 剑指 Offer 67. 把字符串转换成整数

// 遍历：去首部空格 → 处理符号位 → 数字拼接 res=res*10+(c-'0')
// 越界判断：res > INT_MAX/10 或 res==INT_MAX/10 && digit>7 时越界
// O(N)，O(N)

# 108_剑指_Offer_68_I_二叉搜索树的最近公共祖先

### 剑指 Offer 68 - I. 二叉搜索树的最近公共祖先

// 利用 BST 性质：p,q 都在右子树则 root=root.right，都在左子树则 root=root.left，否则为最近公共祖先
// 迭代 O(N)/O(1)，递归 O(N)/O(N)

# 109_剑指_Offer_68_II_二叉树的最近公共祖先

### 剑指 Offer 68 - II. 二叉树的最近公共祖先

// 递归：若 root 为 null/p/q 则返回 root；分别递归左右子树
// left 和 right 同时不为空则 root 为 LCA；left 为空返回 right，right 为空返回 left
// O(N)，O(N)

# 110_剑指_Offer_刷题计划

### 剑指 Offer 刷题计划

| 日程 | 题目 |
| --- | --- |
| **Day 1** | 栈与队列（简单）：09, 30 |
| **Day 2** | 链表（简单）：06, 24, 35 |
| **Day 3** | 字符串（简单）：05, 58-II |
| **Day 4** | 查找算法（简单）：03, 53-I, 53-II |
| **Day 5** | 查找算法（中等）：04, 11, 50 |
| **Day 6** | 搜索与回溯（简单）：32-I, 32-II, 32-III |
| **Day 7** | 搜索与回溯（简单）：26, 27, 28 |
| **Day 8** | 动态规划（简单）：10-I, 10-II, 63 |
| **Day 9** | 动态规划（中等）：42, 47 |
| **Day 10** | 动态规划（中等）：46, 48 |
| **Day 11** | 双指针（简单）：18, 22 |
| **Day 12** | 双指针（简单）：25, 52 |
| **Day 13** | 双指针（简单）：21, 57, 58-I |
| **Day 14** | 搜索与回溯（中等）：12, 13 |
| **Day 15** | 搜索与回溯（中等）：34, 36, 54 |
| **Day 16** | 排序（简单）：45, 61 |
| **Day 17** | 排序（中等）：40, 41 |
| **Day 18** | 搜索与回溯（中等）：55-I, 55-II |
| **Day 19** | 搜索与回溯（中等）：64, 68-I, 68-II |
| **Day 20** | 分治算法（中等）：07, 16, 33 |
| **Day 21** | 位运算（简单）：15, 65 |
| **Day 22** | 位运算（中等）：56-I, 56-II |
| **Day 23** | 数学（简单）：39, 66 |
| **Day 24** | 数学（中等）：14-I, 57-II, 62 |
| **Day 25** | 模拟（中等）：29, 31 |
| **Day 26** | 字符串（中等）：20, 67 |
| **Day 27** | 栈与队列（困难）：59-I, 59-II |
| **Day 28** | 搜索与回溯（困难）：37, 38 |
| **Day 29** | 动态规划（困难）：19, 49, 60 |
| **Day 30** | 分治算法（困难）：17, 51 |
| **Day 31** | 数学（困难）：14-II, 43, 44 |

# 111_剑指_Offer_题目分类

### 剑指 Offer 题目分类

| 题目 | 算法分类 | 数据结构分类 |
| --- | --- | --- |
| 03. 数组中重复的数字 | 查找算法 | 数组 |
| 04. 二维数组中的查找 | 查找算法 | 数组 |
| 05. 替换空格 | | 字符串 |
| 06. 从尾到头打印链表 | | 栈/队列，链表 |
| 07. 重建二叉树 | 分治算法 | 树，哈希表 |
| 09. 用两个栈实现队列 | | 栈/队列 |
| 10-I. 斐波那契数列 | 动态规划 | |
| 10-II. 青蛙跳台阶问题 | 动态规划 | |
| 11. 旋转数组的最小数字 | 查找算法 | 数组 |
| 12. 矩阵中的路径 | 回溯算法，搜索算法 | 数组，图 |
| 13. 机器人的运动范围 | 回溯算法，搜索算法 | 数组，图 |
| 14-I. 剪绳子 | 贪心算法，数学 | |
| 14-II. 剪绳子 II | 贪心，分治，数学 | |
| 15. 二进制中1的个数 | 位运算 | |
| 16. 数值的整数次方 | 分治，位运算 | |
| 17. 打印从1到最大的n位数 | | 数组 |
| 18. 删除链表的节点 | 双指针 | 链表 |
| 19. 正则表达式匹配 | 动态规划 | 字符串 |
| 20. 表示数值的字符串 | | 字符串 |
| 21. 调整数组顺序使奇数位于偶数前面 | 双指针 | 数组 |
| 22. 链表中倒数第k个节点 | 双指针 | 链表 |
| 24. 反转链表 | 双指针 | 链表 |
| 25. 合并两个排序的链表 | 双指针 | 链表 |
| 26. 树的子结构 | 搜索算法 | 树 |
| 27. 二叉树的镜像 | 搜索算法 | 栈/队列，树 |
| 28. 对称的二叉树 | 搜索算法 | 树 |
| 29. 顺时针打印矩阵 | 模拟 | 数组 |
| 30. 包含min函数的栈 | 排序 | 栈/队列 |
| 31. 栈的压入、弹出序列 | 模拟 | 栈/队列 |
| 32-I. 从上到下打印二叉树 | 搜索算法 | 栈/队列，树 |
| 32-II. 从上到下打印二叉树 II | 搜索算法 | 栈/队列，树 |
| 32-III. 从上到下打印二叉树 III | 搜索算法 | 栈/队列，树 |
| 33. 二叉搜索树的后序遍历序列 | 分治算法 | 栈/队列，树 |
| 34. 二叉树中和为某一值的路径 | 回溯算法，搜索算法 | 树 |
| 35. 复杂链表的复制 | | 链表 |
| 36. 二叉搜索树与双向链表 | 搜索算法，双指针 | 树 |
| 37. 序列化二叉树 | 搜索算法 | 树 |
| 38. 字符串的排列 | 回溯算法 | 字符串，哈希表 |
| 39. 数组中出现次数超过一半的数字 | | 数组 |
| 40. 最小的k个数 | 排序 | 数组，堆 |
| 41. 数据流中的中位数 | 排序 | 堆 |
| 42. 连续子数组的最大和 | 动态规划 | 数组 |
| 43. 1～n整数中1出现的次数 | 数学 | |
| 44. 数字序列中某一位的数字 | 数学 | |
| 45. 把数组排成最小的数 | 排序 | 字符串 |
| 46. 把数字翻译成字符串 | 动态规划 | 字符串 |
| 47. 礼物的最大价值 | 动态规划 | 数组 |
| 48. 最长不含重复字符的子字符串 | 动态规划，双指针 | 哈希表 |
| 49. 丑数 | 动态规划 | |
| 50. 第一个只出现一次的字符 | | 哈希表 |
| 51. 数组中的逆序对 | 分治算法 | 数组 |
| 52. 两个链表的第一个公共节点 | 双指针 | 链表 |
| 53-I. 在排序数组中查找数字 I | 查找算法 | 数组 |
| 53-II. 0～n-1中缺失的数字 | 查找算法 | 数组 |
| 54. 二叉搜索树的第k大节点 | 搜索算法 | 树 |
| 55-I. 二叉树的深度 | 搜索算法 | 树 |
| 55-II. 平衡二叉树 | 搜索算法 | 树 |
| 56-I. 数组中数字出现的次数 | 位运算 | 数组 |
| 56-II. 数组中数字出现的次数 II | 位运算 | 数组 |
| 57. 和为s的两个数字 | 双指针 | 数组 |
| 57-II. 和为s的连续正数序列 | 双指针 | 数组 |
| 58-I. 翻转单词顺序 | 双指针 | 字符串 |
| 58-II. 左旋转字符串 | | 字符串 |
| 59-I. 滑动窗口的最大值 | 排序 | 数组，栈/队列 |
| 59-II. 队列的最大值 | 排序 | 数组，栈/队列 |
| 60. n个骰子的点数 | 动态规划 | |
| 61. 扑克牌中的顺子 | 排序 | 数组，哈希表 |
| 62. 圆圈中最后剩下的数字 | 数学 | |
| 63. 股票的最大利润 | 动态规划 | 数组 |
| 64. 求1+2+…+n | | |
| 65. 不用加减乘除做加法 | 位运算 | |
| 66. 构建乘积数组 | 数学 | 数组 |
| 67. 把字符串转换成整数 | | 字符串 |
| 68-I. 二叉搜索树的最近公共祖先 | 搜索算法 | 树 |
| 68-II. 二叉树的最近公共祖先 | 搜索算法 | 树 |

---

# 11_137_只出现一次的数字_II

### 137. 只出现一次的数字 II

// 统计所有数字各二进制位 1 出现次数，对 3 求余即为结果
// 方法一：有限状态自动机（位运算），ones=ones^num&~twos, twos=twos^num&~ones
// 方法二：遍历统计 counts[32]，逐位对 3 求余恢复数字
// O(N)，O(1)

# 12_142_环形链表_II

### 142. 环形链表 II

// 快慢指针：第一次相遇时 fast 比 slow 多走 nb（n个环周长）
// slow 再走 a 步到环入口，将 fast 重置为 head，同速走到相遇即为入口
// O(N)，O(1)

# 13_15_三数之和

### 15. 三数之和

// 排序 + 双指针：固定 k，双指针 i,j 在 (k, len(nums)) 两端
// nums[k]>0 直接 break；跳过重复 nums[k]；按 s=nums[k]+nums[i]+nums[j] 结果移动指针
// 去重：i++ 和 j-- 时跳过重复值
// O(N²)，O(1)

# 14_167_两数之和_II

### 167. 两数之和 II

// 排序数组上的双指针（对撞指针），s=numbers[i]+numbers[j]，根据与 target 比较移动 i 或 j
// 正确性证明：每次移动消去的状态都不可能是解
// O(N)，O(1)

# 15_169_多数元素

### 169. 多数元素

// 摩尔投票法：票数正负抵消。众数票数+1，非众数-1，票数和>0。
// 当 votes==0 时假设当前数字为众数，最终返回 x
// O(N)，O(1)

# 16_179_最大数

### 179. 最大数

// 贪心：比较 x+y 与 y+x 的字典序来决定排序规则
// 用内置 sort 或快速排序，比较规则为 y+x < x+y
// 处理全 0 情况返回 "0"
// O(N log N)，O(N)

# 17_1823_找出游戏的获胜者

### 1823. 找出游戏的获胜者

// 约瑟夫环 DP：dp[i] = (dp[i-1] + k) % i，dp[1]=0
// 返回 x+1（题目编号从 1 开始）
// O(n)，O(1)

// 位1的个数：方法一逐位判断 n&1 再右移，O(log n)
// 方法二：n &= n-1 消去最右边1，循环计数，O(M)（M为1的个数）

# 18_198_打家劫舍

### 198. 打家劫舍

// DP：dp[n+1] = max(dp[n], dp[n-1] + num)
// cur 和 pre 两变量交替记录
// O(N)，O(1)

# 19_207_课程表

### 207. 课程表

// 判断有向无环图，拓扑排序（BFS）
// 统计入度表，将入度为0的节点入队，依次出队并减少相邻节点入度
// 最终出队次数 == 课程数则无环
// O(N+M)，O(N+M)

# 20_213_打家劫舍_II

### 213. 打家劫舍 II

// 环形排列，约化为两个单排子问题：不偷第一家（nums[1:]）和不偷最后一家（nums[:n-1]）
// 取较大值，内部 DP 同 198
// O(N)，O(1)

# 21_230_二叉搜索树中第K小的元素

### 230. 二叉搜索树中第K小的元素

// BST 中序遍历为递增序列，递归中序遍历到第 k 个节点
// 优化：在找到第 k 个后提前返回
// O(N)，O(N)

# 22_231_2_的幂

### 231. 2 的幂

// n > 0 && (n & (n-1)) == 0

# 23_238_除自身以外数组的乘积

### 238. 除自身以外数组的乘积

// 不能用除法，用上下三角法：先计算下三角 ans[i]=ans[i-1]*nums[i-1]
// 再倒序遍历，用 tmp 累积上三角乘积乘入 ans
// O(N)，O(1)

# 24_240_搜索二维矩阵_II

### 240. 搜索二维矩阵 II

// 从矩阵左下角/右上角开始搜索，类似二叉搜索树
// flag > target 行减1（上移），flag < target 列加1（右移）
// O(M+N)，O(1)

# 25_242_有效的字母异位词

### 242. 有效的字母异位词

// 哈希表统计字符数量，s 加 t 减，最后检查是否全为0
// O(M+N)，O(1)

# 26_266_回文排列

### 266. 回文排列

// 最多只有一个字符出现奇数次
// 哈希表统计字符出现次数，奇数个字符种类 >1 则 false
// O(N)，O(N)

# 27_278_第一个错误的版本

### 278. 第一个错误的版本

// 二分查找：若 m 是错误版本则在 [i, m-1] 继续，否则 [m+1, j]
// O(log n)，O(1)

# 28_287_寻找重复数

### 287. 寻找重复数

// 方法一：哈希表 O(N)/O(N)
// 方法二：原地交换（利用 nums[i] ∈ [0, n-1]）O(N)/O(1)
// 方法三：环形链表（看成链表找环入口）O(N)/O(1)

# 29_295_数据流的中位数

### 295. 数据流的中位数

// 小顶堆 A 存较大一半，大顶堆 B 存较小一半
// 当 m=n 时向 A 添加（先将 num 插入 B，再将 B 堆顶插入 A）
// 当 m≠n 时向 B 添加（先将 num 插入 A，再将 A 堆顶插入 B）
// 中位数：m=n → (A.top+B.top)/2，m≠n → A.top
// 添加 O(log N)，查找 O(1)

# 30_300_最长递增子序列

### 300. 最长递增子序列

// DP：dp[i] = max(dp[i], dp[j]+1) for j∈[0,i) if nums[i] > nums[j]
// 初始 dp[i]=1，返回 max(dp)
// O(N²)，O(N)

# 31_39_组合总和

### 39. 组合总和

// 回溯+剪枝：从 start 开始遍历避免重复子集，排序+target递减剪枝
// 选择序列满足 i1 ≤ i2 ≤ ... ≤ im
// O(...)，递归深度 O(target/min)

# 32_394_字符串解码

### 394. 字符串解码

// 辅助栈法：遇到数字记录 multi，遇到 '[' 将 multi 和 res 入栈后重置
// 遇到 ']' 出栈拼接 last_res + cur_multi * res
// O(N)，O(N)

# 33_409_最长回文串

### 409. 最长回文串

// 各字符出现次数向下取偶数和，若有奇数则加1（中心）
// O(N)，O(1)

# 34_415_字符串相加

### 415. 字符串相加

// 双指针从末尾模拟加法，carry = tmp/10，当前位 tmp%10
// O(max(M,N))，O(1)

# 35_46_全排列

### 46. 全排列

// DFS + 交换：固定第 x 位，将 nums[i] 与 nums[x] 交换后递归 x+1，再还原
// O(N!N)，O(N)

# 36_47_全排列_II

### 47. 全排列 II

// 在 46 基础上用 Set 剪枝，同一固定位不重复交换相同元素
// O(N!N)，O(N²)

# 37_48_旋转图像

### 48. 旋转图像

// 方法一：辅助矩阵，matrix[j][n-1-i] = tmp[i][j]，O(N²)/O(N²)
// 方法二：原地修改，四元素旋转，遍历前 n/2 行、(n+1)/2 列，O(N²)/O(1)

# 38_方法一_辅助矩阵

### 方法一：辅助矩阵

// matrix[i][j] → matrix[j][n-1-i]

# 39_方法二_原地修改

### 方法二：原地修改

// 四元素旋转：tmp → A, D → A, C → D, B → C, tmp → B
// 覆盖范围 i∈[0,n/2), j∈[0,(n+1)/2)

// 快速幂（二进制角度）：xⁿ = x^{1b₁+2b₂+...}，循环 x=x², n>>=1，O(log n)
// 快速幂（分治角度）：偶数 x²^{n//2}，奇数 x(x²)^{n//2}，O(log n)

// 连续子数组最大和（DP）：dp[i]=dp[i-1]+nums[i] (dp[i-1]>0) else nums[i]
// 直接在 nums 上修改，O(N)/O(1)

// 顺时针打印矩阵：上下左右四边界模拟，循环打印每方向后收缩边界，O(MN)/O(1)

// 最小路径和（二维 DP）：grid[i][j] += min(grid[i-1][j], grid[i][j-1])，O(MN)/O(1)

// 有效数字判断（有限状态自动机）：states 二维表，9种状态，按字符类型转移，O(N)/O(1)

// 青蛙跳台阶：斐波那契变体 f(0)=1,f(1)=1,f(n)=f(n-1)+f(n-2)

# 40_704_二分查找

### 704. 二分查找

// 标准二分，闭区间 [i,j]，m=i+(j-i)/2
// nums[m]<target → i=m+1；>target → j=m-1；== → return m
// O(log N)，O(1)

# 41_724_寻找数组的中心下标

### 724. 寻找数组的中心下标

// sumLeft 和 sumRight 遍历，求总和后逐元素更新
// sumLeft == sumRight 时返回 i
// O(N)，O(1)

# 42_768_最多能完成排序的块_II

### 768. 最多能完成排序的块 II

// 栈维护各块最大值 head，遍历 num，若 num < stack.top() 则合并块
// 最终 stack.size() 即为块数
// O(N)，O(N)

# 43_796_旋转字符串

### 796. 旋转字符串

// 充要条件：s 和 goal 长度相等，且 goal+goal 包含 s
// O(N)（KMP），O(N)

# 44_86_分隔链表

### 86. 分隔链表

// 新建两个哑节点，分别接 <x 和 >=x 的节点，最后拼接
// O(N)，O(1)

# 45_876_链表的中间结点

### 876. 链表的中间结点

// 快慢指针，fast 每轮 2 步，slow 每轮 1 步
// 偶数时返回第二个中间节点
// O(N)，O(1)

# 46_89_格雷编码

### 89. 格雷编码

// G(n+1) = G(n) ∪ reverse(G(n)) 高位补1
// head = 1 << i，倒序遍历 G(n) 加上 head 得到 R'(n)
// O(2ⁿ)，O(1)

---

> **以下开始 剑指 Offer 系列题目解析**

# 47_剑指_Offer_03_数组中重复的数字

### 剑指 Offer 03. 数组中重复的数字

// 方法一：哈希表/Set，O(N)/O(N)
// 方法二：原地交换，利用数字在 [0, n-1] 范围内，nums[i] 与 nums[nums[i]] 交换
// 当 nums[nums[i]] == nums[i] 时找到重复，O(N)/O(1)

# 48_剑指_Offer_04_二维数组中的查找

### 剑指 Offer 04. 二维数组中的查找

// 从矩阵左下角或右上角搜索，类似 BST
// flag > target 上移，flag < target 右移
// O(M+N)，O(1)

# 49_剑指_Offer_05_替换空格

### 剑指 Offer 05. 替换空格

// C++：先统计空格数，resize 长度，倒序遍历替换
// O(N)，O(1)（原地）

# 50_剑指_Offer_06_从尾到头打印链表

### 剑指 Offer 06. 从尾到头打印链表

// 方法一：递归回溯时添加节点值，O(N)/O(N)
// 方法二：辅助栈，入栈后出栈，O(N)/O(N)

# 51_剑指_Offer_07_重建二叉树

### 剑指 Offer 07. 重建二叉树

// 前序+中序重建：前序首位为根，中序划分左右子树
// 哈希表加速中序索引查找，递归构建
// O(N)，O(N)

# 52_剑指_Offer_09_用两个栈实现队列

### 剑指 Offer 09. 用两个栈实现队列

// A 栈负责入队，B 栈负责出队（倒序）
// deleteHead: B 不空则 B.top()，否则将 A 全部倒入 B
// 均摊 O(1)，O(N)

# 53_剑指_Offer_10_I_斐波那契数列

### 剑指 Offer 10- I. 斐波那契数列

// DP：a=0,b=1,sum=(a+b)%1000000007，循环 n 次
// 需取模防大数越界
// O(n)，O(1)

# 54_剑指_Offer_12_矩阵中的路径

### 剑指 Offer 12. 矩阵中的路径

// DFS+剪枝：从每个格子出发，朝四个方向搜索 word
// 标记已访问字符，回溯时恢复
// O(3^K × MN)，O(K)

# 55_剑指_Offer_13_机器人的运动范围

### 剑指 Offer 13. 机器人的运动范围

// DFS/BFS 遍历矩阵，只需向右和向下移动
// 数位和增量公式：(x+1)%10 != 0 ? s_x+1 : s_x-8
// O(MN)，O(MN)

# 56_剑指_Offer_14_I_剪绳子

### 剑指 Offer 14- I. 剪绳子

// 数学推导：将绳子等分为多段乘积最大，最优段长度为3
// 切分规则：n≤3 返回 n-1；n>3 时尽可能切3
// b=0 → 3^a；b=1 → 3^{a-1}×4；b=2 → 3^a×2
// O(1)，O(1)

# 57_剑指_Offer_14_II_剪绳子_II

### 剑指 Offer 14- II. 剪绳子 II

// 与 14-I 相同但涉及大数求余
// 循环求余或快速幂求余：(xy)⊙p = [(x⊙p)(y⊙p)]⊙p
// O(log a)，O(1)

# 58_剑指_Offer_16_数值的整数次方

### 剑指 Offer 16. 数值的整数次方

// 快速幂：二分法/二进制角度，res*=x（当前位为1时），x*=x，n>>=1
// 处理 n<0：x=1/x, n=-n
// O(log n)，O(1)

# 59_剑指_Offer_17_打印从_1_到最大的_n_位数

### 剑指 Offer 17. 打印从 1 到最大的 n 位数

// 不考虑大数：直接 for 循环到 10ⁿ-1
// O(10ⁿ)，O(1)

# 60_剑指_Offer_19_正则表达式匹配

### 剑指 Offer 19. 正则表达式匹配

// 二维 DP：dp[i][j] 表示 s[:i] 与 p[:j] 能否匹配
// 当 p[j-1]=='*'：dp[i][j-2] 或 (dp[i-1][j] && (s[i-1]==p[j-2] || p[j-2]=='.'))
// 当 p[j-1]!='*'：dp[i-1][j-1] && (p[j-1]=='.' || s[i-1]==p[j-1])
// O(MN)，O(MN)

# 61_剑指_Offer_20_表示数值的字符串

### 剑指 Offer 20. 表示数值的字符串

// 有限状态自动机，9种状态，状态转移表
// 合法结束状态：2,3,7,8
// O(N)，O(1)

# 62_剑指_Offer_22_链表中倒数第_k_个节点

### 剑指 Offer 22. 链表中倒数第 k 个节点

// 前后双指针：former 先走 k 步，然后共同走到 former 为 null
// O(N)，O(1)

# 63_剑指_Offer_24_反转链表

### 剑指 Offer 24. 反转链表

// 迭代（双指针）：cur 遍历，pre 保存前驱，tmp=cur->next, cur->next=pre, pre=cur, cur=tmp
// 递归：recur(cur->next, cur) 再修改 cur->next=pre
// O(N)

# 64_剑指_Offer_25_合并两个排序的链表

### 剑指 Offer 25. 合并两个排序的链表

// 双指针+哑节点，比较 l1->val 和 l2->val 决定顺序
// O(M+N)，O(1)

# 65_剑指_Offer_26_树的子结构

### 剑指 Offer 26. 树的子结构

// 先序遍历 A 每个节点 + recur 判断以该节点为根的子树是否包含 B
// recur：B=null→true；A=null→false；值不等→false
// O(MN)，O(M)

# 66_剑指_Offer_27_二叉树的镜像

### 剑指 Offer 27. 二叉树的镜像

// 递归：交换左右子树
// 辅助栈/队列：层序遍历，每个节点交换左右子节点
// O(N)

# 67_剑指_Offer_28_对称的二叉树

### 剑指 Offer 28. 对称的二叉树

// 递归判断 L.left vs R.right && L.right vs R.left
// O(N)

# 68_剑指_Offer_29_顺时针打印矩阵

### 剑指 Offer 29. 顺时针打印矩阵

// 设置上下左右四边界，循环打印方向，每次收缩边界
// O(MN)，O(1)

# 69_剑指_Offer_30_包含_min_函数的栈

### 剑指 Offer 30. 包含 min 函数的栈

// 辅助栈 B 存储非严格降序元素，B.top() 始终为最小值
// push: 若 B 为空或 x≤B.top() 则压入 B
// pop: 若弹出的 A.top() == B.top() 则 B 也弹出
// O(1)

# 70_剑指_Offer_31_栈的压入_弹出序列

### 剑指 Offer 31. 栈的压入、弹出序列

// 模拟：辅助栈按 pushed 顺序入栈，当栈顶等于 popped[i] 时出栈
// 最后栈空则合法
// O(N)，O(N)

# 71_剑指_Offer_32_I_从上到下打印二叉树

### 剑指 Offer 32 - I. 从上到下打印二叉树

// BFS 队列实现
// O(N)，O(N)

# 72_剑指_Offer_32_II_从上到下打印二叉树_II

### 剑指 Offer 32 - II. 从上到下打印二叉树 II

// BFS + 每层打印到一行：按队列长度循环
// O(N)，O(N)

# 73_剑指_Offer_32_III_从上到下打印二叉树_III

### 剑指 Offer 32 - III. 从上到下打印二叉树 III

// 锯齿形打印：双端队列（奇数层尾部加、偶数层头部加）或偶数层 reverse
// O(N)，O(N)

# 74_剑指_Offer_33_二叉搜索树的后序遍历序列

### 剑指 Offer 33. 二叉搜索树的后序遍历序列

// 方法一：递归分治，划分左右子树（找第一个大于根节点的位置 m），检查右子树是否都>根
// 方法二：辅助单调栈，倒序遍历
// O(N²)/O(N) vs O(N)/O(N)

# 75_剑指_Offer_34_二叉树中和为某一值的路径

### 剑指 Offer 34. 二叉树中和为某一值的路径

// 回溯法：先序遍历+路径记录，路径更新→目标值更新→满足条件记录→递归→路径恢复
// O(N)

# 76_剑指_Offer_35_复杂链表的复制

### 剑指 Offer 35. 复杂链表的复制

// 方法一（哈希表）：原→新节点映射，再构建 next 和 random 指向
// 方法二（拼接+拆分）：原节点后复制新节点，构建 random 指向后拆分
// O(N)，O(N) vs O(1)

# 77_剑指_Offer_36_二叉搜索树与双向链表

### 剑指 Offer 36. 二叉搜索树与双向链表

// 中序遍历构建双向链表，pre 和 cur 建立双向引用
// 最后 head.left=pre, pre.right=head 构建循环
// O(N)，O(N)

# 78_剑指_Offer_37_序列化二叉树

### 剑指 Offer 37. 序列化二叉树

// BFS 层序遍历序列化（含 null）
// 反序列化：按层构建，队列+指针 i 确定左右子节点索引
// O(N)，O(N)

# 79_剑指_Offer_39_数组中出现次数超过一半的数字

### 剑指 Offer 39. 数组中出现次数超过一半的数字

// 摩尔投票法：votes=0 时假设当前为众数，相同+1不同-1
// 最终 x 即为众数
// O(N)，O(1)

# 80_剑指_Offer_40_最小的_k_个数

### 剑指 Offer 40. 最小的 k 个数

// 方法一：排序取前 k 个，O(N log N)
// 方法二：基于快速排序的数组划分，哨兵划分后判断 i 与 k 的关系递归
// O(N)，O(log N)

# 81_剑指_Offer_41_数据流中的中位数

### 剑指 Offer 41. 数据流中的中位数

// 小顶堆 A 存较大一半，大顶堆 B 存较小一半
// m=n→A添（num先入B再B堆顶入A），m≠n→B添（num先入A再A堆顶入B）
// 中位数：m=n→(A.top+B.top)/2，m≠n→A.top
// 添加 O(log N)，查找 O(1)

# 82_剑指_Offer_43_1_n_整数中_1_出现的次数

### 剑指 Offer 43. 1～n 整数中 1 出现的次数

// 逐位计算：cur=0→high×digit；cur=1→high×digit+low+1；else→(high+1)×digit
// 从个位往上递推
// O(log n)，O(1)

# 83_剑指_Offer_44_数字序列中某一位的数字

### 剑指 Offer 44. 数字序列中某一位的数字

// 三步：确定位数（减数位数量 count）→ 确定数字 num = start + (n-1)/digit → 确定数位
// O(log n)，O(log n)

# 84_剑指_Offer_45_把数组排成最小的数

### 剑指 Offer 45. 把数组排成最小的数

// 排序规则：x+y < y+x 则 x 排在 y 前
// 快排或内置 sort，自定义比较函数
// O(N log N)，O(N)

# 85_剑指_Offer_46_把数字翻译成字符串

### 剑指 Offer 46. 把数字翻译成字符串

// DP：dp[i] = dp[i-1] + dp[i-2]（若 x_{i-1}x_i ∈ [10,25]），否则 dp[i] = dp[i-1]
// dp[0]=dp[1]=1
// O(N)，O(N)（可优化为 O(1) 空间）

# 86_剑指_Offer_48_最长不含重复字符的子字符串

### 剑指 Offer 48. 最长不含重复字符的子字符串

// DP+哈希表：dp[j] 以 s[j] 为结尾的最长不重复子串长度
// dp[j] = dp[j-1]+1 (dp[j-1] < j-i) 或 j-i (dp[j-1] ≥ j-i)
// 也可用双指针+哈希表：i=max(dic[s[j]],i)，res=max(res,j-i)
// O(N)，O(1)

# 87_剑指_Offer_50_第一个只出现一次的字符

### 剑指 Offer 50. 第一个只出现一次的字符

// 哈希表统计各字符出现次数是否为1
// 有序哈希表可减少第二轮遍历
// O(N)，O(1)

# 88_剑指_Offer_51_数组中的逆序对

### 剑指 Offer 51. 数组中的逆序对

// 归并排序过程中统计逆序对
// 合并时若左子数组当前元素 > 右子数组当前元素，则产生 m-i+1 个逆序对
// O(N log N)，O(N)

# 89_剑指_Offer_52_两个链表的第一个公共节点

### 剑指 Offer 52. 两个链表的第一个公共节点

// 双指针 A 和 B 分别遍历 headA→headB 和 headB→headA
// 相遇点即为公共节点
// O(a+b)，O(1)

# 90_剑指_Offer_53_I_在排序数组中查找数字_I

### 剑指 Offer 53 - I. 在排序数组中查找数字 I

// 二分查找 target 和 target-1 的右边界，right_diff = helper(target) - helper(target-1)
// helper() 返回插入点（即右边界索引）
// O(log N)，O(1)

# 91_剑指_Offer_53_II_0_n_1_中缺失的数字

### 剑指 Offer 53 - II. 0～n-1 中缺失的数字

// 二分：nums[m]==m → 右子数组在右侧；nums[m]!=m → 左子数组末位在左侧
// 最终 i 指向缺失的数字
// O(log N)，O(1)

# 92_剑指_Offer_54_二叉搜索树的第_k_大节点

### 剑指 Offer 54. 二叉搜索树的第 k 大节点

// 中序遍历倒序（右→根→左），第 k 个即为第 k 大
// 提前返回：k 减到 0 时记录并返回
// O(N)，O(N)

# 93_剑指_Offer_55_I_二叉树的深度

### 剑指 Offer 55 - I. 二叉树的深度

// DFS（后序遍历）：max(maxDepth(left), maxDepth(right)) + 1
// BFS（层序遍历）：每层 res++
// O(N)，O(N)

# 94_剑指_Offer_55_II_平衡二叉树

### 剑指 Offer 55 - II. 平衡二叉树

// 后序遍历+剪枝：返回深度或 -1（不平衡）
// 先序遍历+判断深度（有重复计算）
// O(N) vs O(N log N)

# 95_剑指_Offer_56_I_数组中数字出现的次数

### 剑指 Offer 56 - I. 数组中数字出现的次数

// 全体异或得 x⊕y，找到任意一个为1的二进制位分组
// 各组异或得到两个数字
// O(N)，O(1)

# 96_剑指_Offer_56_II_数组中数字出现的次数_II

### 剑指 Offer 56 - II. 数组中数字出现的次数 II

// 有限状态自动机：ones=ones^num&~twos, twos=twos^num&~ones
// 或遍历统计 counts[32] 并对 3 求余
// O(N)，O(1)

# 97_剑指_Offer_57_II_和为_s_的连续正数序列

### 剑指 Offer 57 - II. 和为 s 的连续正数序列

// 方法一：求和公式，解一元二次方程求 j
// 方法二：滑动窗口（双指针），根据 s 与 target 关系移动左右边界
// O(N)，O(1)

# 98_剑指_Offer_57_和为_s_的两个数字

### 剑指 Offer 57. 和为 s 的两个数字

// 对撞双指针，O(N)/O(1)

# 99_剑指_Offer_58_I_翻转单词顺序

### 剑指 Offer 58 - I. 翻转单词顺序

// 双指针倒序遍历，确定单词边界后加入结果
// O(N)，O(N)


---


# 设计模式与项目实践

> 说明：当前目录仅保留 README.md，正文为历史文档合并/知识点汇总；若出现旧文件名式标题，表示章节来源，不代表当前目录存在对应文件。

# DesignPattern__README.md

## 设计模式

> 各大设计模式例子参考：CSDN专栏 . C++ 设计模式 系列博文

此文件夹为一个 CLion 工程，由 CMake 构建，各个文件夹为各个设计模式的具体实现。

- 单例模式例子
- 抽象工厂模式例子
- 适配器模式例子
- 桥接模式例子
- 观察者模式例子

# DesignPattern__SingletonPattern__README.md

## 单例模式

// ⚠️ 注意：以下懒汉式单例存在非线程安全、内存泄漏、缺少拷贝构造和赋值运算符禁用的问题。
// 面试中推荐使用 C++11 Magic Static（下方线程安全版本）。

// 懒汉式单例：首次调用 GetInstance 才创建实例，非线程安全，多线程下可能创建多个实例。
// 线程安全单例（C++11 Magic Static）：利用函数局部 static 变量的线程安全初始化特性，无需显式加锁。

# Interview__CrossProcess__README.md

## CrossProcess

面向面试场景的 C++ 跨进程通信示例项目，使用 CMake 组织构建，包含 3 类方案：

- `shared_counter_demo`：基于 POSIX 共享内存 + process-shared pthread_mutex 的共享计数器。
- `shared_queue_demo`：基于 POSIX 共享内存 + semaphore + mutex 的共享队列。
- `file_lock_demo`：基于 fcntl 文件锁的串行化写文件示例。
- `cross_process_tests`：轻量单元测试，可通过 ctest 运行。

目录结构: CrossProcess/ 包含 CMakeLists.txt, README.md, include/ (3个头文件), src/ (6个源文件), tests/ (1个测试文件)

构建方式: cmake -S . -B build && cmake --build build，再通过 ctest 运行测试。

### 示例说明

#### 1. 共享计数器

核心对象：`shm_open`、`ftruncate`、`mmap`、`pthread_mutex_t`。适合多进程共享小段状态（计数器、统计信息）。关键点：`pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED)` 让互斥量跨进程使用。

#### 2. 共享队列

核心对象：共享内存 + `sem_t` + `pthread_mutex_t`。适合父子进程或多进程间高效小消息传递。关键点：`slots` 管空位，`items` 管数据量，`mutex` 保护环形队列指针。

#### 3. 文件锁

核心对象：`fcntl(F_SETLKW)`。适合多进程竞争写同一文件但对性能要求不高的场景。关键点：文件锁偏协调访问，而非高吞吐通信。

### 单元测试覆盖点

多子进程并发自增共享计数器结果正确性、共享队列 FIFO 顺序、文件锁下多进程追加写入保持行记录完整性。

### 面试知识点补充：跨进程与内存池的联系

#### 1. 为什么跨进程场景会谈到内存池

共享内存是一块被多进程映射的连续地址空间，若其中频繁创建对象，就需要分配/回收机制，常见做法是实现共享内存池或对象池。

#### 2. 共享内存内存池和普通进程内内存池的区别

普通内存池保存进程内裸指针；跨进程内存池不能依赖裸指针（不同进程映射虚拟地址不同），应保存偏移量 offset 再通过共享基址恢复。

#### 3. 设计共享内存内存池时的核心点

固定大小块分配最简单（适合消息节点、连接描述符）；空闲链表跨进程共享需配合互斥锁/原子操作/CAS；元数据必须在共享内存中；需考虑进程异常退出后的恢复问题。

#### 4. 面试里高频延伸问题

共享内存速度快：少一次内核态拷贝，多进程直接访问同一物理页。仍需同步：共享数据不共享访问时序。常用固定块：实现简单、碎片少、分配回收时间稳定。STL 容器不能直接放入共享内存：内部保存进程私有地址，跨进程不安全。

#### 5. 一句话回答模板

> 共享内存解决"多进程看到同一块数据"的问题，内存池解决"这块数据里如何高效分配对象"的问题；实际工程常将共享内存作为底层载体，在其上实现固定块内存池、对象池或环形队列。

# Interview__MemoryPool__README.md

## MemoryPool

使用 C++17 + CMake 编写的内存池进阶示例项目，包含：

- 定长块内存池 `FixedMemoryPool`
- 基于 placement new 的对象池封装 `ObjectPool<T>`
- 对象自动归还包装器 `UniquePoolPtr<T>`
- 线程安全版本 `ThreadSafeFixedMemoryPool` 与 `ThreadSafeObjectPool<T>`
- 带越界检测和调试标记的 `DebugFixedMemoryPool`
- 基于分离适配策略的 `VariableMemoryPool`
- 性能基准测试 `memory_pool_benchmark`
- 单元测试 `memory_pool_tests`
- 基于 gcov 的覆盖率统计能力

目录结构: MemoryPool/ 包含 CMakeLists.txt, include/ (2个头文件), src/ (4个源文件), README.md

构建方式: cmake -S . -B build && cmake --build build，运行 demo/benchmark/tests；覆盖率用 -DENABLE_COVERAGE=ON 单独构建目录。

### 本次扩展对应模块 9 的实现

#### 1. 增加线程安全版本

新增 `ThreadSafeFixedMemoryPool` 和 `ThreadSafeObjectPool<T>`，使用 `std::mutex + std::lock_guard` 对分配/释放加锁，适合多线程共享对象池的基础演示。面试可答：最简单的线程安全方案是在 Allocate/Deallocate 上加互斥锁，先保证正确性，再考虑 thread local 或无锁优化。

#### 2. 增加越界检测与调试标记

`DebugFixedMemoryPool` 实现了块头魔数 `kHeadMagic`、块尾魔数 `kTailMagic`、活跃块映射表 `active_blocks_`、重复释放/非法释放检测。回收时检查头尾守卫区是否被破坏，帮助定位越界写和 double free，适合调试环境。

#### 3. 增加对象自动归还包装器

新增 `PoolDeleter<T>`、`UniquePoolPtr<T>`、`ObjectPool<T>::CreateUnique(...)`，将对象池使用与 RAII 结合，entity 离开作用域时自动析构并归还内存。本质是对象池 + 自定义 deleter + `std::unique_ptr`，减少手动 `Destroy` 遗漏。

#### 4. 增加变长块管理策略

`VariableMemoryPool` 采用分离适配思路：维护多组尺寸类别（如 16/32/64/128/256），小于等于桶位大小的请求放进该桶位，超过最大桶位回退系统分配。小对象分配快，代价是可能出现内部碎片。

#### 5. 增加性能基准测试

`memory_pool_benchmark` 对比普通 new/delete、`ObjectPool<T>`、`VariableMemoryPool` 在高频申请/释放场景下的收益。教学用途，结果受编译器/机器/优化等级影响；面试重点在于"你知道如何设计验证方法"。

### 各模块适合怎么讲

`FixedMemoryPool`：定长块、批量扩容、空闲链表、O(1) 分配与回收。
`ObjectPool<T>`：placement new、显式析构、内存生命周期与对象生命周期分离。
`ThreadSafeObjectPool<T>`：互斥锁保护、先保证正确性、后续可扩展为 thread local 或 lock-free。
`DebugFixedMemoryPool`：guard bytes、magic number、double free 检测、调试版内存池。
`VariableMemoryPool`：segregated free lists、size classes、大块请求回退系统分配。

### 面试高频延伸点

#### 1. 为什么线程安全版本不一定更快？

互斥锁带来竞争和上下文切换成本。线程安全版本的目标是先保证多线程下行为正确，而非天然更快。

#### 2. 调试版内存池为什么不适合生产环境？

额外头尾标记、活跃块映射表、更多校验逻辑带来额外时间和空间开销。

#### 3. 变长池为什么常用分离适配？

在实现复杂度和运行效率间较平衡：比完全通用分配器简单，比每次走系统分配更快，适合固定几类常见大小的对象。

#### 4. 自动归还包装器的本质是什么？

用 RAII 管理"对象池归还动作"，将手动 `Destroy` 绑定到智能指针析构过程。

### 推荐运行方式

先运行 `memory_pool_demo` 看功能演示，再运行 `memory_pool_benchmark` 看性能测试，最后运行 `memory_pool_tests` 和 `ctest` 验证正确性。覆盖率用单独的 build-coverage 目录生成。

### 适合面试时的总结表达

> 该示例从基础定长内存池扩展为小型内存池实验项目：包含线程安全封装、越界检测调试池、RAII 自动归还、分离适配变长池和基准测试，能说明内存池怎么写以及在工程中怎么演进。

# Interview__RPC__README.md

## RPC

实现了一个极简 RPC 框架，覆盖请求结构、序列化、方法注册、服务端分发和响应返回等核心点。

构建方式: cmake -S . -B build && cmake --build build，通过 ctest 运行测试。

目标：`rpc_demo` 演示 add 请求从序列化到分发再到响应解析；`rpc_tests` 验证序列化和分发逻辑。

面试知识点：RPC 本质是"像调用本地函数一样调用远端服务"；核心链路包括接口定义、序列化、网络传输、服务发现、路由分发、超时重试；常见协议有 JSON、Protobuf、Thrift、自定义二进制协议；高频追问包括幂等性、超时、重试、负载均衡、限流、熔断、服务治理。

# Interview__ThreadPool__README.md

## ThreadPool

实现了一个固定大小线程池，覆盖任务提交、工作线程循环、条件变量唤醒和 future 结果获取。

构建方式: cmake -S . -B build && cmake --build build，通过 ctest 运行测试。

目标：`thread_pool_demo` 提交平方计算任务并汇总结果；`thread_pool_tests` 验证有/无返回值任务。

面试知识点：线程池的价值是复用线程，降低频繁创建销毁成本；核心结构包括任务队列、工作线程集合、互斥锁、条件变量、停止标记；常见追问包括队列是否有界、如何优雅关闭、任务异常如何传播、是否支持工作窃取；`std::packaged_task + future` 是最小实现方案。

# Interview__Timer__README.md

## Timer

实现了一个最小定时调度器，适合讲延迟任务、最小堆、后台线程和条件变量等待。

构建方式: cmake -S . -B build && cmake --build build，通过 ctest 运行测试。

目标：`timer_demo` 提交 100ms 延迟任务观察执行结果；`timer_tests` 验证单个和多个任务触发行为。

面试知识点：定时器实现依赖最小堆、时间轮或有序链表；最小堆适合中等规模任务调度，实现直观；条件变量让后台线程睡到最近到期点，避免忙等；高频追问包括周期任务、取消任务、重复调度、精度误差、系统时钟回拨。

> "面试鸭"React+Node.js 项目文档已移除（与 C++ 面试主题无关）。

---


---


# 系统设计与分布式

> 2025-2026 年社招高频新增：短链系统、限流组件、断点续传等系统设计题频繁出现。分布式方向常见 Raft 共识、LSM Tree、分布式事务。

# problems__短链系统设计.md
### 短链系统怎么设计？（写入百万、查询千万、10ms 响应）

**核心功能：** 长 URL → 短码 → 重定向到原 URL。
**短码生成：** 哈希法（MD5/SHA1 取前 N 位，需处理冲突）、自增 ID + Base62（可反向解析）、预生成池。
**关键设计：** 发号器用 Snowflake / Redis INCR（需持久化）；MySQL + Redis 缓存；读：查 Redis → 命中 302 → miss 查 MySQL 回写 Redis；写：查重 → 发号 → 落库。
**标准答案要点：** 自增 ID + Base62 保唯一无冲突；MySQL 持久化 + Redis LRU 缓存；读 Redis 集群，写消息队列削峰 + 异步落库。

---

# problems__限流组件设计.md
### 如何设计限流组件？令牌桶和滑动窗口有什么区别？

**算法对比：** 固定窗口（简单但有临界突刺）、滑动窗口（精确但内存大）、漏桶（恒定输出，不能应对突发）、令牌桶（允许突发，最常用）。
// 令牌桶实现：atomic 计数 + 按速率补充，try_consume 用 CAS 原子减
**标准答案要点：** 令牌桶以恒定速率生成令牌，允许短期突发（桶容量内），长期限制速率。Redis 可用 Lua 脚本 INCR+EXPIRE 做固定窗口，或 sorted set 做滑动窗口。生产推荐 Sentinel / RateLimiter。

---

# problems__海量数据TopK统计.md
### 10 亿个 IP，找出访问次数 Top 10 的 IP（内存有限）

**方案一（标准）：** Hash 分桶确保相同 IP 落入同一文件 → 每文件 HashMap 统计 + 小顶堆求 TopK → 合并取最终结果。**方案二：** MapReduce，Map 计数，Reduce 合并排序。
**标准答案要点：** 分治 + 堆。Hash 分桶将数据切为内存可处理的小块，块内 HashMap + 小顶堆。时间复杂度 O(N·logK)，空间取决于分桶大小。

---

# problems__断点续传设计.md
### 断点续传怎么设计？CRC/MD5 校验有什么用？

**核心流程：** 请求 upload_id → 分片（每片带 index + MD5）→ 上传 → 服务端记录状态 → 中断后查询已接收分片 → 跳过已完成续传剩余 → 全部完成合并 + MD5 校验。
**关键点：** 分片 ~1MB；每片 CRC 防传输错误，全文件 MD5 防篡改；乱序分片按 index 拼接；多分片并发上传。
**标准答案要点：** 分片上传 + 状态追踪。服务端维护 `{upload_id, chunk_status[]}` 映射。CRC 逐片校验，MD5 最终验证。

---

# problems__Raft共识算法.md
### Raft 共识算法原理？选主和日志复制怎么实现？

Raft 将共识分解为选主、日志复制、安全性三个子问题。
**选主：** Follower 在随机超时（150-300ms）后变为 Candidate，term++，发送 RequestVote 获多数票当选 Leader，定期发心跳维持地位；平票时随机超时重选。
**日志复制：** 客户端请求 → Leader 追加本地日志 → AppendEntries 给所有 Follower → 多数确认后 commit → 应用到状态机 → 响应客户端。
**关键保证：** Leader 日志包含所有已提交条目；日志匹配属性确保一致性。
**标准答案要点：** 强 Leader 共识算法：(1) 选主：超时发起选举，多数票当选；(2) 日志复制：AppendEntries 复制，多数确认后提交；(3) 安全性：term+index 保证一致，只有最新日志的节点才能当选。

---

# problems__LSM-Tree原理.md
### LSM Tree 是什么？读写放大如何优化？

LSM Tree 写优化存储结构，核心：随机写转为顺序写。
**基本结构：** 写入 → MemTable（跳表）→ 满后刷为 SSTable → Compaction 合并压缩。
**写放大 vs 读放大：** 写放大由 Compaction 重复读写产生（LevelDB 10-30x，RocksDB 可优化至 3-5x）；读放大因需遍历多个 SSTable，用 Bloom Filter 快速排除不存在的 key。
**Compaction 策略：** Leveled（LevelDB，写放大高空间好）、Tiered（Cassandra，写放大低读放大高）、Universal（RocksDB，类 Tiered 适写多读少）。
**标准答案要点：** 内存写入 + 批量刷盘将随机写转顺序写，性能优于 B+ 树。代价是读/写放大。RocksDB 通过分层 Compaction、Bloom Filter、block cache 实现读写平衡。适用于写多读少场景（日志、时序DB、MQ持久化）。

---

# problems__分布式事务.md
### 分布式事务如何实现？2PC 和 TCC 有什么区别？

| 方案 | 原理 | 优缺点 |
|------|------|--------|
| 2PC | Prepare → Commit | 强一致但同步阻塞、协调者单点 |
| TCC | Try → Confirm → Cancel | 不阻塞性能好但业务侵入性强 |
| 消息最终一致性 | 本地事务+消息表+补偿 | 高可用最终一致但延迟不可控 |

**2PC 问题：** 协调者 Commit 后宕机，部分参与者未收到 Commit → 需回查协调者恢复状态。
**标准答案要点：** 2PC 两阶段提交实现强一致，有同步阻塞和单点问题。TCC 拆为三步业务自实现，性能好但有侵入性。大规模系统倾向最终一致性：本地事务 + 消息表 + 定时补偿 + 幂等设计。

---


---


# AI与LLM面试追问

> 2025-2026 新趋势：部分 C++ 社招面试追问 AI/LLM 项目经验（尤其腾讯、快手）。

# problems__大模型接入选型.md

### 大模型怎么接入？如何选型？

接入：API（零部署，延迟不可控）、本地 vLLM/Ollama（低延迟，硬件成本高）、混合部署。选型：延迟、并发、数据安全、成本、中文能力。
> 回答模板：通用用 API，敏感用本地 Qwen/Llama，推理 vLLM，部署 A100/H800。

# problems__RAG原理与优化.md

### RAG 为什么能优化大模型？核心流程是什么？

RAG 生成前从知识库检索文档拼入 prompt。流程：问题→Embedding→向量库 Top-K→拼入 prompt→LLM 生成。解决：知识时效性、幻觉缓解、领域知识注入。优化：chunk 语义切分（512-1024 tokens）；混合检索+BM25+Rerank；Prompt 要求基于文档回答。

# problems__LLM幻觉问题与解决方案.md

### LLM 幻觉是什么？怎么解决？

幻觉：内容与事实不符但合理。类型：事实错误、逻辑矛盾、过度自信。方案：RAG（显著降低）、精细 Prompt（中等）、Few-shot CoT（中等）、输出校验（较高）、Human-in-the-loop（最高但贵）。
> 面试要点：RAG + 输出校验组合，如代码生成后用 AST 验证语法正确性。
