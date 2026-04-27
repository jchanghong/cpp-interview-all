> 说明：当前目录仅保留 README.md，正文为历史文档合并/知识点汇总；若出现旧文件名式标题，表示章节来源，不代表当前目录存在对应文件。

# CODE_OF_CONDUCT.md

## 参与者公约

### 我们的承诺

为建设开放友好的环境，我们贡献者和维护者承诺：不论年龄、体型、身体健全与否、民族、性征、性别认同与表征、经验水平、教育程度、社会地位、国籍、相貌、种族、信仰、性取向，我们项目和社区的参与者皆免于骚扰。

### 我们的准则

有助于创造积极环境的行为包括但不限于：

- 措辞友好且包容
- 尊重不同的观点和经验
- 耐心接受有益批评
- 关注对社区最有利的事情
- 与社区其他成员友善相处

参与者不应采取的行为包括但不限于：

- 发布与性有关的言论或图像、不受欢迎地献殷勤
- 捣乱/煽动/造谣行为、侮辱/贬损的评论、人身及政治攻击
- 公开或私下骚扰
- 未经明确授权便发布他人的资料，如住址、电子邮箱等
- 其他有理由认定为违反职业操守的不当行为

### 我们的义务

项目维护者有义务诠释何谓“妥当行为”，并妥善公正地纠正已发生的不当行为。

项目维护者有权利和义务去删除、编辑、拒绝违背本行为标准的评论（comments）、提交（commits）、代码、wiki 编辑、问题（issues）等贡献；项目维护者可暂时或永久地封禁任何他们认为行为不当、威胁、冒犯、有害的参与者。

### 适用范围

本行为标准适用于本项目。当有人代表本项目或本社区时，本标准亦适用于此人所处的公共平台。

代表本项目或本社区的情形包括但不限于：使用项目的官方电子邮件、通过官方媒体账号发布消息、作为指定代表参与在线或线下活动等。

代表本项目的行为可由项目维护者进一步定义及解释。

### 贯彻落实

可以致信 592789970@qq.com，向项目团队举报滥用、骚扰及不当行为。

维护团队将审议并调查全部投诉，妥善地予以必要的回应。项目团队有义务保密举报者信息。具体执行方针或将另行发布。

未切实遵守或执行本行为标准的项目维护人员，经项目负责人或其他成员决议，可能被暂时或永久地剥夺参与本项目的资格。

### 来源

本行为标准改编自[参与者公约][主页]，版本 1.4 可在此查阅：https://www.contributor-covenant.org/zh-cn/version/1/4/code-of-conduct.html

[主页]: https://www.contributor-covenant.org

# DesignPattern__README.md

## 设计模式

> 各大设计模式例子参考：CSDN专栏 . C++ 设计模式 系列博文

此文件夹为一个 CLion 工程，由 CMake 构建，各个文件夹为各个设计模式的具体实现。文件中可能会有中文乱码问题，请以 `GB2312`（中文） 编码打开。

* 单例模式例子
* 抽象工厂模式例子
* 适配器模式例子
* 桥接模式例子
* 观察者模式例子

# DesignPattern__SingletonPattern__README.md

## 单例模式

// ⚠️ 注意：以下懒汉式单例存在以下问题：
// 1. 非线程安全：多线程下可能创建多个实例
// 2. 内存泄漏：new 的对象从未被 delete
// 3. 使用 0 而非 nullptr（C++11 起推荐用 nullptr）
// 面试中推荐使用 C++11 Magic Static（下方线程安全版本）

```cpp
// 懒汉式单例模式
class Singleton
{
private:
	Singleton() { }
	static Singleton * pInstance;
public:
	static Singleton * GetInstance()
	{
		if (pInstance == nullptr)
			pInstance = new Singleton();
		return pInstance;
	}
};

// 线程安全的单例模式
class Singleton
{
private:
	Singleton() { }
	~Singleton() { }
	Singleton(const Singleton &);
	Singleton & operator = (const Singleton &);
public:
	static Singleton & GetInstance()
	{
		static Singleton instance;
		return instance;
	}
};
```

# Interview__CrossProcess__README.md

## CrossProcess

这是一个面向面试场景的 C++ 跨进程通信示例项目，使用 `CMake` 组织构建，包含 3 类常见跨进程方案：

- `shared_counter_demo`：基于 `POSIX shared memory + pthread_mutex(process-shared)` 的共享计数器。
- `shared_queue_demo`：基于 `POSIX shared memory + semaphore + mutex` 的共享队列。
- `file_lock_demo`：基于 `fcntl` 文件锁的串行化写文件示例。
- `cross_process_tests`：不依赖第三方测试框架的轻量单元测试程序，可直接通过 `ctest` 运行。

### 目录结构

```text
CrossProcess
├── CMakeLists.txt
├── README.md
├── include
│   ├── ipc_file_lock.h
│   ├── ipc_shared_counter.h
│   └── ipc_shared_queue.h
├── src
│   ├── file_lock_demo.cpp
│   ├── ipc_file_lock.cpp
│   ├── ipc_shared_counter.cpp
│   ├── ipc_shared_queue.cpp
│   ├── shared_counter_demo.cpp
│   └── shared_queue_demo.cpp
└── tests
    └── test_cross_process.cpp
```

### 构建与运行

```bash
cd <CrossProcess 项目目录>
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

运行示例程序：

```bash
./build/shared_counter_demo
./build/shared_queue_demo
./build/file_lock_demo
```

### 示例说明

#### 1. 共享计数器

- 核心对象：`shm_open`、`ftruncate`、`mmap`、`pthread_mutex_t`
- 适合场景：多个进程共享一小段状态，例如计数器、统计信息、状态位。
- 关键点：`pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED)` 让互斥量可跨进程使用。

#### 2. 共享队列

- 核心对象：共享内存 + `sem_t` + `pthread_mutex_t`
- 适合场景：父子进程或多个本机进程之间做高效的小消息传递。
- 关键点：`slots` 管空位，`items` 管数据量，`mutex` 保护环形队列指针。

#### 3. 文件锁

- 核心对象：`fcntl(F_SETLKW)`
- 适合场景：多个进程竞争写同一个文件，但对性能要求不如共享内存高。
- 关键点：文件锁更偏“协调访问”，而不是“高吞吐通信”。

### 单元测试覆盖点

- 多子进程并发自增后，共享计数器结果是否正确。
- 共享队列是否保持 FIFO 顺序。
- 文件锁是否让多进程追加写入保持完整的行记录。

### 面试知识点补充：跨进程与内存池的联系

下面这部分专门补充“内存池”相关知识点，方便你在讲跨进程题目时顺带展开。

#### 1. 为什么跨进程场景会谈到内存池

- 共享内存本质上就是一块被多个进程映射的连续地址空间。
- 如果这块共享内存里要频繁创建对象、消息节点、任务描述符，就会遇到“谁来分配/回收”的问题。
- 这时常见做法是在共享内存内部再实现一个“共享内存池”或“对象池”。

#### 2. 共享内存内存池和普通进程内内存池的区别

- 普通内存池保存的是当前进程内可直接使用的裸指针。
- 跨进程内存池不能直接依赖裸指针，因为不同进程映射到的虚拟地址可能不同。
- 更稳妥的方式是保存“偏移量 offset”而不是绝对地址，再通过共享基址恢复访问位置。

#### 3. 设计共享内存内存池时的核心点

- 固定大小块分配最简单，适合消息节点、任务节点、连接描述符等。
- 空闲链表如果跨进程共享，通常要配合互斥锁、原子操作或 CAS。
- 元数据必须也位于共享内存中，否则多个进程看到的池状态不一致。
- 要考虑进程异常退出后的恢复问题，否则锁和空闲链表可能损坏。

#### 4. 面试里高频延伸问题

- 为什么共享内存速度快：因为少了一次内核态数据拷贝，多个进程直接访问同一块物理页。
- 为什么共享内存还需要同步：共享的是数据，不共享访问时序，所以还要锁、信号量或原子变量。
- 共享内存池为什么常用固定块：实现简单、碎片少、分配回收时间稳定。
- 为什么不能把 STL 容器直接丢进共享内存：很多实现内部保存的是进程私有地址，跨进程不安全。

#### 5. 一句话回答模板

如果面试官追问“跨进程通信和内存池有什么结合点”，可以这样回答：

> 共享内存解决的是“多进程看到同一块数据”的问题，内存池解决的是“这块数据里如何高效分配对象”的问题。实际工程里经常把共享内存作为底层载体，再在其上实现固定块内存池、对象池或环形队列。

# Interview__MemoryPool__README.md

## MemoryPool

这是一个使用 `C++17 + CMake` 编写的内存池进阶示例项目。现在它已经按照模块 9 的方向完成了扩展，包含：

- 定长块内存池 `FixedMemoryPool`
- 基于 placement new 的对象池封装 `ObjectPool<T>`
- 对象自动归还包装器 `UniquePoolPtr<T>`
- 线程安全版本 `ThreadSafeFixedMemoryPool` 与 `ThreadSafeObjectPool<T>`
- 带越界检测和调试标记的 `DebugFixedMemoryPool`
- 基于分离适配策略的 `VariableMemoryPool`
- 性能基准测试程序 `memory_pool_benchmark`
- 单元测试程序 `memory_pool_tests`
- 基于 `gcov` 的覆盖率统计能力

### 目录结构

```text
MemoryPool/
├── CMakeLists.txt
├── include/
│   ├── fixed_memory_pool.h
│   └── memory_pool_extensions.h
├── src/
│   ├── benchmark.cpp
│   ├── fixed_memory_pool.cpp
│   ├── main.cpp
│   └── memory_pool_extensions.cpp
└── README.md
```

### 如何构建

```bash
cd <MemoryPool 项目目录>
cmake -S . -B build
cmake --build build
./build/memory_pool_demo
./build/memory_pool_benchmark
./build/memory_pool_tests
ctest --test-dir build --output-on-failure
```

如果你想生成覆盖率统计，可以单独使用覆盖率构建目录：

```bash
cmake -S . -B build-coverage -DENABLE_COVERAGE=ON
cmake --build build-coverage --target coverage
```

### 本次扩展对应模块 9 的实现

#### 1. 增加线程安全版本

项目新增了：

- `ThreadSafeFixedMemoryPool`
- `ThreadSafeObjectPool<T>`

实现方式：

- 使用 `std::mutex + std::lock_guard` 对分配和释放加锁
- 适合多线程下共享对象池的基础演示
- 代码重点在“线程安全思路”，不是极限性能版本

面试可答：

> 最简单的线程安全内存池方案，就是在 Allocate/Deallocate 上加互斥锁，先保证正确性，再考虑 thread local 或无锁结构优化吞吐。

#### 2. 增加越界检测与调试标记

项目新增了 `DebugFixedMemoryPool`，实现了：

- 块头魔数 `kHeadMagic`
- 块尾魔数 `kTailMagic`
- 活跃块映射表 `active_blocks_`
- 重复释放/非法释放检测

作用：

- 在回收时检查头尾守卫区是否被破坏
- 帮助定位越界写和 double free 问题
- 更适合调试环境，不适合直接当生产高性能版本

#### 3. 增加对象自动归还包装器

项目新增了：

- `PoolDeleter<T>`
- `UniquePoolPtr<T>`
- `ObjectPool<T>::CreateUnique(...)`

这样可以把对象池使用方式写成：

```cpp
ObjectPool<GameEntity> pool(8);
auto entity = pool.CreateUnique(1, "Player");
```

当 `entity` 离开作用域时，会自动调用对象析构并把内存归还给对象池。

这一层本质上是：

- 对象池 + 自定义 deleter + `std::unique_ptr`

优点：

- 减少手动 `Destroy` 遗漏
- 更贴近现代 C++ 的 RAII 风格

#### 4. 增加变长块管理策略

项目新增了 `VariableMemoryPool`，采用的是面试中很常见的“分离适配”思路：

- 维护多组尺寸类别，例如 `16/32/64/128/256`
- 小于等于某个桶位大小的请求，放进该桶位
- 超过最大桶位的请求，回退到系统分配

这个设计的特点：

- 小对象分配很快
- 容易实现
- 非常适合说明“变长块池并不一定真的为每个请求都精确分配”

它的代价是：

- 可能出现内部碎片
- 尺寸类别设计不合理时，浪费会明显增加

#### 5. 增加性能基准测试

项目新增可执行文件：

- `memory_pool_benchmark`

测试内容：

- 普通 `new/delete`
- `ObjectPool<T>`
- `VariableMemoryPool`

用途：

- 帮你直观看到对象池在高频申请/释放场景下的收益
- 方便面试时从“理论优化”扩展到“如何验证优化”

注意：

- 这个基准程序是教学用途
- 结果会受编译器、机器、负载和优化等级影响
- 面试中重点不是绝对数值，而是“你知道如何设计验证方法”

### 各模块适合怎么讲

#### `FixedMemoryPool`

关键词：

- 定长块
- 批量扩容
- 空闲链表
- `O(1)` 分配与回收

#### `ObjectPool<T>`

关键词：

- placement new
- 显式析构
- 内存生命周期与对象生命周期分离

#### `ThreadSafeObjectPool<T>`

关键词：

- 互斥锁保护
- 先保证正确性
- 后续可扩展为 thread local 或 lock-free

#### `DebugFixedMemoryPool`

关键词：

- guard bytes
- magic number
- double free 检测
- 调试版内存池

#### `VariableMemoryPool`

关键词：

- segregated free lists
- size classes
- 大块请求回退系统分配

### 面试高频延伸点

#### 1. 为什么线程安全版本不一定更快？

因为互斥锁会带来竞争和上下文切换成本。线程安全版本的目标是先保证共享池在多线程下行为正确，而不是天然更快。

#### 2. 调试版内存池为什么不适合生产环境？

因为它增加了：

- 额外头尾标记
- 活跃块映射表
- 更多校验逻辑

这些都意味着额外时间和空间开销。

#### 3. 变长池为什么常用分离适配？

因为它在实现复杂度和运行效率之间比较平衡：

- 比完全通用分配器简单
- 比每次直接走系统分配更快
- 非常适合固定几类常见大小的对象

#### 4. 自动归还包装器的本质是什么？

本质是用 RAII 管理“对象池归还动作”，把原来必须手动写的 `Destroy` 绑定到智能指针析构过程。

### 推荐运行方式

先看功能演示：

```bash
./build/memory_pool_demo
```

再看性能测试：

```bash
./build/memory_pool_benchmark
```

最后运行单元测试：

```bash
./build/memory_pool_tests
ctest --test-dir build --output-on-failure
```

如果需要覆盖率统计：

```bash
cmake -S . -B build-coverage -DENABLE_COVERAGE=ON
cmake --build build-coverage --target coverage
cat build-coverage/coverage/coverage_summary.txt
```

覆盖率输出位置：

- 文本摘要：`build-coverage/coverage/coverage_summary.txt`
- 测试输出：`build-coverage/coverage/test_output.txt`
- 每个源文件的 `.gcov` 明细：`build-coverage/coverage/`

### 适合面试时的总结表达

可以这样概括：

> 这个示例最初只有一个基础的定长内存池，后续我把它扩展成了一个小型内存池实验项目：包括线程安全封装、越界检测调试池、RAII 对象自动归还、分离适配的变长内存池，以及一个简单基准测试程序。这样不但能说明内存池怎么写，还能说明它在工程里怎么演进。

# Interview__RPC__README.md

## RPC

这个工程实现了一个极简 RPC 框架，覆盖面试中最容易被追问的几个核心点：请求结构、序列化、方法注册、服务端分发和响应返回。

### 构建

```bash
cd <RPC 项目目录>
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

### 目标

- `rpc_demo`：演示一次 `add` 请求从序列化到分发再到响应解析。
- `rpc_tests`：验证序列化和分发逻辑。

### 面试知识点

- RPC 的本质是“像调用本地函数一样调用远端服务”。
- 核心链路一般包括：接口定义、序列化、网络传输、服务发现、路由分发、超时重试。
- 实际工程里常见协议：JSON、Protobuf、Thrift、自定义二进制协议。
- 高频追问：幂等性、超时、重试、负载均衡、限流、熔断、服务治理。


# Interview__ThreadPool__README.md

## ThreadPool

这个工程实现了一个固定大小线程池，覆盖面试中最常见的任务提交、工作线程循环、条件变量唤醒和 `future` 结果获取。

### 构建

```bash
cd <ThreadPool 项目目录>
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

### 目标

- `thread_pool_demo`：提交平方计算任务并汇总结果。
- `thread_pool_tests`：验证有返回值和无返回值任务。

### 面试知识点

- 线程池的价值是复用线程，降低频繁创建销毁线程的成本。
- 核心结构一般包括任务队列、工作线程集合、互斥锁、条件变量、停止标记。
- 常见面试追问：队列是否有界、如何优雅关闭、任务异常如何传播、是否支持工作窃取。
- `std::packaged_task + future` 是一个很经典的最小实现方案。


# Interview__Timer__README.md

## Timer

这个工程实现了一个最小定时调度器，适合面试里讲延迟任务、最小堆、后台线程和条件变量等待。

### 构建

```bash
cd <Timer 项目目录>
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

### 目标

- `timer_demo`：提交一个 100ms 的延迟任务并观察执行结果。
- `timer_tests`：验证单个任务和多个任务的触发行为。

### 面试知识点

- 定时器常见实现依赖最小堆、时间轮或有序链表。
- 最小堆适合中等规模任务调度，实现直观。
- 条件变量可以让后台线程睡到“最近一个到期点”，避免忙等。
- 高频追问：周期任务、取消任务、重复调度、精度误差、系统时钟回拨。


# README.md

## 面试鸭

> ⚠️ **注意**：以下"面试鸭"章节为 React + Node.js + 云开发全栈项目文档，与 C++ 面试相关性较低，建议作为参考或移除。C++ 面试重点应关注上方的设计模式、跨进程通信、内存池、RPC、线程池、定时器等内容。

<!-- 图片占位：原文此处为图片，当前目录未包含对应资源 -->
<!-- 图片占位：原文此处为图片，当前目录未包含对应资源 -->


<!-- ALL-CONTRIBUTORS-BADGE:START - Do not remove or modify this section -->
![All Contributors](#contributors-)
<!-- ALL-CONTRIBUTORS-BADGE:END -->

> 一个面向面试准备的刷题网站。
>
> 开源版为 React + 云开发 / Node.js 全栈项目，包含网站前台 + 管理员后台的完整前后端代码。


在线访问：https://mianshiya.com

- 25 年新版介绍视频
- 24 年教学版开源项目
- 22 年开源版本介绍视频

![面试鸭主页]


### 项目背景

如今面试越来越卷了，每场面试必考八股文，所以不得不背诵面试题。

但网上的面试题过于分散、很多面经也是只有问题没有题解，就导致我们需要一个个去搜索、或者关注这号那号去领取别人整理好的面试题，很麻烦。

因此，我做了【面试鸭】，一个专注面试刷题的网站。有了它，大家可以直接一站式在线刷题看题解，告别搜索和求人，大大提高复习效率。希望它能帮助大家拿到理想的 offer。

### 面试鸭介绍

2024 年，鱼皮带团队上线了新版面试鸭，题目更全面、刷题更方便、背题更省时、题解更优质、知识更丰富、更新更频繁。目标只有一个：**帮助大家更高效地准备面试。**

1. 三端同步，刷题更方便：面试鸭支持网页端、小程序和 IDE 插件，全端数据同步，真正做到随时随地刷题，轻松备战求职面试。
2. 题目全面，命中率高：面试鸭涵盖了 **几乎所有主流编程方向** 的面试题，Java 后端/ C++ / Python / Go / 前端 / 运维 / 计算机基础 / 408 考研，200+ 题库、9000+ 高频面试题，还有秋招热题、企业真实面经、刷题路线等，帮助你 360° 全方位无死角吊打面试官！
3. 题解更优质：我们深知大家背题刷题的效率很大程度上取决于题解的质量，所以专门邀请了多位大厂面试官来创作和优化题解，保证题解是正确易懂的。
4. 更新更频繁：我们现在每天都在高速更新题解、同时优化系统功能。未来也会紧跟面试趋势，持续更新题库和题解，及时添加当下企业面试新题、新知识、新技巧，让你始终走在前列，不要浪费时间在过期的题目和知识上。

### 本开源项目的优势

如果你是

- 求职者 👨：高效刷题、把握高频考点，快速分享面经，顺利通过面试
- 面试官 🧓🏻：快速选题、一键生成试卷，把控面试考点，高效招纳人才
- 学生 👨‍🎓：了解面试考察重点，明确学习方向，积累小题库，稳步提升
- 老师 👩‍🏫：了解企业招聘需求，改进教学方向，帮助更多学生走进大厂

- 干净：界面简洁，纯净无广
- 免费：永久免费，可赚激励
- 全面：海量题目，上百方向
- 共建：全民编辑，共同完善
- 崭新：持续更新，拥抱变化
- 效率：灵活选题，高效刷题
- ~~多端适配（算个毛的优势）~~

### 本开源项目的功能

🌟 亮点功能 🧪 试验功能 🚀 未来计划（画饼）

- 题目
  - 浏览题目
    - 折叠解析
  - 🌟 多维度筛选题目（难度、标签、题型）
  - 🌟 多维度排序题目（热度、收藏数、频率）
  - 🌟 题目推荐
  - 🌟 题目遇见次数
  - 🌟 共同编辑解析
  - 精选题目
  - 上传题目
  - 搜索题目
    - 热门搜索
    - 搜索历史
  - 收藏题目
  - 🧪 语音读题
  - 🚀 选题练习
  - 🚀 分析报告
  - 举报
- 标签
  - 🌟 关注标签
  - 搜索标签
  - 🚀 标签树
- 回答
  - 🌟 回答题目
  - 精选回答
  - 点赞回答
  - 回复

- 试卷
  - 🌟 试题篮
  - 🌟 一键组卷
  - 🌟 下载试卷
  - 查看 / 隐藏解析
  - 试卷可见范围（公开 / 私有）
  - 🧪 语音读卷
  - 🚀 试卷打分
- 用户
  - 设置个人信息
  - 积分
  - 🌟 收藏夹
  - 题目管理
  - 回答管理
  - 试卷管理
  - 消息中心
  - 个人主页
- 其他
  - 排行榜

### 项目展示

多维度题目检索和排序：

<!-- 图片占位：原文此处为图片，当前目录未包含对应资源 -->

题目浏览：

<!-- 图片占位：原文此处为图片，当前目录未包含对应资源 -->

试卷浏览：

<!-- 图片占位：原文此处为图片，当前目录未包含对应资源 -->

快速组卷：

<!-- 图片占位：原文此处为图片，当前目录未包含对应资源 -->

### 技术栈

> 可先观看：面试鸭技术分享视频

#### 前端

- 开发框架：React、Umi
- 脚手架：Ant Design Pro
- 组件库：Ant Design、Ant Design Components
- 语法扩展：TypeScript、Less
- 打包工具：Webpack
- 代码规范：ESLint、StyleLint、Prettier

#### 后端

- 主语言：JavaScript（Node.js / 腾讯云云开发）
- 开发框架：Express（Express Session）
- 数据库：MongoDB
- 中间件：Redis、Elasticsearch
- 对象存储：腾讯云云存储 / 腾讯云 COS

#### 部署

- web 服务：Nginx
- Docker 容器、微信云托管
- CDN 加速

### 整体架构

> 仅供参考

![面试鸭网站架构图]

### 快速上手

#### 前端

环境要求：Node.js >= 14

安装依赖：

```bash
yarn
```

启动：

```bash
yarn start
```

部署：

```bash
yarn build
```

执行命令后会得到 dist 目录，可以放到自己的 web 服务器指定的路径；也可以使用 Docker 容器部署，将 dist、Dockerfile、docker 目录（文件）一起打包即可。

#### Node.js 后端

> 代码均在 server 目录中

请阅读下方“Node 后端”章节（当前目录未包含单独的 `server__README.md`）

#### 云函数后端

> 代码均在 cloudfunctions 目录中

请阅读下方“云函数后端”章节（当前目录未包含单独的 `cloudfunctions__README.md`）

### 🍊 进一步学习

如果想进一步了解项目的完整制作过程、系统设计、数据库设计、经验感悟等，欢迎加入鱼皮的 编程导航学习圈 ，手把手带大家自己开发出这个项目。

### 欢迎贡献

项目需要大家的支持，期待更多小伙伴的贡献，你可以：

- 上传优质题目
- 分享试卷或面经
- 回答题目或优化题目解析
- 提建议或 Bug：反馈平台

### Contributors ✨

Thanks goes to these wonderful people (emoji key):

<!-- ALL-CONTRIBUTORS-LIST:START - Do not remove or modify this section -->
<!-- prettier-ignore-start -->
<!-- markdownlint-disable -->
<table>
  <tr>
    <td align="center"><a href="https://www.code-nav.cn"><img src="https://avatars.githubusercontent.com/u/26037703?v=4?s=100" width="100px;" alt=""/><br /><sub><b>程序员鱼皮</b></sub></a><br /><a href="#infra-liyupi" title="Infrastructure (Hosting, Build-Tools, etc)">🚇</a> <a href="https://github.com/liyupi/mianshiya/commits?author=liyupi" title="Tests">⚠️</a> <a href="https://github.com/liyupi/mianshiya/commits?author=liyupi" title="Code">💻</a></td>
    <td align="center"><a href="http://blog.code520.com.cn"><img src="https://avatars.githubusercontent.com/u/73232691?v=4?s=100" width="100px;" alt=""/><br /><sub><b>周靖</b></sub></a><br /><a href="#content-jokereven" title="Content">🖋</a> <a href="https://github.com/liyupi/mianshiya/commits?author=jokereven" title="Tests">⚠️</a></td>
    <td align="center"><a href="https://github.com/taiyang"><img src="https://avatars.githubusercontent.com/u/11793873?v=4?s=100" width="100px;" alt=""/><br /><sub><b>Tai</b></sub></a><br /><a href="#content-taiyang" title="Content">🖋</a> <a href="https://github.com/liyupi/mianshiya/commits?author=taiyang" title="Tests">⚠️</a> <a href="https://github.com/liyupi/mianshiya/pulls?q=is%3Apr+reviewed-by%3Ataiyang" title="Reviewed Pull Requests">👀</a></td>
    <td align="center"><a href="https://github.com/Luo-0518"><img src="https://avatars.githubusercontent.com/u/92662475?v=4?s=100" width="100px;" alt=""/><br /><sub><b>Luo-0518</b></sub></a><br /><a href="#content-Luo-0518" title="Content">🖋</a> <a href="https://github.com/liyupi/mianshiya/commits?author=Luo-0518" title="Tests">⚠️</a></td>
  </tr>
</table>

<!-- markdownlint-restore -->
<!-- prettier-ignore-end -->

<!-- ALL-CONTRIBUTORS-LIST:END -->

This project follows the all-contributors specification.
Contributions of any kind welcome!

# 云函数后端（来源：cloudfunctions__README.md）

## 面试鸭后端云函数

> ⚠️ 前期使用 serverless 后端架构，代码量多了后改为完整 server 部署，所以本目录下代码逻辑可能不是最新。
>
> 请先阅读 云函数文档 以了解云函数

### 开通云开发

> 云开发是一体化开发环境和工具平台，能够帮你快速开发后台应用

先在 官网 开通云开发，获得环境 id，然后将环境 id 填写到本项目根目录的 `cloudbaserc.json` 中。

### 用命令行操作云函数

> 请先阅读：云开发 CLI 工具文档

先安装云开发 CLI 工具

```bash
npm i -g @cloudbase/cli
```

本地测试

```bash
tcb fn run --name <functionName> --params "{\"userId\": 1}" --path <functionPath>
```

更新函数代码

```bash
tcb fn code update <functionName> --dir <functionPath>
```

部署函数代码

```bash
tcb fn deploy <functionName> --dir <functionPath>
```

### ES 设计

> 涉及项目机密，仅对 星球内 同学公开

题目表 question_v1

#### 搜索示例

GET question/\_search

```json
{
  "query": {
    "bool": {
      "should": [
        {
          "match": {
            "name": ""
          }
        },
        {
          "match": {
            "desc": ""
          }
        }
      ],
      "filter": [
        {
          "term": {
            "reviewStatus": 1
          }
        },
        {
          "term": {
            "isDelete": false
          }
        },
        {
          "term": {
            "tags": "java"
          }
        },
        {
          "term": {
            "tags": "框架"
          }
        }
      ],
      "minimum_should_match": 0
    }
  },
  "from": 0,
  "size": 5,
  "_source": ["name", "_createTime", "desc", "reviewStatus", "priority", "tags"],
  "sort": [
    {
      "priority": {
        "order": "desc"
      }
    },
    {
      "_score": {
        "order": "desc"
      }
    },
    {
      "publishTime": {
        "order": "desc"
      }
    }
  ]
}
```

### 开发 SDK

> 请阅读：一站式后端服务文档

### 部署函数命令

tcb fn deploy esService --dir cloudfunctions/common/esService --force

tcb fn deploy sendEmail --dir cloudfunctions/common/sendEmail --force

tcb fn deploy redisService --dir cloudfunctions/common/redisService --force

tcb fn deploy addQuestion --dir cloudfunctions/question/addQuestion --force

tcb fn deploy addQuestionsToES --dir cloudfunctions/question/addQuestionsToES --force

tcb fn deploy syncQuestionsToES --dir cloudfunctions/question/syncQuestionsToES --force

tcb fn deploy deleteQuestion --dir cloudfunctions/question/deleteQuestion --force

tcb fn deploy favourQuestion --dir cloudfunctions/question/favourQuestion --force

tcb fn deploy reviewQuestion --dir cloudfunctions/question/reviewQuestion --force

tcb fn deploy searchQuestions --dir cloudfunctions/question/searchQuestions --force

tcb fn deploy getRandomQuestion --dir cloudfunctions/question/getRandomQuestion --force

tcb fn deploy shareQuestion --dir cloudfunctions/question/shareQuestion --force

tcb fn deploy updateQuestion --dir cloudfunctions/question/updateQuestion --force

tcb fn deploy viewQuestion --dir cloudfunctions/question/viewQuestion --force

tcb fn deploy listRecommendQuestions --dir cloudfunctions/question/listRecommendQuestions --force

tcb fn deploy getTagsMap --dir cloudfunctions/tag/getTagsMap --force

tcb fn deploy login --dir cloudfunctions/login --force

tcb fn deploy getUser --dir cloudfunctions/user/getUser --force

tcb fn deploy getCaptcha --dir cloudfunctions/getCaptcha --force

tcb fn deploy getSimpleUser --dir cloudfunctions/user/getSimpleUser --force

tcb fn deploy getCurrentUser --dir cloudfunctions/user/getCurrentUser --force

tcb fn deploy listUserCycleRank --dir cloudfunctions/user/listUserCycleRank --force

tcb fn deploy listUserTotalRank --dir cloudfunctions/user/listUserTotalRank --force

tcb fn deploy getUserRank --dir cloudfunctions/user/getUserRank --force

tcb fn deploy updateUser --dir cloudfunctions/user/updateUser --force

tcb fn deploy searchPapers --dir cloudfunctions/paper/searchPapers --force

tcb fn deploy addSearchHistory --dir cloudfunctions/searchHistory/addSearchHistory --force

tcb fn deploy hotSearches --dir cloudfunctions/searchHistory/hotSearches --force

#### 举报

tcb fn deploy addReport --dir cloudfunctions/report/addReport --force

tcb fn deploy reviewReport --dir cloudfunctions/report/reviewReport --force

#### 回答

tcb fn deploy addComment --dir cloudfunctions/comment/addComment --force

tcb fn deploy deleteComment --dir cloudfunctions/comment/deleteComment --force

tcb fn deploy updateComment --dir cloudfunctions/comment/updateComment --force

tcb fn deploy reviewComment --dir cloudfunctions/comment/reviewComment --force

tcb fn deploy searchComments --dir cloudfunctions/comment/searchComments --force

tcb fn deploy thumbUpComment --dir cloudfunctions/comment/thumbUpComment --force

#### 题目修改

tcb fn deploy addQuestionEdit --dir cloudfunctions/questionEdit/addQuestionEdit --force

tcb fn deploy deleteQuestionEdit --dir cloudfunctions/questionEdit/deleteQuestionEdit --force

tcb fn deploy updateQuestionEdit --dir cloudfunctions/questionEdit/updateQuestionEdit --force

tcb fn deploy reviewQuestionEdit --dir cloudfunctions/questionEdit/reviewQuestionEdit --force

tcb fn deploy searchQuestionEdits --dir cloudfunctions/questionEdit/searchQuestionEdits --force

#### 回复

tcb fn deploy addReply --dir cloudfunctions/reply/addReply --force

tcb fn deploy deleteReply --dir cloudfunctions/reply/deleteReply --force

tcb fn deploy searchReplies --dir cloudfunctions/reply/searchReplies --force


#### 遇到题目

tcb fn deploy addMeetQuestion --dir cloudfunctions/meetQuestion/addMeetQuestion --force

tcb fn deploy deleteMeetQuestion --dir cloudfunctions/meetQuestion/deleteMeetQuestion --force

tcb fn deploy updateMeetQuestion --dir cloudfunctions/meetQuestion/updateMeetQuestion --force

tcb fn deploy searchMeetQuestions --dir cloudfunctions/meetQuestion/searchMeetQuestions --force

#### 试卷

tcb fn deploy addPaper --dir cloudfunctions/paper/addPaper --force

tcb fn deploy viewPaper --dir cloudfunctions/paper/viewPaper --force

tcb fn deploy updatePaper --dir cloudfunctions/paper/updatePaper --force

tcb fn deploy deletePaper --dir cloudfunctions/paper/deletePaper --force

#### 消息

tcb fn deploy addMessage --dir cloudfunctions/message/addMessage --force

tcb fn deploy countMyMessages --dir cloudfunctions/message/countMyMessages --force

tcb fn deploy searchMessages --dir cloudfunctions/message/searchMessages --force

tcb fn deploy updateAllMessages --dir cloudfunctions/message/updateAllMessages --force

tcb fn deploy updateMessage --dir cloudfunctions/message/updateMessage --force

#### 用户积分

tcb fn deploy addUserScore --dir cloudfunctions/userScore/addUserScore --force

tcb fn deploy countUserScoreRank --dir cloudfunctions/userScore/countUserScoreRank --force

### 常用代码片段

1. 云函数事务

```javascript
const transaction = await db.startTransaction();
try {
  await xxx();
  await xxx();
  await transaction.commit();
  return id;
} catch (e) {
  console.error('xxx error', e);
  await transaction.rollback();
  return -1;
}
```

# problems__C++单例模式.md

在C++中，单例模式是一种设计模式，用来限制一个类只能创建一个对象。这对于需要全局访问点的情况非常有用，例如日志记录或者数据库连接。

下面是一个简单的单例模式的实现

> ⚠️ **注意**：以下单例示例存在以下问题：
> 1. **非线程安全**：多线程下可能创建多个实例
> 2. **内存泄漏**：`new` 的对象从未被 `delete`
> 3. **使用 `0` 而非 `nullptr`**（C++11 起推荐用 `nullptr`）
> 4. **缺少拷贝构造函数和赋值运算符的禁用**
> 
> 面试中推荐使用 C++11 Magic Static（局部静态变量）实现线程安全单例。

```C++
class Singleton {
private:
    static Singleton* instance;
    // 私有构造函数，防止从其他地方创建对象。
    Singleton() {}

public:
    // 静态访问方法。
    static Singleton* getInstance() {
        if (instance == 0) {
            instance = new Singleton();
        }
        return instance;
    }
};

// 初始化指针为零，这样在第一次调用getInstance时可以初始化
Singleton* Singleton::instance = 0;

int main() {
    // 创建一个新的Singleton对象。
    Singleton* s = Singleton::getInstance();

    // 使用Singleton对象。

    return 0;
}

```


# problems__单线程怎么保证高并发？.md

1. **事件驱动模型**：
   - 使用事件驱动的方式，将任务分解成独立的事件或任务，并通过事件循环机制依次处理这些事件。
   - 单线程通过事件驱动模型可以实现非阻塞IO操作，提高系统的并发处理能力。
2. **异步编程**：
   - 利用异步编程模型，将IO操作转化为异步调用，在等待IO操作完成时不会阻塞主线程执行。
   - 可以通过回调函数、Promise对象、async/await等方式来处理异步任务，充分利用CPU资源。
3. **线程池**：
   - 单线程环境下可以使用线程池来管理多个任务的执行，通过合理配置线程池的大小和任务队列来提高并发处理能力。
   - 在单线程中采用线程池技术可以实现任务的并行处理，提高系统的性能。
4. **利用缓存**：
   - 对于需要频繁访问的数据，可以使用缓存机制来提高数据读取速度，减少对数据库或文件系统的IO操作。
   - 合理利用内存缓存可以降低系统负载，提高响应速度。
5. **优化算法和数据结构**：
   - 设计高效的算法和数据结构可以减少计算和存储开销，提高系统的运行效率。
   - 在单线程环境中，通过优化算法和数据结构可以减少计算时间和空间复杂度，从而实现高并发处理。

# problems__在C++中，三个全局变量相互依赖，程序应该如何初始化呢？300个呢？.md

在C++中，如果有多个全局变量相互依赖，初始化顺序可能会导致问题，因为**全局变量的初始化顺序在不同编译单元之间是未定义的**。这意味着，如果一个全局变量依赖于另一个尚未初始化的全局变量，那么程序可能会出现运行时错误。

我们可以利用**构造函数中的初始化技巧，确保每个变量首次访问的时候进行初始化**，像这样，并且在变量多的时候去利用单例模式去控制他们的初始化顺序：

```c++
class GlobalResources {
private:
    A a;
    B b; // 假设B依赖A
    C c; // 假设C依赖A和B
    // ... 其他依赖项

    GlobalResources() : a(), b(a), c(a, b) /* etc. */ {}

public:
    static GlobalResources& instance() {
        static GlobalResources instance;
        return instance;
    }

    A& getA() { return a; }
    B& getB() { return b; }
    C& getC() { return c; }
    // ... 提供对其他资源的访问
};

```


# Node 后端（来源：server__README.md）

## 面试鸭后端 Node 版

> Node + Express 框架，推荐用这个整体后台

### 快速开发

进入该目录，安装依赖：

```
yarn
```

#### 启动服务

> 记得先把 config 中的 mongodb 和 redis 地址换成自己的

以测试环境启动：

```
npm run start:dev
```

以线上环境启动：
```
npm run start
```

### 部署发布

#### 编写 Dockerfile

```dockerfile
# 使用官方 Node.js 12 轻量级镜像.
# https://hub.docker.com/_/node
FROM node:12-slim

# 定义工作目录
WORKDIR /usr/src/app

# 将依赖定义文件拷贝到工作目录下
COPY package*.json ./

# 以 production 形式安装依赖
RUN npm install --only=production

# 将本地代码复制到工作目录内
COPY ../function-to-run ./

# 启动服务
CMD [ "node", "server.js" ]
```

#### 上传代码包

将目录下所有文件压缩为 zip：

<!-- 图片占位：原文此处为图片，当前目录未包含对应资源 -->

进入微信云托管，创建环境和服务，然后发布一个版本。

- 上传方式为本地代码
- 附件类型为 ZIP 压缩包（即上一步中产生的压缩包）
- 监听端口为 3000

<!-- 图片占位：原文此处为图片，当前目录未包含对应资源 -->

随后点击确定，即可创建一个版本，后续发布流程可以参考微信云托管文档。


### 相对于云函数的优点

1. 搜索调试爽
2. 便于区分多环境
3. 代码更容易复用
4. 便于自己封装逻辑
5. 成本相对可控，不容易被刷量
6. 更安全
7. 便于迁移成其他的 web 框架
